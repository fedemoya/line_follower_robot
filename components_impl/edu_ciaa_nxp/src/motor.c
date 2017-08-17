/*==================[inclusions]=============================================*/

#include "motor.h"

#include "board_pinout.h"
#include "mcu_sct.h"

/*==================[macros and definitions]=================================*/

#define SCTPWM_FREC 1000 /* 1Khz */

#define SPEED_STEP 85


/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external functions definition]==========================*/

void Motor_Start(uint8_t pin) {

    Sct_Init(SCTPWM_FREC);

    uint8_t sct_number = Board_Pin2Sct(pin);

    if (sct_number >= 0) {
        Sct_EnablePwmFor(sct_number);
        Sct_SetDutyCycle(sct_number, SPEED_STEP);
    }
}

void Motor_IncreaseSpeed(uint8_t pin) {

    uint8_t speed = Sct_GetDutyCycle(Board_Pin2Sct(pin));

    if (speed <= 255 - SPEED_STEP) {
        speed += SPEED_STEP;
    }

    uint8_t sct_number = Board_Pin2Sct(pin);

    if (sct_number >= 0)
        Sct_SetDutyCycle(sct_number, speed);
}

void Motor_DecreaseSpeed(uint8_t pin) {

    uint8_t speed = Sct_GetDutyCycle(Board_Pin2Sct(pin));

    if (speed >= 0 + SPEED_STEP) {
        speed -= SPEED_STEP;
    }

    uint8_t sct_number = Board_Pin2Sct(pin);

    if (sct_number >= 0)
        Sct_SetDutyCycle(sct_number, speed);
}

void Motor_Stop(uint8_t pin) {

    uint8_t sct_number = Board_Pin2Sct(pin);

    if (sct_number >= 0)
        Sct_SetDutyCycle(sct_number, 0);
}

/*==================[end of file]============================================*/
