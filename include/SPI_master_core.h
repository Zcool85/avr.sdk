/*
 * @file      SPI_master_core.h
 *
 * @author    Zéro Cool
 * @date      10/06/2015 20:50:15
 * @brief     Core du protocole de communication SPI maitre
 *
 * @details   Fichier core définissant les routines du protocole de communication
 *            SPI. Cette implémentation fonctionne pour tous les AVR disposant nativement
 *            d'une interface hardware SPI.
 *
 * @ingroup   PROTOCOLES
 */

#ifndef _SPI_MASTER_CORE_H_
#define _SPI_MASTER_CORE_H_

void SPI_Initialize(void)
{
  // Default mode for software ISP :
  // - MSB first
  // - Polarity : Leading edge Raising + Railling Edge Falling
  // - Phase : Leading edge Sample + Trailing Edge Setup
  // - Clock : equal to F_CPU

  SPI_DDR |=  _BV(SPI_MOSI_PIN);  // SDI output (MOSI)
  SPI_DDR |=  _BV(SPI_SCK_PIN);   // SCK output (Clock)
  SPI_DDR |=  _BV(SPI_SS_PIN);	  // Slave Select Output
  SPI_DDR &= ~_BV(SPI_MISO_PIN);	// SDO input (MISO)

  SPI_PORT |=  _BV(SPI_MISO_PIN);	// Pull-up MISO

#ifdef SPI_SOFTWARE
  SPI_PORT |=  _BV(SPI_SS_PIN);   // SS to high
  SPI_PORT |=  _BV(SPI_MOSI_PIN); // SDI to high
  SPI_PORT &= ~_BV(SPI_SCK_PIN);  // SCK to low
#else
  /**
   * @todo revoir le paramétrage pour les options du mode hardware
   */
  SPCR = //_BV(SPIE)  |   // Interupt Enable
         _BV(SPE)   |   // Enable SPI
         //_BV(DORD)  |   // LSB first
         _BV(MSTR)  |   // Master
         //_BV(CPOL)  |   // Clock Polarity (0 => Leading edge Raising + Railling Edge Falling)
         //               //                (1 => Leading edge Falling + Railling Edge Raising)
         //_BV(CPHA)  |   // Clock phase (0 => Leading edge Sample + Trailing Edge Setup)
         //               //             (1 => Leading edge Setup + Trailing Edge Sample)
         _BV(SPR0);     // fck/16
  SPSR &= ~_BV(SPI2X);  // Pas de double vitesse
#endif
}

uint8_t SPI_SendByte(uint8_t byte)
{
#ifdef SPI_SOFTWARE
  uint8_t recv = 0x00;

  for(uint8_t i = 0; i < 8; ++i)
  {
    if (byte & 0x80)
    {
      SPI_PORT |=  _BV(SPI_MOSI_PIN); // SDI to high
    }
    else
    {
      SPI_PORT &= ~_BV(SPI_MOSI_PIN); // SDI to low
    }
    SPI_PORT |=  _BV(SPI_SCK_PIN); // SCK to hi
    recv <<= 1;
    if ( SPI_PIN & _BV(SPI_MISO_PIN) )
    {
      recv |= 0x01;
    }
    SPI_PORT &= ~_BV(SPI_SCK_PIN); // SCK to low
    byte <<= 1;
  }

  return recv;
#else
  SPDR = byte;
  while(!(SPSR & _BV(SPIF)));
  return SPDR;
#endif
}

#endif /* _SPI_MASTER_CORE_H_ */
