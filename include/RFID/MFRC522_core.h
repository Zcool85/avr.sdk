/*
 * @file      MFRC522_core.h
 *
 * @author    Zéro Cool
 * @date      15/06/2015 18:13:32
 * @brief     Coeur de la bibliothèque de contrôle du lecteur MFRC522
 *
 * @details   Coeur de la bibliothèque d'utilisation du reader RFID MFRC522 en mode SPI.
 *
 * @todo      Revoir les commentaires des enum
 *
 * @ingroup   RFID
 */

#ifndef _MFRC522_CORE_H_
#define _MFRC522_CORE_H_

#include <avr/pgmspace.h>
#include <util/delay.h>
#include <SPI_master.h>

/**
 * @brief     Registres sur MFRC522
 * @details   Enumération des registres disponibles du MFRC522 (Cf. Chapitre 9 de la datasheet)
 */
typedef enum
{
  // Page 0 : Commandes et statut
  //                          0x00    /**< reserved for future use */
  PCD_REG_CommandReg        = 0x01,   /**< starts and stops command execution */
  PCD_REG_ComIEnReg         = 0x02,   /**< enable and disable interrupt request control bits */
  PCD_REG_DivIEnReg         = 0x03,   /**< enable and disable interrupt request control bits */
  PCD_REG_ComIrqReg         = 0x04,   /**< interrupt request bits */
  PCD_REG_DivIrqReg         = 0x05,   /**< interrupt request bits */
  PCD_REG_ErrorReg          = 0x06,   /**< error bits showing the error status of the last command executed */
  PCD_REG_Status1Reg        = 0x07,   /**< communication status bits */
  PCD_REG_Status2Reg        = 0x08,   /**< receiver and transmitter status bits */
  PCD_REG_FIFODataReg       = 0x09,   /**< input and output of 64 byte FIFO buffer */
  PCD_REG_FIFOLevelReg      = 0x0A,   /**< number of bytes stored in the FIFO buffer */
  PCD_REG_WaterLevelReg     = 0x0B,   /**< level for FIFO underflow and overflow warning */
  PCD_REG_ControlReg        = 0x0C,   /**< miscellaneous control registers */
  PCD_REG_BitFramingReg     = 0x0D,   /**< adjustments for bit-oriented frames */
  PCD_REG_CollReg           = 0x0E,   /**< bit position of the first bit-collision detected on the RF interface */
  //                          0x0F    /**< reserved for future use */

  // Page 1 : Commandes
  //                          0x10    /**< reserved for future use */
  PCD_REG_ModeReg           = 0x11,   /**< defines general modes for transmitting and receiving */
  PCD_REG_TxModeReg         = 0x12,   /**< defines transmission data rate and framing */
  PCD_REG_RxModeReg         = 0x13,   /**< defines reception data rate and framing */
  PCD_REG_TxControlReg      = 0x14,   /**< controls the logical behavior of the antenna driver pins TX1 and TX2 */
  PCD_REG_TxASKReg          = 0x15,   /**< controls the setting of the transmission modulation */
  PCD_REG_TxSelReg          = 0x16,   /**< selects the internal sources for the antenna driver */
  PCD_REG_RxSelReg          = 0x17,   /**< selects internal receiver settings */
  PCD_REG_RxThresholdReg    = 0x18,   /**< selects thresholds for the bit decoder */
  PCD_REG_DemodReg          = 0x19,   /**< defines demodulator settings */
  //                          0x1A    /**< reserved for future use */
  //                          0x1B    /**< reserved for future use */
  PCD_REG_MfTxReg           = 0x1C,   /**< controls some MIFARE communication transmit parameters */
  PCD_REG_MfRxReg           = 0x1D,   /**< controls some MIFARE communication receive parameters */
  //                          0x1E    /**< reserved for future use */
  PCD_REG_SerialSpeedReg    = 0x1F,   /**< selects the speed of the serial UART interface */

  // Page 2 : Configuration
  //                          0x20    /**< reserved for future use */
  PCD_REG_CRCResultRegH			= 0x21,   /**< shows the MSB and LSB values of the CRC calculation */
  PCD_REG_CRCResultRegL			= 0x22,
  //                          0x23    /**< reserved for future use */
  PCD_REG_ModWidthReg				= 0x24,   /**< controls the ModWidth setting? */
  //                          0x25    /**< reserved for future use */
  PCD_REG_RFCfgReg          = 0x26,   /**< configures the receiver gain */
  PCD_REG_GsNReg            = 0x27,   /**< selects the conductance of the antenna driver pins TX1 and TX2 for modulation */
  PCD_REG_CWGsPReg          = 0x28,   /**< defines the conductance of the p-driver output during periods of no modulation */
  PCD_REG_ModGsPReg         = 0x29,   /**< defines the conductance of the p-driver output during periods of modulation */
  PCD_REG_TModeReg          = 0x2A,   /**< defines settings for the internal timer */
  PCD_REG_TPrescalerReg     = 0x2B,   /**< the lower 8 bits of the TPrescaler value. The 4 high bits are in TModeReg. */
  PCD_REG_TReloadRegH       = 0x2C,   /**< defines the 16-bit timer reload value */
  PCD_REG_TReloadRegL       = 0x2D,
  PCD_REG_TCounterValueRegH = 0x2E,   /**< shows the 16-bit timer value */
  PCD_REG_TCounterValueRegL = 0x2F,

  // Page 3 : Registres de tests
  //                          0x30    /**< reserved for future use */
  PCD_REG_TestSel1Reg       = 0x31,   /**< general test signal configuration */
  PCD_REG_TestSel2Reg       = 0x32,   /**< general test signal configuration */
  PCD_REG_TestPinEnReg      = 0x33,   /**< enables pin output driver on pins D1 to D7 */
  PCD_REG_TestPinValueReg   = 0x34,   /**< defines the values for D1 to D7 when it is used as an I/O bus */
  PCD_REG_TestBusReg        = 0x35,   /**< shows the status of the internal test bus */
  PCD_REG_AutoTestReg       = 0x36,   /**< controls the digital self test */
  PCD_REG_VersionReg        = 0x37,   /**< shows the software version */
  PCD_REG_AnalogTestReg     = 0x38,   /**< controls the pins AUX1 and AUX2 */
  PCD_REG_TestDAC1Reg       = 0x39,   /**< defines the test value for TestDAC1 */
  PCD_REG_TestDAC2Reg       = 0x3A,   /**< defines the test value for TestDAC2 */
  PCD_REG_TestADCReg        = 0x3B    /**< shows the value of ADC I and Q channels */
  //                          0x3C    /**< reserved for production tests */
  //                          0x3D    /**< reserved for production tests */
  //                          0x3E    /**< reserved for production tests */
  //                          0x3F    /**< reserved for production tests */
} PCD_REG;

/**
 * @brief     Commandes du MFRC522
 * @details   Enumération des commandes du MFRC522 (Cf. Chapitre 10 de la datasheet)
 */
typedef enum
{
  PCD_CMD_Idle              = 0x00,   /**< no action, cancels current command execution */
  PCD_CMD_Mem               = 0x01,   /**< stores 25 bytes into the internal buffer */
  PCD_CMD_GenerateRandomID  = 0x02,   /**< generates a 10-byte random ID number */
  PCD_CMD_CalcCRC           = 0x03,   /**< activates the CRC coprocessor or performs a self test */
  PCD_CMD_Transmit          = 0x04,   /**< transmits data from the FIFO buffer */
  PCD_CMD_NoCmdChange       = 0x07,   /**< no command change, can be used to modify the CommandReg register bits without affecting the command, for example, the PowerDown bit */
  PCD_CMD_Receive           = 0x08,   /**< activates the receiver circuits */
  PCD_CMD_Transceive        = 0x0C,   /**< transmits data from FIFO buffer to antenna and automatically activates the receiver after transmission */
  PCD_CMD_MFAuthent         = 0x0E,   /**< performs the MIFARE standard authentication as a reader */
  PCD_CMD_SoftReset         = 0x0F    /**< resets the MFRC522 */
} PCD_CMD;

/**
 * @brief     Commandes envoyé à un PICC
 * @details   Enumération des commandes possibles d'envoyer à un PICC
 */
typedef enum
{
  // The commands used by the PCD to manage communication with several PICCs (ISO 14443-3, Type A, section 6.4)
  PICC_CMD_REQA             = 0x26,   /**< REQuest command, Type A. Invites PICCs in state IDLE to go to READY and prepare for anticollision or selection. 7 bit frame. */
  PICC_CMD_WUPA             = 0x52,   /**< Wake-UP command, Type A. Invites PICCs in state IDLE and HALT to go to READY(*) and prepare for anticollision or selection. 7 bit frame. */
  PICC_CMD_CT               = 0x88,   /**< Cascade Tag. Not really a command, but used during anti collision. */
  PICC_CMD_SEL_CL1          = 0x93,   /**< Anti collision/Select, Cascade Level 1 */
  PICC_CMD_SEL_CL2          = 0x95,   /**< Anti collision/Select, Cascade Level 2 */
  PICC_CMD_SEL_CL3          = 0x97,   /**< Anti collision/Select, Cascade Level 3 */
  PICC_CMD_HLTA             = 0x50,   /**< HaLT command, Type A. Instructs an ACTIVE PICC to go to state HALT. */
  // The commands used for MIFARE Classic (from http://www.nxp.com/documents/data_sheet/MF1S503x.pdf, Section 9)
  // Use PCD_MFAuthent to authenticate access to a sector, then use these commands to read/write/modify the blocks on the sector.
  // The read/write commands can also be used for MIFARE Ultralight.
  PICC_CMD_MF_AUTH_KEY_A    = 0x60,   /**< Perform authentication with Key A */
  PICC_CMD_MF_AUTH_KEY_B    = 0x61,   /**< Perform authentication with Key B */
  PICC_CMD_MF_READ          = 0x30,   /**< Reads one 16 byte block from the authenticated sector of the PICC. Also used for MIFARE Ultralight. */
  PICC_CMD_MF_WRITE         = 0xA0,   /**< Writes one 16 byte block to the authenticated sector of the PICC. Called "COMPATIBILITY WRITE" for MIFARE Ultralight. */
  PICC_CMD_MF_DECREMENT     = 0xC0,   /**< Decrements the contents of a block and stores the result in the internal data register. */
  PICC_CMD_MF_INCREMENT     = 0xC1,   /**< Increments the contents of a block and stores the result in the internal data register. */
  PICC_CMD_MF_RESTORE       = 0xC2,   /**< Reads the contents of a block into the internal data register. */
  PICC_CMD_MF_TRANSFER      = 0xB0,   /**< Writes the contents of the internal data register to a block. */
  // The commands used for MIFARE Ultralight (from http://www.nxp.com/documents/data_sheet/MF0ICU1.pdf, Section 8.6)
  // The PICC_CMD_MF_READ and PICC_CMD_MF_WRITE can also be used for MIFARE Ultralight.
  PICC_CMD_UL_WRITE         = 0xA2    /**< Writes one 4 byte page to the PICC. */
} PICC_CMD;

/**
 * @brief     Firmware de la version 1.0 du MFRC522
 * @details   Firmware de référence pour les tests du MFRC522 en version 1.0
 */
const uint8_t PROGMEM MFRC522_firmware_referenceV1_0[] = {
	0x00, 0xC6, 0x37, 0xD5, 0x32, 0xB7, 0x57, 0x5C,
	0xC2, 0xD8, 0x7C, 0x4D, 0xD9, 0x70, 0xC7, 0x73,
	0x10, 0xE6, 0xD2, 0xAA, 0x5E, 0xA1, 0x3E, 0x5A,
	0x14, 0xAF, 0x30, 0x61, 0xC9, 0x70, 0xDB, 0x2E,
	0x64, 0x22, 0x72, 0xB5, 0xBD, 0x65, 0xF4, 0xEC,
	0x22, 0xBC, 0xD3, 0x72, 0x35, 0xCD, 0xAA, 0x41,
	0x1F, 0xA7, 0xF3, 0x53, 0x14, 0xDE, 0x7E, 0x02,
	0xD9, 0x0F, 0xB5, 0x5E, 0x25, 0x1D, 0x29, 0x79
};

/**
 * @brief     Firmware de la version 2.0 du MFRC522
 * @details   Firmware de référence pour les tests du MFRC522 en version 2.0
 */
