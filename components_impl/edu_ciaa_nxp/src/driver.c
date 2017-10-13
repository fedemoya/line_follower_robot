/* Copyright 2014, Your Name <youremail@domain.com>
 * All rights reserved.
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


/*==================[inclusions]=============================================*/
#include "driver.h"

/*==================[macros and definitions]=================================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

MotorSpeed leftMotorSpeed = SPEED_0;
MotorSpeed rightMotorSpeed = SPEED_0;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

void Driver_Start(uint8_t leftMotorPin, uint8_t rightMotorPin) {
    Motor_Init(leftMotorPin);
    Motor_Init(rightMotorPin);
    Motor_SetSpeed(SPEED_20, leftMotorPin);
    Motor_SetSpeed(SPEED_20, rightMotorPin);
}

void Driver_Stop(uint8_t leftMotorPin, uint8_t rightMotorPin) {
    Motor_SetSpeed(SPEED_0, leftMotorPin);
    Motor_SetSpeed(SPEED_0, rightMotorPin);
}

void Driver_HardTurnRight(uint8_t leftMotorPin, uint8_t rightMotorPin) {
    Motor_SetSpeed(SPEED_0, rightMotorPin);
    Motor_SetSpeed(SPEED_20, leftMotorPin);
}

void Driver_HardTurnLeft(uint8_t leftMotorPin, uint8_t rightMotorPin) {
    Motor_SetSpeed(SPEED_0, leftMotorPin);
    Motor_SetSpeed(SPEED_20, rightMotorPin);
}

void Driver_SoftTurnRight(uint8_t leftMotorPin, uint8_t rightMotorPin) {
//    Motor_SetSpeed(SPEED_20, rightMotorPin);
//    Motor_SetSpeed(SPEED_24, leftMotorPin);
}

void Driver_SoftTurnLeft(uint8_t leftMotorPin, uint8_t rightMotorPin) {
//    Motor_SetSpeed(SPEED_20, leftMotorPin);
//    Motor_SetSpeed(SPEED_24, rightMotorPin);
}

void Driver_GoStraightOn(uint8_t leftMotorPin, uint8_t rightMotorPin) {
//    Motor_SetSpeed(SPEED_20, leftMotorPin);
//    Motor_SetSpeed(SPEED_20, rightMotorPin);
}

/*==================[end of file]============================================*/
