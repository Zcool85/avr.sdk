/*
 * @file      I2C_master_core.h
 *
 * @author    Zéro Cool
 * @date      09/06/2015 21:33:29
 * @brief     Core du protocole de communication I2C vision maitre
 *
 * @details   Fichier core définissant les routines du protocole de communication
 *            I2C ou TWI. Cette implémentation se limite à un seul bus I2C.
 *            Cette implémentation fonctionne pour tous les AVR disposant nativement
 *            d'une interface hardware I2C.
 *
 * @ingroup   PROTOCOLES
 */

#ifndef _I2C_MASTER_CORE_H_
#define _I2C_MASTER_CORE_H_

#include <util/twi.h>

void I2C_Initialize(void)
{
  TWSR = 0;                          // pas de préscaler
  TWBR = ((F_CPU/SCL_CLOCK)-16)/2;   // Doit être supérieur à 10 pour que les opérations soient stables
	TWDR = 0xFF;                       // SDA released
	TWCR = (0<<TWINT)|                 // Disable Interupt
         (0<<TWEA)|                  // Désactivation du bit d'aquittement
         (0<<TWSTA)|                 // Désactivation du bit de condition start
         (0<<TWSTO)|                 // Désactivation du bit de condition stop
	       (0<<TWWC)|                  // Désactivation du drapeau de collision
	       (1<<TWEN)|                  // Activation de l'interface TWI
	       (0<<TWIE);                  // Désactivation des interruptions TWI
}

void I2C_Stop(void)
{
  // Envoi d'une condition STOP
  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

  // Attente jusqu'à ce que la condition STOP soit transmise et que le bus soit relaché
  while (TWCR & (1<<TWSTO));
}

uint8_t I2C_Start(const uint8_t address)
{
  // Envoi de la condition START
  TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

  // Attente de la fin de transmission
  while (!(TWCR & (1<<TWINT)));

  // Vérification du registre de status TWI
  if (   (TW_STATUS != TW_START)
      && (TW_STATUS != TW_REP_START) )
  {
    return 1;
  }

  // Envoi de l'adresse du périphérique
  TWDR = address;
  TWCR = (1<<TWINT) | (1<<TWEN);

  // Attente de la réception d'un ACK ou NACK
  while(!(TWCR & (1<<TWINT)));

  // Vérification du registre de status TWI
  if (   (TW_STATUS != TW_MT_SLA_ACK)
      && (TW_STATUS != TW_MR_SLA_ACK) )
  {
    return 1;
  }

  return 0;
}

uint8_t I2C_RepeatedStart(const uint8_t address)
{
  return I2C_Start(address);
}

void I2C_StartWait(const uint8_t address)
{
  while (1)
  {
    // Envoi de la condition START
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

    // Attente de la fin de transmission
  	while (!(TWCR & (1<<TWINT)));

    // Vérification du registre de status TWI
  	if (  (TW_STATUS != TW_START)
       && (TW_STATUS != TW_REP_START) )
    {
      continue;
    }

    // Envoi de l'adresse du périphérique
  	TWDR = address;
  	TWCR = (1<<TWINT) | (1<<TWEN);

    // Attente de la fin de transmission
  	while (!(TWCR & (1<<TWINT)));

    // Vérification du registre de status TWI
  	if (   (TW_STATUS == TW_MT_SLA_NACK)
        || (TW_STATUS == TW_MR_DATA_NACK) )
  	{
      I2C_Stop();

  	  continue;
  	}

  	break;
  }
}

uint8_t I2C_Send(const uint8_t data)
{
	// Envoi des données
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// Attente de la fin de transmission
	while (!(TWCR & (1<<TWINT)));

  // Vérification du registre de status TWI
	if (TW_STATUS != TW_MT_DATA_ACK)
  {
    return 1;
  }

	return 0;
}

uint8_t I2C_ReadAck(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);

  // Attente de la fin de transmission
	while (!(TWCR & (1<<TWINT)));

  return TWDR;
}

uint8_t I2C_ReadNak(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);

  // Attente de la fin de transmission
	while (!(TWCR & (1<<TWINT)));

  return TWDR;
}

#endif /* _I2C_MASTER_CORE_H_ */