const uint8_t PROGMEM MFRC522_firmware_referenceV2_0[] = {
	0x00, 0xEB, 0x66, 0xBA, 0x57, 0xBF, 0x23, 0x95,
	0xD0, 0xE3, 0x0D, 0x3D, 0x27, 0x89, 0x5C, 0xDE,
	0x9D, 0x3B, 0xA7, 0x00, 0x21, 0x5B, 0x89, 0x82,
	0x51, 0x3A, 0xEB, 0x02, 0x0C, 0xA5, 0x00, 0x49,
	0x7C, 0x84, 0x4D, 0xB3, 0xCC, 0xD2, 0x1B, 0x81,
	0x5D, 0x48, 0x76, 0xD5, 0x71, 0x61, 0x21, 0xA9,
	0x86, 0x96, 0x83, 0x38, 0xCF, 0x9D, 0x5B, 0x6D,
	0xDC, 0x15, 0xBA, 0x3E, 0x7D, 0x95, 0x3B, 0x2F
};

/**
 * @brief     Firmware de la version Fudan Semiconductor FM17522 (clone du MFRC522)
 * @details   Firmware de référence pour les tests du Fudan Semiconductor FM17522 (clone du MFRC522)
 */
const uint8_t PROGMEM FM17522_firmware_reference[] = {
	0x00, 0xD6, 0x78, 0x8C, 0xE2, 0xAA, 0x0C, 0x18,
	0x2A, 0xB8, 0x7A, 0x7F, 0xD3, 0x6A, 0xCF, 0x0B,
	0xB1, 0x37, 0x63, 0x4B, 0x69, 0xAE, 0x91, 0xC7,
	0xC3, 0x97, 0xAE, 0x77, 0xF4, 0x37, 0xD7, 0x9B,
	0x7C, 0xF5, 0x3C, 0x11, 0x8F, 0x15, 0xC3, 0xD7,
	0xC1, 0x5B, 0x00, 0x2A, 0xD0, 0x75, 0xDE, 0x9E,
	0x51, 0x64, 0xAB, 0x3E, 0xE9, 0x15, 0xB5, 0xAB,
	0x56, 0x9A, 0x98, 0x82, 0x26, 0xEA, 0x2A, 0x62
};

/**
 * @brief     Les cartes MIFARE Classic utilisent un ACK/NAK de 4 bits. Tout autres valeur que 0xA est un NAK.
 */
#define MFRC522_MIFARE_ACK       0xA

/////////////////////////////////////////////////////////////////////////////////////
// Fonctions de base pour communiquer avec le MFRC522
/////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief     Ecrit une valeur dans un registre du MFRC522
 * @details   Interface décrite dans la datasheet au paragraphe 8.1.2
 *
 * @param     [in]    reg     Adresse du registre dans lequel écrire la valeur
 * @param     [in]    value   Valeur à écrire dans le registre
 */
void MFRC522_PCD_WriteRegister(PCD_REG reg, uint8_t value)
{
  SPI_EnableSlave();
  // MSB == 0 pour l'écriture. LSB n'est pas utilisé pour l'adresse. Paragraphe 8.1.2.3 de la datasheet.
  SPI_SendByte((reg << 1) & 0x7E);
  SPI_SendByte(value);
  SPI_DisableSlave();
}

/**
 * @brief     Ecrit un ensemble de valeurs dans un registre du MFRC522
 * @details   Interface décrite dans la datasheet au paragraphe 8.1.2
 *
 * @param     [in]    reg     Adresse du registre dans lequel écrire les valeurs
 * @param     [in]    count   Nombre de valeurs à écrire dans le registre
 * @param     [in]    values  Contenu des valeurs à écrure dans le resitre
 *
 * @note      l'adresse @c values doit pointer sur un tableau de @c count valeurs.
 *
 * @warning   Aucun test de dépassement de capacité n'est fait
 */
void MFRC522_PCD_WriteRegisterArray(PCD_REG reg, uint8_t count, uint8_t * values)
{
  SPI_EnableSlave();
  // MSB == 0 pour l'écriture. LSB n'est pas utilisé pour l'adresse. Paragraphe 8.1.2.3 de la datasheet.
  SPI_SendByte((reg << 1) & 0x7E);
  for (uint8_t i = 0; i < count; i++)
  {
    SPI_SendByte(values[i]);
  }
  SPI_DisableSlave();
}

/**
 * @brief     Lit la valeur d'un registre du MFRC522
 * @details   Interface décrite dans la datasheet au paragraphe 8.1.2
 *
 * @param     [in]    reg     Adresse du registre à lire
 *
 * @return    valeur lue dans le registre
 */
uint8_t MFRC522_PCD_ReadRegister(PCD_REG reg)
{
  uint8_t value;
  SPI_EnableSlave();
  // MSB == 1 pour la lecture. LSB n'est pas utilisé pour l'adresse. Paragraphe 8.1.2.3 de la datasheet.
  SPI_SendByte(0x80 | ((reg << 1) & 0x7E));
  value = SPI_SendByte(0x00);
  SPI_DisableSlave();

  return value;
}

/**
 * @brief     Lit les valeurs d'un registre du MFRC522
 * @details   Interface décrite dans la datasheet au paragraphe 8.1.2
 *
 * @param     [in]    reg       Adresse du registre à lire
 * @param     [in]    count     Nombre d'octet du registre à lire
 * @param     [out]   values    Tableau mémoire où stocker les octets lus du registre
 * @param     [in]    rxAlign   ???Only bit positions rxAlign..7 in values[0] are updated
 *
 * @return    valeur lue dans le registre
 */
void MFRC522_PCD_ReadRegisterArray(PCD_REG reg, uint8_t count, uint8_t * values, uint8_t rxAlign)
{
  if (count == 0)
  {
    return;
  }

  // MSB == 1 pour la lecture. LSB n'est pas utilisé pour l'adresse. Paragraphe 8.1.2.3 de la datasheet.
  uint8_t address = 0x80 | ((reg << 1) & 0x7E);
  // Index dans le tableau résultat "values"
  uint8_t index = 0;

  SPI_EnableSlave();
  // Première lecture
  count--;
  // On indique quelle adresse nous désirons lire
  SPI_SendByte(address);

  // Tant qu'il reste des choses à lire
  while (index < count)
  {
    if (index == 0 && rxAlign)
    {
      // Only update bit positions rxAlign..7 in values[0]
      // Create bit mask for bit positions rxAlign..7
      uint8_t mask = 0;
      for (uint8_t i = rxAlign; i <= 7; i++)
      {
        mask |= (1 << i);
      }

      // Lecture de la valeur totu en indiquant que l'on veut encore lire la même adresse
      uint8_t value = SPI_SendByte(address);

      // Apply mask to both current value of values[0] and the new data in value.
      values[0] = (values[index] & ~mask) | (value & mask);
    }
    else
    {
      // Lecture normale
      // Lecture de la valeur totu en indiquant que l'on veut encore lire la même adresse
      values[index] = SPI_SendByte(address);
    }

    index++;
  }

  // Lecture de l'octet terminal
  values[index] = SPI_SendByte(0x00);

  SPI_DisableSlave();
}

/**
 * @brief     Postionne un mask sur un regitre donné
 *
 * @param     [in]    reg       Adresse du registre sur lequel appliquer le masque
 * @param     [in]    mask      Masque à appliquer
 */
void MFRC522_PCD_SetRegisterBitMask(PCD_REG reg, uint8_t mask)
{
  uint8_t tmp = MFRC522_PCD_ReadRegister(reg);
  MFRC522_PCD_WriteRegister(reg, tmp | mask);
}

/**
 * @brief     Inverse le mask sur un regitre donné
 *
 * @param     [in]    reg       Adresse du registre sur lequel appliquer le masque
 * @param     [in]    mask      Masque à appliquer
 */
void MFRC522_PCD_ClearRegisterBitMask(PCD_REG reg, uint8_t mask)
{
  uint8_t tmp = MFRC522_PCD_ReadRegister(reg);
  MFRC522_PCD_WriteRegister(reg, tmp & (~mask));
}

/**
 * @brief     Calcul le CRC d'un tableau
 * @details   Utilise le coprocesseur du MFRC522 pour calculer le CRC_A
 *
 * @param     [in]    data      Pointeur sur les données à transférer dans la FIFO pour calculer le CRC_A
 * @param     [in]    length    Nombre d'octets à transférer
 * @param     [out]   result    Pointeur du résultat. Le résultat est écrit dans un entier non signé sur 16 bits, octet de poid faible en premier.
 *
 * @return    Status du traitement (MFRC522_STATUS_OK si tout va bien)
 *
 * @warning   Aucun test de dépassement de capacité n'est fait
 */
MFRC522_STATUS MFRC522_PCD_CalculateCRC(uint8_t * data, uint8_t length, uint16_t * result)
{
  MFRC522_PCD_WriteRegister(PCD_REG_CommandReg, PCD_CMD_Idle);       // Arrêt de toute commande en cours
  MFRC522_PCD_WriteRegister(PCD_REG_DivIrqReg, 0x04);                // Netoyage du bit d'intéruption CRCIRq
  MFRC522_PCD_SetRegisterBitMask(PCD_REG_FIFOLevelReg, 0x80);        // FlushBuffer = 1, FIFO initialization
  MFRC522_PCD_WriteRegisterArray(PCD_REG_FIFODataReg, length, data);      // Ecriture des données dans la FIFO
  MFRC522_PCD_WriteRegister(PCD_REG_CommandReg, PCD_CMD_CalcCRC);    // Démarrage du calcul du CRC

  // Attente que le calcul du CRC soit terminé. (Chaque itération prent 17.73us)
  uint16_t i = 5000;
  uint8_t n;
  while (1)
  {
    // DivIrqReg[7..0] bits are: Set2 reserved reserved MfinActIRq reserved CRCIRq reserved reserved
    n = MFRC522_PCD_ReadRegister(PCD_REG_DivIrqReg);

    // Si le bit CRCIRq est positionné, c'est que le calcul du CRC est terminé
    if (n & 0x04)
    {
      break;
    }

    // Timeout si le calcul est trop long (89ms environ)
    if (--i == 0)
    {
      return MFRC522_STATUS_TIMEOUT;
    }
  }

  // Arrêt du calcul du CRC
  MFRC522_PCD_WriteRegister(PCD_REG_CommandReg, PCD_CMD_Idle);

  // Transfert du réulstat du registre en sortie
  *result  = MFRC522_PCD_ReadRegister(PCD_REG_CRCResultRegL) << 8;
  *result |= MFRC522_PCD_ReadRegister(PCD_REG_CRCResultRegH);

  return MFRC522_STATUS_OK;
}

void MFRC522_PCD_Reset()
{
  MFRC522_PCD_WriteRegister(PCD_REG_CommandReg, PCD_CMD_SoftReset);
  _delay_us(37.74);
  while (MFRC522_PCD_ReadRegister(PCD_REG_CommandReg) & _BV(4))
  {
    // Le PCD est toujours en train de rebooter... Il faut attendre !
  }
}

void MFRC522_PCD_AntennaOn()
{
  uint8_t value = MFRC522_PCD_ReadRegister(PCD_REG_TxControlReg);
  if ((value & 0x03) != 0x03)
  {
    MFRC522_PCD_WriteRegister(PCD_REG_TxControlReg, value | 0x03);
  }
}

void MFRC522_PCD_AntennaOff()
{
  MFRC522_PCD_ClearRegisterBitMask(PCD_REG_TxControlReg, 0x03);
}

