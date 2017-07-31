/* Copyright 2017, Gustavo Muro
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

/** \brief PASE APP EXAMPLE
 **
 ** ejemplo de aplicación usando CIAA Firmware
 **
 **/

/** \addtogroup
 ** @{ */
/** \addtogroup
 ** @{ */
/** \addtogroup
 ** @{ */

/*==================[inclusions]=============================================*/
#include "bsp.h"
#include "bsp_keyboard.h"
#include "board.h"
#include "mcu_pwm.h"
#include "mcu_uart.h"
#include "os.h"

#include "stdint.h"
#include "string.h"


/*==================[macros and definitions]=================================*/

#define BAUDRATE 115200

#define QUEUE_SIZE 10
#define MAX_INTENSITY 256
#define INTENSITY_STEP 32

typedef enum {INC, DEC} state_direction_type;
typedef enum {RUN, PAUSED, ENDED} state_status_type;

typedef struct
{
    uint16_t currentIntensity;
    state_direction_type direction;
    state_status_type status;
    board_ledId_enum led;
} state_type;

typedef struct
{
    char *data;
    size_t size;
} message_type;

/*==================[internal data declaration]==============================*/

int timer = 0;


size_t messageQueueWriteIndex = 0;
size_t messageQueueReadIndex = 0;

message_type messageQueue[QUEUE_SIZE];

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

// Initial state
state_type appState = {
        0,
        INC,
        ENDED,
        BOARD_LED_ID_1
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

void log_message(const char *data) {

    GetResource(MessageQueueAccess);

    static char buffer[100];
    memset(buffer, '\0', sizeof(buffer));
    char ch_timer[10];
    memset(ch_timer, '\0', sizeof(ch_timer));
    itoa(timer, ch_timer, 10);
    strcpy(buffer, ch_timer);
    strcat(buffer, ": ");
    strcat(buffer, data);
    queue_sync_write(buffer, strlen(buffer));

    ReleaseResource(MessageQueueAccess);
}

/* \ messageQueue synchronized write
 *
 * This method synchronize the write operations to the queue.
 * Also synchronize between writes and reads.
 */
void queue_sync_write(char *data, size_t size) {
    size_t index;
    static char buffer[100];

    strcpy(buffer, data);
    index = messageQueueWriteIndex % QUEUE_SIZE;
    messageQueue[index].data = buffer;
    messageQueue[index].size = size;
    messageQueueWriteIndex++;
}

/* \ messageQueue synchronized read
 *
 * This method synchronize the read operations to the queue.
 * Also synchronize between writes and reads.
 */
message_type queue_sync_read() {

    size_t index;
    message_type message = {"", 0};

    GetResource(MessageQueueAccess);

    if (messageQueueReadIndex < messageQueueWriteIndex) {
        index = messageQueueReadIndex % QUEUE_SIZE;
        message = messageQueue[index];
        messageQueueReadIndex++;
    }

    ReleaseResource(MessageQueueAccess);

    return message;
}

void send_message(message_type message) {
    if (message.size > 0) {
        mcu_uart_write(message.data, message.size);
    }
}

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
    // Initialize message queue.
    int var;
    for (var = 0; var < QUEUE_SIZE; ++var) {
       message_type message = {NULL, 0};
       messageQueue[var] = message;
    }

    bsp_init();
    bsp_keyboardInit();

    mcu_uart_init(BAUDRATE);

    board_enableLedIntensity(BOARD_LED_ID_1);
    board_enableLedIntensity(BOARD_LED_ID_2);

    TerminateTask();
}

TASK(IncDecIntensityTask)
{

    if(appState.status == RUN) {

        if (appState.direction == INC && appState.currentIntensity == 0) {

            log_message("Encendiendo Led Rojo\r\n");

            appState.currentIntensity += INTENSITY_STEP;
            board_ledSetIntensity( appState.led, appState.currentIntensity-1);

        } else if (appState.direction == INC && appState.currentIntensity < MAX_INTENSITY) {

            appState.currentIntensity += INTENSITY_STEP;
            board_ledSetIntensity( appState.led, appState.currentIntensity-1);

        } else if (appState.direction == INC && appState.currentIntensity == MAX_INTENSITY) {

            appState.direction = DEC;
            appState.currentIntensity -= INTENSITY_STEP;
            board_ledSetIntensity( appState.led, appState.currentIntensity);

            if (appState.led == BOARD_LED_ID_1) {
                log_message("Intensidad maxima: Led Rojo\r\n");
            } else {
                log_message("Intensidad maxima: Led Amarillo\r\n");
            }

        } else if (appState.direction == DEC && appState.currentIntensity > 0) {

            appState.currentIntensity -= INTENSITY_STEP;
            board_ledSetIntensity( appState.led, appState.currentIntensity);

        } else if (appState.direction == DEC && appState.currentIntensity == 0) {

            appState.direction = INC;
            appState.currentIntensity += INTENSITY_STEP;

            if (appState.led == BOARD_LED_ID_1) {
                appState.led = BOARD_LED_ID_2;
                log_message("Encendiendo Led Amarillo\r\n");
            } else {
                appState.led = BOARD_LED_ID_1;
                log_message("Encendiendo Led Rojo\r\n");
            }

            board_ledSetIntensity( appState.led, appState.currentIntensity);

        }
    }

    TerminateTask();
}

TASK(CheckSwitchTask)
{

    board_switchId_enum tec = bsp_keyboardGet();

    if (tec == BOARD_TEC_ID_1 && appState.status == ENDED) {
        appState.status = RUN;
        log_message("Inicio secuencia\r\n");
    } else if (tec == BOARD_TEC_ID_1) {
        // A continuación volvemos el estado a los valores iniciales.
        appState.status = ENDED;
        // Nota en relación a la concurrencia:
        // La tarea IncDecIntensityTask tiene más alta prioridad que esta.
        // Eso implica que el planificador puede sacarle el
        // procesador a esta tarea y dárselo a IncDecIntensityTask.
        // Si ese cambio de contexto ocurre en las lineas previas a este comentario
        // no hay problema porque la tarea IncDecIntensityTask no modifica la variable
        // appSate.status.
        // Si el cambio de contexto ocurre después de este comentario, tampoco hay problema
        // porque la tarea IncDecIntensityTask no va a ejecutar el código que modifica las variables
        // compartidas debido a que el status es "ENDED".
        // Por lo tanto no necesitamos aplicar ningún mecanismo de exclusión mutua o sincronización.
        board_ledSetIntensity(BOARD_LED_ID_1, 0);
        board_ledSetIntensity(BOARD_LED_ID_2, 0);
        appState.currentIntensity = 0;
        appState.direction = INC;
        appState.led = BOARD_LED_ID_1;

        log_message("Secuencia finalizada\r\n");
    }

    if (tec == BOARD_TEC_ID_2  && appState.status == RUN) {
        appState.status = PAUSED;

        log_message("Secuencia pausada\r\n");
    } else if (tec == BOARD_TEC_ID_2 && appState.status == PAUSED) {
        appState.status = RUN;

        log_message("Secuencia reanudada\r\n");
    }

    TerminateTask();
}

TASK(LogMessagesTask) {

    message_type message = queue_sync_read();
    send_message(message);

    TerminateTask();
}

TASK(KeyboardTask) {
    bsp_keyboard_task();
    TerminateTask();
}

ALARMCALLBACK(CallBackIncreaseTimer)
{
    timer++;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

