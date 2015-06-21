/**
 * @file      MFRC522.h
 *
 * @author    Zéro Cool
 * @date      15/06/2015 18:13:32
 * @brief     Bibliothèque de contrôle du lecteur MFRC522
 *
 * @details   Bibliothèque d'utilisation du reader RFID MFRC522 en mode SPI.
 *
 * @note      PCD : Proximity Coupling Device (i.e. Lecteur NXP MFRC522 sans contactes)
 * @note      PICC : Proximity Integrated Circuit Card (i.e. Carte ou Tag utilisant l'interface ISO 14443A)
 *
 * @par
 * Le lecteur de carte et le microcontroller communiquent en mode SPI.@n
 * Le protocole est décrit dans la ([datasheet]http://www.nxp.com/documents/data_sheet/MFRC522.pdf)
 *
 *
 * @par
 * Le lecteur de carte et le tag communiquent par un champ électromagnétique de 13.56MHz.@n
 * Le protocole d'identification s'appuie sur l'ISO/IEC 14443-3 : "Contactless integrated circuit cards -- Proximity cards -- Part 3: Initialization and anticollision".@n
 * Une vesion gratuite du draft final est disponible ([ici]http://wg8.de/wg8n1496_17n3613_Ballot_FCD14443-3.pdf).@n
 * Les détails se trouvent au chapitre 6, Type A – Initialization and anticollision.@n
 *
 * @par
 * Si seulement l'UID du PICC est désiré, alors les documentations précédentes sont sufficantes.@n
 * Pour lire ou écrire un PICC MIFARE, le bon protocol doit être sélectionné.
 *
 * @par
 * Pour les tags "MIFARE Classic", les protocoles sont décrits ici :@n
 *		@li 1K : http://www.nxp.com/documents/data_sheet/MF1S503x.pdf
 * 		@li 4K : http://www.nxp.com/documents/data_sheet/MF1S703x.pdf
 * 		@li Mini : http://www.idcardmarket.com/download/mifare_S20_datasheet.pdf
 *
 * @par
 * Pour les tags "MIFARE Ultralight", les protocoles sont décrits ici :
 *		@li Ultralight : http://www.nxp.com/documents/data_sheet/MF0ICU1.pdf
 * 		@li Ultralight C : http://www.nxp.com/documents/short_data_sheet/MF0ICU2_SDS.pdf
 *
 * @par
 * Résumé des caractéristiques des tags MIFARE :
 * @li MIFARE Classic 1K (MF1S503x) :@n
 * 		Has 16 sectors * 4 blocks/sector * 16 bytes/block = 1024 bytes.@n
 * 		The blocks are numbered 0-63.@n
 * 		Block 3 in each sector is the Sector Trailer. See http://www.nxp.com/documents/data_sheet/MF1S503x.pdf sections 8.6 and 8.7 :@n
 * 				- Bytes 0-5:   Key A@n
 * 				- Bytes 6-8:   Access Bits@n
 * 				- Bytes 9:     User data@n
 * 				- Bytes 10-15: Key B (or user data)@n
 * 		Block 0 is read-only manufacturer data.@n
 * 		To access a block, an authentication using a key from the block's sector must be performed first.@n
 * 		Example: To read from block 10, first authenticate using a key from sector 3 (blocks 8-11).@n
 * 		All keys are set to FFFFFFFFFFFFh at chip delivery.@n
 * 		Warning: Please read section 8.7 "Memory Access". It includes this text: if the PICC detects a format violation the whole sector is irreversibly blocked.@n
 *		To use a block in "value block" mode (for Increment/Decrement operations) you need to change the sector trailer. Use PICC_SetAccessBits() to calculate the bit patterns.
 *
 * @li MIFARE Classic 4K (MF1S703x) :@n
 * 		Has (32 sectors * 4 blocks/sector + 8 sectors * 16 blocks/sector) * 16 bytes/block = 4096 bytes.@n
 * 		The blocks are numbered 0-255.@n
 * 		The last block in each sector is the Sector Trailer like above.
 *
 * @li MIFARE Classic Mini (MF1 IC S20) :@n
 * 		Has 5 sectors * 4 blocks/sector * 16 bytes/block = 320 bytes.@n
 * 		The blocks are numbered 0-19.@n
 * 		The last block in each sector is the Sector Trailer like above.
 *
 * @li MIFARE Ultralight (MF0ICU1) :@n
 * 		Has 16 pages of 4 bytes = 64 bytes.@n
 * 		Pages 0 + 1 is used for the 7-byte UID.@n
 * 		Page 2 contains the last check digit for the UID, one byte manufacturer internal data, and the lock bytes (see http://www.nxp.com/documents/data_sheet/MF0ICU1.pdf section 8.5.2)@n
 * 		Page 3 is OTP, One Time Programmable bits. Once set to 1 they cannot revert to 0.@n
 * 		Pages 4-15 are read/write unless blocked by the lock bytes in page 2.
 *
 * @li MIFARE Ultralight C (MF0ICU2) :@n
 * 		Has 48 pages of 4 bytes = 192 bytes.@n
 * 		Pages 0 + 1 is used for the 7-byte UID.@n
 * 		Page 2 contains the last check digit for the UID, one byte manufacturer internal data, and the lock bytes (see http://www.nxp.com/documents/data_sheet/MF0ICU1.pdf section 8.5.2)@n
 * 		Page 3 is OTP, One Time Programmable bits. Once set to 1 they cannot revert to 0.@n
 * 		Pages 4-39 are read/write unless blocked by the lock bytes in page 2.@n
 * 		Page 40 Lock bytes@n
 * 		Page 41 16 bit one way counter@n
 * 		Pages 42-43 Authentication configuration@n
 * 		Pages 44-47 Authentication key

 * @par
 * Branchements préconisés :
 *
 * ----------------------------------
 *            MFRC522
 *            Reader/PCD
 * Signal     Pin            Variable
 * ----------------------------------
 * RST/Reset  RST            MFRC522_RESET_PIN
 * SPI SS     NSS            SPI_SS_PIN
 *            ou SDA(SS)
 * SPI MOSI   MOSI           SPI_MOSI_PIN
 * SPI MISO   MISO           SPI_MISO_PIN
 * SPI SCK    SCK            SPI_SCK_PIN
 *
 * @warning   La broche RST (ou Reset) doit se trouver sur le même port que la
 *            broche SPI_SS_PIN
 *
 * @todo      Exemple d'utilisation à faire
 *
 * @ingroup   RFID
 */