void MFRC522_PCD_Init()
{
  SPI_Initialize();

  SPI_DisableSlave();

  // Broche Reset en sortie
  MFRC522_DDR |=  _BV(MFRC522_RESET_PIN);

  if (!(MFRC522_PORT & _BV(MFRC522_RESET_PIN)))
  {
    // Reset PIN to Hi
    MFRC522_PORT |=  _BV(MFRC522_RESET_PIN);
    // Attente de 37,4us le temps que le cristal ce mette en route (Cf. Section 8.8.2)
    _delay_us(37.74);
  }
  else
  {
    // Reset par software
    MFRC522_PCD_Reset();
  }

  // When communicating with a PICC we need a timeout if something goes wrong.
	// f_timer = 13.56 MHz / (2*TPreScaler+1) where TPreScaler = [TPrescaler_Hi:TPrescaler_Lo].
	// TPrescaler_Hi are the four low bits in TModeReg. TPrescaler_Lo is TPrescalerReg.

  // TAuto=1; timer starts automatically at the end of the transmission in all communication modes at all speeds
  MFRC522_PCD_WriteRegister(PCD_REG_TModeReg, 0x80);
  // TPreScaler = TModeReg[3..0]:TPrescalerReg, ie 0x0A9 = 169 => f_timer=40kHz, ie a timer period of 25us.
  MFRC522_PCD_WriteRegister(PCD_REG_TPrescalerReg, 0xA9);
  // Reload timer with 0x3E8 = 1000, ie 25ms before timeout.
  MFRC522_PCD_WriteRegister(PCD_REG_TReloadRegH, 0x03);
  MFRC522_PCD_WriteRegister(PCD_REG_TReloadRegL, 0xE8);

  // Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
  MFRC522_PCD_WriteRegister(PCD_REG_TxASKReg, 0x40);
  // Default 0x3F. Set the preset value for the CRC coprocessor for the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)
  MFRC522_PCD_WriteRegister(PCD_REG_ModeReg, 0x3D);
  // Enable the antenna driver pins TX1 and TX2 (they were disabled by the reset)
  MFRC522_PCD_AntennaOn();
}

MFRC522_RX_GAIN MFRC522_PCD_GetAntennaGain()
{
  return (MFRC522_PCD_ReadRegister(PCD_REG_RFCfgReg) & (0x07<<4)) >> 4;
}

void MFRC522_PCD_SetAntennaGain(MFRC522_RX_GAIN gain)
{
  if (MFRC522_PCD_GetAntennaGain() != gain)
  {
    // clear needed to allow 000 pattern
    MFRC522_PCD_ClearRegisterBitMask(PCD_REG_RFCfgReg, (0x07<<4));
    // only set RxGain[2:0] bits
    MFRC522_PCD_SetRegisterBitMask(PCD_REG_RFCfgReg, (gain << 4) & (0x07<<4));
  }
}

MFRC522_STATUS MFRC522_PCD_PerformSelfTest(void)
{
  // On effectue les étapes définie dans le paragraphe 16.1.1 de la datasheet

  // 1. Soft Reset
  MFRC522_PCD_Reset();

  // 2. Initialisation du buffer interne avec 25 octets à 0x00.
  uint8_t zeroes[25] = {0x00};
  MFRC522_PCD_SetRegisterBitMask(PCD_REG_FIFOLevelReg, 0x80);        // Vidage de la FIFO
  MFRC522_PCD_WriteRegisterArray(PCD_REG_FIFODataReg, 25, zeroes);   // Ecriture des 25 octets vide dans la FIFO
  MFRC522_PCD_WriteRegister(PCD_REG_CommandReg, PCD_CMD_Mem);        // Transfert des données dans la mémoire interne

  // 3. Activation du self-test
  MFRC522_PCD_WriteRegister(PCD_REG_AutoTestReg, 0x09);

  // 4. Ecriture d'un octet à 0x00 dans la FIFO
  MFRC522_PCD_WriteRegister(PCD_REG_FIFODataReg, 0x00);

  // 5. Démarrage du test en démarrant le calcul CRC
  MFRC522_PCD_WriteRegister(PCD_REG_CommandReg, PCD_CMD_CalcCRC);

  // 6. Attente de la fin du test
  for (uint8_t i = 0; i < 0xFF; i++)
  {
    // DivIrqReg[7..0] bits are: Set2 reserved reserved MfinActIRq reserved CRCIRq reserved reserved
    uint8_t n = MFRC522_PCD_ReadRegister(PCD_REG_DivIrqReg);
    // CRCIRq bit set - calculation done
    if (n & 0x04)
    {
      break;
    }
  }

  MFRC522_PCD_WriteRegister(PCD_REG_CommandReg, PCD_CMD_Idle);		// Stop calculating CRC for new content in the FIFO.

  // 7. Lecture des 64 octets de la FIFO
  uint8_t result[64];
  MFRC522_PCD_ReadRegisterArray(PCD_REG_FIFODataReg, 64, result, 0);

  // Fin du Self test
  // On réinitialise le registre AutoTestReg pour revenir à la normal
  MFRC522_PCD_WriteRegister(PCD_REG_AutoTestReg, 0x00);

  // Détermination de la version du firmware (Cf. paragraphe 9.3.4.8 de la datasheet)
  uint8_t version = MFRC522_PCD_ReadRegister(PCD_REG_VersionReg);

  // Pick the appropriate reference values
  const uint8_t * reference;
  switch (version)
  {
    // Fudan Semiconductor FM17522 clone
    case 0x88:
      reference = FM17522_firmware_reference;
      break;
    // Version 1.0
    case 0x91:
      reference = MFRC522_firmware_referenceV1_0;
      break;
    // Version 2.0
    case 0x92:
      reference = MFRC522_firmware_referenceV2_0;
      break;
    // Version inconnue
    default:
      return MFRC522_STATUS_ERROR;
  }

  // Vérification du résultat
  for (uint8_t i = 0; i < 64; i++)
  {
    if (result[i] != pgm_read_byte(&(reference[i])))
    {
      return MFRC522_STATUS_ERROR;
    }
  }

  return MFRC522_STATUS_OK;
}

/////////////////////////////////////////////////////////////////////////////////////
// Functions for communicating with PICCs
/////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief     Exécute la commande de transfert dans la FIFO du MFRC522
 * @details   Exécute la commande PCD_CMD_Transceive qui envoi les données de la FIFO
 *            vers l'antenne
 *
 * @note      La validation CRC ne peut être faite que si @c backData et @c backLen sont spécifiés
 *
 * @param     [in]      command     Commande à exécuter
 * @param     [in]      waitIrq     Mask de bit du registre PCD_REG_ComIrqReg signalant la bonne terminaison de la commande
 * @param     [in]      sendData    Pointeur des données à transférer dans la FIFO
 * @param     [in]      sendLen     Nombre d'octets à transférer dans la FIFO
 * @param     [out]     backData    NULL ou pointeur vers le buffer devant récupérer les données après transfert
 * @param     [in]      backLen     Valeurs possibles :
 *                                  @li @c in : Nombre max d'octets pouvant être écrit dans le buffer backData
 *                                  @li @c out : Nombre d'octets retournés
 * @param     [in,out]  validBits   Nombre de bits valids pour le dernier octets (0 à 8). NULL par défaut
 * @param     [in]      rxAlign     Définit la position du bit dans backData[0] pour le premier bit reçu. Par défaut 0.
 * @param     [in]      checkCRC    Valeurs possible :
 *                                  @li @c 0 -> Pas de contrôle CRC.
 *                                  @li @c 1 -> Les deux derniers octets sont considérés comme CRC_A et sont donc validés.
 *
 * @warning   Aucun dépassement de capacité n'est testé.
 *
 * @return    MFRC522_STATUS_OK si le test est OK.
 *            MFRC522_STATUS_* en cas d'erreur.
 */
MFRC522_STATUS MFRC522_PCD_CommunicateWithPICC(PCD_CMD command, uint8_t waitIrq, uint8_t * sendData, uint8_t sendLen, uint8_t * backData,
  uint8_t * backLen, uint8_t * validBits, uint8_t rxAlign, uint8_t checkCRC)
{
  uint8_t n;
  uint8_t _validBits = 0;

  // Préparation des valeurs pour le registre PCD_REG_BitFramingReg
  uint8_t txLastBits = 0;
  if (validBits != NULL)
  {
    txLastBits = *validBits;
  }
  // RxAlign    = BitFramingReg[6..4]
  // TxLastBits = BitFramingReg[2..0]
  uint8_t bitFraming = (rxAlign << 4) + txLastBits;

  // Stop any active command.
  MFRC522_PCD_WriteRegister(PCD_REG_CommandReg, PCD_CMD_Idle);
  // Clear all seven interrupt request bits
  MFRC522_PCD_WriteRegister(PCD_REG_ComIrqReg, 0x7F);
  // FlushBuffer = 1, FIFO initialization
  MFRC522_PCD_SetRegisterBitMask(PCD_REG_FIFOLevelReg, 0x80);
  // Write sendData to the FIFO
  MFRC522_PCD_WriteRegisterArray(PCD_REG_FIFODataReg, sendLen, sendData);
  // Bit adjustments
  MFRC522_PCD_WriteRegister(PCD_REG_BitFramingReg, bitFraming);
  // Execute the command
  MFRC522_PCD_WriteRegister(PCD_REG_CommandReg, command);
  if (command == PCD_CMD_Transceive)
  {
    // StartSend=1, transmission of data starts
    MFRC522_PCD_SetRegisterBitMask(PCD_REG_BitFramingReg, 0x80);
  }

  // Wait for the command to complete.
  // In MFRC522_PCD_Init() we set the TAuto flag in TModeReg. This means the timer automatically starts when the PCD stops transmitting.
  // Each iteration of the do-while-loop takes 17.86us.
  uint16_t i = 2000;
  while (1)
  {
    // ComIrqReg[7..0] bits are: Set1 TxIRq RxIRq IdleIRq HiAlertIRq LoAlertIRq ErrIRq TimerIRq
    n = MFRC522_PCD_ReadRegister(PCD_REG_ComIrqReg);
    // One of the interrupts that signal success has been set.
    if (n & waitIrq)
    {
      break;
    }
    // Timer interrupt - nothing received in 25ms
    if (n & 0x01)
    {
      return MFRC522_STATUS_TIMEOUT;
    }
    // The emergency break. If all other condions fail we will eventually terminate on this one after 35.7ms.
    // Communication with the MFRC522 might be down.
    if (--i == 0)
    {
      return MFRC522_STATUS_TIMEOUT;
    }
  }

  // Stop now if any errors except collisions were detected.
  // ErrorReg[7..0] bits are: WrErr TempErr reserved BufferOvfl CollErr CRCErr ParityErr ProtocolErr
  uint8_t errorRegValue = MFRC522_PCD_ReadRegister(PCD_REG_ErrorReg);
  if (errorRegValue & 0x13)
  {
    // BufferOvfl ParityErr ProtocolErr
    return MFRC522_STATUS_ERROR;
  }

  // If the caller wants data back, get it from the MFRC522.
  if (backData != NULL && backLen != NULL)
  {
    // Number of bytes in the FIFO
    n = MFRC522_PCD_ReadRegister(PCD_REG_FIFOLevelReg);
    if (n > *backLen)
    {
      return MFRC522_STATUS_NO_ROOM;
    }
    // Number of bytes returned
    *backLen = n;
    // Get received data from FIFO
    MFRC522_PCD_ReadRegisterArray(PCD_REG_FIFODataReg, n, backData, rxAlign);
    // RxLastBits[2:0] indicates the number of valid bits in the last received byte. If this value is 000b, the whole byte is valid.
    _validBits = MFRC522_PCD_ReadRegister(PCD_REG_ControlReg) & 0x07;
    if (validBits != NULL)
    {
      *validBits = _validBits;
    }
  }

  // Tell about collisions
  if (errorRegValue & 0x08)
  {
    // CollErr
    return MFRC522_STATUS_COLLISION;
  }

  // Perform CRC_A validation if requested.
  if (backData != NULL && backLen != NULL && checkCRC != 0)
  {
    // In this case a MIFARE Classic NAK is not OK.
    if (*backLen == 1 && _validBits == 4)
    {
      return MFRC522_STATUS_MIFARE_NACK;
    }

    // We need at least the CRC_A value and all 8 bits of the last byte must be received.
    if (*backLen < 2 || _validBits != 0)
    {
      return MFRC522_STATUS_CRC_WRONG;
    }

    // Verify CRC_A - do our own calculation and store the control in controlBuffer.
    uint16_t controlBuffer;
    n = MFRC522_PCD_CalculateCRC(&backData[0], *backLen - 2, &controlBuffer);
    if (n != MFRC522_STATUS_OK)
    {
      return n;
    }

    if (   (backData[*backLen - 2] != ((controlBuffer & 0xFF00) >> 8))
        || (backData[*backLen - 1] != (controlBuffer & 0xFF)))
    {
      return MFRC522_STATUS_CRC_WRONG;
    }
  }

  return MFRC522_STATUS_OK;
}

