# NeedleDCF77
## General idea
This project is clock that shows time using three analog meters (with needle) for time and one VFD for date. Main time reference is [DCF77](https://en.wikipedia.org/wiki/DCF77) signal attainable in most of Europe.
## Hardware
### Alredy decided
  - 'OLD!: Main microcontroller is *STM32F415RG*, using MikroE MINI-M4 For STM32 for develop'
    - New proposal is to use *STM32L051C8T6*, it is low power...
  - Power supply is implemnted around Analog Device's *LTC3558*
  - DCF77 receive module is *EB6180B1COB77K5A1*
  - ? [VFD](https://en.wikipedia.org/wiki/Vacuum_fluorescent_display), have old one, yet to decide if to use it!

### To decide
  - Missing two analog meters (Aliexpres, eBay, Kp)
  - Project enclosure (some wooden box)
  - Battery
  - VFD driver

## Software
### Hardware design
Corelated to Hardware, Shematic and PCB is designed in *KiCad*.

### IDE and language
For developing purpose we will use *STMCubeIDE*, because of it's flexibility, configurability and support. Also it provide easy access to RTOS option. We use C programing language.

### RTOS
To implemet firmware in this projet we use [FreeRTOS](https://www.freertos.org/). Main reasone to use RTOS instead programing barebone firmware is contribution implementation, so people have less chance to influence other people work. Great chance to learn something about tasks, semafors, mesage queues and other useful features of RTOS! :)

### To decide
  - How to implement and test algorithm for received signal analisation?


## Geting stared soon
Coming when some starting point is achived