#ifndef _MFRC522_H_
#define _MFRC522_H_

#if !defined(MFRC522_RESET_PIN)
#  error "MFRC522.h requires MFRC522_RESET_PIN to be defined"
#endif

#if !defined(MFRC522_DDR)
#  error "SPI_master.h requires MFRC522_DDR to be defined"
#endif

#if !defined(MFRC522_PORT)
#  error "SPI_master.h requires MFRC522_PORT to be defined"
#endif

/**
 * @brief     Codes retour des fonctions de la bibliothèque
 * @details   Enumération des codes retous possibles de la bibliothèque MFRC522
 */
typedef enum
{
  MFRC522_STATUS_OK             = 1,  /**< Success */
  MFRC522_STATUS_ERROR          = 2,  /**< Erreur de communication */
  MFRC522_STATUS_COLLISION      = 3,  /**< Collision détectée */
  MFRC522_STATUS_TIMEOUT        = 4,  /**< Timeout */
  MFRC522_STATUS_NO_ROOM        = 5,  /**< Un buffer n'est pas assez grand */
  MFRC522_STATUS_INTERNAL_ERROR = 6,  /**< Erreur interne */
  MFRC522_STATUS_INVALID        = 7,  /**< Argument non valide */
  MFRC522_STATUS_CRC_WRONG      = 8,  /**< Le CRC_A ne correspond pas */
  MFRC522_STATUS_MIFARE_NACK    = 9,  /**< Une carte MIFARE a répondu par NAK */
  MFRC522_STATUS_PROPRIETARY_ANTICOLLISION = 10,   /**< Mode anticllision propriétaire */
  MFRC522_STATUS_BCC_ERROR      = 11  /**< Erreur de contrôle de la valeur du BCC */
} MFRC522_STATUS;

