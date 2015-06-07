/*
 * alpha_core.h
 *
 * Created: 12/09/2013 13:34:33
 *  Author: Zéro Cool
 */

#ifndef _ALPHA_CORE_H_
#define _ALPHA_CORE_H_

void SPI_Init(void)
{
	ALPHA_SPI_DIR |=  _BV(ALPHA_SPI_MOSI_PIN);	// MOSI Output
	ALPHA_SPI_DIR |=  _BV(ALPHA_SPI_SCK_PIN);	// Clock Output
	ALPHA_SPI_DIR |=  _BV(ALPHA_SPI_SS_PIN);	// Slave Select Output
	ALPHA_SPI_DIR &= ~_BV(ALPHA_SPI_MISO_PIN);	// MISO Input

	ALPHA_SPI_PORT |=  _BV(ALPHA_SPI_MISO_PIN);	// Pull-up MISO

	//SPCR = _BV(SPE)|_BV(MSTR)|_BV(SPR0);		// TODO : Pour le moment SPI activé en hardware
	SPCR = _BV(SPE)|_BV(MSTR);		// IDem que la ligne du dessus, mais en 2MHz
}

void SPI_SendWord(uint16_t data)
{
	ALPHA_SPI_PORT &= ~_BV(ALPHA_SPI_SS_PIN);	// SS to low

	SPDR = data>>8;
	while(!(SPSR & _BV(SPIF)));

	SPDR = data & 0xFF;
	while(!(SPSR & _BV(SPIF)));

	ALPHA_SPI_PORT |= _BV(ALPHA_SPI_SS_PIN);	// SS to high
}

void SPI_SendByte(uint8_t data)
{
	ALPHA_SPI_PORT &= ~_BV(ALPHA_SPI_SS_PIN);	// SS to low

	SPDR = data;
	while(!(SPSR & _BV(SPIF)));

	ALPHA_SPI_PORT |= _BV(ALPHA_SPI_SS_PIN);	// SS to high
}

