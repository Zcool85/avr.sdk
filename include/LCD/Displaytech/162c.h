/**
 * @file      162c.h
 *
 * @author    Zéro Cool
 * @date      12/09/2013 13:34:33
 * @brief     Driver pour afficheur LCD de la série 162c
 *
 * @details   Fichier Driver permettant de gérer un afficheur LCD de la marque
 *            displaytech pour un afficheur de la série 162c
 *
 * @todo Créer la fonction d'affichage d'un entier
 * @todo Créer la fonction d'affichage d'un double
 * @todo Créer la fonction LCD_RegisterCharacter_P pour enregistrer un caractère depuis l'EEPROM
 * @todo Faire un exemple complet et global d'utilisation de cette API
 *
 * @ingroup   LCD
 */

#ifndef _162C_H_
#define _162C_H_

#include <stdint.h>

/**
 * @brief     Directions d'écriture du LCD
 * @details   Enumération des directions d'écriture du LCD
 */
typedef enum
{
    LCD_CURSOR_DIR_DECREMENT = 0,   /**< Direction d'écriture décrémentale */
    LCD_CURSOR_DIR_INCREMENT = 1    /**< Direction d'écriture incrémentale */
} LCD_CURSOR_DIR;

/**
 * @brief     Types d'affichages du LCD
 * @details   Enumération des types d'affichages du LCD
 */
typedef enum
{
    LCD_DISPLAY_TYPE_NORMAL = 0,     /**< Type d'affichage du LCD normale */
    LCD_DISPLAY_TYPE_INVERT = 1      /**< Type d'affichage inversé */
} LCD_DISPLAY_TYPE;

/**
 * @brief     Types d'activation du curseur sur le LCD
 * @details   Enumération des types d'activation du curseur sur le LCD
 */
typedef enum
{
    LCD_CURSOR_ACTIF_OFF = 0,        /**< Curseur inactif (non affiché) */
    LCD_CURSOR_ACTIF_ON  = 1         /**< Curseur actif (affiché) */
} LCD_CURSOR_ACTIF;

/**
 * @brief     Types de clignotement du curseur sur le LCD
 * @details   Enumération des types de clignotement du curseur sur le LCD
 */
typedef enum
{
    LCD_CURSOR_BLINKING_OFF = 0,     /**< Clignotement inactif */
    LCD_CURSOR_BLINKING_ON  = 1      /**< Clignotement actif */
} LCD_CURSOR_BLINKING;

/**
 * @brief     Nombres de lignes gérées par le LCD
 * @details   Enumération des nombres de lignes que le LCD doit gérer
 */
typedef enum
{
    LCD_LINES_1 = 0,                 /**< Affichage sur une ligne */
    LCD_LINES_2 = 1                  /**< Affichage sur deux lignes */
} LCD_LINES;

/**
 * @brief     Police dots à utiliser par le LCD
 * @details   Enumération des polices dots de l'afficheur LCD
 */
typedef enum
{
    LCD_POLICE_5X7  = 0,             /**< Affichage 5x7 points */
    LCD_POLICE_5X10 = 1              /**< Affichage 5x10 points */
} LCD_POLICE;

/**
 * @brief     Adresses CGRAM
 * @details   Enumération des adresses CGRAM disponibles pour l'enregistrement de nouveaux caractères
 */
typedef enum
{
    LCD_CGRAM_00 = 0x00,            /**< Adresse 0b00000000 (0x00) */
    LCD_CGRAM_01 = 0x01,            /**< Adresse 0b00000001 (0x01) */
    LCD_CGRAM_02 = 0x02,            /**< Adresse 0b00000010 (0x02) */
    LCD_CGRAM_03 = 0x03,            /**< Adresse 0b00000011 (0x03) */
    LCD_CGRAM_04 = 0x04,            /**< Adresse 0b00000100 (0x04) */
    LCD_CGRAM_05 = 0x05,            /**< Adresse 0b00000101 (0x05) */
    LCD_CGRAM_06 = 0x06,            /**< Adresse 0b00000110 (0x06) */
    LCD_CGRAM_07 = 0x07,            /**< Adresse 0b00000111 (0x07) */
} LCD_CGRAM;