/**
 * @brief     Gain de l'antenne du MFRC522
 * @details   Enumération des gains possible pour l'antenne du MFRC522. Cf. 9.3.36 de la datasheet
 */
typedef enum
{
  MFRC522_RX_GAIN_18dB          = 0x00, /**< 000b - 18 dB, minimum */
  MFRC522_RX_GAIN_23dB          = 0x01, /**< 001b - 23 dB */
  MFRC522_RX_GAIN_18dB_2        = 0x02, /**< 010b - 18 dB, it seems 010b is a duplicate for 000b */
  MFRC522_RX_GAIN_23dB_2        = 0x03, /**< 011b - 23 dB, it seems 011b is a duplicate for 001b */
  MFRC522_RX_GAIN_33dB          = 0x04, /**< 100b - 33 dB, average, and typical default */
  MFRC522_RX_GAIN_38dB          = 0x05, /**< 101b - 38 dB */
  MFRC522_RX_GAIN_43dB          = 0x06, /**< 110b - 43 dB */
  MFRC522_RX_GAIN_48dB          = 0x07, /**< 111b - 48 dB, maximum */
  MFRC522_RX_GAIN_min           = 0x00, /**< 000b - 18 dB, minimum, convenience for RxGain_18dB */
  MFRC522_RX_GAIN_avg           = 0x04, /**< 100b - 33 dB, average, convenience for RxGain_33dB */
  MFRC522_RX_GAIN_max           = 0x07  /**< 111b - 48 dB, maximum, convenience for RxGain_48dB */
} MFRC522_RX_GAIN;

/**
 * @brief     Types de PICC ou de TAG
 * @details   Enumération des types de PICC. Cela permet d'en déduire les protocoles à utiliser par la suite.
 */
typedef enum
{
  MFRC522_PICC_TYPE_UNKNOWN         = 0,  /**< Type de PICC inconnu */
  MFRC522_PICC_TYPE_ISO_14443_4     = 1,  /**< PICC compatible avec la norme ISO/IEC 14443-4 */
  MFRC522_PICC_TYPE_ISO_18092       = 2,  /**< PICC compatible avec la norme ISO/IEC 18092 (NFC) */
  MFRC522_PICC_TYPE_MIFARE_MINI     = 3,  /**< Protocole MIFARE Classic, 320 octets */
  MFRC522_PICC_TYPE_MIFARE_1K       = 4,  /**< Protocole MIFARE Classic, 1 Ko */
  MFRC522_PICC_TYPE_MIFARE_4K       = 5,  /**< Protocole MIFARE Classic, 4 Ko */
  MFRC522_PICC_TYPE_MIFARE_UL       = 6,  /**< MIFARE Ultralight ou Ultralight C */
  MFRC522_PICC_TYPE_MIFARE_PLUS     = 7,  /**< MIFARE Plus */
  MFRC522_PICC_TYPE_TNP3XXX         = 8,  /**< Mentionné uniquement in NXP AN 10833 : MIFARE Type Identification Procedure */
  MFRC522_PICC_TYPE_MIFARE_DESFIRE  = 9,  /**< MMIFARE Desfire */
  MFRC522_PICC_TYPE_JCOP30          = 10, /**< Protocole JEWEL */
  MFRC522_PICC_TYPE_MIFARE_4K_EMUL  = 11, /**< Emulation MIFARE 4K */
  MFRC522_PICC_TYPE_MIFARE_1K_INFINEON = 12, /**< Infineon MIFARE 1K */
  MFRC522_PICC_TYPE_MPCOS           = 13, /**< Protocole MPCOS */
  MFRC522_PICC_TYPE_NOT_COMPLETE    = 255 /**< SAK indique que l'UID n'est pas complet */
} MFRC522_PICC_TYPE;

