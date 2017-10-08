

/*==================[inclusions]=============================================*/

//#include <stdbool.h>

#include "stdint.h"
#include "string.h"

#include "ciaaPOSIX_stdlib.h"
#include "os.h"

#include "mcu_uart.h"

#include "board_pinout.h"
#include "bsp.h"
#include "bsp_keyboard.h"
#include "line_tracker_sensor.h"
#include "driver.h"

/*==================[macros and definitions]=================================*/

#define BAUDRATE 115200

#define LEFT_MOST_SENSOR GPIO3
#define LEFT_SENSOR GPIO4
#define CENTER_SENSOR GPIO5
#define RIGHT_SENSOR GPIO6
#define RIGHT_MOST_SENSOR GPIO7

#define LEFT_MOTOR GPIO2
#define RIGHT_MOTOR GPIO8

typedef enum {STARTED, STOPPED} state_status_type;

typedef struct
{
    uint8_t lmost_sensor_touched_count;
    uint8_t left_sensor_touched_count;
    uint8_t center_sensor_touched_count;
    uint8_t right_sensor_touched_count;
    uint8_t rmost_sensor_touched_count;
    state_status_type status;
} state_type;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

// Initial state
state_type appState = {
        0,
        0,
        0,
        0,
        0,
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

    LineTrackerSensor_Init(LEFT_MOST_SENSOR);
    LineTrackerSensor_Init(LEFT_SENSOR);
    LineTrackerSensor_Init(CENTER_SENSOR);
    LineTrackerSensor_Init(RIGHT_SENSOR);
    LineTrackerSensor_Init(RIGHT_MOST_SENSOR);

//    int *i = (int *) ciaaPOSIX_malloc(sizeof(int)*10);

    mcu_uart_init(BAUDRATE);

    TerminateTask();
}

TASK(KeyboardTask) {
//    bsp_keyboard_task();
    static char lala[] = "La reconcha de tu madre \r\n";
    mcu_uart_write(lala, strlen(lala));
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

TASK(ReadLTSensorTask) {

    if (LineTrackerSensor_LineTouched(LEFT_MOST_SENSOR)) {
        appState.lmost_sensor_touched_count++;
    }

    if (LineTrackerSensor_LineTouched(LEFT_SENSOR)) {
        appState.left_sensor_touched_count++;
    }

    if (LineTrackerSensor_LineTouched(CENTER_SENSOR)) {
        appState.center_sensor_touched_count++;
    }

    if (LineTrackerSensor_LineTouched(RIGHT_SENSOR)) {
        appState.right_sensor_touched_count++;
    }

    if (LineTrackerSensor_LineTouched(RIGHT_MOST_SENSOR)) {
        appState.rmost_sensor_touched_count++;
    }

    TerminateTask();
}

TASK(TrajectoryControlTask) {

//    uint8_t max_touched_sensor = LEFT_MOST_SENSOR;
//    unint8_t max_touched_count = appState.lmost_sensor_touched_count;
//
//    if (appState.left_sensor_touched_count > max_touched_count) {
//        max_touched_count = appState.left_sensor_touched_count;
//        max_touched_sensor = LEFT_SENSOR;
//    } else if (appState.center_sensor_touched_count > max_touched_count) {
//        max_touched_count = appState.center_sensor_touched_count;
//        max_touched_sensor = CENTER_SENSOR;
//    } else if (appState.right_sensor_touched_count > max_touched_count) {
//        max_touched_count = appState.right_sensor_touched_count;
//        max_touched_sensor = RIGHT_SENSOR;
//    } else if (appState.rmost_sensor_touched_count > max_touched_count) {
//        max_touched_count = appState.rmost_sensor_touched_count;
//        max_touched_sensor = RIGHT_MOST_SENSOR;
//    }

    static char message[100];
    memset(message, '\0', sizeof(message));

    strcpy(message, "Left most: ");
    static char lmost_count_str[4];
    itoa(appState.lmost_sensor_touched_count, lmost_count_str, 4);
    strcat(message, lmost_count_str);

    strcat(message, " - Left: ");
    static char left_count_str[4];
    itoa(appState.left_sensor_touched_count, left_count_str, 4);
    strcat(message, left_count_str);

    strcat(message, " - Center: ");
    static char center_count_str[4];
    itoa(appState.center_sensor_touched_count, center_count_str, 4);
    strcat(message, center_count_str);

    strcat(message, " - Right: ");
    static char right_count_str[4];
    itoa(appState.right_sensor_touched_count, right_count_str, 4);
    strcat(message, right_count_str);

    strcat(message, " - Right most: ");
    static char rmost_count_str[4];
    itoa(appState.rmost_sensor_touched_count, rmost_count_str, 4);
    strcat(message, rmost_count_str);

    strcat(message, "\r\n");

    mcu_uart_write(message, strlen(message));

    appState.lmost_sensor_touched_count = 0;
    appState.left_sensor_touched_count = 0;
    appState.center_sensor_touched_count = 0;
    appState.right_sensor_touched_count = 0;
    appState.rmost_sensor_touched_count = 0;

    TerminateTask();
}

/*==================[end of file]============================================*/