/**
 * @brief      Initialise l'afficheur LCD
 *
 * @details    Fonction permettant d'initialiser le LCD pour qu'il soit prêt à
 *             l'utilisation
 *
 * @note       Cette fonction est à appeler avant toute utilisation du LCD
 *
 * Exemple :
 * @code
 *    LCD_Initilize();
 * @endcode
 */
void LCD_Initialize(void);

/**
 * @brief      Efface l'afficheur LCD
 *
 * @details    Fonction permettant d'effacer tout ce qui est actuellement écrit sur le LCD
 *
 * Exemple :
 * @code
 *    LCD_DisplayClear();
 * @endcode
 */
void LCD_DisplayClear(void);

/**
 * @brief      Ecrit un caractère sur le LCD
 *
 * @details    Fonction permettant d'écrire un caractère sur le LCD.
 *             Le caractère sera ajouté à la fin de la ligne courante et avancera le curseur d'un caractère.
 *
 * @param      [in]      character    Caractère à imprimer sur le LCD
 *
 * Exemple :
 * @code
 *    LCD_PrintChar('A');
 * @endcode
 * ou :
 * @code
 *    LCD_PrintChar(0x31);
 * @endcode
 */
void LCD_PrintChar(const unsigned char character);

/**
 * @brief      Ecrit une chaine de caractères sur le LCD
 *
 * @details    Fonction permettant d'écrire une chaine de caractères sur le LCD.
 *             La chaine de caractères sera ajoutée à la fin de la ligne courante
 *             et le curseur avancera d'autant de caractères contenu dans la chaine.
 *
 * @param      [in]      string    Chaine de caractères à imprimer sur le LCD
 *
 * @note       La chaine de caractères doit être délimitée par un zéro binaire en fin de chaine.
 *             Le zéro binaire n'est pas écrit sur le LCD.
 *
 * Exemple :
 * @code
 *    LCD_PrintString("Hello World");
 * @endcode
 */
void LCD_PrintString(const char * string);

/**
 * @brief      Positionne le curseur à la première ligne, première colonne d'écriture du LCD
 *
 * @details    Fonction permettant de placer le curseur d'écriture sur la première ligne et la première colonne du LCD.
 *
 * @note       Le contenu affiché par le LCD n'est pas supprimé. Seul le curseur est déplacé.
 *
 * Exemple :
 * @code
 *    LCD_ReturnHome();
 * @endcode
 */
void LCD_ReturnHome(void);

/**
 * @brief      Sélection du mode d'insertion
 * @details    Fonction permettant de choisir le mode d'insertion des nouveaux
 *             caractères sur le LCD
 *
 * @param      [in]      cursor_direction     Indique la direction du curseur
 * @param      [in]      display_type         Chaine de caractères à imprimer sur le LCD
 *
 * Exemple pour un mode d'affichage normal (non inversé) avec direction du curseur incrémentale :
 * @code
 * LCD_EntryMode(LCD_CURSOR_DIR_INCREMENT, LCD_DISPLAY_TYPE_NORMAL);
 * @endcode
 */
void LCD_EntryMode(const LCD_CURSOR_DIR cursor_direction, const LCD_DISPLAY_TYPE display_type);

/**
 * @brief      Active l'affichage du LCD
 * @details    Fonction permettant d'allumer le LCD et de configurer son comportement.
 *
 * @param      [in]      cursor_actif       Indique si le curseur doit être actif ou non
 * @param      [in]      cursor_blinking    Indique si le curseur doit clignoter ou non
 *
 * Exemple pour activer le LCD, afficher le curseur et le rendre clignotant :
 * @code
 * LCD_DisplayOn(LCD_CURSOR_ACTIF_ON, LCD_CURSOR_BLINKING_ON);
 * @endcode
 */
void LCD_DisplayOn(const LCD_CURSOR_ACTIF cursor_actif, const LCD_CURSOR_BLINKING cursor_blinking);

/**
 * @brief      Désactive l'affichage du LCD
 * @details    Fonction permettant de désactiver l'affichage du LCD.
 *
 * Exemple :
 * @code
 * LCD_DisplayOff();
 * @endcode
 */
void LCD_DisplayOff(void);