MFRC522_STATUS MFRC522_PCD_TransceiveData(uint8_t * sendData, uint8_t sendLen, uint8_t * backData,
  uint8_t * backLen, uint8_t * validBits, uint8_t rxAlign, uint8_t checkCRC)
{
  // RxIRq and IdleIRq
  uint8_t waitIRq = 0x30;
  return MFRC522_PCD_CommunicateWithPICC(PCD_CMD_Transceive, waitIRq, sendData, sendLen, backData, backLen, validBits, rxAlign, checkCRC);
}

/**
 * @brief     Envoi une commande Wakeup ou Request de type A.
 * @details   Envoi une commande Wakeup ou Request de type A.@n
 *            Frame de 7 bits.@n
 *
 * @warning   Quand deux PICC sont dans le champ au même moment, un MFRC522_STATUS_TIMEOUT sera probablement déclenché.
 *
 * @param     [in]      command     Command PICC à envoyer
 * @param     [in,out]  uid         Pointeur sur une structure de type MFRC522_PICC_UID.@n
 *                                  La zone ATQA est alimentée en sortie
 *
 * @warning   Aucun dépassement de capacité n'est testé.
 *
 * @return    MFRC522_STATUS_OK si le test est OK.
 *            MFRC522_STATUS_* en cas d'erreur.
 */
MFRC522_STATUS MFRC522_PICC_REQA_or_WUPA(PICC_CMD command, MFRC522_PICC_UID * uid)
{
  uint8_t validBits;
  MFRC522_STATUS status;

  uint8_t bufferSize = sizeof(uid->atqa);

  // ValuesAfterColl=1 => Suppression de tous les bits reçu après une collision
  MFRC522_PCD_ClearRegisterBitMask(PCD_REG_CollReg, 0x80);
  // For REQA and WUPA we need the short frame format - transmit only 7 bits of the last (and only) byte. TxLastBits = BitFramingReg[2..0]
  validBits = 7;
  status = MFRC522_PCD_TransceiveData(&command, 1, (uint8_t *)&uid->atqa, &bufferSize, &validBits, 0, 0);
  if (status != MFRC522_STATUS_OK)
  {
    return status;
  }

  if (bufferSize != 2 || validBits != 0)
  {
    // ATQA must be exactly 16 bits.
    return MFRC522_STATUS_ERROR;
  }

  return MFRC522_STATUS_OK;
}

MFRC522_STATUS MFRC522_PICC_RequestA(MFRC522_PICC_UID * uid)
{
  return MFRC522_PICC_REQA_or_WUPA(PICC_CMD_REQA, uid);
}

MFRC522_STATUS MFRC522_PICC_WakeupA(MFRC522_PICC_UID * uid)
{
  return MFRC522_PICC_REQA_or_WUPA(PICC_CMD_WUPA, uid);
}

MFRC522_STATUS MFRC522_PICC_Select(MFRC522_PICC_UID * uid)
{
  // Pour le debug
  //char string[60];

  // Description de la structure du buffer :
  //    Byte 0: SEL 				      Indicates the Cascade Level: PICC_CMD_SEL_CL1, PICC_CMD_SEL_CL2 or PICC_CMD_SEL_CL3
  //    Byte 1: NVB					      Number of Valid Bits (in complete command, not just the UID): High nibble: complete bytes, Low nibble: Extra bits. CRC non compté
  //    Byte 2: UID-data or CT    See explanation below. CT means Cascade Tag.
  //    Byte 3: UID-data
  //    Byte 4: UID-data
  //    Byte 5: UID-data
  //    Byte 6: BCC               Block Check Character - XOR of bytes 2-5
  //    Byte 7: CRC_A
  //    Byte 8: CRC_A
  // The BCC and CRC_A is only transmitted if we know all the UID bits of the current Cascade Level.
  //
  // Description of bytes 2-5: (Section 6.5.4 of the ISO/IEC 14443-3 draft: UID contents and cascade levels)
  //    UID size  Cascade level Byte2 Byte3 Byte4 Byte5
  //    ========  ============= ===== ===== ===== =====
  //     4 bytes              1  uid0  uid1  uid2  uid3
  //     7 bytes              1    CT  uid0  uid1  uid2
  //                          2  uid3  uid4  uid5  uid6
  //    10 bytes              1    CT  uid0  uid1  uid2
  //                          2    CT  uid3  uid4  uid5
  //                          3  uid6  uid7  uid8  uid9
  uint8_t buffer[9];
  MFRC522_STATUS result;
  uint8_t rxAlign;
  uint8_t txLastBits;
  uint8_t responseBuffer[10];    // La réponse à un cascade level est l'UID complet (10 octets max)
  uint8_t responseLength;

  // On vérifie que le PICC ou le TAG dispose bien d'une bit frame anticollision (Cf. 6.4.1 ISO/IEC 14443-3)
  uint8_t bit_frame_anticollision = uid->atqa & 0x001F;
  if (   bit_frame_anticollision != 0x01
      && bit_frame_anticollision != 0x02
      && bit_frame_anticollision != 0x04
      && bit_frame_anticollision != 0x08
      && bit_frame_anticollision != 0x10
     )
  {
    return MFRC522_STATUS_PROPRIETARY_ANTICOLLISION;
  }


  /**
   * @todo     Gérer la sélection d'un PICC déjà connu (en entrée de la fonction Select)
   */

   // ValuesAfterColl=1 => Suppression de tous les bits reçu après une collision
   MFRC522_PCD_ClearRegisterBitMask(PCD_REG_CollReg, 0x80);

  //////////////////////////////////////////
  // Cascade level 1
  //////////////////////////////////////////

  //USART_SendString("Cascade 1\n");
  // octet 1 : Cascade level 1
  buffer[0] = PICC_CMD_SEL_CL1;
  // octet 2 : High nibble => 2 octets plein, Low nibble => Pas d'extra bits
  buffer[1] = 0x20;
  // 0 => Tous les octets sont plein
  txLastBits = 0x00;
  rxAlign = 0x00;
  responseLength = 10;
  result = MFRC522_PCD_TransceiveData(buffer, 2, responseBuffer, &responseLength, &txLastBits, rxAlign, 0);
  if (result != MFRC522_STATUS_OK)
  {
    return result;
  }

  if (result == MFRC522_STATUS_COLLISION)
  {
    /**
     * @todo     Gérer les collisions
     */
    return result;
  }

  /*
  USART_SendString("Buffer =");
  for (uint8_t i = 0; i < responseLength; i++)
  {
    USART_SendString(" 0x");
    itoa(responseBuffer[i], string, 16);
    USART_SendString(string);
  }
  USART_SendString("\n");
  */

  // Vérification du retour => 4 octets de l'UID + 1 octet BCC
  if (   responseLength != 5
      || txLastBits != 0)
  {
    return MFRC522_STATUS_ERROR;
  }
  else
  {
    uint8_t bcc = responseBuffer[0] ^ responseBuffer[1] ^ responseBuffer[2] ^ responseBuffer[3];
    if (bcc != responseBuffer[4])
    {
      return MFRC522_STATUS_BCC_ERROR;
    }
  }

  uid->uid[0] = responseBuffer[0];
  uid->uid[1] = responseBuffer[1];
  uid->uid[2] = responseBuffer[2];
  uid->uid[3] = responseBuffer[3];
  uid->uidsize = 4;

  // Sélection de la carte
  // octet 1 : Cascade level 1
  buffer[0] = PICC_CMD_SEL_CL1;
  // octet 2 : High nibble => 7 octets plein, Low nibble => Pas d'extra bits
  buffer[1] = 0x70;
  // octets 3 à 6 : 4 octets de l'UID à sélectionner
  buffer[2] = responseBuffer[0];
  buffer[3] = responseBuffer[1];
  buffer[4] = responseBuffer[2];
  buffer[5] = responseBuffer[3];
  // BCC - Block Check Character
  buffer[6] = responseBuffer[4];
  // CRC_A
  uint16_t crc = 0x0000;
  result = MFRC522_PCD_CalculateCRC(buffer, 7, &crc);
  if (result != MFRC522_STATUS_OK)
  {
    return result;
  }
  buffer[7] = (crc & 0xFF00) >> 8;
  buffer[8] = crc & 0x00FF;
  // 0 => Tous les octets sont plein
  txLastBits = 0x00;
  rxAlign = 0x00;
  responseLength = 10;
  result = MFRC522_PCD_TransceiveData(buffer, 9, responseBuffer, &responseLength, &txLastBits, rxAlign, 1);
  if (result != MFRC522_STATUS_OK)
  {
    return result;
  }

  // A ce stade nous devons avoir une réponse SAK (Select Acknowledge) => 1 octet + CRC_A
  if (   responseLength != 3
      || txLastBits != 0)
  {
    return MFRC522_STATUS_ERROR;
  }

  if (responseBuffer[0] & 0x04)
  {
    //USART_SendString("Need more...\n");
    /**
     * @todo    Faire les autres cascade levels
     */
    return MFRC522_STATUS_INTERNAL_ERROR;
  }

  uid->sak = responseBuffer[0];

  return MFRC522_STATUS_OK;






/*
  uint8_t useCascadeTag = 0;
  uint8_t cascadeLevel = 1;
  MFRC522_STATUS result;
  uint8_t count;
  uint8_t index;
  uint8_t uidIndex;                 // The first index in uid->uidByte[] that is used in the current Cascade Level.
	int8_t currentLevelKnownBits;     // The number of known UID bits in the current Cascade Level.
  uint8_t buffer[9];                // The SELECT/ANTICOLLISION commands uses a 7 byte standard frame + 2 bytes CRC_A
  uint8_t bufferUsed;               // The number of bytes used in the buffer, ie the number of bytes to transfer to the FIFO.
  uint8_t rxAlign;                  // Used in BitFramingReg. Defines the bit position for the first bit received.
  uint8_t txLastBits;               // Used in BitFramingReg. The number of valid bits in the last transmitted byte.
  uint8_t *responseBuffer;
  uint8_t responseLength;

	// Sanity checks
	if (validBits > 80)
  {
		return MFRC522_STATUS_INVALID;
	}

	// Préparation du MFRC522
  // ValuesAfterColl=1 => Bits received after collision are cleared.
  MFRC522_PCD_ClearRegisterBitMask(PCD_REG_CollReg, 0x80);

	// On cycle sur les cascade levels tant que l'on a pas un UID complet
  uint8_t uidComplete = 0;
	while (uidComplete == 0)
  {
		// Set the Cascade Level in the SEL byte, find out if we need to use the Cascade Tag in byte 2.
		switch (cascadeLevel)
    {
			case 1:
      USART_SendString("Cascade 1\n");
				buffer[0] = PICC_CMD_SEL_CL1;
				uidIndex = 0;
        // When we know that the UID has more than 4 bytes
				useCascadeTag = (validBits && uid->size > 4) ? 1 : 0;
				break;

			case 2:
      USART_SendString("Cascade 2\n");
				buffer[0] = PICC_CMD_SEL_CL2;
				uidIndex = 3;
        // When we know that the UID has more than 7 bytes
				useCascadeTag = (validBits && uid->size > 7) ? 1 : 0;
				break;

			case 3:
      USART_SendString("Cascade 3\n");
				buffer[0] = PICC_CMD_SEL_CL3;
				uidIndex = 6;
        // Never used in CL3.
				useCascadeTag = 0;
				break;

			default:
				return MFRC522_STATUS_INTERNAL_ERROR;
				break;
		}

		// How many UID bits are known in this Cascade Level?
		currentLevelKnownBits = validBits - (8 * uidIndex);
		if (currentLevelKnownBits < 0)
    {
			currentLevelKnownBits = 0;
		}

		// Copy the known bits from uid->uidByte[] to buffer[]
    // destination index in buffer[]
		index = 2;
		if (useCascadeTag != 0)
    {
			buffer[index++] = PICC_CMD_CT;
		}

    // The number of bytes needed to represent the known bits for this level.
		uint8_t bytesToCopy = currentLevelKnownBits / 8 + (currentLevelKnownBits % 8 ? 1 : 0);
		if (bytesToCopy != 0)
    {
      // Max 4 bytes in each Cascade Level. Only 3 left if we use the Cascade Tag
      uint8_t maxBytes = (useCascadeTag != 0) ? 3 : 4;
			if (bytesToCopy > maxBytes)
      {
				bytesToCopy = maxBytes;
			}

			for (count = 0; count < bytesToCopy; count++)
      {
				buffer[index++] = uid->uid[uidIndex + count];
			}
		}

		// Now that the data has been copied we need to include the 8 bits in CT in currentLevelKnownBits
		if (useCascadeTag != 0)
    {
			currentLevelKnownBits += 8;
		}

		// Repeat anti collision loop until we can transmit all UID bits + BCC and receive a SAK - max 32 iterations.
    uint8_t selectDone = 0;
		while (selectDone == 0)
    {
    USART_SendString("  currentLevelKnownBits=");
    char b[60];
    itoa(currentLevelKnownBits, b, 16);
    USART_SendString(b);
    USART_SendString("\n");
			// Find out how many bits and bytes to send and receive.
			if (currentLevelKnownBits >= 32)
      {
        // All UID bits in this Cascade Level are known. This is a SELECT.
        // NVB - Number of Valid Bits: Seven whole bytes
				buffer[1] = 0x70;
				// Calculate BCC - Block Check Character
				buffer[6] = buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5];
				// Calculate CRC_A
				result = MFRC522_PCD_CalculateCRC(buffer, 7, &buffer[7]);
				if (result != MFRC522_STATUS_OK)
        {
					return result;
				}

        // 0 => All 8 bits are valid.
				txLastBits = 0;
				bufferUsed = 9;
				// Store response in the last 3 bytes of buffer (BCC and CRC_A - not needed after tx)
				responseBuffer = &buffer[6];
				responseLength = 3;
			}
			else
      {
        // This is an ANTICOLLISION.
				txLastBits = currentLevelKnownBits % 8;
        // Number of whole bytes in the UID part.
				count = currentLevelKnownBits / 8;
        // Number of whole bytes: SEL + NVB + UIDs
				index = 2 + count;
        // NVB - Number of Valid Bits
				buffer[1] = (index << 4) + txLastBits;
				bufferUsed = index + ((txLastBits != 0) ? 1 : 0);
				// Store response in the unused part of buffer
				responseBuffer = &buffer[index];
				responseLength = sizeof(buffer) - index;
			}

			// Set bit adjustments
      // Having a seperate variable is overkill. But it makes the next line easier to read.
			rxAlign = txLastBits;
      // RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]
			MFRC522_PCD_WriteRegister(PCD_REG_BitFramingReg, (rxAlign << 4) + txLastBits);

			// Transmit the buffer and receive the response.
      USART_SendString("Tranceive...\n");
			result = MFRC522_PCD_TransceiveData(buffer, bufferUsed, responseBuffer, &responseLength, &txLastBits, rxAlign, 0);
			if (result == MFRC522_STATUS_COLLISION)
      {
      USART_SendString("Collision !\n");
        // More than one PICC in the field => collision.
        // CollReg[7..0] bits are: ValuesAfterColl reserved CollPosNotValid CollPos[4:0]
				result = MFRC522_PCD_ReadRegister(PCD_REG_CollReg);
				if (result & 0x20)
        {
        USART_SendString("Collision vérifiée\n");
          // CollPosNotValid
          // Without a valid collision position we cannot continue
					return MFRC522_STATUS_COLLISION;
				}

        // Values 0-31, 0 means bit 32.
				uint8_t collisionPos = result & 0x1F;
				if (collisionPos == 0)
        {
					collisionPos = 32;
				}

				if (collisionPos <= currentLevelKnownBits)
        {
          // No progress - should not happen
					return MFRC522_STATUS_INTERNAL_ERROR;
				}

				// Choose the PICC with the bit set.
				currentLevelKnownBits = collisionPos;
        // The bit to modify
				count = (currentLevelKnownBits - 1) % 8;
        // First byte is index 0.
				index = 1 + (currentLevelKnownBits / 8) + ((count != 0) ? 1 : 0);
				buffer[index]	|= (1 << count);
			}
			else
      {
        if (result != MFRC522_STATUS_OK)
        {
        USART_SendString("  currentLevelKnownBits 2=");
        itoa(currentLevelKnownBits, b, 16);
        USART_SendString(b);
        USART_SendString("\n");
  				return result;
  			}

        // STATUS_OK
				if (currentLevelKnownBits >= 32)
        {
          USART_SendString("Select done\n");
          // This was a SELECT.
          // No more anticollision
					selectDone = 1;
					// We continue below outside the while.
				}
				else
        {
          USART_SendString("ANTICOLLISION\n");
          // This was an ANTICOLLISION.
					// We now have all 32 bits of the UID in this Cascade Level
					currentLevelKnownBits = 32;
					// Run loop again to do the SELECT.
				}
  		}

      USART_SendString("T OK !...\n");
		} // End of while (selectDone == 0)

    USART_SendString("Fin whiel\n");
		// We do not check the CBB - it was constructed by us above.

		// Copy the found UID bytes from buffer[] to uid->uidByte[]
    // source index in buffer[]
		index = (buffer[2] == PICC_CMD_CT) ? 3 : 2;
		bytesToCopy = (buffer[2] == PICC_CMD_CT) ? 3 : 4;
		for (count = 0; count < bytesToCopy; count++)
    {
			uid->uid[uidIndex + count] = buffer[index++];
		}

		// Check response SAK (Select Acknowledge)
		if (responseLength != 3 || txLastBits != 0)
    {
      // SAK must be exactly 24 bits (1 byte + CRC_A).
			return MFRC522_STATUS_ERROR;
		}

		// Verify CRC_A - do our own calculation and store the control in buffer[2..3] - those bytes are not needed anymore.
    USART_SendString("CRC N°2...\n");
		result = MFRC522_PCD_CalculateCRC(responseBuffer, 1, &buffer[2]);
		if (result != MFRC522_STATUS_OK)
    {
			return result;
		}
    USART_SendString("OK !...\n");

		if ((buffer[2] != responseBuffer[1]) || (buffer[3] != responseBuffer[2]))
    {
			return MFRC522_STATUS_CRC_WRONG;
		}

		if (responseBuffer[0] & 0x04)
    {
      // Cascade bit set - UID not complete yes
			cascadeLevel++;
		}
		else
    {
			uidComplete = 1;
			uid->sak = responseBuffer[0];
		}
	} // End of while (uidComplete == 0)

	// Set correct uid->size
	uid->size = 3 * cascadeLevel + 1;

	return MFRC522_STATUS_OK;
  */
}