void ALPHA_RxInit(void)
{
	ALPHA_NIRQ_DIR  &= ~_BV(ALPHA_NIRQ_PIN);	// nIRQ Input
	ALPHA_NIRQ_PORT |=  _BV(ALPHA_NIRQ_PIN);	// nIRQ Pull-up

	ALPHA_FFS_DIR   &= ~_BV(ALPHA_FFS_PIN);		// FSK Input
	ALPHA_FFS_PORT  |=  _BV(ALPHA_FFS_PIN);		// FSK Pull-up

	SPI_Init();

	// Configuration Setting Command
	// -----------------------------
	//   100         : Command
	//   b1 b0       : Frequency Band (01 => 433 MHz)
	//   eb          : Enable low battery detector
	//   et          : Enable wake-up timer
	//   ex          : Is set id the crystal is active during sleep mode.
	//   x3 x2 x1 x0 : Crystal Load Capacitance (1000 => 12.5 pF)
	//   i2 i1 i0    : Baseband Bandwith (101 => 134 KHz)
	//   dc          : Disable the clock Output
	SPI_SendWord(0b1000100110001011);

	// Frequency Setting Command
	// -------------------------
	//   1010                                  : Command
	//   f11 f10 f9 f8 f7 f6 f5 f4 f3 f2 f1 f0 : Should be in range of 96 and 3903 (011000100000 => 1568)
	//
	// Fo = 10 MHz * (43 + F / 4000)    => Fo = 433.92 MHz
	//
	// NOTE : Configure Frequency BEFORE starting Synthesizer
	SPI_SendWord(0b1010011000100000);

	// Receiver Setting Command
	// ------------------------
	//   11000000 : Command
	//   d1 d0    : Select VDI (Valid Data Indicator) signal (11 => VDI Always on)
	//   g1 g0    : LNA Gain (00 => 0 dB relative to max G)
	//   r2 r1 r0 : DRSSI Threshold (000 => -103 dBm)
	//   en       : Enable whole receiver chain (wake-up & low battery detector are not affected by this setting)
	//
	// RSSIth = RSSIsetth + Glna => RSSIth = -103 + LNA Gain
	SPI_SendWord(0b1100000011000001);

	// Wake-Up Timer Command
	// ---------------------
	//   111                     : Command
	//   r4 r3 r2 r1 r0          : Should be in range of 0 to 23
	//   m7 m6 m5 m4 m3 m2 m1 m0 :
	//
	// T = M * 2^R => 0ms
	//
	// NOTE : For continual operation, bit et must be cleared and set
	//SPI_SendWord(0b1110000000000000);

	// Low Duty-Cycle Command
	// ----------------------
	//   11001100             : Command
	//   d6 d5 d4 d3 d2 d1 d0 : (0000111 => 7)
	//   en                   : Enable low duty cycle mode
	SPI_SendWord(0b1100110000001110);	// Low Duty-Cycle Command : DutyCycle = (D * 2 + 1) / M * 100%; en = 0

	// Low Battery Detector & Microcontroller Clock Divider Command
	// ------------------------------------------------------------
	//   11000010       : Command
	//   d2 d1 d0       : Clock Ouptut Frequency (111 => 10 MHz)
	//   t4 t3 t2 t1 t0 : Threshold voltage
	//
	// Vlowbat = 2.2 + T * 0.1 => Vlowbat = 2.2 V
	SPI_SendWord(0b1100001011100000);

	// AFC Command
	// -----------
	//   11000110 : Command
	//   a1 a0    : Select AFC auto-mode (11 => Keeps independently from VDI)
	//   rl1 rl0  : Range limit (11 => +3-4)
	//   st       : When goes high, the actual latest calculated frequency error is stored into the output registers of the AFC block
	//   fi       : Enable high accuracy mode. The processing time is about 4 times longer
	//   oe       : Enable the output (frequency offset) register
	//   en       : Enable calculation of the offset frequency by the AFC circuit (if allows the addition of the content of the output register to the frequency control word of the PPL)
	SPI_SendWord(0b1100011011110111);

	// Data Filter Command
	// -------------------
	//   11000100 : Command
	//   al       : Enable Clock Recovery auto lock control. It means that the CR start in fast mode after locking it automatically switches to slow mode
	//   ml       : Enable fast mode
	//   1        : -
	//   s1 s0    : Type of data Filter (01 => Digital filter)
	//   f2 f1 f0 : DQD threchold (100 => 4)
	SPI_SendWord(0b1100010011101100);

	// Data Rate Command
	// -----------------
	//   11001000             : Command
	//   cs                   :
	//   r6 r5 r4 r3 r2 r1 r0 : DataRate (0100011 => 35)
	//
	// BaudRate = 10 MHz / 29 / (DataRate + 1) / (1 + cs * 7) => BaudRate = 9578,5440613026819923371647509579 bauds
	//
	// Set the Receiver DataRate according the next function : DataRate = (10 MHz / 29 / (1 + cs * 7) / BaudRate) - 1
	SPI_SendWord(0b1100100000100011);

	// Output and FIFO mode Command
	// ----------------------------
	//   11001110    : Command
	//   f3 f2 f1 f0 : FIFO IT level. The FIFO generate IT when number of the received data bits reaches this level (1000 => 8)
	//   s1 s0       : Select the input of the FIFO fill start condition (10 => VDI & Sync-word)
	//   ff          : Enable FIFO fill after synchron word reception. FIFO fill stop when this bit is cleared
	//   fe          : Enables the 16 bits FIFO mode. To clear the FIFO's counter and content, it has to be set zero
	//
	// NOTE : Synchron word is 2DD4h
	// NOTE : To restart the synchron word reception, bit ff should be cleared and set. This action will initialize the FIFO and clear its content.
	// NOTE : Bit fe modifies the function of pin 3 and pin 4. Pin 3 (nFFS) will become input if fe is set to 1. If the chip is used in FIFO mode, do not allow this to be a floating input.
	SPI_SendWord(0b1100111010001000);
	DELAI_US(250);
	SPI_SendWord(0b1100111010001011);
	DELAI_US(250);

	// Reset Mode
	// ----------
	//   110110100000000 : Command
	//   dr              : Disable the higly sensitive RESET mode. If this bit is cleared, a 600mV glitch in the power supply may cause a system reset.
	SPI_SendWord(0b1101101000000001);
}

