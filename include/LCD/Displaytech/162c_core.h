/*
 * @file      162c_core.h
 *
 * @author    Zéro Cool
 * @date      12/09/2013 13:34:33
 * @brief     Core Driver pour afficheur LCD de la série 162c
 *
 * @details   Fichier coeur du driver permettant de gérer un afficheur LCD de la marque
 *            displaytech pour un afficheur de la série 162c
 *
 * @ingroup   LCD
 */

#ifndef _162C_CORE_H_
#define _162C_CORE_H_

#include <util/delay.h>
#include <avr/pgmspace.h>

// Liste des commandes
#define LCD_CMD_CLEAR             0x01
#define LCD_CMD_HOME              0x02
#define LCD_CMD_ENTRY             0x04
#define LCD_CMD_DISP              0x08
#define LCD_CMD_SHIFT             0x10
#define LCD_CMD_FUNC              0x20
#define LCD_CMD_SET_CGRAM         0x40
#define LCD_CMD_SET_DDRAM         0x80

// Définition des valeurs pour la commande LCD_CMD_ENTRY
#define LCD_BIT_ENTRY_SHIFT       0
#define LCD_BIT_ENTRY_INC         1
// Définition des valeurs pour la commande LCD_CMD_DISP
#define LCD_BIT_DISP_BLINK        0
#define LCD_BIT_DISP_CURS         1
#define LCD_BIT_DISP_DISP         2
// Définition des valeurs pour la commande LCD_CMD_SHIFT
#define LCD_BIT_SHIFT_TYPE        3
#define LCD_BIT_SHIFT_DIR         2
// Définition des valeurs pour la commande LCD_CMD_FUNC
#define LCD_BIT_FUNC_INTF         4
#define LCD_BIT_FUNC_LINS         3
#define LCD_BIT_FUNC_DOTS         2

// Masque définissant si l'afficheur LCD est occupée ou non
#define LCD_MSK_BUSY_FLG          0x80

// Offset permettant de passer d'une ligne à l'autre
#define LCD_OFFSET_LINE           0x40

static void Strobe(void)
{
	// On allume la limière
	LCD_CONTROL_PORT |=  _BV(LCD_CONTROL_EN_PIN);
	__asm__("NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;");
	__asm__("NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;");
	// On éteint la lumière pour que le LCD puisse "réfléchir"
	LCD_CONTROL_PORT &= ~_BV(LCD_CONTROL_EN_PIN);
}

static void CheckIfLcdIsBusy(void)
{
	// On positionne le bus de donnée du µC en lecture
	LCD_DATA_DIR = 0x00;

	// set R/W et unset RS
	LCD_CONTROL_PORT &= ~_BV(LCD_CONTROL_RS_PIN);	// Turn on Mr LCD's Command Mode (RS off)
	LCD_CONTROL_PORT |=  _BV(LCD_CONTROL_RW_PIN);	// Set Mr. LCD to Read (RW on)

	while (LCD_DATA_PORT >= LCD_MSK_BUSY_FLG)
	{
		Strobe();
	}

	// On positionne le bus de donnée du µC en écriture
	LCD_DATA_DIR = 0xFF;
}

static void SendCommand(const unsigned char command)
{
	CheckIfLcdIsBusy();
	LCD_DATA_PORT = command;
	LCD_CONTROL_PORT &= ~(_BV(LCD_CONTROL_RW_PIN)|_BV(LCD_CONTROL_RS_PIN));	// (RS off) (RW off)
	Strobe();
	LCD_DATA_PORT = 0x00;
}

void LCD_PrintChar(const unsigned char character)
{
	CheckIfLcdIsBusy();
	LCD_DATA_PORT = character;
	LCD_CONTROL_PORT &= ~_BV(LCD_CONTROL_RW_PIN);		// (RW off)
	LCD_CONTROL_PORT |=  _BV(LCD_CONTROL_RS_PIN);		// (RS on)
	Strobe();
	_delay_us(46);
	LCD_DATA_PORT = 0x00;
}

void LCD_PrintString(const char * string)
{
	while(*string)
	{
		LCD_PrintChar(*string++);
	}
}

void LCD_Initialize(void)
{
	LCD_CONTROL_DIR |= _BV(LCD_CONTROL_EN_PIN)|_BV(LCD_CONTROL_RW_PIN)|_BV(LCD_CONTROL_RS_PIN);	// (EN on) (RS on) (RW on)
	LCD_DATA_DIR    |= 0xFF;	// Port des datas en sortie

	// wait 10 ms for busy state
	_delay_ms(10);
	// Attente de la fin d'initialisation du LCD
	CheckIfLcdIsBusy();
}