MFRC522_STATUS MFRC522_PICC_HaltA()
{
  MFRC522_STATUS result;
  uint8_t buffer[4];

  // Construction du buffer pour la commande
  buffer[0] = PICC_CMD_HLTA;
  buffer[1] = 0x00;

  // Calcule du CRC_A
  uint16_t crc = 0x0000;
  result = MFRC522_PCD_CalculateCRC(buffer, 2, &crc);
  if (result != MFRC522_STATUS_OK)
  {
    return result;
  }
  buffer[2] = (crc & 0xFF00) >> 8;
  buffer[3] = crc & 0x00FF;

  // Envoi de la commande
  // Le standard dit :
  //    Si le PICC répond sans modulation durant une période de 1ms après la din de la frame contenant la commande
  //    HALT, sa réponse devrait être interprétée comme "non acquité".
  // L'interprétation faite ici : Seul un timeout est un succès
  result = MFRC522_PCD_TransceiveData(buffer, 4, NULL, NULL, NULL, 0, 0);
  if (result == MFRC522_STATUS_TIMEOUT)
  {
    return MFRC522_STATUS_OK;
  }

  if (result == MFRC522_STATUS_OK)
  {
    return MFRC522_STATUS_ERROR;
  }

  return result;
}

/**
 * @brief     Fonction utilitaire pour encapsuler le protocole de communication MIFARE
 * @details   Cette fonction prend le buffer à envoyer, ajoute le CRC_A et vérifie en fin d'excution
 *            que l'on a bien reçu un MF_ACK ou un timeout.
 *
 * @param     [in]      sendData       Buffer des données à envoyer
 * @param     [in]      sendLen        Taille des données à envoyer
 * @param     [in]      acceptTimeout  Valeurs possibles :
 *                                     @li @c 0 : Le timeout est considéré comme une erreur
 *                                     @li @c 1 : Le timeout n'est pas considéré comme une erreur
 *
 * @warning   Aucun dépassement de capacité n'est testé.
 *
 * @return    MFRC522_STATUS_OK si le test est OK.
 *            MFRC522_STATUS_* en cas d'erreur.
 */
MFRC522_STATUS MFRC522_MIFARE_Transceive(uint8_t * sendData, uint8_t sendLen, uint8_t acceptTimeout)
{
  MFRC522_STATUS result;
  // Le buffer fera 16 octets maximum + 2 octets de CRC_A
  uint8_t buffer[18];

  // Préparation du buffer
  for (uint8_t i = 0; i < sendLen; i++)
  {
    buffer[i] = sendData[i];
  }

  uint16_t crc = 0x0000;
  result = MFRC522_PCD_CalculateCRC(buffer, sendLen, &crc);
  if (result != MFRC522_STATUS_OK)
  {
    return result;
  }
  buffer[sendLen++]     = (crc & 0xFF00) >> 8;
  buffer[sendLen++] = crc & 0x00FF;

  // Envoi des données et récupération du résultat dans buffer
  // RxIRq and IdleIRq
  uint8_t waitIRq = 0x30;
  uint8_t bufferSize = 18;
  uint8_t validBits = 0;
  result = MFRC522_PCD_CommunicateWithPICC(PCD_CMD_Transceive, waitIRq, buffer, sendLen, buffer, &bufferSize, &validBits, 0, 0);
  if (acceptTimeout && result == MFRC522_STATUS_TIMEOUT)
  {
    return MFRC522_STATUS_OK;
  }
  if (result != MFRC522_STATUS_OK)
  {
    return result;
  }

  // Le PICC doit répondre un ACK sur 4bits (MF_ACK)
  if (bufferSize != 1 || validBits != 4)
  {
    return MFRC522_STATUS_ERROR;
  }
  if (buffer[0] != MFRC522_MIFARE_ACK)
  {
    return MFRC522_STATUS_MIFARE_NACK;
  }
  return MFRC522_STATUS_OK;
}

/**
 * @brief     Fonction utilitaire pour encapsuler le protocole de communication MIFARE des opérations Decrement, Increment et Restore
 * @details   Cette fonction prend le buffer à envoyer, ajoute le CRC_A et vérifie en fin d'excution
 *            que l'on a bien reçu un MF_ACK ou un timeout.
 *
 * @param     [in]      command        Commande à envoyer (PICC_CMD_MF_DECREMENT ou PICC_CMD_MF_INCREMENT ou PICC_CMD_MF_RESTORE)
 * @param     [in]      blockAddr      Adresse du block sur lequel travailler
 * @param     [in]      data           Donnée à envoyer
 *
 * @return    MFRC522_STATUS_OK si le test est OK.
 *            MFRC522_STATUS_* en cas d'erreur.
 */
 MFRC522_STATUS MFRC522_MIFARE_TwoStepHelper(PICC_CMD command, uint8_t blockAddr, int32_t data)
 {
   MFRC522_STATUS result;
   uint8_t buffer[2];

	// Etape 1 : Envoi de la commande et de l'adresse du block
  buffer[0] = command;
  buffer[1] = blockAddr;
	result = MFRC522_MIFARE_Transceive(buffer, 2, 0);
	if (result != MFRC522_STATUS_OK)
  {
		return result;
	}

	// Etape 2 : Envoi des données
	result = MFRC522_MIFARE_Transceive((uint8_t *)&data, 4, 1);  // Un timeout est un succès
	if (result != MFRC522_STATUS_OK)
  {
		return result;
	}

	return MFRC522_STATUS_OK;
}

