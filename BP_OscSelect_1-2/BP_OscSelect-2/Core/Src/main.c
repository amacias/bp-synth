/* BP-OscSelect-2 */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
* Adding Pulse and MorphSaw Osc's
*
  ******************************************************************************
  */
/* USER CODE END Header */
/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; If not, see <http://www.gnu.org/licenses/>.
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2S_HandleTypeDef hi2s1;
DMA_HandleTypeDef hdma_spi1_tx;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint16_t audiobuff[BUFF_LEN]; // THE audio buffer

//---MIDI Interface variables
uint8_t rx_buff[3];
uint8_t MIDI_rx[3];
uint8_t MIDIcmd;
uint8_t MIDInote;
uint8_t MIDIvel;

uint8_t voice[16];
uint8_t noteCount=0;
uint8_t currNote=54;//initial note value
uint8_t noteByteFlag=0;

//---pitchbend variables
uint8_t bendRange=6;
float bendFactor=1.0f;

extern Oscillator_t 		op1 ;
extern ADSR_t amp_EG;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2S1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void procMIDI(void);
void MIDInoteOn(uint8_t note);
void MIDInoteOff(uint8_t note);
void doPitchBnd(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* -------these two callbacks keep the circular DMA buffer loaded ------------*/

//when DMA output to dac is halfway done, we will reload the first 1000 values
//while the DMA output continues with the 2nd half of the buffer
//the BUFF_LEN_DIV4=500 but we are doing left and right samples (*2)= 1000 samples.
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {

	make_sound((uint16_t *) audiobuff, BUFF_LEN_DIV4);

}

//The DMA output has reached the end of the buffer.  The 1st half is done loading and the DMA
//is going to start sending the 1st half of elements/values to the DAC Chip again.  While it is
//doing that we want to fill in the 2nd half again starting at 1000 elements past &audiobuff
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s) {
	make_sound((uint16_t *) (audiobuff + BUFF_LEN_DIV2), BUFF_LEN_DIV4);
}


//callback when a value is received at the midi port
// (the HAL_NVIC_SetPriority in HAL_UART_MspInit must be set to 0!)
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

  //ignore 0xFx commands
	if (rx_buff[0] >= 0xF0) {
		//reset the interrupt for the MIDI port
		HAL_UART_Receive_IT(&huart2, rx_buff, 1);
		return;
	}



//save a command byte to MIDI_rx[0]
	if (rx_buff[0] & 0x80) {
		MIDI_rx[0] = rx_buff[0];
		noteByteFlag = 0;// clear the flag
		HAL_UART_Receive_IT(&huart2, rx_buff, 1);
		return;
	}

//when we get here we are saving a note Velocity or CC value
	if (noteByteFlag) {
		noteByteFlag = 0;
		MIDI_rx[2] = rx_buff[0];
		HAL_UART_Receive_IT(&huart2, rx_buff, 1);
		procMIDI();//we have three bytes, LET's GO PROCESS THEM!
		return;
	}

	//if we get to here, then we must be rx'ing a MIDI note or Control Change number
	MIDI_rx[1] = rx_buff[0];
	noteByteFlag = 1;//set flag so we stop at above evaluation
	HAL_UART_Receive_IT(&huart2, rx_buff, 1);

}




void procMIDI(void) {

	MIDIcmd = MIDI_rx[0];
	MIDInote = MIDI_rx[1];
	MIDIvel = MIDI_rx[2];

	if ((!MIDIvel) && (MIDIcmd == 0x90)) {
		MIDInoteOff(MIDInote);
		HAL_UART_Receive_IT(&huart2, rx_buff, 1);
		return;
	}



	if (MIDIcmd == 0x80) {
		MIDInoteOff(MIDInote);
		HAL_UART_Receive_IT(&huart2, rx_buff, 1);
		return;
	}


	if (MIDIcmd == 0x90) {
		MIDInoteOn(MIDInote);
		HAL_UART_Receive_IT(&huart2, rx_buff, 1);
		return;
	}


	if (MIDIcmd == 0xB0) {
		ProcessRXdMidiCCs();
		HAL_UART_Receive_IT(&huart2, rx_buff, 1);
		return;
	}

	if (MIDIcmd == 0xE0) {
		doPitchBnd();
		HAL_UART_Receive_IT(&huart2, rx_buff, 1);
		return;
	}
}