void LCD_SoftwareReset(void)
{
	// Init by software
	SendCommand(0b00110000);
	_delay_ms(4.1);
	SendCommand(0b00110000);
	_delay_ms(100);
	SendCommand(0b00110000);
	CheckIfLcdIsBusy();
	SendCommand(0b00111100);
	_delay_us(42);
	SendCommand(0b00001000);
	_delay_us(42);
	SendCommand(0b00000001);
	_delay_ms(1.64);
	SendCommand(0b00000010);
	_delay_us(42);
}

void LCD_DisplayClear(void)
{
	SendCommand(LCD_CMD_CLEAR);
	_delay_ms(1.64);
}

void LCD_ReturnHome(void)
{
	SendCommand(LCD_CMD_HOME);
	_delay_ms(1.64);
}

void LCD_EntryMode(const LCD_CURSOR_DIR cursor_direction, const LCD_DISPLAY_TYPE display_type)
{
	SendCommand(LCD_CMD_ENTRY
	| (display_type << LCD_BIT_ENTRY_SHIFT)
	| (cursor_direction << LCD_BIT_ENTRY_INC));
	_delay_us(42);
}

void LCD_DisplayOn(const LCD_CURSOR_ACTIF cursor_actif, const LCD_CURSOR_BLINKING cursor_blinking)
{
	SendCommand(LCD_CMD_DISP
	| (1 << LCD_BIT_DISP_DISP)
	| (cursor_actif << LCD_BIT_DISP_CURS)
	| (cursor_blinking << LCD_BIT_DISP_BLINK));
	_delay_us(42);
}

void LCD_DisplayOff(void)
{
	SendCommand(LCD_CMD_DISP | (0 << LCD_BIT_DISP_DISP));
	_delay_us(42);
}

static void LCD_SetShift(const uint8_t type, const uint8_t directory)
{
	SendCommand(LCD_CMD_SHIFT
	| (type << LCD_BIT_SHIFT_TYPE)
	| (directory << LCD_BIT_SHIFT_DIR));
	_delay_us(42);
}

void LCD_SetFunction(const LCD_LINES lines, const LCD_POLICE police)
{
	/**
	 * @todo L'interface LCD est forcée à 8 bits pour le moment. Permettre
	 * l'utilisation en interface 4 bits.
	 */
	SendCommand(LCD_CMD_FUNC
	| (1 << LCD_BIT_FUNC_INTF)
	| (lines << LCD_BIT_FUNC_LINS)
	| (police << LCD_BIT_FUNC_DOTS));
	_delay_us(42);
}

void LCD_MoveCursorRight(uint8_t count)
{
	while(count--)
	{
		LCD_SetShift(0, 1);
	}
}

void LCD_MoveCursorLeft(uint8_t count)
{
	while(count--)
	{
		LCD_SetShift(0, 0);
	}
}

void LCD_MoveDisplayRight(uint8_t count)
{
	while(count--)
	{
		LCD_SetShift(1, 1);
	}
}

void LCD_MoveDisplayLeft(uint8_t count)
{
	while(count--)
	{
		LCD_SetShift(1, 0);
	}
}

void LCD_MoveCursor(const uint8_t line, const uint8_t column)
{
	SendCommand(LCD_CMD_SET_DDRAM | ((LCD_OFFSET_LINE * line) + column));
	_delay_us(42);
}

static void LCD_RegisterCharacter_P(const LCD_CGRAM adress, const uint8_t data[])
{
	uint8_t command = LCD_CMD_SET_CGRAM | (adress << 3);

	for (uint8_t i = 0; i < 8; i++)
	{
		SendCommand(command++);
		_delay_us(42);
		LCD_PrintChar(pgm_read_byte(&(data[i])));
	}
}

void LCD_RegisterCharacter(const LCD_CGRAM adress, const uint8_t data[])
{
	uint8_t command = LCD_CMD_SET_CGRAM | (adress << 3);

	for (uint8_t i = 0; i < 8; i++)
	{
		SendCommand(command++);
		_delay_us(42);
		LCD_PrintChar(data[i]);
	}
}

#endif /* _162C_CORE_H_ */