MFRC522_STATUS MFRC522_MIFARE_Authenticate(MFRC522_AUTH_KEY keytype, uint8_t blockAddr, MFRC522_MIFARE_KEY * key, MFRC522_PICC_UID * uid)
{
  // IdleIRq
  uint8_t waitIRq = 0x10;

  uint8_t sendData[2 + MFRC522_AUTH_KEY_SIZE + 4];
  uint8_t sendLen = 2 + MFRC522_AUTH_KEY_SIZE + 4;
  switch (keytype)
  {
    case MFRC522_AUTH_KEY_A:
      sendData[0] = PICC_CMD_MF_AUTH_KEY_A;
      break;
    case MFRC522_AUTH_KEY_B:
      sendData[0] = PICC_CMD_MF_AUTH_KEY_B;
      break;
    default:
      return MFRC522_STATUS_INTERNAL_ERROR;
  }
  sendData[1] = blockAddr;
  for (uint8_t i = 0; i < MFRC522_AUTH_KEY_SIZE; i++)
  {
    sendData[2 + i] = key->keybyte[i];
  }
  for (uint8_t i = 0; i < 4; i++)
  {
    sendData[2 + MFRC522_AUTH_KEY_SIZE + i] = uid->uid[i];
  }

  return MFRC522_PCD_CommunicateWithPICC(PCD_CMD_MFAuthent, waitIRq, sendData, sendLen, NULL, NULL, NULL, 0, 0);
}

void MFRC522_MIFARE_StopCrypto1()
{
  // Status2Reg[7..0] bits are: TempSensClear I2CForceHS reserved reserved MFCrypto1On ModemState[2:0]
  MFRC522_PCD_ClearRegisterBitMask(PCD_REG_Status2Reg, 0x08);
}

MFRC522_STATUS MFRC522_MIFARE_Read(uint8_t blockAddr, uint8_t * buffer, uint8_t * bufferSize)
{
  MFRC522_STATUS result;

  // Construction du buffer
  buffer[0] = PICC_CMD_MF_READ;
  buffer[1] = blockAddr;
  // Calcule du CRC_A
  uint16_t crc = 0x0000;
  result = MFRC522_PCD_CalculateCRC(buffer, 2, &crc);
  if (result != MFRC522_STATUS_OK)
  {
    return result;
  }
  buffer[2] = (crc & 0xFF00) >> 8;
  buffer[3] = crc & 0x00FF;

  return MFRC522_PCD_TransceiveData(buffer, 4, buffer, bufferSize, NULL, 0, 1);
}

MFRC522_STATUS MFRC522_MIFARE_Write(uint8_t blockAddr, uint8_t * buffer, uint8_t bufferSize)
{
  MFRC522_STATUS result;

  // Le protocole MIFARE impose d'avoir deux étapes de communications
  // Etape 1 : Indique au PICC que l'on désire écrire
  uint8_t cmdBuffer[2];
  cmdBuffer[0] = PICC_CMD_MF_WRITE;
  cmdBuffer[1] = blockAddr;
  result = MFRC522_MIFARE_Transceive(cmdBuffer, 2, 0);
  if (result != MFRC522_STATUS_OK)
  {
    return result;
  }

  // Etape 2: Transfert des données
  result = MFRC522_MIFARE_Transceive(buffer, bufferSize, 0);
  if (result != MFRC522_STATUS_OK)
  {
    return result;
  }

  return MFRC522_STATUS_OK;
}

MFRC522_STATUS MFRC522_MIFARE_Decrement(uint8_t blockAddr, int32_t delta)
{
  return MFRC522_MIFARE_TwoStepHelper(PICC_CMD_MF_DECREMENT, blockAddr, delta);
}

MFRC522_STATUS MFRC522_MIFARE_Increment(uint8_t blockAddr, int32_t delta)
{
  return MFRC522_MIFARE_TwoStepHelper(PICC_CMD_MF_INCREMENT, blockAddr, delta);
}

MFRC522_STATUS MFRC522_MIFARE_Restore(uint8_t blockAddr)
{
  return MFRC522_MIFARE_TwoStepHelper(PICC_CMD_MF_RESTORE, blockAddr, 0);
}

MFRC522_STATUS MFRC522_MIFARE_Transfer(uint8_t blockAddr)
{
  uint8_t buffer[2];

  buffer[0] = PICC_CMD_MF_TRANSFER;
  buffer[1] = blockAddr;
  return MFRC522_MIFARE_Transceive(buffer, 2, 0);
}

MFRC522_STATUS MFRC522_MIFARE_Ultralight_Write(uint8_t page, uint8_t * buffer, uint8_t bufferSize)
{
  uint8_t cmdBuffer[6];
  cmdBuffer[0] = PICC_CMD_UL_WRITE;
  cmdBuffer[1] = page;
  for (uint8_t i = 0; i < 4; i++)
  {
    cmdBuffer[2 + i] = buffer[i];
  }

	return MFRC522_MIFARE_Transceive(cmdBuffer, 6, 0);
}

MFRC522_STATUS MFRC522_MIFARE_Classic_GetValue(uint8_t blockAddr, int32_t * value)
{
  MFRC522_STATUS status;
	uint8_t buffer[18];
  uint8_t size = 18;

	status = MFRC522_MIFARE_Read(blockAddr, buffer, &size);
	if (status == MFRC522_STATUS_OK)
  {
		*value = ((int32_t)(buffer[3]) << 24) |
             ((int32_t)(buffer[2]) << 16) |
             ((int32_t)(buffer[1]) <<  8) |
             (int32_t)(buffer[0]);
	}

	return status;
}

MFRC522_STATUS MFRC522_MIFARE_Classic_SetValue(uint8_t blockAddr, int32_t value)
{
  uint8_t buffer[18];

  // Transformation de l'entier sur 4 octets. Répété 2 fois dans le block.
  buffer[0] = buffer[ 8] = (value & 0xFF);
  buffer[1] = buffer[ 9] = (value & 0xFF00) >> 8;
  buffer[2] = buffer[10] = (value & 0xFF0000) >> 16;
  buffer[3] = buffer[11] = (value & 0xFF000000) >> 24;
  // On inverse les 4 octets de la valeur au bon endroit dans le bloc.
  buffer[4] = ~buffer[0];
  buffer[5] = ~buffer[1];
  buffer[6] = ~buffer[2];
  buffer[7] = ~buffer[3];
  // Ecriture de l'adresse 2 fois et en mode inversé
  buffer[12] = buffer[14] = blockAddr;
  buffer[13] = buffer[15] = ~blockAddr;

  // Ecriture de l'ensemble du block
  return MFRC522_MIFARE_Write(blockAddr, buffer, 16);
}

MFRC522_PICC_TYPE MFRC522_PICC_GetType(MFRC522_PICC_UID * uid)
{
  if (uid->sak & 0x04)
  {
    // UID incomplet
    return MFRC522_PICC_TYPE_NOT_COMPLETE;
  }

  switch (uid->sak)
  {
    case 0x00:
      return MFRC522_PICC_TYPE_MIFARE_UL;
      break;
    case 0x01:
      return MFRC522_PICC_TYPE_TNP3XXX;
      break;
    case 0x08:
      return MFRC522_PICC_TYPE_MIFARE_1K;
      break;
    case 0x09:
      return MFRC522_PICC_TYPE_MIFARE_MINI;
      break;
    case 0x10:
    case 0x11:
      return MFRC522_PICC_TYPE_MIFARE_PLUS;
      break;
    case 0x18:
      return MFRC522_PICC_TYPE_MIFARE_4K;
      break;
    case 0x20:
      return MFRC522_PICC_TYPE_MIFARE_DESFIRE;
      break;
    case 0x28:
      return MFRC522_PICC_TYPE_JCOP30;
      break;
    case 0x38:
      return MFRC522_PICC_TYPE_MIFARE_4K_EMUL;
      break;
    case 0x88:
      return MFRC522_PICC_TYPE_MIFARE_1K_INFINEON;
      break;
    case 0x98:
      return MFRC522_PICC_TYPE_MPCOS;
      break;
    default:
      break;
  }

  if (uid->sak & 0x20)
  {
    return MFRC522_PICC_TYPE_ISO_14443_4;
  }

  if (uid->sak & 0x40)
  {
    return MFRC522_PICC_TYPE_ISO_18092;
  }

  return MFRC522_PICC_TYPE_UNKNOWN;
}

void MFRC522_MIFARE_SetAccessBits(uint8_t * accessBitBuffer, uint8_t g0, uint8_t g1, uint8_t g2, uint8_t g3)
{
  uint8_t c1 = ((g3 & 4) << 1) | ((g2 & 4) << 0) | ((g1 & 4) >> 1) | ((g0 & 4) >> 2);
  uint8_t c2 = ((g3 & 2) << 2) | ((g2 & 2) << 1) | ((g1 & 2) << 0) | ((g0 & 2) >> 1);
  uint8_t c3 = ((g3 & 1) << 3) | ((g2 & 1) << 2) | ((g1 & 1) << 1) | ((g0 & 1) << 0);

	accessBitBuffer[0] = (~c2 & 0xF) << 4 | (~c1 & 0xF);
	accessBitBuffer[1] =          c1 << 4 | (~c3 & 0xF);
	accessBitBuffer[2] =          c3 << 4 | c2;
}

#endif /* _MFRC522_CORE_H_ */





/*
#define NR_KNOWN_KEYS   8
// Known keys, see: https://code.google.com/p/mfcuk/wiki/MifareClassicDefaultKeys
byte knownKeys[NR_KNOWN_KEYS][MFRC522::MF_KEY_SIZE] =  {
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, // FF FF FF FF FF FF = factory default
    {0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5}, // A0 A1 A2 A3 A4 A5
    {0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5}, // B0 B1 B2 B3 B4 B5
    {0x4d, 0x3a, 0x99, 0xc3, 0x51, 0xdd}, // 4D 3A 99 C3 51 DD
    {0x1a, 0x98, 0x2c, 0x7e, 0x45, 0x9a}, // 1A 98 2C 7E 45 9A
    {0xd3, 0xf7, 0xd3, 0xf7, 0xd3, 0xf7}, // D3 F7 D3 F7 D3 F7
    {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}, // AA BB CC DD EE FF
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // 00 00 00 00 00 00
};
*/