/**
 * @brief     Types d'authentifications
 * @details   Enumération des types d'authentification possibles des blocs
 */
typedef enum
{
  MFRC522_AUTH_KEY_A            = 0,    /**< Clef d'authenfification A */
  MFRC522_AUTH_KEY_B            = 1     /**< Clef d'authenfification A */
} MFRC522_AUTH_KEY;

/**
 * @brief     Structure contenant l'UID d'un PICC
 */
typedef struct
{
  uint16_t atqa;         /**< réponse ATQA */
  uint8_t uid[10];       /**< UID */
  uint8_t uidsize;       /**< Nombre d'octets du UID. 4, 7 ou 10. */
  uint8_t sak;           /**< Octet SAK (Select acknowledge) retourné par le PICC après une sélection réussie. */
} MFRC522_PICC_UID;

#define MFRC522_AUTH_KEY_SIZE        6

/**
 * @brief     Structure d'une clef Crypto1 MIFARE
 */
typedef struct
{
  uint8_t keybyte[MFRC522_AUTH_KEY_SIZE];     /**< Clef MIFARE Crypto1 */
} MFRC522_MIFARE_KEY;

/**
 * @brief       Initialise l'interface SPI
 *
 * @note        Cette méthode n'est à appeler qu'une seule fois
 */
void MFRC522_PCD_Init(void);

/**
 * @brief     Réinitialise le composant MFRC522
 * @details   Effectue une réinitialisation software du composant MFRC522
 */
void MFRC522_PCD_Reset(void);

/**
 * @brief     Allume l'antenne
 * @details   Met en marche l'antenne du composant MFRC522
 */
void MFRC522_PCD_AntennaOn(void);

/**
 * @brief     Eteint l'antenne
 * @details   Eteint l'antenne du composant MFRC522
 */
void MFRC522_PCD_AntennaOff(void);

/**
 * @brief     Obtient le gain de l'antenne
 * @details   Récupère le gain de l'antenne du composant MFRC522
 *
 * @return    Valeur du gain de l'antenne
 */
 MFRC522_RX_GAIN MFRC522_PCD_GetAntennaGain(void);

/**
 * @brief     Définit le gain de l'antenne
 * @details   Définit le gain de l'antenne du composant MFRC522
 */
void MFRC522_PCD_SetAntennaGain(MFRC522_RX_GAIN gain);

/**
 * @brief     Test automatique du MFRC522
 * @details   Effectue un test du firmware du MFRC522
 *
 * @return    MFRC522_STATUS_OK si le test est OK.
 *            MFRC522_STATUS_ERROR en cas d'erreur.
 */
MFRC522_STATUS MFRC522_PCD_PerformSelfTest(void);

/**
 * @brief     Exécute la commande de transfert dans la FIFO du MFRC522
 * @details   Exécute la commande PCD_CMD_Transceive qui envoi les données de la FIFO
 *            vers l'antenne
 *
 * @note      La validation CRC ne peut être faite que si @c backData et @c backLen sont spécifiés
 *
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
MFRC522_STATUS MFRC522_PCD_TransceiveData(uint8_t * sendData, uint8_t sendLen, uint8_t * backData,
  uint8_t * backLen, uint8_t * validBits, uint8_t rxAlign, uint8_t checkCRC);

/**
 * @brief     Envoi une commande Request type A.
 * @details   Invite le PICC de passer de l'état IDLE à l'état READY et se prépare pour l'anticollision ou la sélection.@n
 *            Frame de 7 bits.@n
 *
 * @warning   Quand deux PICC sont dans le champ au même moment, un MFRC522_STATUS_TIMEOUT sera probablement déclenché.
 *
 * @param     [in,out]  uid         Pointeur sur une structure de type MFRC522_PICC_UID.@n
 *                                  La zone ATQA est alimentée en sortie
 *
 * @warning   Aucun dépassement de capacité n'est testé.
 *
 * @return    MFRC522_STATUS_OK si le test est OK.
 *            MFRC522_STATUS_* en cas d'erreur.
 */
