/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 * Copyright (c) 2015-2016 Nico Tonnhofer wurstnase.reprap@gmail.com
 * Copyright (c) 2017 Victor Perez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#include "../platforms.h"

#ifdef HAL_STM32

#include "../../inc/MarlinConfig.h"
#include "../shared/Delay.h"
#include "HAL.h"
#include "bsp_rmu.h"

#include "usb_serial.h"

#ifdef USBCON
  DefaultSerial1 MSerialUSB(false, SerialUSB);
#endif

#if HAS_SD_HOST_DRIVE
  #include "msc_sd.h"
  #include "usbd_cdc_if.h"
#endif

// ------------------------
// Public Variables
// ------------------------

uint16_t MarlinHAL::adc_result;

// ------------------------
// Public functions
// ------------------------

#if ENABLED(POSTMORTEM_DEBUGGING)
  extern void install_min_serial();
#endif

// HAL initialization task
void MarlinHAL::init() {
  // Ensure F_CPU is a constant expression.
  // If the compiler breaks here, it means that delay code that should compute at compile time will not work.
  // So better safe than sorry here.
  //constexpr int cpuFreq = F_CPU;
  //UNUSED(cpuFreq);
	
  NVIC_SetPriorityGrouping(0x3);
	
  #if ENABLED(SDSUPPORT) && DISABLED(SDIO_SUPPORT) && (defined(SDSS) && SDSS != -1)
    OUT_WRITE(SDSS, HIGH); // Try to set SDSS inactive before any other SPI users start up
  #endif

  #if PIN_EXISTS(LED)
    OUT_WRITE(LED_PIN, LOW);
  #endif

  #if PIN_EXISTS(AUTO_LEVEL_TX)
    OUT_WRITE(AUTO_LEVEL_TX_PIN, HIGH);
    delay(10);
    OUT_WRITE(AUTO_LEVEL_TX_PIN, LOW);
    delay(300);
    OUT_WRITE(AUTO_LEVEL_TX_PIN, HIGH);
  #endif
  //SetTimerInterruptPriorities();

  #if ENABLED(EMERGENCY_PARSER) && (USBD_USE_CDC || USBD_USE_CDC_MSC)
    USB_Hook_init();
  #endif

  TERN_(POSTMORTEM_DEBUGGING, install_min_serial());    // Install the min serial handler

  TERN_(HAS_SD_HOST_DRIVE, MSC_SD_init());              // Enable USB SD card access

  #if PIN_EXISTS(USB_CONNECT)
    OUT_WRITE(USB_CONNECT_PIN, !USB_CONNECT_INVERTING); // USB clear connection
    delay(1000);                                        // Give OS time to notice
    WRITE(USB_CONNECT_PIN, USB_CONNECT_INVERTING);
  #endif
}

// HAL idle task
void MarlinHAL::idletask() {
  #if HAS_SHARED_MEDIA
    // Stm32duino currently doesn't have a "loop/idle" method
    CDC_resume_receive();
    CDC_continue_transmit();
  #endif
}

void MarlinHAL::reboot() { NVIC_SystemReset(); }

uint8_t MarlinHAL::get_reset_source() {
    uint8_t res;

    res = rmu_get_reset_cause();

    return res;
}

void MarlinHAL::clear_reset_source() { 
    rmu_clear_reset_cause();
}

// ------------------------
// Watchdog Timer
// ------------------------

#if ENABLED(USE_WATCHDOG)

  #define WDT_TIMEOUT_US TERN(WATCHDOG_DURATION_8S, 8000000, 4000000) // 4 or 8 second timeout

	#include "../cores/iwdg.h"

  bool wdt_init_flag = false;

  void MarlinHAL::watchdog_init() {
    iwdg_init();
    wdt_init_flag = true;
  }

  void MarlinHAL::watchdog_refresh() {
    if(!wdt_init_flag) return;
    iwdg_feed();
  }
	
	void watchdogSetup() {
  // do whatever. don't remove this function.
  }

#endif

extern "C" {
  extern unsigned int _ebss; // end of bss section
}

// Reset the system to initiate a firmware flash
void flashFirmware(const int16_t) { NVIC_SystemReset(); }

#endif // HAL_STM32
