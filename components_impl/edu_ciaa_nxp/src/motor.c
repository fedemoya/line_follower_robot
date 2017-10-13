/*==================[inclusions]=============================================*/

#include "motor.h"

#include "board_pinout.h"
#include "mcu_sct.h"

/*==================[macros and definitions]=================================*/

#define SCTPWM_FREC 1000 /* 1Khz */

#define SPEED_STEP 32


/*==================[internal data declaration]==============================*/

int speedMap[] = {0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96,
                  104, 112, 120, 128, 136, 144, 152, 160, 168, 176,
                  184, 192, 200, 208, 2016, 224, 232, 240, 248, 255};

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external functions definition]==========================*/

void Motor_Init(uint8_t pin) {

    Sct_Init(SCTPWM_FREC);

    int8_t sct_number = Board_Pin2Sct(pin);

    if (sct_number >= 0) {
        Sct_EnablePwmFor(sct_number);
    }
}

void Motor_SetSpeed(MotorSpeed s, uint8_t pin) {

    int8_t sct_number = Board_Pin2Sct(pin);

    if (sct_number >= 0) {
        Sct_SetDutyCycle(sct_number, speedMap[s]);
    }
}

/*==================[end of file]============================================*/