MFRC522_STATUS MFRC522_PICC_RequestA(MFRC522_PICC_UID * uid);

/**
 * @brief     Envoi une commande Wakeup type A.
 * @details   Invite le PICC de passer de l'état IDLE et HALT à l'état READY et se prépare pour l'anticollision ou la sélection.@n
 *            Frame de 7 bits.@n
 *
 * @warning   Quand deux PICC sont dans le champ au même moment, un MFRC522_STATUS_TIMEOUT sera probablement déclenché.
 *
 * @param     [in,out]  uid         Pointeur sur une structure de type MFRC522_PICC_UID.@n
 *                                  La zone ATQA est alimentée en sortie
 *
 * @warning   Aucun dépassement de capacité n'est testé.
 *
 * @return    MFRC522_STATUS_OK si le test est OK.
 *            MFRC522_STATUS_* en cas d'erreur.
 */
MFRC522_STATUS MFRC522_PICC_WakeupA(MFRC522_PICC_UID * uid);

/**
 * @brief     Sélectionne un PICC
 * @details   Envoi une command SELECT/ANTICOLLISION à un seul PICC.
 *
 * @warning   Avant d'utiliser cette fonction, le PICC doit être placé dans un état READY (@see MFRC522_PICC_RequestA ou @see MFRC522_PICC_WakeupA)
 * @note      En cas de succès :
 *            @li Le PICC choisi passe à l'état ACTIVE et tous les autres PICC retourne à l'état IDLE/HALT (Figure 7 of the ISO/IEC 14443-3 draft.)
 *            @li La taille du UID et la valeur du PICC choisie est retourné dans la structure @c *uid avec le code SAK retour.
 *
 * @note      L'UID d'un PICC est constitué de 4, 7 ou 10 octets :@n
 *            Seul 4 octets sont spécifiés dans la commande SELECT, donc, pour les UID de 7 et 10 octets, il faut respectivement 2 et 3 itérations (cascade level).
 *
 * @param     [in,out]  uid         Pointeur sur une structure de type MFRC522_PICC_UID.@n
 *                                  Permet de spécifier en entrée un UID connu.@n
 *                                  Alimenté en sortie avec l'UID du PICC sélectionné.
 *
 * @warning   Aucun dépassement de capacité n'est testé.
 *
 * @return    MFRC522_STATUS_OK si le test est OK.
 *            MFRC522_STATUS_* en cas d'erreur.
 */
MFRC522_STATUS MFRC522_PICC_Select(MFRC522_PICC_UID * uid);

/**
 * @brief     Envoi une commande Halt type A.
 * @details   Invite le PICC de passer de l'état ACTIVE à l'état HALT.
 * @return    MFRC522_STATUS_OK si le test est OK.
 *            MFRC522_STATUS_* en cas d'erreur.
 */
MFRC522_STATUS MFRC522_PICC_HaltA(void);

/////////////////////////////////////////////////////////////////////////////////////
// Functions for communicating with MIFARE PICCs
/////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief     Effectue l'authentification MIFARE sur un block spécifié
 * @details   Effectue une authentification MIFARE pour sécuriser les accès MIFARE Mini, MIFARE 1K et MIFARE 4K.@n
 *            L'authentification est décrite dans le paragraphe 10.3.1.9 de la datasheet et dans la section 10.1 de la datasheet http://www.nxp.com/documents/data_sheet/MF1S503x.pdf pour l'utilisation avec une carte MIFARE Classic PICCs.
 *
 * @note      Le PICC doit être sélectionné avant d'utiliser cette focntion.
 * @note      Il convient d'appeler MFRC522_MIFARE_StopScrypto1() après une communication authentifiée avec le PICC (Sinon plus aucune communication n'est possible).
 * @note      Toutes les clefs sont positionnées à 0xFFFFFFFFFFFF en sortie d'usine.
 *
 * @param     [in]      keytype        Type de clef à utiliser pour l'authentification
 * @param     [in]      blockAddr      Adresse du bloc sur lequel s'authentifier
 * @param     [in]      key            Clef à utiliser pour l'authentification
 * @param     [in]      uid            Carte sur laquelle effectuer l'authentification
 *
 * @warning   Aucun dépassement de capacité n'est testé.
 *
 * @return    MFRC522_STATUS_OK si le test est OK.
 *            MFRC522_STATUS_* en cas d'erreur (Timeout si la clef fournie n'est pas correcte).
 */
