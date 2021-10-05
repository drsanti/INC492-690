/************************************************************
 * File:    esp.h                                           *
 * Author:  Dr.Santi Nuratch                                *
 *          Embedded Computing and Control Laboratory       *
 * Update:  05 July 2017, 12.36 PM                          *
 ************************************************************/
 
#ifndef __ESP_H__
#define __ESP_H__

#include "at.h"


#define ESP_STATE_NOTFOUND  0
#define ESP_STATE_DETECTED  1

typedef struct 
{
    timer_t *pTimer;
    uint8_t state;
    os_callback_t callback;
}esp_module_t;

void ESP_Init( os_callback_t callback );
void ESP_Service( void *evt );

#endif //!! __ESP_H__