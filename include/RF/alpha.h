/**
 * @file      alpha.h
 *
 * @author    Zéro Cool
 * @date      12/09/2013 13:34:33
 * @brief     Driver pour les modules RF alpha
 *
 * @details   Fichier Driver permettant de gérer les modules RF alpha
 *
 * @warning   API non finalisée non opérationnelle.
 *
 * @todo Faire fonctionner l'API
 *
 * @ingroup   RFM
 */

#ifndef _ALPHA_H_
#define _ALPHA_H_

void ALPHA_TxInit(void);
void ALPHA_RxInit(void);
void ALPHA_SendData(uint8_t data);

#include <RF/alpha_core.h>

#endif /* _ALPHA_H_ */
