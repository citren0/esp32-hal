# Overview

This is a rudimentary Hardware Abstraction Layer (HAL) for the ESP32S3 which provides features such as peripheral and CPU initialization, GPIO control, and precise sleep.

This software runs on bare metal and contains a linker script which will create the ELF file in the correct format to be flashed by esptool to your devboard.

This software was heavily inspired by this blog post [https://vivonomicon.com/2019/03/30/getting-started-with-bare-metal-esp32-programming/](https://vivonomicon.com/2019/03/30/getting-started-with-bare-metal-esp32-programming/) and Magnus and Tim from Tensentric who showed me how C code can be compiled and linked to run on bare metal.

The register locations and layouts come from the ESP32 Technical Reference Manual at [https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)

# Compilation

$ make all

# Flashing

$ make flash