/**
 * Transmits SELECT/ANTICOLLISION commands to select a single PICC.
 * Before calling this function the PICCs must be placed in the READY(*) state by calling PICC_RequestA() or PICC_WakeupA().
 * On success:
 * 		- The chosen PICC is in state ACTIVE(*) and all other PICCs have returned to state IDLE/HALT. (Figure 7 of the ISO/IEC 14443-3 draft.)
 * 		- The UID size and value of the chosen PICC is returned in *uid along with the SAK.
 *
 * A PICC UID consists of 4, 7 or 10 bytes.
 * Only 4 bytes can be specified in a SELECT command, so for the longer UIDs two or three iterations are used:
 * 		UID size	Number of UID bytes		Cascade levels		Example of PICC
 * 		========	===================		==============		===============
 * 		single				 4						1				MIFARE Classic
 * 		double				 7						2				MIFARE Ultralight
 * 		triple				10						3				Not currently in use?
 *
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
 /*
byte MFRC522::PICC_Select(	Uid *uid,			///< Pointer to Uid struct. Normally output, but can also be used to supply a known UID.
							byte validBits		///< The number of known UID bits supplied in *uid. Normally 0. If set you must also supply uid->size.
						 ) {
	bool uidComplete;
	bool selectDone;
	bool useCascadeTag;
	byte cascadeLevel = 1;
	byte result;
	byte count;
	byte index;
	byte uidIndex;					// The first index in uid->uidByte[] that is used in the current Cascade Level.
	int8_t currentLevelKnownBits;		// The number of known UID bits in the current Cascade Level.
	byte buffer[9];					// The SELECT/ANTICOLLISION commands uses a 7 byte standard frame + 2 bytes CRC_A
	byte bufferUsed;				// The number of bytes used in the buffer, ie the number of bytes to transfer to the FIFO.
	byte rxAlign;					// Used in BitFramingReg. Defines the bit position for the first bit received.
	byte txLastBits;				// Used in BitFramingReg. The number of valid bits in the last transmitted byte.
	byte *responseBuffer;
	byte responseLength;

	// Description of buffer structure:
	//		Byte 0: SEL 				Indicates the Cascade Level: PICC_CMD_SEL_CL1, PICC_CMD_SEL_CL2 or PICC_CMD_SEL_CL3
	//		Byte 1: NVB					Number of Valid Bits (in complete command, not just the UID): High nibble: complete bytes, Low nibble: Extra bits.
	//		Byte 2: UID-data or CT		See explanation below. CT means Cascade Tag.
	//		Byte 3: UID-data
	//		Byte 4: UID-data
	//		Byte 5: UID-data
	//		Byte 6: BCC					Block Check Character - XOR of bytes 2-5
	//		Byte 7: CRC_A
	//		Byte 8: CRC_A
	// The BCC and CRC_A is only transmitted if we know all the UID bits of the current Cascade Level.
	//
	// Description of bytes 2-5: (Section 6.5.4 of the ISO/IEC 14443-3 draft: UID contents and cascade levels)
	//		UID size	Cascade level	Byte2	Byte3	Byte4	Byte5
	//		========	=============	=====	=====	=====	=====
	//		 4 bytes		1			uid0	uid1	uid2	uid3
	//		 7 bytes		1			CT		uid0	uid1	uid2
	//						2			uid3	uid4	uid5	uid6
	//		10 bytes		1			CT		uid0	uid1	uid2
	//						2			CT		uid3	uid4	uid5
	//						3			uid6	uid7	uid8	uid9

	// Sanity checks
	if (validBits > 80) {
		return STATUS_INVALID;
	}

	// Prepare MFRC522
	PCD_ClearRegisterBitMask(CollReg, 0x80);		// ValuesAfterColl=1 => Bits received after collision are cleared.

	// Repeat Cascade Level loop until we have a complete UID.
	uidComplete = false;
	while (!uidComplete) {
		// Set the Cascade Level in the SEL byte, find out if we need to use the Cascade Tag in byte 2.
		switch (cascadeLevel) {
			case 1:
				buffer[0] = PICC_CMD_SEL_CL1;
				uidIndex = 0;
				useCascadeTag = validBits && uid->size > 4;	// When we know that the UID has more than 4 bytes
				break;

			case 2:
				buffer[0] = PICC_CMD_SEL_CL2;
				uidIndex = 3;
				useCascadeTag = validBits && uid->size > 7;	// When we know that the UID has more than 7 bytes
				break;

			case 3:
				buffer[0] = PICC_CMD_SEL_CL3;
				uidIndex = 6;
				useCascadeTag = false;						// Never used in CL3.
				break;

			default:
				return STATUS_INTERNAL_ERROR;
				break;
		}

		// How many UID bits are known in this Cascade Level?
		currentLevelKnownBits = validBits - (8 * uidIndex);
		if (currentLevelKnownBits < 0) {
			currentLevelKnownBits = 0;
		}
		// Copy the known bits from uid->uidByte[] to buffer[]
		index = 2; // destination index in buffer[]
		if (useCascadeTag) {
			buffer[index++] = PICC_CMD_CT;
		}
		byte bytesToCopy = currentLevelKnownBits / 8 + (currentLevelKnownBits % 8 ? 1 : 0); // The number of bytes needed to represent the known bits for this level.
		if (bytesToCopy) {
			byte maxBytes = useCascadeTag ? 3 : 4; // Max 4 bytes in each Cascade Level. Only 3 left if we use the Cascade Tag
			if (bytesToCopy > maxBytes) {
				bytesToCopy = maxBytes;
			}
			for (count = 0; count < bytesToCopy; count++) {
				buffer[index++] = uid->uidByte[uidIndex + count];
			}
		}
		// Now that the data has been copied we need to include the 8 bits in CT in currentLevelKnownBits
		if (useCascadeTag) {
			currentLevelKnownBits += 8;
		}

		// Repeat anti collision loop until we can transmit all UID bits + BCC and receive a SAK - max 32 iterations.
		selectDone = false;
		while (!selectDone) {
			// Find out how many bits and bytes to send and receive.
			if (currentLevelKnownBits >= 32) { // All UID bits in this Cascade Level are known. This is a SELECT.
				//Serial.print(F("SELECT: currentLevelKnownBits=")); Serial.println(currentLevelKnownBits, DEC);
				buffer[1] = 0x70; // NVB - Number of Valid Bits: Seven whole bytes
				// Calculate BCC - Block Check Character
				buffer[6] = buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5];
				// Calculate CRC_A
				result = PCD_CalculateCRC(buffer, 7, &buffer[7]);
				if (result != STATUS_OK) {
					return result;
				}
				txLastBits		= 0; // 0 => All 8 bits are valid.
				bufferUsed		= 9;
				// Store response in the last 3 bytes of buffer (BCC and CRC_A - not needed after tx)
				responseBuffer	= &buffer[6];
				responseLength	= 3;
			}
			else { // This is an ANTICOLLISION.
				//Serial.print(F("ANTICOLLISION: currentLevelKnownBits=")); Serial.println(currentLevelKnownBits, DEC);
				txLastBits		= currentLevelKnownBits % 8;
				count			= currentLevelKnownBits / 8;	// Number of whole bytes in the UID part.
				index			= 2 + count;					// Number of whole bytes: SEL + NVB + UIDs
				buffer[1]		= (index << 4) + txLastBits;	// NVB - Number of Valid Bits
				bufferUsed		= index + (txLastBits ? 1 : 0);
				// Store response in the unused part of buffer
				responseBuffer	= &buffer[index];
				responseLength	= sizeof(buffer) - index;
			}

			// Set bit adjustments
			rxAlign = txLastBits;											// Having a seperate variable is overkill. But it makes the next line easier to read.
			PCD_WriteRegister(BitFramingReg, (rxAlign << 4) + txLastBits);	// RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]

			// Transmit the buffer and receive the response.
			result = PCD_TransceiveData(buffer, bufferUsed, responseBuffer, &responseLength, &txLastBits, rxAlign);
			if (result == STATUS_COLLISION) { // More than one PICC in the field => collision.
				result = PCD_ReadRegister(CollReg); // CollReg[7..0] bits are: ValuesAfterColl reserved CollPosNotValid CollPos[4:0]
				if (result & 0x20) { // CollPosNotValid
					return STATUS_COLLISION; // Without a valid collision position we cannot continue
				}
				byte collisionPos = result & 0x1F; // Values 0-31, 0 means bit 32.
				if (collisionPos == 0) {
					collisionPos = 32;
				}
				if (collisionPos <= currentLevelKnownBits) { // No progress - should not happen
					return STATUS_INTERNAL_ERROR;
				}
				// Choose the PICC with the bit set.
				currentLevelKnownBits = collisionPos;
				count			= (currentLevelKnownBits - 1) % 8; // The bit to modify
				index			= 1 + (currentLevelKnownBits / 8) + (count ? 1 : 0); // First byte is index 0.
				buffer[index]	|= (1 << count);
			}
			else if (result != STATUS_OK) {
				return result;
			}
			else { // STATUS_OK
				if (currentLevelKnownBits >= 32) { // This was a SELECT.
					selectDone = true; // No more anticollision
					// We continue below outside the while.
				}
				else { // This was an ANTICOLLISION.
					// We now have all 32 bits of the UID in this Cascade Level
					currentLevelKnownBits = 32;
					// Run loop again to do the SELECT.
				}
			}
		} // End of while (!selectDone)

		// We do not check the CBB - it was constructed by us above.

		// Copy the found UID bytes from buffer[] to uid->uidByte[]
		index			= (buffer[2] == PICC_CMD_CT) ? 3 : 2; // source index in buffer[]
		bytesToCopy		= (buffer[2] == PICC_CMD_CT) ? 3 : 4;
		for (count = 0; count < bytesToCopy; count++) {
			uid->uidByte[uidIndex + count] = buffer[index++];
		}

		// Check response SAK (Select Acknowledge)
		if (responseLength != 3 || txLastBits != 0) { // SAK must be exactly 24 bits (1 byte + CRC_A).
			return STATUS_ERROR;
		}
		// Verify CRC_A - do our own calculation and store the control in buffer[2..3] - those bytes are not needed anymore.
		result = PCD_CalculateCRC(responseBuffer, 1, &buffer[2]);
		if (result != STATUS_OK) {
			return result;
		}
		if ((buffer[2] != responseBuffer[1]) || (buffer[3] != responseBuffer[2])) {
			return STATUS_CRC_WRONG;
		}
		if (responseBuffer[0] & 0x04) { // Cascade bit set - UID not complete yes
			cascadeLevel++;
		}
		else {
			uidComplete = true;
			uid->sak = responseBuffer[0];
		}
	} // End of while (!uidComplete)

	// Set correct uid->size
	uid->size = 3 * cascadeLevel + 1;

	return STATUS_OK;
} // End PICC_Select()
*/





/**
 * Performs the "magic sequence" needed to get Chinese UID changeable
 * Mifare cards to allow writing to sector 0, where the card UID is stored.
 *
 * Note that you do not need to have selected the card through REQA or WUPA,
 * this sequence works immediately when the card is in the reader vicinity.
 * This means you can use this method even on "bricked" cards that your reader does
 * not recognise anymore (see MFRC522::MIFARE_UnbrickUidSector).
 *
 * Of course with non-bricked devices, you're free to select them before calling this function.
 */
/*
bool MFRC522::MIFARE_OpenUidBackdoor(bool logErrors) {
	// Magic sequence:
	// > 50 00 57 CD (HALT + CRC)
	// > 40 (7 bits only)
	// < A (4 bits only)
	// > 43
	// < A (4 bits only)
	// Then you can write to sector 0 without authenticating

	PICC_HaltA(); // 50 00 57 CD

	byte cmd = 0x40;
	byte validBits = 7; // Our command is only 7 bits. After receiving card response, this will contain amount of valid response bits.
	byte response[32]; // Card's response is written here
	byte received;
	byte status = PCD_TransceiveData(&cmd, (byte)1, response, &received, &validBits, (byte)0, false); // 40
	if(status != STATUS_OK) {
		if(logErrors) {
			Serial.println(F("Card did not respond to 0x40 after HALT command. Are you sure it is a UID changeable one?"));
			Serial.print(F("Error name: "));
			Serial.println(GetStatusCodeName(status));
		}
		return false;
	}
	if (received != 1 || response[0] != 0x0A) {
		if (logErrors) {
			Serial.print(F("Got bad response on backdoor 0x40 command: "));
			Serial.print(response[0], HEX);
			Serial.print(F(" ("));
			Serial.print(validBits);
			Serial.print(F(" valid bits)\r\n"));
		}
		return false;
	}

	cmd = 0x43;
	validBits = 8;
	status = PCD_TransceiveData(&cmd, (byte)1, response, &received, &validBits, (byte)0, false); // 43
	if(status != STATUS_OK) {
		if(logErrors) {
			Serial.println(F("Error in communication at command 0x43, after successfully executing 0x40"));
			Serial.print(F("Error name: "));
			Serial.println(GetStatusCodeName(status));
		}
		return false;
	}
	if (received != 1 || response[0] != 0x0A) {
		if (logErrors) {
			Serial.print(F("Got bad response on backdoor 0x43 command: "));
			Serial.print(response[0], HEX);
			Serial.print(F(" ("));
			Serial.print(validBits);
			Serial.print(F(" valid bits)\r\n"));
		}
		return false;
	}

	// You can now write to sector 0 without authenticating!
	return true;
} // End MIFARE_OpenUidBackdoor()

*/

/**
 * Reads entire block 0, including all manufacturer data, and overwrites
 * that block with the new UID, a freshly calculated BCC, and the original
 * manufacturer data.
 *
 * It assumes a default KEY A of 0xFFFFFFFFFFFF.
 * Make sure to have selected the card before this function is called.
 */