MFRC522_STATUS MFRC522_MIFARE_Authenticate(MFRC522_AUTH_KEY keytype, uint8_t blockAddr, MFRC522_MIFARE_KEY * key, MFRC522_PICC_UID * uid);

/**
 * @brief     Stop l'utilisation de l'authentification MIFARE
 * @details   Stop l'utilisation de l'authentification MIFARE
 */
void MFRC522_MIFARE_StopCrypto1(void);

/**
 * @brief     Lit 16 octets (+2 CRC_A) du PICC ACTIF
 * @details   Lit 16 octets (+2 CRC_A) du PICC ACTIF. La focntion vérifie le CRC_A avant de retourner @c MFRC511_STATUS_OK
 *
 * @note      Pour les cartes MIFARE Classic, le secteur contenant le bloc à lire doit avoir été authentifié avant d'utiliser cette fonction.
 *
 * @param     [in]      blockAddr   Adresse du bloc à lire
 * @param     [out]     buffer      Buffer où seront stocké les données lues
 * @param     [in,out]  bufferSize  Taille du buffer (doit être au moins de 18 octets). Le CRC_A est également retourné
 *
 * @warning   Aucun dépassement de capacité n'est testé.
 *
 * @return    MFRC522_STATUS_OK si le test est OK.
 *            MFRC522_STATUS_* en cas d'erreur.
 */
MFRC522_STATUS MFRC522_MIFARE_Read(uint8_t blockAddr, uint8_t * buffer, uint8_t * bufferSize);

/**
 * @brief     Ecrit 16 octets sur le PICC ACTIF
 * @details   Ecrit 16 octets sur le PICC ACTIF
 *
 * @note      Pour les cartes MIFARE Classic, le secteur contenant le bloc à écrire doit avoir été authentifié avant d'utiliser cette fonction.
 * @note      Pour les cartes MIFARE Ultralight, cette opération est nommée "COMPATIBILITY WRITE". Même Si 16 octets sont transmis au PICC, seuls
 *            les 4 premiers octets sont réellement écrit à l'adresse spécifiée. Il est recommandé de placer tous les octets suivant à la valeur 0x00.
 *
 * @param     [in]      blockAddr   Adresse du bloc à écrire
 * @param     [in]      buffer      Buffer des 16 octets à écrire
 * @param     [in]      bufferSize  Taille du buffer (doit être au moins de 16 octets - Seul les 16 premiers octets sont envoyés au PICC)
 *
 * @warning   Aucun dépassement de capacité n'est testé.
 *
 * @return    MFRC522_STATUS_OK si le test est OK.
 *            MFRC522_STATUS_* en cas d'erreur.
 */
MFRC522_STATUS MFRC522_MIFARE_Write(uint8_t blockAddr, uint8_t * buffer, uint8_t bufferSize);

/**
 * @brief     Soustrait le delta de la valeur stockée à l'adresse du bloc et stock le résultat dans la mémoire volatile.
 * @details   MIFARE Decrement soustrait le delta passé en paramètre de la valeur présente à l'adresse fournie en entrée.
 *            Le résultat est stocké dans la mémoire volatile. Utiliser la fonction @c MFRC522_MIFARE_Transfer() pour
 *            stoker le résultat de la mémoire volatile dans un block particulier.
 *
 * @note      Pour les PICC MIFARE Classic, le secteur contenant le block doit être authentifié avant d'appeler cette fonction.@n
 *            Cette restriction n'est que pour les blocks en mode "valide" (bits d'accès [C1 C2 C3] = [110] ou [001]).
 *
 * @param     [in]      blockAddr   Addresse du block à soustraire
 * @param     [in]      delta       Valeur à soustraire
 *
 * @return    MFRC522_STATUS_OK si le test est OK.
 *            MFRC522_STATUS_* en cas d'erreur.
 */
