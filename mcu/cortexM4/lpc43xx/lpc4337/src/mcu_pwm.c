/* Copyright 2016, Ian Olivieri
 * Copyright 2016, Eric Pernia.
 * Copyleft 2017, Federico Moya.
 * All rights reserved.
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */


/* All functions relative to the microcontroller */

/*==================[inclusions]=============================================*/

#include "mcu_pwm.h"
#include "mcu_sct.h"

/*==================[macros and definitions]=================================*/

#define PWM_FREC          1000 /* 1Khz */
#define PWM_PERIOD        1000 /* 1000uS = 1ms*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/


/*==================[internal data definition]===============================*/

/*
 * Enter a pwm number, get a sct number
 * Since this module works with pwm numbers, but uses sct channels to generate
 * the signal, its necessary to connect pwm number with the SctMap_t.
 * This way the user sets "pwms", while using the sct peripheral internally*/
static const SctMap_t pwmMap[] = {
    [MCU_PWM_PIN_ID_104] =  CTOUT2,
   [MCU_PWM_PIN_ID_105] =  CTOUT5,
   [MCU_PWM_PIN_ID_106] = CTOUT4,
   [MCU_PWM_PIN_ID_82] =  CTOUT6,
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

void mcu_pwm_init() {
    Sct_Init(PWM_FREC);
}

void mcu_pwm_enable(mcu_pwm_pinId_enum pin) {
    Sct_EnablePwmFor(6);
}

void mcu_pwm_write(mcu_pwm_pinId_enum pin, uint8_t value ){
    Sct_SetDutyCycle(6, value);
}

/*==================[end of file]============================================*/
