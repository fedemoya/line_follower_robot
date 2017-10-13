/*==================[inclusions]=============================================*/

#include <stdint.h>

/*==================[macros and definitions]=================================*/

typedef enum {
    SPEED_0 = 0,
    SPEED_1,
    SPEED_2,
    SPEED_3,
    SPEED_4,
    SPEED_5,
    SPEED_6,
    SPEED_7,
    SPEED_8,
    SPEED_9,
    SPEED_10,
    SPEED_11,
    SPEED_12,
    SPEED_13,
    SPEED_14,
    SPEED_15,
    SPEED_16,
    SPEED_17,
    SPEED_18,
    SPEED_19,
    SPEED_20,
    SPEED_21,
    SPEED_22,
    SPEED_23,
    SPEED_24,
    SPEED_25,
    SPEED_26,
    SPEED_27,
    SPEED_28,
    SPEED_29,
    SPEED_30,
    SPEED_31,
    SPEED_32
} MotorSpeed;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

void Motor_Init(uint8_t pin);
void Motor_SetSpeed(MotorSpeed s, uint8_t pin);

/*==================[end of file]============================================*/

