# NeedleDCF77
## General idea
This project is a clock that shows time using three analog meters (with needle) for hours, minutes and seconds, and a VFD for the date. The time reference is the [DCF77](https://en.wikipedia.org/wiki/DCF77) longwave signal, receivable across most of Europe — no manual time setting, no NTP, no RTC drift correction needed beyond the daily resync.

### Assembled clock
![DSC_9274edit](https://github.com/Dzalex/NeedleDCF77/assets/75816219/7fe31634-a44e-4f18-a4cd-9f0b9831dc4f)

## Hardware
  - Main microcontroller: *STM32L051C8T6*
  - Power supply built around Analog Devices' *LTC3558* (Li-Po charge + regulation)
  - DCF77 receiver module: *EB6180B1COB77K5A1*
  - [VFD](https://en.wikipedia.org/wiki/Vacuum_fluorescent_display) for the date — a salvaged Itron FG95A
    - Driver: PT6312
    - Filament driver: LM4871 (alternative: LM9022)
    - Inverting regulator: LT1617
  - Analog meters, repurposed as needle displays:
    - Simpson 60 x 60 mm — hours
    - 85C1-A — minutes
    - 91C4 — seconds
  - Enclosure: wooden box, 20 x 10 x 6 cm
  - Battery: Panasonic 18650B

Schematic and PCB are designed in *KiCad* — see [`hardware/NeedleDCF77`](hardware/NeedleDCF77), including a rendered [schematic PDF](hardware/NeedleDCF77/DCF77Needle_schematic.pdf).

## Software
Firmware lives in [`sw/STM32CubeIDE_Project`](sw/STM32CubeIDE_Project), written in C on top of **FreeRTOS**, developed with **STM32CubeIDE** / STM32CubeMX and built with **CMake**.

### Layout
  - `Core/` — STM32CubeMX-generated init code, HAL configuration, and the FreeRTOS task wiring (`freertos.c`).
  - `BSP/` — hardware-facing drivers: DCF77 signal capture (`dcf77.c`), needle PWM output (`needle.c`), VFD control (`vfd.c`), battery/power management (`powerManagement.c`).
  - `App/` — hardware-independent application logic: DCF77 frame decoding (`dcf77Decoding.c`), needle/VFD interface state (`interface.c`).

### What it does
  - `StartDCF77Task` samples the DCF77 signal, decodes the time/date frame, validates it (parity + range/sanity checks) and syncs the RTC once per day (2 AM) or on button press.
  - `StartInterfaceTask` drives the needles from RTC time every second and shows the date on the VFD on button press; the display is suspended while a sync is in progress.
  - FreeRTOS tickless idle is enabled (`Core/Inc/FreeRTOSConfig.h`) so the MCU sleeps between scheduled work; `powerManagement.c` separately handles battery voltage readout and Li-Po charger control (LTC3558).

### Building the firmware
```sh
cmake --preset Debug      # or Release
cmake --build build/Debug
```
The project can also be opened directly in STM32CubeIDE.

### Running the unit tests
DCF77 frame decoding (`App/Src/dcf77Decoding.c`) is unit-tested on the host with [Unity](sw/STM32CubeIDE_Project/test/subprojects/unity), independent of any HAL/hardware dependency:
```sh
cd sw/STM32CubeIDE_Project/test
meson setup build
meson test -C build
```

## Improvements to come
  - Algorithm to index the current bit by time elapsed from the minute mark, so a missed/noisy pulse doesn't throw off subsequent bits, and to retain good bits from a previous sample.
  - Improve battery monitoring and charging. Alerting user on that regard.

