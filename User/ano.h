#ifndef __ANO_H__
#define __ANO_H__

#include "stm32f4xx.h"
#include "usart.h"

void FANO_Send_Data(int8_t *data);
void FANO_Send_MAG(int8_t *data);
void FANO_Send_ACC_GRY(int8_t *data);

#endif