/**
 * @brief      Paramètre certaines fonctions du LCD
 * @details    Fonction permettant de paramétrer le comportement du LCD.
 *
 * @param      [in]      lines            Indique le nombre de lignes utilisées par le LCD
 * @param      [in]      police           Indique la police de cactères à utiliser pour le LCD
 *
 * Exemple pour utiliser le LCD sur 2 lignes en police 5x10 dots :
 * @code
 * LCD_SetFunction(LCD_LINES_2, LCD_POLICE_5X10);
 * @endcode
 */
void LCD_SetFunction(const LCD_LINES lines, const LCD_POLICE police);

/**
 * @brief      Déplace le curseur sur la droite
 * @details    Fonction permettant de déplacer le curseur du LCD sur la droite
 *
 * @param      [in]      count            Indique le nombre de caractères de déplacement
 *
 * Exemple pour déplacer le curseur de 4 positions sur la droite :
 * @code
 * LCD_MoveCursorRight(4);
 * @endcode
 */
void LCD_MoveCursorRight(uint8_t count);

/**
 * @brief      Déplace le curseur sur la gauche
 * @details    Fonction permettant de déplacer le curseur du LCD sur la gauche
 *
 * @param      [in]      count            Indique le nombre de caractères de déplacement
 *
 * Exemple pour déplacer le curseur de 2 positions sur la gauche :
 * @code
 * LCD_MoveCursorLeft(2);
 * @endcode
 */
void LCD_MoveCursorLeft(uint8_t count);

/**
 * @brief      Déplace l'affichage sur la droite
 * @details    Fonction permettant de déplacer l'affichage du LCD sur la droite à patir de la position courante du curseur
 *
 * @param      [in]      count            Indique le nombre de caractères de déplacement
 *
 * Exemple pour déplacer l'affichage de 3 positions sur la droite :
 * @code
 * LCD_MoveDisplayRight(3);
 * @endcode
 */
void LCD_MoveDisplayRight(uint8_t count);

/**
 * @brief      Déplace l'affichage sur la gauche
 * @details    Fonction permettant de déplacer l'affichage du LCD sur la gauche à patir de la position courante du curseur
 *
 * @param      [in]      count            Indique le nombre de caractères de déplacement
 *
 * Exemple pour déplacer l'affichage de 6 positions sur la gauche :
 * @code
 * LCD_MoveDisplayLeft(6);
 * @endcode
 */
void LCD_MoveDisplayLeft(uint8_t count);

/**
 * @brief      Déplace le curseur
 * @details    Fonction permettant de déplacer le curseur sur le LCD
 *
 * @param      [in]      line             Indique le numéro de ligne sur laquelle placer le curseur (indice de base 0)
 * @param      [in]      column           Indique le numéro de la ligne sur laquelle placer le curseur (indice de base 0)
 *
 * @warning    Les dépacements de capacités ne sont pas vérifiés.
 *
 * Exemple pour déplacer le curseur sur la ligne 1 en colonne 3 :
 * @code
 * LCD_MoveCursor(0, 2);
 * @endcode
 */
void LCD_MoveCursor(const uint8_t line, const uint8_t column);

/**
 * @brief      Réinitialisation du LCD
 * @details    Fonction permettant de réinitialiser l'afficheur LCD par software
 *
 * Exemple :
 * @code
 * LCD_SoftwareReset();
 * @endcode
 */
void LCD_SoftwareReset(void);

/**
 * @brief      Enregistre un caractère en CGRAM
 * @details    Fonction permettant d'enregistrer un caratère en CGRAM du LCD
 *
 * @param      [in]       address         Donne l'adresse en CGRAM où enregistrer le caractère
 * @param      [in]       data            Tableau d'octets contenant la représentation du caractère.
 *                                        Le tableau doit avoir 8 occurences. Pour chaque occurences,
 *                                        seul les 5 bits de poids faibles sont utilisés.
 *
 * @note       Seul 8 caractères peuvent être enresitrés (8 adresses disponibles)
 *
 * @warning    Il n'y a aucun contrôle de dépassement de capacité
 *
 * @todo       Faire un exemple complet
 */
// Créé un nouveau caractère en CGRAM
// adress   : de 0b0000 à 0b0111 (soit 8 caracteres possibles)
// data     : Tableau de 8x5bits (pour chacune des 8 occurences, seuls les 5 bits de poids faibles sont utilisés)
void LCD_RegisterCharacter(const LCD_CGRAM address, const uint8_t data[]);

#include <LCD/Displaytech/162c_core.h>

#endif /* _162C_H_ */
