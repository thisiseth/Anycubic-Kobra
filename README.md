here i try to rebase anycubic kobra fw from marlin 2.0 to 2.1+ using the same dirty hacks as the anycubic team because i don't understand what i'm doing

current state of affairs: it builds, it works to some extent

prints are being printed but steppers sound scary at times - scarier that OG 2.8.2 with the same settings (no LA). possibly anycubic's repo does not contain actual config values used by the firmware provided on their website

same with the holy grail linear advance feature - it kind of works but suitable config values are to be found. most of all i have doubts about EJERK and stepper configuration e.g. current and so on

feel free to try

issues found by me so far:
* after triggering the filament runout and pressing resume on the screen it.. doesnt resume, most likely i messed something up in dgus_tft.cpp
* after flashing new firmware the printer does not restart properly, needs debugging though this sounds VERY minor