ISR(PCINT0_vect)
{
	// TODO un jour...
	// PCINT6 => FFIT
}

void ALPHA_TxInit(void)
{
	ALPHA_NIRQ_DIR  &= ~_BV(ALPHA_NIRQ_PIN);	// nIRQ Input
	ALPHA_NIRQ_PORT |=  _BV(ALPHA_NIRQ_PIN);	// nIRQ Pull-up

	ALPHA_FFS_DIR   |=  _BV(ALPHA_FFS_PIN);		// FFS Output

	ALPHA_FFIT_DIR  &= ~_BV(ALPHA_FFIT_PIN);	// FFIT Input
	ALPHA_FFIT_PORT |=  _BV(ALPHA_FFIT_PIN);	// FFIT Pull-up

	ALPHA_NIRQ_DIR  &= ~_BV(ALPHA_NIRQ_PIN);	// nIRQ Input
	ALPHA_NIRQ_PORT |=  _BV(ALPHA_NIRQ_PIN);	// nIRQ Pull-up

	// FFS must be hight
	ALPHA_FFS_PORT |= _BV(ALPHA_FFS_PIN);

	PCICR |= _BV(0);    //  Pin Change Interrupt Enable 0
	PCMSK0 = _BV(6);    //  PCINT6 = PB6 pour la FFIT

	SPI_Init();

	//// Configuration Setting Command
	//// -----------------------------
	////   100         : Command
	////   b1 b0       : Frequency Band (01 => 433 MHz)
	////   d2 d1 d0    : Clock Output Frequency (111 => 10 MHz)
	////   x3 x2 x1 x0 : Crystal Load Capacitance (1000 => 12.5 pF)
	////   ms          : Modulation polarity
	////   m2 m1 m0    : Frequency deviation (000 => 30 KHz)
	////
	//// The resulting output frequency can be calculated as :
	////  Fout = Fo - (-1)^sign * (M + 1) * (30 kHz)
	//// Where
	////   Fo   : Channel center Frequency (Cf. Frequency Setting Command)
	////   M    : binary number m2 m1 m0 (In range from 0 to 6)
	////   sign : ms XOR FSKinput
	//SPI_SendWord(0b1000111110000000);
	//
	//// Frequency Setting Command
	//// -------------------------
	////   1010                                  : Command
	////   f11 f10 f9 f8 f7 f6 f5 f4 f3 f2 f1 f0 : Should be in range of 96 and 3903 (011000100000 => 1568)
	////
	//// Fo = 10 MHz * (43 + F / 4000)    => Fo = 433.92 MHz
	////
	//// NOTE : Configure Frequency BEFORE starting Synthesizer
	//SPI_SendWord(0b1010011000100000);
	//
	//// Data Rate Command
	//// -----------------
	////   11001000                : Command
	////   r7 r6 r5 r4 r3 r2 r1 r0 : DataRate (00100011 => 35)
	////
	//// BaudRate = 10 MHz / 29 / (DataRate + 1) => BaudRate = 9578,5440613026819923371647509579 bauds
	//SPI_SendWord(0b1100100000100011);
	//
	//// Power Setting Command
	//// ---------------------
	////   1011     : Command
	////   0        : ook (non dispo sur Alpha-TX433)
	////   p2 p1 p0 : Relative Output Power (dB) (000 => 0 dB)
	//SPI_SendByte(0b10110000);
	//
	//// Low Battery Detector & Tx bit Synchronization Command
	//// -----------------------------------------------------
	////   11000010       : Command
	////   dwc            : Disable the wake-up timer periodical (every 30 second) calibration
	////   0              : -
	////   ebs            : Enable the TX bit synchronization circuit. The data rate must be set by the Data Rate Command
	////   t4 t3 t2 t1 t0 : Threshold voltage
	////
	//// Vlowbat = 2.2 + T * 0.1 => Vlowbat = 2.2 V
	//SPI_SendWord(0b1100001000100000);
	//
	//// Sleep Command
	//// -------------
	////   11000100                : Command
	////   s7 s6 s5 s4 s3 s2 s1 s0 : Periods before stop crystal oscillator (00010000 => 16)
	////
	//// The effect of this command depends on the Power Management Command. It immediately disable the power amplifier (if a0=1 and ea=0) and
	//// the synthesizer (if a1=1 and es=0). Stops the crystal oscillator after S periods of the microcontroller clock (if a1=1 and ex=0) to enable
	//// the microcontroller to execute all necessary commands before entering sleep mode itself.
	////SPI_SendWord(0b1100010000010000);
	//
	//// Wake-Up Timer Command
	//// ---------------------
	////   111                     : Command
	////   r4 r3 r2 r1 r0          : Should be in range of 0 to 23
	////   m7 m6 m5 m4 m3 m2 m1 m0 :
	////
	//// T = M * 2^R => 0ms
	////
	//// NOTE : For continual operation, bit et must be cleared and set
	////SPI_SendWord(0b1110000000000000);
	//
	//// Power Management Command
	//// ------------------------
	////   11000000 : Command
	////   a1 a0    : If bit a1 is set, the cristal oscillator and the synthesizer are controlled automatically.
	////				Data Transmit Command starts up the crystal oscillator and as soon as a stable reference frequency is available the synthesizer starts.
	////              After a subsequent delay to allow locking of the PPL, if a0 is set the power amplifier is turned on as well.
	////   ex       : Enable crystal oscillator
	////   es       : Enable synthesizer
	////   ea       : Enable power amplifier
	////   eb       : Enable low battery detection function
	////   et       : Enable wake-up timer
	////   dc       : Disable clock output (CLK pin)
	////
	//// NOTES :
	////   To enable the automatic internal control of the crystal oscillator, the synthesizer and the power amplifier, the corresponding bits (ex, es, ea) must be zero.
	////   The ex bit should be set for the correct control os es and ea. The oscillator can be switched off by clearing the ex bit after the transmission.
	////   The Sleep Command can be used to indicate the end of the data transmission process, because the Data Transmit Command does not contain the length of the TX data.
	//SPI_SendWord(0b1100000000111001);
}

