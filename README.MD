# REMCU Library

![logo](img/logo.png)

---

## Overview

**REMCU** Toolkit is a powerful tool specifically designed for embedded developers, providing them with a convenient and efficient way to build and utilize MCU Software Development Kit (SDK) libraries sourced from various chip vendors such as ST, NXP, Infineon and more. This comprehensive toolkit supports different high-level platforms, including Windows, Linux, and MacOS, allowing developers to seamlessly integrate the SDK APIs into their PC applications. By leveraging the innovative technology of [MCU Peripheral Forwarding](https://remotemcu.com/chip-peripheral-forwarding), **REMCU** Toolkit enables developers to harness the full potential of the SDK's functionality and unlock new possibilities for embedded software development.

REMCU Toolkit leverages the power of [**LLVM ADIN fork**](https://github.com/remotemcu/adin-llvm) to modify the function code within the MCU Software Development Kit (SDK) libraries. By employing this technique, REMCU Toolkit is able to intercept peripheral operations performed by the SDK, such as storing data to registers and loading data from registers.

When a peripheral operation is encountered, **REMCU** Toolkit redirects the execution of these operations to be processed on the target chip. It achieves this by utilizing [OpenOCD](https://github.com/ilg-archived/openocd/releases/tag/v0.10.0-12-20190422) (Open On-Chip Debugger) and GDB (GNU Debugger) server. These tools facilitate the communication and interaction with the target microcontroller, allowing REMCU Toolkit to send the intercepted peripheral operations to be executed on the actual hardware.

By employing this interception and redirection mechanism, **REMCU** Toolkit enables developers to utilize the SDK's APIs within PC applications. The intercepted operations are forwarded to the target chip, ensuring that the functionality of the SDK is executed on the intended hardware. This innovative approach bridges the gap between PC applications and embedded systems, providing developers with the capability to access and control MCU peripherals directly from their PC environment.

## How to Use

Examples to use in [Examples](https://github.com/remotemcu/remcu_examples) and the [Tutorials](https://remotemcu.com/tutorials)