/*
bool MFRC522::MIFARE_SetUid(byte *newUid, byte uidSize, bool logErrors) {

	// UID + BCC byte can not be larger than 16 together
	if (!newUid || !uidSize || uidSize > 15) {
		if (logErrors) {
			Serial.println(F("New UID buffer empty, size 0, or size > 15 given"));
		}
		return false;
	}

	// Authenticate for reading
	MIFARE_Key key = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	byte status = PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, (byte)1, &key, &uid);
	if (status != STATUS_OK) {

		if (status == STATUS_TIMEOUT) {
			// We get a read timeout if no card is selected yet, so let's select one

			// Wake the card up again if sleeping
//			  byte atqa_answer[2];
//			  byte atqa_size = 2;
//			  PICC_WakeupA(atqa_answer, &atqa_size);

			if (!PICC_IsNewCardPresent() || !PICC_ReadCardSerial()) {
				Serial.println(F("No card was previously selected, and none are available. Failed to set UID."));
				return false;
			}

			status = PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, (byte)1, &key, &uid);
			if (status != STATUS_OK) {
				// We tried, time to give up
				if (logErrors) {
					Serial.println(F("Failed to authenticate to card for reading, could not set UID: "));
					Serial.println(GetStatusCodeName(status));
				}
				return false;
			}
		}
		else {
			if (logErrors) {
				Serial.print(F("PCD_Authenticate() failed: "));
				Serial.println(GetStatusCodeName(status));
			}
			return false;
		}
	}

	// Read block 0
	byte block0_buffer[18];
	byte byteCount = sizeof(block0_buffer);
	status = MIFARE_Read((byte)0, block0_buffer, &byteCount);
	if (status != STATUS_OK) {
		if (logErrors) {
			Serial.print(F("MIFARE_Read() failed: "));
			Serial.println(GetStatusCodeName(status));
			Serial.println(F("Are you sure your KEY A for sector 0 is 0xFFFFFFFFFFFF?"));
		}
		return false;
	}

	// Write new UID to the data we just read, and calculate BCC byte
	byte bcc = 0;
	for (int i = 0; i < uidSize; i++) {
		block0_buffer[i] = newUid[i];
		bcc ^= newUid[i];
	}

	// Write BCC byte to buffer
	block0_buffer[uidSize] = bcc;

	// Stop encrypted traffic so we can send raw bytes
	PCD_StopCrypto1();

	// Activate UID backdoor
	if (!MIFARE_OpenUidBackdoor(logErrors)) {
		if (logErrors) {
			Serial.println(F("Activating the UID backdoor failed."));
		}
		return false;
	}

	// Write modified block 0 back to card
	status = MIFARE_Write((byte)0, block0_buffer, (byte)16);
	if (status != STATUS_OK) {
		if (logErrors) {
			Serial.print(F("MIFARE_Write() failed: "));
			Serial.println(GetStatusCodeName(status));
		}
		return false;
	}

	// Wake the card up again
	byte atqa_answer[2];
	byte atqa_size = 2;
	PICC_WakeupA(atqa_answer, &atqa_size);

	return true;
}
*/

/**
 * Resets entire sector 0 to zeroes, so the card can be read again by readers.
 */
/*
bool MFRC522::MIFARE_UnbrickUidSector(bool logErrors) {
	MIFARE_OpenUidBackdoor(logErrors);

	byte block0_buffer[] = {0x01, 0x02, 0x03, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	// Write modified block 0 back to card
	byte status = MIFARE_Write((byte)0, block0_buffer, (byte)16);
	if (status != STATUS_OK) {
		if (logErrors) {
			Serial.print(F("MIFARE_Write() failed: "));
			Serial.println(GetStatusCodeName(status));
		}
		return false;
	}
}

*/









/**
 * Dumps memory contents of a MIFARE Classic PICC.
 * On success the PICC is halted after dumping the data.
 */
 /*
void MFRC522::PICC_DumpMifareClassicToSerial(	Uid *uid,		///< Pointer to Uid struct returned from a successful PICC_Select().
												byte piccType,	///< One of the PICC_Type enums.
												MIFARE_Key *key	///< Key A used for all sectors.
											) {
	byte no_of_sectors = 0;
	switch (piccType) {
		case PICC_TYPE_MIFARE_MINI:
			// Has 5 sectors * 4 blocks/sector * 16 bytes/block = 320 bytes.
			no_of_sectors = 5;
			break;

		case PICC_TYPE_MIFARE_1K:
			// Has 16 sectors * 4 blocks/sector * 16 bytes/block = 1024 bytes.
			no_of_sectors = 16;
			break;

		case PICC_TYPE_MIFARE_4K:
			// Has (32 sectors * 4 blocks/sector + 8 sectors * 16 blocks/sector) * 16 bytes/block = 4096 bytes.
			no_of_sectors = 40;
			break;

		default: // Should not happen. Ignore.
			break;
	}

	// Dump sectors, highest address first.
	if (no_of_sectors) {
		Serial.println(F("Sector Block   0  1  2  3   4  5  6  7   8  9 10 11  12 13 14 15  AccessBits"));
		for (int8_t i = no_of_sectors - 1; i >= 0; i--) {
			PICC_DumpMifareClassicSectorToSerial(uid, key, i);
		}
	}
	PICC_HaltA(); // Halt the PICC before stopping the encrypted session.
	PCD_StopCrypto1();
} // End PICC_DumpMifareClassicToSerial()

*/

/**
 * Dumps memory contents of a sector of a MIFARE Classic PICC.
 * Uses PCD_Authenticate(), MIFARE_Read() and PCD_StopCrypto1.
 * Always uses PICC_CMD_MF_AUTH_KEY_A because only Key A can always read the sector trailer access bits.
 */
/*
void MFRC522::PICC_DumpMifareClassicSectorToSerial(Uid *uid,			///< Pointer to Uid struct returned from a successful PICC_Select().
													MIFARE_Key *key,	///< Key A for the sector.
													byte sector			///< The sector to dump, 0..39.
													) {
	byte status;
	byte firstBlock;		// Address of lowest address to dump actually last block dumped)
	byte no_of_blocks;		// Number of blocks in sector
	bool isSectorTrailer;	// Set to true while handling the "last" (ie highest address) in the sector.

	// The access bits are stored in a peculiar fashion.
	// There are four groups:
	//		g[3]	Access bits for the sector trailer, block 3 (for sectors 0-31) or block 15 (for sectors 32-39)
	//		g[2]	Access bits for block 2 (for sectors 0-31) or blocks 10-14 (for sectors 32-39)
	//		g[1]	Access bits for block 1 (for sectors 0-31) or blocks 5-9 (for sectors 32-39)
	//		g[0]	Access bits for block 0 (for sectors 0-31) or blocks 0-4 (for sectors 32-39)
	// Each group has access bits [C1 C2 C3]. In this code C1 is MSB and C3 is LSB.
	// The four CX bits are stored together in a nible cx and an inverted nible cx_.
	byte c1, c2, c3;		// Nibbles
	byte c1_, c2_, c3_;		// Inverted nibbles
	bool invertedError;		// True if one of the inverted nibbles did not match
	byte g[4];				// Access bits for each of the four groups.
	byte group;				// 0-3 - active group for access bits
	bool firstInGroup;		// True for the first block dumped in the group

	// Determine position and size of sector.
	if (sector < 32) { // Sectors 0..31 has 4 blocks each
		no_of_blocks = 4;
		firstBlock = sector * no_of_blocks;
	}
	else if (sector < 40) { // Sectors 32-39 has 16 blocks each
		no_of_blocks = 16;
		firstBlock = 128 + (sector - 32) * no_of_blocks;
	}
	else { // Illegal input, no MIFARE Classic PICC has more than 40 sectors.
		return;
	}

	// Dump blocks, highest address first.
	byte byteCount;
	byte buffer[18];
	byte blockAddr;
	isSectorTrailer = true;
	for (int8_t blockOffset = no_of_blocks - 1; blockOffset >= 0; blockOffset--) {
		blockAddr = firstBlock + blockOffset;
		// Sector number - only on first line
		if (isSectorTrailer) {
			if(sector < 10)
				Serial.print(F("   ")); // Pad with spaces
			else
				Serial.print(F("  ")); // Pad with spaces
			Serial.print(sector);
			Serial.print(F("   "));
		}
		else {
			Serial.print(F("       "));
		}
		// Block number
		if(blockAddr < 10)
			Serial.print(F("   ")); // Pad with spaces
		else {
			if(blockAddr < 100)
				Serial.print(F("  ")); // Pad with spaces
			else
				Serial.print(F(" ")); // Pad with spaces
		}
		Serial.print(blockAddr);
		Serial.print(F("  "));
		// Establish encrypted communications before reading the first block
		if (isSectorTrailer) {
			status = PCD_Authenticate(PICC_CMD_MF_AUTH_KEY_A, firstBlock, key, uid);
			if (status != STATUS_OK) {
				Serial.print(F("PCD_Authenticate() failed: "));
				Serial.println(GetStatusCodeName(status));
				return;
			}
		}
		// Read block
		byteCount = sizeof(buffer);
		status = MIFARE_Read(blockAddr, buffer, &byteCount);
		if (status != STATUS_OK) {
			Serial.print(F("MIFARE_Read() failed: "));
			Serial.println(GetStatusCodeName(status));
			continue;
		}
		// Dump data
		for (byte index = 0; index < 16; index++) {
			if(buffer[index] < 0x10)
				Serial.print(F(" 0"));
			else
				Serial.print(F(" "));
			Serial.print(buffer[index], HEX);
			if ((index % 4) == 3) {
				Serial.print(F(" "));
			}
		}
		// Parse sector trailer data
		if (isSectorTrailer) {
			c1  = buffer[7] >> 4;
			c2  = buffer[8] & 0xF;
			c3  = buffer[8] >> 4;
			c1_ = buffer[6] & 0xF;
			c2_ = buffer[6] >> 4;
			c3_ = buffer[7] & 0xF;
			invertedError = (c1 != (~c1_ & 0xF)) || (c2 != (~c2_ & 0xF)) || (c3 != (~c3_ & 0xF));
			g[0] = ((c1 & 1) << 2) | ((c2 & 1) << 1) | ((c3 & 1) << 0);
			g[1] = ((c1 & 2) << 1) | ((c2 & 2) << 0) | ((c3 & 2) >> 1);
			g[2] = ((c1 & 4) << 0) | ((c2 & 4) >> 1) | ((c3 & 4) >> 2);
			g[3] = ((c1 & 8) >> 1) | ((c2 & 8) >> 2) | ((c3 & 8) >> 3);
			isSectorTrailer = false;
		}

		// Which access group is this block in?
		if (no_of_blocks == 4) {
			group = blockOffset;
			firstInGroup = true;
		}
		else {
			group = blockOffset / 5;
			firstInGroup = (group == 3) || (group != (blockOffset + 1) / 5);
		}

		if (firstInGroup) {
			// Print access bits
			Serial.print(F(" [ "));
			Serial.print((g[group] >> 2) & 1, DEC); Serial.print(F(" "));
			Serial.print((g[group] >> 1) & 1, DEC); Serial.print(F(" "));
			Serial.print((g[group] >> 0) & 1, DEC);
			Serial.print(F(" ] "));
			if (invertedError) {
				Serial.print(F(" Inverted access bits did not match! "));
			}
		}

		if (group != 3 && (g[group] == 1 || g[group] == 6)) { // Not a sector trailer, a value block
			long value = (long(buffer[3])<<24) | (long(buffer[2])<<16) | (long(buffer[1])<<8) | long(buffer[0]);
			Serial.print(F(" Value=0x")); Serial.print(value, HEX);
			Serial.print(F(" Adr=0x")); Serial.print(buffer[12], HEX);
		}
		Serial.println();
	}

	return;
} // End PICC_DumpMifareClassicSectorToSerial()
*/

/**
 * Dumps memory contents of a MIFARE Ultralight PICC.
 */
/*
void MFRC522::PICC_DumpMifareUltralightToSerial() {
	byte status;
	byte byteCount;
	byte buffer[18];
	byte i;

	Serial.println(F("Page  0  1  2  3"));
	// Try the mpages of the original Ultralight. Ultralight C has more pages.
	for (byte page = 0; page < 16; page +=4) { // Read returns data for 4 pages at a time.
		// Read pages
		byteCount = sizeof(buffer);
		status = MIFARE_Read(page, buffer, &byteCount);
		if (status != STATUS_OK) {
			Serial.print(F("MIFARE_Read() failed: "));
			Serial.println(GetStatusCodeName(status));
			break;
		}
		// Dump data
		for (byte offset = 0; offset < 4; offset++) {
			i = page + offset;
			if(i < 10)
				Serial.print(F("  ")); // Pad with spaces
			else
				Serial.print(F(" ")); // Pad with spaces
			Serial.print(i);
			Serial.print(F("  "));
			for (byte index = 0; index < 4; index++) {
				i = 4 * offset + index;
				if(buffer[i] < 0x10)
					Serial.print(F(" 0"));
				else
					Serial.print(F(" "));
				Serial.print(buffer[i], HEX);
			}
			Serial.println();
		}
	}
} // End PICC_DumpMifareUltralightToSerial()

*/
