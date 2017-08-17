
// EDU-CIAA-NXP Asignación de Pines
// http://www.proyecto-ciaa.com.ar/devwiki/lib/exe/fetch.php?media=edu-ciaa-nxp:edu-ciaa-nxp_pinout_a4_v3r2_es.pdf

#include <stdint.h>

#define LEDR 75
#define LEDG 81
#define LEDB 84
#define LED1 104
#define LED2 105
#define LED3 106
#define TEC_1 38
#define TEC_2 42
#define TEC_3 43
#define TEC_4 49
#define GPIO0 74
#define GPIO1 80
#define GPIO2 82
#define GPIO3 85
// TODO: Add the missing pins.

int8_t Board_Pin2Sct(uint8_t);