void ALPHA_SendFSK(uint8_t data)
{
	for (uint8_t offset = 7; offset >= 0; offset--)
	{
		while ((PINB & _BV(ALPHA_NIRQ_PIN)) == 0);	// Wait nIRQ Hi
		while ((PINB & _BV(ALPHA_NIRQ_PIN)) != 0);	// Wait nIRQ Lo

		if (data & _BV(offset))
		{
			ALPHA_FFS_PORT |= _BV(ALPHA_FFS_PIN);
		}
		else
		{
			ALPHA_FFS_PORT &= ~_BV(ALPHA_FFS_PIN);
		}
	}
}

void ALPHA_SendData(uint8_t data)
{
	SPI_SendWord(0b1100000000110001);	// Power Management Command - extinction de l'amplificateur
	DELAI_US(250);
	SPI_SendWord(0b1100000000111001);	// Power Management Command - allumage amplificateur (Tx_Open)
	DELAI_US(250);		// Wait PLL startup time
	DELAI_MS(5);		// Wait crystal oscillator startup time

	uint8_t checksum = ~data;

	ALPHA_SendFSK(0xAA);	// Preamble
	ALPHA_SendFSK(0xAA);	// Preamble
	ALPHA_SendFSK(0x2D);	// Synch byte
	ALPHA_SendFSK(0xD4);	// Synch byte
	ALPHA_SendFSK(data);	// Data byte
	ALPHA_SendFSK(checksum);	// checksum
	ALPHA_SendFSK(0xAA);	// dummy

	SPI_SendWord(0b1100000000110001);	// Power Management Command - extinction de l'amplificateur
}

#endif /* _ALPHA_CORE_H_ */
