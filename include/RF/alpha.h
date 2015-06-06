/*
 * alpha.h
 *
 * Created: 12/09/2013 13:34:33
 *  Author: Zéro Cool
 */ 

#ifndef _ALPHA_H_
#define _ALPHA_H_

/****************************************************************
  Fonctions disponibles
 ****************************************************************/

void ALPHA_TxInit(void);
void ALPHA_RxInit(void);

void ALPHA_SendData(uint8_t data);


/****************************************************************
  Implémentations
 ****************************************************************/

#include <RF/alpha_core.h>

#endif /* _ALPHA_H_ */