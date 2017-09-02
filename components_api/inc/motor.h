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
    SPEED_8
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

