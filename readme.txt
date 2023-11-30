# [DAC]

## Overview
This project demonstrates the use of a DAC (Digital-to-Analog Converter) to generate different waveforms using the STM32 microcontroller. The code allows the user to switch between a triangle waveform and an escalator waveform by pressing a button. The project utilizes HAL (Hardware Abstraction Layer) libraries for STM32 to manage the DAC, timers, and interrupts.

## Features
- DAC initialization and de-initialization.
- Generation of triangle and escalator waveforms.
- Use of Timer 6 for DAC triggering.
- Button press handling using external interrupts.
- Error handling with an infinite loop and LED indication.

## Prerequisites
Before you begin, ensure you have met the following requirements:
- STM32F4xx series microcontroller board (or compatible hardware).
- HAL libraries for STM32 (usually provided within the STM32Cube software package).
- An IDE supporting STM32 development (e.g., STM32CubeIDE, Keil µVision).
- Basic knowledge of microcontroller programming.

## Setup and Configuration
1. **Clone/Download the Code**: Download the provided source code files to your local machine.
2. **Project Configuration**: 
   - Open your STM32 compatible IDE.
   - Create a new project for your specific microcontroller model.
   - Include the downloaded source files in your project.
   - Ensure HAL libraries for DAC, Timer, GPIO, and Power control (PWR) are included.
3. **Hardware Setup**:
   - Connect an LED to the LED3 pin on your microcontroller board.
   - Ensure a button is connected for user input, triggering waveform changes.

## Running the Program
1. **Compile the Code**: Build the project in your IDE to compile the code.
2. **Upload to Microcontroller**: Once compiled successfully, upload the binary to your STM32 microcontroller.
3. **Operation**:
   - After the upload, the microcontroller will start running the program.
   - Press the user button to switch between the triangle and escalator waveforms.
   - The LED3 will light up in case of any errors.

## Troubleshooting
- **LED3 Turns On**: Indicates an error in the system. Check connections and configurations.
- **No Waveform Change**: Ensure the button is correctly configured and working. Check the external interrupt setup.
