/**
 * @addtogroup TEST_APPDOWNLOAD
 * @brief   TODO
 *
 * @{
 * @file    main_appdownloadtest.c
 * @brief   TODO
 * TODO
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/crc16.h>
#include <util/delay.h>
#include <stdint.h>

#include "modcfg_common.h"
#include "eeprom_spi.h"
#include "spi.h"
#include "uart.h"

// --- Definitions -------------------------------------------------------------

#define XON                     17       /* XON Zeichen */
#define XOFF                    19       /* XOFF Zeichen */
#define START_SIGN              ':'      /* Hex-Datei Zeilenstartzeichen */

/* Zustände des Bootloader-Programms */
#define BOOT_STATE_EXIT         0
#define BOOT_STATE_PARSER       1
#define BOOT_STATE_IDLE         2

/* Zustände des Hex-File-Parsers */
#define PARSER_STATE_START      0
#define PARSER_STATE_SIZE       1
#define PARSER_STATE_ADDRESS    2
#define PARSER_STATE_TYPE       3
#define PARSER_STATE_DATA       4
#define PARSER_STATE_CHECKSUM   5
#define PARSER_STATE_ERROR      6

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static uint16_t hex2num (const uint8_t * ascii, uint8_t num)
{
    uint8_t  i;
    uint16_t val = 0;

    for (i=0; i<num; i++)
    {
        uint8_t c = ascii[i];

        /* Hex-Ziffer auf ihren Wert abbilden */
        if (c >= '0' && c <= '9')            c -= '0';
        else if (c >= 'A' && c <= 'F')       c -= 'A' - 10;
        else if (c >= 'a' && c <= 'f')       c -= 'a' - 10;

        val = 16 * val + c;
    }

    return val;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main ( void )
{
    uint32_t        flash_length = 0;
                    /* Empfangenes Zeichen + Statuscode */
    uint16_t        c = 0,
                    /* Intel-HEX Zieladresse */
                    hex_addr = 0,
                    /* Zu schreibende Flash-Page */
                    flash_page = 0,
                    /* Intel-HEX Checksumme zum Überprüfen des Daten */
                    hex_check = 0,
                    /* Positions zum Schreiben in der Datenpuffer */
                    flash_cnt = 0;
                    /* Flag zum steuern des Programmiermodus */
    uint8_t         boot_state = BOOT_STATE_IDLE,
                    /* Empfangszustandssteuerung */
                    parser_state = PARSER_STATE_START,
                    /* Flag zum ermitteln einer neuen Flash-Page */
                    flash_page_flag = 1,
                    /* Datenpuffer für die Hexdaten*/
                    flash_data[SPM_PAGESIZE],
                    /* Position zum Schreiben in den HEX-Puffer */
                    hex_cnt = 0,
                    /* Puffer für die Umwandlung der ASCII in Binärdaten */
                    hex_buffer[5],
                    /* Intel-HEX Datenlänge */
                    hex_size = 0,
                    /* Zähler für die empfangenen HEX-Daten einer Zeile */
                    hex_data_cnt = 0,
                    /* Intel-HEX Recordtype */
                    hex_type = 0,
                    /* empfangene HEX-Checksumme */
                    hex_checksum=0;

    // disable all interrupts
    cli();

    /* Füllen der Puffer mit definierten Werten */
    for (uint8_t ii=0; ii<sizeof(hex_buffer); ii++) hex_buffer[ii] = 0x00;
    for (uint8_t ii=0; ii<sizeof(flash_data); ii++) flash_data[ii] = 0xFF;

    // initialize UART, SPI and EEProm
    UART_vInit(9600);
    // enable sender and receiver
    DDRD |= (1<<PD2);
    PORTD |= (1<<PD2);

    SPI_vMasterInitBlk();
    EEP_vInit();

    UART_vPutString("TESTApplication 111 download\n\r");
    _delay_ms(2000);

    c = eeprom_read_byte((uint8_t*)MOD_eCfg_BldFlag);
    UART_vPutHex(c);
    UART_vPutString("\n\r");
    do
    {
        c = UART_uReceive();
        if(!(c & (1<<eUartFlag_NoData))) {
             /* Programmzustand: Parser */
             if(boot_state == BOOT_STATE_PARSER)
             {
                  switch(parser_state)
                  {
                      /* Warte auf Zeilen-Startzeichen */
                      case PARSER_STATE_START:
                          if((uint8_t)c == START_SIGN)
                          {
                              UART_vPutChar(XOFF);
                              parser_state = PARSER_STATE_SIZE;
                              hex_cnt = 0;
                              hex_check = 0;
                              UART_vPutChar(XON);
                          }
                          break;
                      /* Parse Datengröße */
                      case PARSER_STATE_SIZE:
                          hex_buffer[hex_cnt++] = (uint8_t)c;
                          if(hex_cnt == 2)
                          {
                              UART_vPutChar(XOFF);
                              parser_state = PARSER_STATE_ADDRESS;
                              hex_cnt = 0;
                              hex_size = (uint8_t)hex2num(hex_buffer, 2);
                              hex_check += hex_size;
                              UART_vPutChar(XON);
                           }
                           break;
                      /* Parse Zieladresse */
                      case PARSER_STATE_ADDRESS:
                          hex_buffer[hex_cnt++] = (uint8_t)c;
                          if(hex_cnt == 4)
                          {
                              UART_vPutChar(XOFF);
                              parser_state = PARSER_STATE_TYPE;
                              hex_cnt = 0;
                              hex_addr = hex2num(hex_buffer, 4);
                              hex_check += (uint8_t) hex_addr;
                              hex_check += (uint8_t) (hex_addr >> 8);
                              if(flash_page_flag)
                              {
                                  flash_page = hex_addr - hex_addr % SPM_PAGESIZE;
                                  flash_page_flag = 0;
                              }
                              UART_vPutChar(XON);
                           }
                           break;
                      /* Parse Zeilentyp */
                      case PARSER_STATE_TYPE:
                           hex_buffer[hex_cnt++] = (uint8_t)c;
                           if(hex_cnt == 2)
                           {
                               UART_vPutChar(XOFF);
                               hex_cnt = 0;
                               hex_data_cnt = 0;
                               hex_type = (uint8_t)hex2num(hex_buffer, 2);
                               hex_check += hex_type;
                               switch(hex_type)
                               {
                                   case 0: parser_state = PARSER_STATE_DATA; break;
                                   case 1: parser_state = PARSER_STATE_CHECKSUM; break;
                                   default: parser_state = PARSER_STATE_DATA; break;
                               }
                               UART_vPutChar(XON);
                           }
                           break;
                      /* Parse Flash-Daten */
                      case PARSER_STATE_DATA:
                          hex_buffer[hex_cnt++] = (uint8_t)c;
                          if(hex_cnt == 2)
                          {
                              UART_vPutChar(XOFF);
                              UART_vPutChar('.');
                              hex_cnt = 0;
                              flash_data[flash_cnt] = (uint8_t)hex2num(hex_buffer, 2);
                              hex_check += flash_data[flash_cnt];
                              flash_cnt++;
                              hex_data_cnt++;
                              if(hex_data_cnt == hex_size)
                              {
                                  parser_state = PARSER_STATE_CHECKSUM;
                                  hex_data_cnt=0;
                                  hex_cnt = 0;
                              }
                              /* Puffer voll -> schreibe Page */
                              if(flash_cnt == SPM_PAGESIZE)
                              {
                                  UART_vPutString("P\n\r");
                                  _delay_ms(100);
                                  EEP_uWrite((uint16_t)flash_page + MOD_eExtEEPAddr_AppStart,
                                             SPM_PAGESIZE, flash_data);
                                  for (uint8_t ii=0; ii<sizeof(flash_data); ii++)
                                      flash_data[ii] = 0xFF;
                                  flash_cnt = 0;
                                  flash_page_flag = 1;
                                  if (flash_page + SPM_PAGESIZE > flash_length) {
                                      flash_length = flash_page + SPM_PAGESIZE;
                                  }
                              }
                              UART_vPutChar(XON);
                          }
                          break;
                      /* Parse Checksumme */
                      case PARSER_STATE_CHECKSUM:
                          hex_buffer[hex_cnt++] = (uint8_t)c;
                          if(hex_cnt == 2)
                          {
                              UART_vPutChar(XOFF);
                              hex_checksum = (uint8_t)hex2num(hex_buffer, 2);
                              hex_check += hex_checksum;
                              hex_check &= 0x00FF;
                              /* Dateiende -> schreibe Restdaten */
                              if(hex_type == 1)
                              {
                                  UART_vPutString("P\n\r");
                                  _delay_ms(100);
                                  EEP_uWrite((uint16_t)flash_page + MOD_eExtEEPAddr_AppStart, SPM_PAGESIZE, flash_data);
                                  if (flash_page + SPM_PAGESIZE > flash_length) {
                                      flash_length = flash_page + SPM_PAGESIZE;
                                  }
                                  boot_state = BOOT_STATE_EXIT;
                              }
                              /* Überprüfe Checksumme -> muss '0' sein */
                              if(hex_check == 0) parser_state = PARSER_STATE_START;
                              else parser_state = PARSER_STATE_ERROR;
                              UART_vPutChar(XON);
                          }
                          break;
                      /* Parserfehler (falsche Checksumme) */
                      case PARSER_STATE_ERROR:
                          UART_vPutString("#\n");
                          break;
                      default:
                          break;
                  }
             }
             /* Programmzustand: UART Kommunikation */
             else if(boot_state != BOOT_STATE_PARSER)
             {
                 switch((uint8_t)c)
                 {
                     case 'p':
                         boot_state = BOOT_STATE_PARSER;
                         UART_vPutString("Programmiere das ext EEProm!\n\r");
                         UART_vPutString("Kopiere die Hex-Datei und füge sie"
                                   " hier ein (rechte Maustaste)\n\r");
                         break;
                     case 'q':
                         boot_state = BOOT_STATE_EXIT;
                         UART_vPutString("Verlasse die Applikation!\n\r");
                         break;
                     default:
                         UART_vPutString("Du hast folgendes Zeichen gesendet: ");
                         UART_vPutChar((unsigned char)c);
                         UART_vPutString("\n\r");
                         break;
                 }
             }
        }
    }
    while(boot_state!=BOOT_STATE_EXIT);

    UART_vPutString("Calculate CRC... ");

    // write length and crc to eeprom

    uint32_t ii;
    uint16_t crc = 0;
    uint8_t byte = 0;

    for (ii = 0; ii<flash_length; ii++) {
        EEP_uRead(MOD_eExtEEPAddr_AppStart, 1, &byte);
        crc = _crc16_update(crc, byte);
    }
    UART_vPutString("...done!\n\r");
    EEP_uWrite(MOD_eExtEEPAddr_AppSize, 4, (uint8_t*)&flash_length);
    EEP_uWrite(MOD_eExtEEPAddr_AppCrc, 2, (uint8_t*)&crc);
    eeprom_write_byte((uint8_t*)MOD_eCfg_BldFlag, 1<<eBldFlagAppProgram);

    UART_vPutString("Reset AVR!\n\r");
    _delay_ms(1000);
    return 0;
}

/** @} */
