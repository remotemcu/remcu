# REMCU Library

[![Ubunt1](https://github.com/remotemcu/remcu/actions/workflows/Ubuntu.yml/badge.svg)](https://github.com/remotemcu/remcu/actions/workflows/Ubuntu.yml)

![logo](img/logo.png)

---

## Overview

**REMCU** stands as an toolkit designed to facilitate the building and instrumentation of MCU's driver SDKs across diverse high-level platforms, encompassing Windows, MacOS, and even Linux, including the embedded variant. By leveraging REMCU, the source code of the MCU's driver can be prepared to execute on a PC host or Linux Embedded Device. Crucially, REMCU orchestrates the seamless execution of peripheral operations such as peripheral's register storage and loading on the target MCU. This process is accomplished through interfaces like OpenOCD, GDB server, or custom implementations. In effect, REMCU empowers developers to govern the peripherals of the MCU from the comfort of their high-level PC applications. In sum, REMCU liberates MCU development from conventional constraints, negating the need for firmware and communication protocol development.
More detailed in [Memfault Article](https://interrupt.memfault.com/blog/mcu-peripheral-forwarding)

![MCU Peripheral Forwarding](https://raw.githubusercontent.com/remculib/site/master/post/forwarding/title.png)


## How to Use

Examples to use in [Examples](https://github.com/remotemcu/remcu_examples) and the [Tutorials](https://remotemcu.com/tutorials)

## How to Build

The library is built as part of [REMCU CHIP SDK Collection](https://github.com/remotemcu/remcu-chip-sdks)