//MUST CHANGE "HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);"
// from <hal_msp.c> to "NVIC_PRIORITYGROUP_3" or major glitching will happen!!
void MIDInoteOn(uint8_t note)
{
  if (!noteCount) {
    voice [0] = note;
    ADSR_keyOn(&amp_EG);
    currNote = voice[0];
    noteCount++;
    return;
  }

  //op1.amp=0;//may not need this for basic playing
  for (int i = 0; i < 16; i++) {
    if (!voice[i]) {
      voice[i] = note;
      currNote = voice[i];
      ADSR_keyOn(&amp_EG);
      noteCount++;
      break;
    }
  }

}

void MIDInoteOff(uint8_t note)
{


  if (note == currNote && noteCount>1) {
	  //op1.amp=0;//may not need this for basic playing
    for (int i = 0; i < 16; i++) {
      if (note == voice[i]) {
        currNote = voice[i - 1];
        ADSR_keyOn(&amp_EG);
        voice[i] = 0;
        noteCount--;
        break;
      }

    }
          return;
  }


  if (noteCount < 2) {
	  ADSR_keyOff(&amp_EG);
    voice[0] = 0;
    noteCount--;
    if (noteCount < 0) {
      noteCount = 0;
    }
  }
  if (noteCount > 1) {  //if a note is released that was held down, but not playing
    for (int i = 0; i < 16; i++) {// find the note in the array
      if (voice[i] == note) {
        voice[i] = 0; //remove the note
        for (int c = (i + 1); c < 16; c++) {
          voice[i] = voice[c]; //shift all notes past the removed one, left by one place in array
          ++i;
        }
        voice[15] = 0;
        noteCount--;
        break;
      }
    }
  }

}


void doPitchBnd(void) {

	uint16_t pBendMSB = MIDIvel;
	uint16_t pBendLSB = MIDInote;
	pBendMSB = pBendMSB << 7;
	uint16_t pBendVal = pBendMSB | pBendLSB;
	float mapPitch = pBendVal - 8192;
	float semiPitch = mapPitch / 8192;
	float adjSemiPitch = semiPitch * bendRange;
	float preBendFctr = adjSemiPitch / 12;
	bendFactor = powf(2.0, preBendFctr);

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2S1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  Synth_Init();//inits some basic settings to the oscillator

  //init the I2S interface
  HAL_I2S_Init(&hi2s1);

  /*start the DMA and inform how long the buffer is so it knows
   where the halfway point is and the endpoint (when completed)
   VVVV---THIS starts up the audio stream!---VVV  */
  HAL_I2S_Transmit_DMA(&hi2s1, (uint16_t*) &audiobuff[0], BUFF_LEN);

  //clear array value to 0
  MIDI_rx[1] = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {


	//enables the interrupt for the MIDI port
	//in MX_DMA_Init, be sure we have "HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 13, 0); "
	HAL_UART_Receive_IT(&huart2, rx_buff, 1);



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 194;
  PeriphClkInitStruct.PLLI2S.PLLI2SM = 16;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2S1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2S1_Init(void)
{

  /* USER CODE BEGIN I2S1_Init 0 */

  /* USER CODE END I2S1_Init 0 */

  /* USER CODE BEGIN I2S1_Init 1 */

  /* USER CODE END I2S1_Init 1 */
  hi2s1.Instance = SPI1;
  hi2s1.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s1.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s1.Init.DataFormat = I2S_DATAFORMAT_16B;
  hi2s1.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
  hi2s1.Init.AudioFreq = I2S_AUDIOFREQ_32K;
  hi2s1.Init.CPOL = I2S_CPOL_LOW;
  hi2s1.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s1.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  if (HAL_I2S_Init(&hi2s1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2S1_Init 2 */

  /* USER CODE END I2S1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 31250;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 13, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
