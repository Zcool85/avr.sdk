/**
 * @file      SPI_master.h
 *
 * @author    Zéro Cool
 * @date      10/06/2015 20:50:15
 * @brief     Protocole de communication SPI maitre
 *
 * @details   Fichier définissant les routines du protocole de communication
 *            SPI. Cette implémentation fonctionne pour tous les AVR disposant nativement
 *            d'une interface hardware SPI.
 *
 * @warning   Toutes les broches utilisées pour le SPI (hardware ou software) doivent
 *            être sur le même PORT.
 *
 * @warning   Lors de l'utilisation du mode hardware, la configuration des SPI_* doit
 *            correspondre aux broches physiques utilisées par le hardware du microcontrolleur.
 *
 * @note      La définition de SPI_PIN n'est à faire qu'en mode software.
 *
 * Exemple de code en mode hardware :
 * @code
 * #include <avr/io.h>
 * #include <stdint.h>
 *
 * #define SPI_DDR                 DDRB
 * #define SPI_PORT                PORTB
 * #define SPI_MOSI_PIN            PINB1
 * #define SPI_MISO_PIN            PINB7
 * #define SPI_SCK_PIN             PINB6
 * #define SPI_SS_PIN              PINB2
 *
 * #include <SPI_master.h>
 *
 * int main(void)
 * {
 *    // Initilisation du SPI
 *    SPI_Initialize();
 *
 *    uint8_t receved = 0x00;
 *
 *    // Activation de l'esclave
 *    SPI_EnableSlave();
 *
 *    // Transmission des données (ici 2 octets)
 *    uint8_t receved1 = SPI_SendByte(0x12);
 *    uint8_t receved2 = SPI_SendByte(0x34);
 *
 *    SPI_DisableSlave();
 *
 *    while (1)
 *    {
 *    }
 * }
 * @endcode
 *
 * Exemple de code en mode software :
 * @code
 * #include <avr/io.h>
 * #include <stdint.h>
 *
 * #define SPI_DDR                 DDRB
 * #define SPI_PORT                PORTB
 * #define SPI_PIN                 PINB
 * #define SPI_MOSI_PIN            PINB1
 * #define SPI_MISO_PIN            PINB7
 * #define SPI_SCK_PIN             PINB6
 * #define SPI_SS_PIN              PINB2
 *
 * #define SPI_SOFTWARE
 *
 * #include <SPI_master.h>
 *
 * int main(void)
 * {
 *    // Initilisation du SPI
 *    SPI_Initialize();
 *
 *    uint8_t receved = 0x00;
 *
 *    // Activation de l'esclave
 *    SPI_EnableSlave();
 *
 *    // Transmission des données (ici 2 octets)
 *    uint8_t receved1 = SPI_SendByte(0x12);
 *    uint8_t receved2 = SPI_SendByte(0x34);
 *
 *    SPI_DisableSlave();
 *
 *    while (1)
 *    {
 *    }
 * }
 * @endcode
 *
 * @ingroup   PROTOCOLES
 */

#ifndef _SPI_MASTER_H_
#define _SPI_MASTER_H_

#if !defined(SPI_DDR)
#  error SPI_DDR non définie
#endif

#if !defined(SPI_PORT)
#  error SPI_DDR non définie
#endif

#if !defined(SPI_MOSI_PIN)
#  error SPI_DDR non définie
#endif

#if !defined(SPI_MISO_PIN)
#  error SPI_DDR non définie
#endif

#if !defined(SPI_SCK_PIN)
#  error SPI_DDR non définie
#endif

#if !defined(SPI_SS_PIN)
#  error SPI_DDR non définie
#endif

#if defined(SPI_SOFTWARE)
#  if !defined(SPI_PIN)
#    error SPI_PIN doit être déclaré lorsque le mode software est activé
#  endif
#endif

#include <stdint.h>

/**
 * @brief       Initialise l'interface SPI
 *
 * @note        Cette méthode n'est à appeler qu'une seule fois
 */
void SPI_Initialize(void);

/**
 * @brief       Transmet un byte sur la liaison SPI
 *
 * @param       [in]     byte       Octet à transmettre
 *
 * @return      octet reçu après transmission
 */
uint8_t SPI_SendByte(uint8_t byte);

/**
 * @brief       Active l'esclave SPI
 *
 * @note        Cette macro doit être utilisée avant l'envoie des données à l'esclave
 */
#define SPI_EnableSlave()       (SPI_PORT &= ~_BV(SPI_SS_PIN))  // SS to low

/**
 * @brief       Désactive l'esclave SPI
 *
 * @note        Cette macro doit être utilisée une fois toutes les données transmise à l'esclave
 */
#define SPI_DisableSlave()      (SPI_PORT |=  _BV(SPI_SS_PIN))  // SS to high

#include <SPI_master_core.h>

#endif /* _SPI_MASTER_H_ */
