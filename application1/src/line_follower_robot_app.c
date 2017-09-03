

/*==================[inclusions]=============================================*/

//#include <stdbool.h>

#include "os.h"

#include "board_pinout.h"
#include "bsp.h"
#include "bsp_keyboard.h"
#include "line_tracker_sensor.h"
#include "driver.h"

/*==================[macros and definitions]=================================*/

#define LEFT_SENSOR GPIO1
#define RIGHT_SENSOR GPIO6
#define LEFT_MOTOR GPIO2
#define RIGHT_MOTOR GPIO8

typedef enum {STARTED, STOPPED} state_status_type;

typedef struct
{
    state_status_type status;
} state_type;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

// Initial state
state_type appState = {
        STOPPED
};

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

int main(void)
{
    /* Starts the operating system in the Application Mode 1 */
    /* This example has only one Application Mode */
    StartOS(AppMode1);

    /* StartOs shall never returns, but to avoid compiler warnings or errors
    * 0 is returned */
    return 0;
}

/** \brief Error Hook function
 *
 * This fucntion is called from the os if an os interface (API) returns an
 * error. Is for debugging proposes. If called this function triggers a
 * ShutdownOs which ends in a while(1).
 *
 * The values:
 *    OSErrorGetServiceId
 *    OSErrorGetParam1
 *    OSErrorGetParam2
 *    OSErrorGetParam3
 *    OSErrorGetRet
 *
 * will provide you the interface, the input parameters and the returned value.
 * For more details see the OSEK specification:
 * http://portal.osek-vdx.org/files/pdf/specs/os223.pdf
 *
 */
void ErrorHook(void)
{
   ShutdownOS(0);
}

/** \brief Initial task
 *
 * This task is started automatically in the application mode 1.
 */
TASK(InitTask)
{
    bsp_init();
    bsp_keyboardInit();
    LineTrackerSensor_Init(LEFT_SENSOR);
    LineTrackerSensor_Init(RIGHT_SENSOR);
    TerminateTask();
}

TASK(KeyboardTask) {
    bsp_keyboard_task();
    TerminateTask();
}

TASK(CheckSwitchTask)
{

    board_switchId_enum tec = bsp_keyboardGet();
    if (tec == BOARD_TEC_ID_1 && appState.status == STOPPED) {
        appState.status = STARTED;
        Driver_Start(LEFT_MOTOR, RIGHT_MOTOR);
    } else if (tec == BOARD_TEC_ID_1 && appState.status == STARTED) {
        appState.status = STOPPED;
        Driver_Stop(LEFT_MOTOR, RIGHT_MOTOR);
    }
    TerminateTask();
}

TASK(CheckLTSensorTask) {

    bool touched;

    touched = LineTrackerSensor_LineTouched(LEFT_SENSOR);
    if (touched) {
        Driver_TurnLeft(LEFT_MOTOR, RIGHT_MOTOR);
        bsp_ledAction(BOARD_LED_ID_1, BOARD_LED_STATE_ON);
    } else {
        touched = LineTrackerSensor_LineTouched(RIGHT_SENSOR);
        if (touched) {
            Driver_TurnRight(LEFT_MOTOR, RIGHT_MOTOR);
            bsp_ledAction(BOARD_LED_ID_2, BOARD_LED_STATE_ON);
        } else {
            if (appState.status == STARTED) {
                Driver_GoStraightOn(LEFT_MOTOR, RIGHT_MOTOR);
            }
            bsp_ledAction(BOARD_LED_ID_1, BOARD_LED_STATE_OFF);
            bsp_ledAction(BOARD_LED_ID_2, BOARD_LED_STATE_OFF);
        }
    }

    TerminateTask();
}

/*==================[end of file]============================================*/

