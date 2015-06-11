/**
 * @file      I2C_master.h
 *
 * @author    Zéro Cool
 * @date      09/06/2015 21:33:29
 * @brief     Protocole de communication I2C vision maitre
 *
 * @details   Fichier définissant les routines du protocole de communication
 *            I2C ou TWI. Cette implémentation se limite à un seul bus I2C.
 *            Cette implémentation fonctionne pour tous les AVR disposant nativement
 *            d'une interface hardware I2C.
 *
 * @note      Des résistances pull-ups de 5.6k Ohms sont à utiliser sur les lignes SDA et SCL.
 *
 * @todo      Retrouver la règle de calcul de la valeur des résistances pull-ups
 *
 * Exemple de code :
 * @code
 * #define SCL_CLOCK               100000L
 * #define LM75_ADDR               0b10010000
 * #include <I2C_master.h>
 *
 * int main(void)
 * {
 *   I2C_Initialize();
 *
 *   // Envoi d'un byte au périphérique I2C (Démarrage en écriture pour envoyer l'adresse à lire)
 *   I2C_StartWait(LM75_ADDR + TW_WRITE);
 *   if (I2C_Send(0x00) != 0)
 *   {
 *     // Erreur d'envoi du registre à lire
 *   }
 *   I2C_Stop();
 *
 *   // Cette fois-ci activation du périphérique en lecture pour lire le registre
 *   I2C_StartWait(LM75_ADDR + TW_READ);
 *   uint8_t I2C_buffer[2];
 *   I2C_buffer[0] = I2C_ReadAck();
 *   I2C_buffer[1] = I2C_ReadNak();
 *   I2C_Stop();
 *
 *   while(1)
 *   {
 *   }
 * }
 * @endcode
 *
 * @ingroup   PROTOCOLES
 */

#ifndef _I2C_MASTER_H_
#define _I2C_MASTER_H_

#ifndef F_CPU
#  error "I2C_master.h requires F_CPU to be defined"
#endif

#ifndef SCL_CLOCK
#  error "I2C_master.h requires SCL_CLOCK to be defined"
#endif

#if !(F_CPU)
#  error "F_CPU must be a constant value"
#endif

#if !(SCL_CLOCK)
#  error "F_CPU must be a constant value"
#endif

#include <stdint.h>
#include <avr/io.h>

/**
 * @brief       Initialise l'interface I2C en mode maitre (master)
 *
 * @note        Cette méthode n'est à appeler qu'une seule fois
 */
void I2C_Initialize(void);

/**
 * @brief       Termine le transfert des données et libère le buss I2C
 */
void I2C_Stop(void);


/**
 * @brief       Déclanche le démarrage de la transmission
 * @details     Démarrer la transmission des données vers le périphérique dont l'adresse
 *              est fournie en paramètre.
 *
 * @param       [in]      address      Adresse du périphérique avec son mode de transmission (TW_READ ou TW_WRITE)
 *
 * @return      Valeur indiquant si le périphérique est accessible ou non
 *
 * @retval      0x00   Périphérique accessible (pas d'erreur)
 * @retval      0x01   Périphérique innaccessible (Erreur rencontrée)
 *
 * @warning     La direction des données est à fournir avec l'adresse.
 *
 * Exemple :
 * @code
 * // Adresse du périphérique tel que spécifié dans sa datasheet
 * #define PERIPH_ADDR    0b01101010
 *
 * if (I2C_Start(PERIPH_ADDR + TW_READ) != 0)
 * {
 *     // Erreur d'accès au périphérique
 * }
 * @endcode
 */
uint8_t I2C_Start(const uint8_t address);


/**
 * @brief       Déclanche le démarrage répété de la transmission
 * @details     Envoi un démarrage répété de la transmission des données vers le périphérique dont l'adresse
 *              est fournie en paramètre.
 *
 * @param       [in]      address      Adresse du périphérique avec son mode de transmission (TW_READ ou TW_WRITE)
 *
 * @return      Valeur indiquant si le périphérique est accessible ou non
 *
 * @retval      0x00   Périphérique accessible (pas d'erreur)
 * @retval      0x01   Périphérique innaccessible (Erreur rencontrée)
 *
 * @warning     La direction des données est à fournir avec l'adresse.
 *
 * Exemple :
 * @code
 * // Adresse du périphérique tel que spécifié dans sa datasheet
 * #define PERIPH_ADDR    0b01101010
 *
 * if (I2C_RepeatedStart(PERIPH_ADDR + TW_READ) != 0)
 * {
 *     // Erreur d'accès au périphérique
 * }
 * @endcode
 */
uint8_t I2C_RepeatedStart(const uint8_t address);


/**
 * @brief       Déclanche le démarrage de la transmission
 * @details     Démarre une transmission et attend que le pérophérique soit prêt.
 *              Si le périphérique est occupé, la fonction effectue du polling jusqu'à
 *              ce qu'un ack soit reçu indiquant que le périphérique est disponible.

 * @param       [in]      address      Adresse du périphérique avec son mode de transmission (TW_READ ou TW_WRITE)
 *
 * @warning     La direction des données est à fournir avec l'adresse.
 *
 * Exemple :
 * @code
 * // Adresse du périphérique tel que spécifié dans sa datasheet
 * #define PERIPH_ADDR    0b01101010
 *
 * I2C_StartWait(PERIPH_ADDR + TW_READ);
 * @endcode
 */
void I2C_StartWait(const uint8_t address);


/**
 * @brief       Envoi un byte au périphérique I2C
 *
 * @param       [in]    data     byte à transmettre
 *
 * @return      Valeur indiquant si l'envoi a réussi ou non
 *
 * @retval      0x00   Envoi réussi (pas d'erreur)
 * @retval      0x01   Echec de l'envoi (Erreur rencontrée)
 *
 * Exemple :
 * @code
 * if (I2C_Send(0x55) != 0)
 * {
 *     // Erreur d'envoi du byte 0x55 au périphérique
 * }
 * @endcode
 */
uint8_t I2C_Send(const uint8_t data);


/**
 * @brief       Lit un byte du périphérique I2C et demande la transmission des données suivantes
 *
 * @return      Byte lut du périphérique
 *
 * Exemple :
 * @code
 * uint8_t byte = I2C_ReadAck();
 * @endcode
 */
uint8_t I2C_ReadAck(void);

/**
 * @brief       Lit un byte du périphérique I2C. La lecture est suivie par l'envoi d'une condition STOP.
 *
 * @return      Byte lut du périphérique
 *
 * Exemple :
 * @code
 * uint8_t byte = I2C_ReadNak();
 * @endcode
 */
uint8_t I2C_ReadNak(void);

#include <I2C_master_core.h>

#endif /* _I2C_MASTER_H_ */
