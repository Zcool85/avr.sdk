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
 * @ingroup   LCD
 */

#ifndef _162C_H_
#define _162C_H_

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
 */
// Sélection du mode d'insertion
// cursor_direction : 1 = incrémentation - 0 = décrémentation
// display_inversion : 1 = inversion - 0 = non inversion
void LCD_EntryMode(const uint8_t cursor_direction, const uint8_t display_inversion);

// Configuration de l'affichage LCD
// is_on : 1 = afficheur actif - 0 = afficheur inactif
// is_cursor_actif : 1 = curseur actif - 0 = curseur inactif
// is_cursor_blinking : 1 = curseur clignotant - 0 = curseur non clignotant
void LCD_DisplayConfiguration(const uint8_t is_on, const uint8_t is_cursor_actif, const uint8_t is_cursor_blinking);

void LCD_SetFunction(const uint8_t with2lines, const uint8_t police10dots);

// DÃ©placement du curseur de n caractÃ¨res vers la droite
void LCD_MoveCursorRight(uint8_t count);

// DÃ©placement du curseur de n caractÃ¨res vers la gauche
void LCD_MoveCursorLeft(uint8_t count);

// Déplacement de n caractères vers la droite des données à partir du curseur
void LCD_MoveDisplayRight(uint8_t count);

// Déplacement de n caractères vers la gauche des données à partir du curseur
void LCD_MoveDisplayLeft(uint8_t count);

// Déplacement du curseur
// line : numéro de la ligne (indice de base zéro)
// column : numéro de la colonne (indice de base zéro)
// ATTENTION : Les dépassements de capacités ne sont pas vérifiés !
void LCD_MoveCursor(uint8_t line, const const uint8_t column);

// Ré-ibitialise l'afficheur par software
void LCD_SoftwareReset(void);

// Créé un nouveau caractère en CGRAM
// adress   : de 0b0000 à 0b0111 (soit 8 caracteres possibles)
// data     : Tableau de 8x5bits (pour chacune des 8 occurences, seuls les 5 bits de poids faibles sont utilisés)
void LCD_CreateCharacter(const uint8_t adress, const char data[]);

// TODO : Afficher entier
// TODO : Afficher double

/****************************************************************
  Implémentations
 ****************************************************************/

#define LCD_CMD_CLEAR			0x01
#define LCD_CMD_HOME			0x02
#define LCD_CMD_ENTRY			0x04
#define LCD_CMD_DISP			0x08
#define LCD_BIT_DISP_BLINK		0
#define LCD_BIT_DISP_CURS		1
#define LCD_BIT_DISP_DISP		2
#define LCD_CMD_SHIFT			0x10
#define LCD_BIT_SHIFT_TYPE		3
#define LCD_BIT_SHIFT_DIR		2
#define LCD_CMD_FUNC			0x20
#define LCD_BIT_FUNC_INTF		4
#define LCD_BIT_FUNC_LINS		3
#define LCD_BIT_FUNC_DOTS		2
#define LCD_CMD_SET_CGRAM		0x40
#define LCD_CMD_SET_DDRAM		0x80

#define LCD_MSK_BUSY_FLG		0x80

#define LCD_OFFSET_LINE			0x40

#include <LCD/Displaytech/162c_core.h>

#endif /* _162C_H_ */
