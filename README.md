here i try to rebase anycubic kobra fw from marlin 2.0 to 2.1+ using the same dirty hacks as the anycubic team because i don't understand what i'm doing

based on marlin 2.1.2.1+ (https://github.com/MarlinFirmware/Marlin/commit/2e51bf423a3e26b22204d64e592b6c85112e9ebe)

contains 
1) original external dependencies and build env
2) marlin 
3) minimal set of hacks from anycubic to make it run: HAL and hardware related workarounds
4) my set of hacks (mostly the same as anycubic's) to make it compilable by the atrociously outdated toolchain
5) some ebdedded gcode was modified by anycubic -- left it as is since i don't understand it

thanks u/jojos38 / [@jojos38](https://github.com/jojos38/) for findings on the hdsc sdk and other stuff

to build check his step-by-step build tutorial on r https://www.reddit.com/r/anycubic/comments/y2waxu/tutorial_how_to_build_anycubic_marlin_source_code/

current state of affairs: it builds, it works to some extent

prints are being printed but steppers sound scary at times - scarier that OG 2.8.2 with the same settings (no LA). possibly anycubic's repo does not contain actual config values used by the firmware provided on their website

same with the holy grail linear advance feature - it kind of works but suitable config values are to be found. most of all i have doubts about EJERK and stepper configuration e.g. current and so on

feel free to try

issues found by me so far:
* after triggering the filament runout and pressing resume on the screen it.. doesnt resume, most likely i messed something up in dgus_tft.cpp
* after flashing new firmware the printer does not restart properly, needs debugging though this sounds VERY minor
