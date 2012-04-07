/**
 * @addtogroup TODO_MODULE_NAME_TODO
 *
 * @{
 * @file    main.c
 * @brief   Test UART functionality.
 * @author  Robert Müller
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
//#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "delay.h"

// --- Definitions -------------------------------------------------------------


#define U8  unsigned char
#define U16 unsigned int

#define PORT PORTD
#define PORTREAD PIND
#define PORTOUTPUT DDRD

#define CONNECTOR PORTC
#define CONNECTOROUTPUT DDRC
#define CONNECTORREAD PINC

#define TestLED       0b00010000
#define ExpLED        0b01000000
#define TestButton    0b00100000
#define ExpButton     0b10000000
#define Driver        0b00000100
#define Receiverstop  0b00001000

#define ShiftRegisterClock   (1<<PC0)
#define SerialDataOut        (1<<PC1)
#define StorageRegisterClock (1<<PC2)
#define SerialOutPort CONNECTOR

#define SET_SCK SerialOutPort |=  (1<<PC0)
#define CLR_SCK SerialOutPort &= ~(1<<PC0)
#define SET_SO  SerialOutPort |=  (1<<PC1)
#define CLR_SO  SerialOutPort &= ~(1<<PC1)
#define SET_RCK SerialOutPort |=  (1<<PC2)
#define CLR_RCK SerialOutPort &= ~(1<<PC2)

#define Channelbitmasktwobytes    0b01000000            // needed for test if 2ByteMode-Message
#define Channelbitmaskacknowledge 0b00010000            // needed for test if Acknowledge-Message

#define EEPROM_SECTION  __attribute__ ((section (".eeprom")))

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

/*
** this global variables are stored in EEPROM
*/
uint16_t  dummy                 EEPROM_SECTION  = 0;  // avoid using lowest addresses

//——————————————————————————
// section 1: default settings
uint8_t  eeprom_def_OwnAddress   EEPROM_SECTION  = 2;  // EEPROM address 0002
uint8_t  eeprom_def_Scheduled    EEPROM_SECTION  = 4;  // EEPROM address 0004
uint8_t  eeprom_def_Freebus      EEPROM_SECTION  = 5;  // EEPROM address 0005
uint8_t  eeprom_def_Devicemode   EEPROM_SECTION  = 6;  // EEPROM address 0006

unsigned char ButtonStatus = 1;
unsigned char TestledStatus = 0;
unsigned char ExpledStatus = 0;
unsigned char True = 1;
unsigned char False = 0;
unsigned char Pressed = 0;
unsigned char Released = 1;
unsigned char Disabled = 0;
unsigned char Enabled = 1;
unsigned char LEDon = 1;
unsigned char LEDoff = 0;

unsigned short ADCword;
unsigned char ADChigh;
unsigned char ADClow;
unsigned char Ownaddress;
unsigned char Masteraddress;
unsigned char Modebyte;
unsigned char LEDbyte;
unsigned char Receiverbyte;
unsigned char Senderbyte;
unsigned char Scheduled;
unsigned char Devicemode;
unsigned char Freebuspriority;
unsigned char Sendflag;
unsigned char Ackwaitingflag;
unsigned char Ackwaitingphases;
unsigned char Receivedeventflag;
unsigned char Sendeventflag;
unsigned char Sendmessage[16];
unsigned char Receivedmessage[16];
unsigned char IR;
unsigned char B;
unsigned char Channelzerohigh;
unsigned char Channelzerolow;
unsigned char Channelonehigh;
unsigned char Channelonelow;
unsigned char Channeloneacknowledge;

unsigned char Index;


void ToggleLED(void);
void sendByteValue(unsigned int aVal);
void get_eeprom_byte( uint8_t setting );
void set_eeprom_byte( uint8_t eeprom_byte, uint8_t setting );

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------


int main(void)
{

  CONNECTOROUTPUT |= ShiftRegisterClock;
  CONNECTOROUTPUT |= SerialDataOut;
  CONNECTOROUTPUT |= StorageRegisterClock;

  PORTOUTPUT |= TestLED;
  PORTOUTPUT |= ExpLED;
  PORTOUTPUT |= Driver;
  PORTOUTPUT |= Receiverstop;
  PORTOUTPUT &= ~(TestButton);

  PORT  &= ~(Driver);                                  // Port-Pin PD2=0
  PORT  &= ~(Receiverstop);                            // Port-Pin PD3=0
  PORT |= TestButton;

  PORT |= (TestLED);


  Ackwaitingflag = False;
  Sendeventflag = False;
  Receivedeventflag = False;
  Sendflag = False;
#if defined (__AVR_ATmega8__)
  UBRRL  = 103;
  UBRRH  = 0;
  UCSRB |= 0b10011000;                    // Init. UART
#elif defined (__AVR_ATmega88__)
  UBRR0L  = 51;
  UBRR0H  = 0;
  UCSR0B |= 0b10011000;                   // Init. UART
#endif

  PORT  |= (Driver);                                  // Port-Pin PD2=0
  PORT  |= (Receiverstop);                            // Port-Pin PD3=0
  //sei();                                  //enable interrupts


  while(1)
  {

    if ((PORTREAD & TestButton) == Pressed)     // If test-button is pressed
    {
      if (ButtonStatus == Released)
      {
        ToggleLED();                        // toggle own test-led
        Sendeventflag = True;               // and there will be something to send when we are allowed to
      }
      ButtonStatus = Pressed;
    }
    else
    {
      ButtonStatus = Released;
    }



    if(True == Sendeventflag)
    {
      Sendeventflag = False;
      PORT |= (ExpLED);
#if   defined (__AVR_ATmega8__)
      while (!(UCSRA & 0b00100000)) {}                // wait until transmit-buffer is ready for next char
      UDR = 'Z';                          // next char in buffer

      while (!(UCSRA & 0b00100000)) {}
#elif defined (__AVR_ATmega88__)
      while (!(UCSR0A & 0b00100000)) {}    // wait until transmit-buffer is ready for next char
      UDR0 = 'Z';                          // next char in buffer
      while (!(UCSR0A & 0b00100000)) {}
#endif



      TimerDelayMsec(500);
      PORT &= ~(ExpLED);
    }
  }
}

void ToggleLED(void)                                             // Toggle the Test-LED
{
  if (TestledStatus == 1)
  {
    TestledStatus=0;
    LEDbyte=0;                                               // the data-byte to encode the LED-Status
    PORT &= ~(TestLED);
  }
  else
  {
    TestledStatus=1;
    LEDbyte=1;
    PORT |= (TestLED);
  }
}

/** @} */
