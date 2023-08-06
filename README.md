# NeedleDCF77
## General idea
This project is clock that shows time using three analog meters (with needle) for time and one VFD for date. Main time reference is [DCF77](https://en.wikipedia.org/wiki/DCF77) signal attainable in most of Europe.
## Hardware
### Alredy decided
  - Main microcontroller is *STM32L051C8T6*
  - Power supply is implemnted around Analog Device's *LTC3558*
  - DCF77 receive module is *EB6180B1COB77K5A1*
  - [VFD](https://en.wikipedia.org/wiki/Vacuum_fluorescent_display), have old one - itron FG95A
    - Driver PT6312
    - Driver for fillament - LM4871 (alternative for LM9022)
    - Inverting regulator - LT1617
  - Analog meters are:
    - Simpson 60 x 60 mm for Hours
    - 85C1-A for Minutes
    - 91C4 for Seconds
  - Enclosure is wooden box from Alie, 20 x 10 x 6 cm
  - Battery: Panasonic 18650B

## Software
### Hardware design
Corelated to Hardware, Shematic and PCB is designed in *KiCad*.

### IDE and language
For developing purpose we will use *STMCubeIDE*, because of it's flexibility, configurability and support. Also it provide easy access to RTOS option. We use C programing language.

### RTOS
To implemet firmware in this projet we use [FreeRTOS](https://www.freertos.org/). Main reasone to use RTOS instead programing barebone firmware is contribution implementation, so people have less chance to influence other people work. Great chance to learn something about tasks, semafors, mesage queues and other useful features of RTOS! :)

### To decide
  - Planing to use python to implement app for generating time and dates to be able to test.


## Geting stared soon
Coming when some starting point is achived