MFRC522_STATUS MFRC522_MIFARE_Decrement(uint8_t blockAddr, int32_t delta);

/**
 * @brief     Ajoute le delta de la valeur stockée à l'adresse du bloc et stock le résultat dans la mémoire volatile.
 * @details   MIFARE Increment ajoute le delta passé en paramètre de la valeur présente à l'adresse fournie en entrée.
 *            Le résultat est stocké dans la mémoire volatile. Utiliser la fonction @c MFRC522_MIFARE_Transfer() pour
 *            stoker le résultat de la mémoire volatile dans un block particulier.
 *
 * @note      Pour les PICC MIFARE Classic, le secteur contenant le block doit être authentifié avant d'appeler cette fonction.@n
 *            Cette restriction n'est que pour les blocks en mode "valide" (bits d'accès [C1 C2 C3] = [110] ou [001]).
 *
 * @param     [in]      blockAddr   Addresse du block à incrémenter
 * @param     [in]      delta       Valeur à incrémenter
 *
 * @return    MFRC522_STATUS_OK si le test est OK.
 *            MFRC522_STATUS_* en cas d'erreur.
 */
MFRC522_STATUS MFRC522_MIFARE_Increment(uint8_t blockAddr, int32_t delta);

/**
 * @brief     Place la valeur stockée à l'adresse d'un bloc dans la mémoire volatile.
 * @details   MIFARE Restore place la valeur présente à l'adresse d'un block fournie en entrée dans la mémoire volatile.
 *            Utiliser la fonction @c MFRC522_MIFARE_Transfer() pour stoker le résultat de la mémoire volatile dans un block particulier.
 *
 * @note      Pour les PICC MIFARE Classic, le secteur contenant le block doit être authentifié avant d'appeler cette fonction.@n
 *            Cette restriction n'est que pour les blocks en mode "valide" (bits d'accès [C1 C2 C3] = [110] ou [001]).
 *
 * @param     [in]      blockAddr   Addresse du block à utiliser
 *
 * @return    MFRC522_STATUS_OK si le test est OK.
 *            MFRC522_STATUS_* en cas d'erreur.
 */
MFRC522_STATUS MFRC522_MIFARE_Restore(uint8_t blockAddr);

/**
 * @brief     Ecrit le contenu de la mémoire volatile dans un block.
 * @details   MIFARE Transfert place le contenu de la mémoire volatile dans le block à l'adresse fournie en entrée.
 *
 * @note      Pour les PICC MIFARE Classic, le secteur contenant le block doit être authentifié avant d'appeler cette fonction.@n
 *            Cette restriction n'est que pour les blocks en mode "valide" (bits d'accès [C1 C2 C3] = [110] ou [001]).
 *
 * @param     [in]      blockAddr   Addresse du block à utiliser
 *
 * @return    MFRC522_STATUS_OK si le test est OK.
 *            MFRC522_STATUS_* en cas d'erreur.
 */
MFRC522_STATUS MFRC522_MIFARE_Transfer(uint8_t blockAddr);

/**
 * @brief     Ecrit 4 octets dans une page du MIFARE Ultralight ACTIF
 * @details   Permet d'écrire 4 octets dans une page données pour un PICC MIFARE Ultralight.
 *
 * @param     [in]      page        Page où stocker les données
 * @param     [in]      buffer      Buffer de 4 octets à écrire dans la page
 * @param     [in]      bufferSize  Taille du buffer (Doit être supérieur à 4). Seul les 4 premiers octets du buffer sont utilisés.
 *
 * @warning   Aucun dépassement de capacité n'est fait.
 *
 * @return    MFRC522_STATUS_OK si le test est OK.
 *            MFRC522_STATUS_* en cas d'erreur.
 */
