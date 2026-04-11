# BP-Synth Code Tutorials

STM32-based monosynth project based on the tutorial series by **Blaine Perkins (Synthetech)**.

## Original Author

All original code and tutorials are the work of **Blaine Perkins**.

- YouTube Channel: [Synthetech](https://www.youtube.com/@Synthetech)
- Tutorial Playlist: [BP-Synth Code Tutorials](https://www.youtube.com/playlist?list=PLOjbFVchrTEyJBfXwbhBicpxuOOPgtQ3v)

## About

The BP-Synth is a monosynth built around the STM32F411 (BlackPill) microcontroller. The tutorial series
walks through building up the synth incrementally — from basic waveform generation to MIDI control,
envelopes, filters, and more. Each project folder in this repo corresponds to a stage in that series.

## Project Directory Structure

```
bp-synth/
│
├── BP_Init/                  # Initial STM32CubeIDE project setup (I2S, clock config)
│
├── BP_BasicSaw/              # Basic sawtooth oscillator over I2S
│
├── BP_BasicMIDI/             # MIDI input via UART; note-on/off driving the oscillator
│
├── BP_Pitchbend/             # MIDI pitchbend support
│
├── BP_Vibrato/               # LFO-based vibrato effect
│
├── BP_MIDI-CCs/              # MIDI CC (Control Change) parameter control
│
├── BP_OscSelect_1-2/         # Oscillator waveform selection (two variants)
│
├── BP_ADSR_EG/               # ADSR envelope generator (two variants)
│
├── BP_Filter_CODE/           # DSP low-pass filter (two variants)
│
└── BPsynthFiles_Basic/       # Supporting files: schematic images, GUI app,
                              #   pre-built hex files, MIDI CC list PDF,
                              #   and hardware setup instructions
```

Each code project folder contains a standard STM32CubeIDE workspace with:
- `Core/Src/` — application source files (`main.c`, `oscillators.c`, `waveGen.c`, etc.)
- `Core/Inc/` — header files
- `Drivers/` — STM32 HAL and CMSIS drivers
- `startup/` — startup assembly file
- `*.ioc` — STM32CubeMX project configuration file

## Note on Build Artifacts

Build output directories (`Debug/`, `Release/`) and compiled artifacts (`.o`, `.d`, `.elf`, `.hex`,
`.map`, etc.) have been removed from version control and are excluded via `.gitignore`. These are
generated locally by STM32CubeIDE when you build a project and do not need to be tracked in git.