MFRC522_STATUS MFRC522_MIFARE_Ultralight_Write(uint8_t page, uint8_t * buffer, uint8_t bufferSize);

/**
 * @brief     Lit la valeur d'un block d'un PICC MIFARE Classic
 * @details   Permet de lire facilement la valeur stockée dans un bloc.
 *
 * @note      Le secteur contenant le block doit être authentifié avant d'appeler cette fonction.@n
 *            Cette restriction n'est que pour les blocks en mode "valide" (bits d'accès [C1 C2 C3] = [110] ou [001]).
 *
 * @param     [in]      blockAddr   Addresse du block à utiliser (0x00 -> 0xFF)
 * @param     [out]     value       Valeur lue dans le block
 *
 * @return    MFRC522_STATUS_OK si le test est OK.
 *            MFRC522_STATUS_* en cas d'erreur.
 */
MFRC522_STATUS MFRC522_MIFARE_Classic_GetValue(uint8_t blockAddr, int32_t * value);

/**
 * @brief     Ecrit la valeur d'un block d'un PICC MIFARE Classic
 * @details   Permet d'écrire facilement la valeur dans un bloc.
 *
 * @note      Le secteur contenant le block doit être authentifié avant d'appeler cette fonction.@n
 *            Cette restriction n'est que pour les blocks en mode "valide" (bits d'accès [C1 C2 C3] = [110] ou [001]).
 *
 * @param     [in]      blockAddr   Addresse du block à utiliser (0x00 -> 0xFF)
 * @param     [out]     value       Nouvelle valeur à stocker dans le block
 *
 * @return    MFRC522_STATUS_OK si le test est OK.
 *            MFRC522_STATUS_* en cas d'erreur.
 */
MFRC522_STATUS MFRC522_MIFARE_Classic_SetValue(uint8_t blockAddr, int32_t value);

/////////////////////////////////////////////////////////////////////////////////////
// Support functions
/////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief     Récupère le type de carte en fonction de son SAK
 * @details   Permet d'identifier le type de carte.
 *
 * @param     [in]      uid         Structure UID de la carte à analyser
 *
 * @return    Le type de carte déterminé.
 */
MFRC522_PICC_TYPE MFRC522_PICC_GetType(MFRC522_PICC_UID * uid);

/**
 * @brief     Calcul le bit pattern des bits d'accès
 * @details   Calcul le bit pattern des bits d'accès
 *
 * @note      In the [C1 C2 C3] tupples C1 is MSB (=4) and C3 is LSB (=1).
 *
 * @param     [out]     accessBitBuffer   Buffer des bits d'accès à mettre à jour (pointeur vers les octets 6, 7 et 8 du sector trailer)
 * @param     [in]      g0                bits d'accès [C1 C2 C3] du block 0 (secteurs 0-31) ou block 0-4 (secteurs 32-39)
 * @param     [in]      g1                bits d'accès [C1 C2 C3] du block 1 (secteurs 0-31) ou block 5-9 (secteurs 32-39)
 * @param     [in]      g2                bits d'accès [C1 C2 C3] du block 2 (secteurs 0-31) ou block 10-14 (secteurs 32-39)
 * @param     [in]      g3                bits d'accès [C1 C2 C3] du sector trailer, block 3 (secteurs 0-31) ou block 15 (secteurs 32-39)
 */
void MFRC522_MIFARE_SetAccessBits(uint8_t * accessBitBuffer, uint8_t g0, uint8_t g1, uint8_t g2, uint8_t g3);

/*
bool PICC_IsNewCardPresent();
*/

/*
// Pour les cartes chinoises...
bool MIFARE_OpenUidBackdoor(bool logErrors);
bool MIFARE_SetUid(byte *newUid, byte uidSize, bool logErrors);
bool MIFARE_UnbrickUidSector(bool logErrors);
*/

#include <RFID/MFRC522_core.h>

#endif /* _MFRC522_H_ */
