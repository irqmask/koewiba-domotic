/*********************************************************************************************//**
 *   @file       PortableSerialLib.h
 *
 *
 *   @author     Copyright (c) 2012 René Staffen r.staffen@gmx.de
 *   @version    $Id: PortableSerialLib.h 17 2012-03-10 22:36:04Z Vlad $
 *
 *   @copyright  This program is free software; you can redistribute it
 *               and/or modify it under the terms of the GNU General Public
 *               License as published by the Free Software Foundation;
 *               either version 2 of the License, or (at your option) any
 *               later version.
 *               For a full text of the license look at the enclosed gpl2.txt
 *               or at http://www.gnu.org/licenses/gpl-2.0.html
 *
 *   @details
 *
 *
 *//*********************************************************************************************/

#ifndef _PortableSerialLib_h__3322127
#define _PortableSerialLib_h__3322127

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * if 0 only a short name is reported by PSerLib_getAvailablePorts
 * function (without the device path)
 *
 * @todo consider make argument PSerLib_getAvailablePorts
 */
#define PSL_REPORT_FULL_DEVICE_NAME 0

/** Handle that represents a serial port object */
typedef struct PSerLibHandleInternal_t * PSerLibHandle_t;

/** Indicates a bad Handle */
#define PSL_NOPORT_HANDLE ((PSerLibHandle_t)(0))

/**
 * predefined baud rate constants \see PSerLib_setParams
 * some may not supported by different platforms
 */
typedef enum PSL_Baudrates_e{
PSL_BR_50   =0,
PSL_BR_75     ,
PSL_BR_110    ,
PSL_BR_134    ,
PSL_BR_150    ,
PSL_BR_200    ,
PSL_BR_300    ,
PSL_BR_600    ,
PSL_BR_1200   ,
PSL_BR_1800   ,
PSL_BR_2400   ,
PSL_BR_4800   ,
PSL_BR_9600   ,
PSL_BR_14400  ,
PSL_BR_19200  ,
PSL_BR_28800  ,
PSL_BR_38400  ,
PSL_BR_56000  ,
PSL_BR_57600  ,
PSL_BR_115200 ,
PSL_BR_128000 ,
PSL_BR_230400 ,
PSL_BR_256000 ,
PSL_BR_460800 ,
PSL_BR_500000 ,
PSL_BR_576000 ,
PSL_BR_921600 ,
PSL_BR_1000000,
PSL_BR_1152000,
PSL_BR_1500000,
PSL_BR_2000000,
PSL_BR_2500000,
PSL_BR_3000000,
PSL_BR_3500000,
PSL_BR_4000000,
PSL_BR_CONSTANTS_COUNT
}PSL_Baudrates_e;

/**
 * Constants for configuring number of data bits per transmitted character.
 * \see PSerLib_setParams
 */
typedef enum PSL_NumDataBits_e{
  PSL_DB_5 = 0,
  PSL_DB_6,
  PSL_DB_7,
  PSL_DB_8,
  PSL_DB_CONSTANTS_COUNT
}PSL_NumDataBits_e;

/** Constants for parity settings \see PSerLib_setParams */
typedef enum PSL_Parity_e{
  PSL_P_none  = 0,
  PSL_P_odd,
  PSL_P_even,
  PSL_P_mark,
  PSL_P_space,
  PSL_P_CONSTANTS_COUNT,
}PSL_Parity_e;

/**
 * Constants for configuring number of stop bits per transmitted character.
 * \see PSerLib_setParams
 */
typedef enum PSL_StopBits_e{
  PSL_SB_1 = 0,
  PSL_SB_1p5,
  PSL_SB_2,
  PSL_SB_CONSTANTS_COUNT,
}PSL_StopBits_e;

/**
 * Constants for configuring flow control.
 * \see PSerLib_setParams
 */
typedef enum PSL_FlowControl_e{
  PSL_FC_none = 0,
  PSL_FC_rts_cts,
  PSL_FC_xon_xoff,
  PSL_FC_rts_cts__xon_xoff,
  PSL_FC_CONSTANTS_COUNT,
}PSL_FlowControl_e;


/**
 *  Error codes that may be returned by the functions.
 *  \see PSerLib_getErrorMessage
 */
typedef enum PSL_ErrorCodes_e{
  PSL_ERROR_none = 0,
  PSL_ERROR_couldNotOpen,
  PSL_ERROR_invalidHandle,
  PSL_ERROR_unsupportedBitrate,
  PSL_ERROR_unsupportedConfig,
  PSL_ERROR_configurePort,
  PSL_ERROR_notYetImplemented,
  PSL_ERROR_bufferToSmall,

  PSL_ERROR_writeDataFailed,
  PSL_ERROR_readDataFailed,


  PSL_ERROR_START_WIN_SPECIFIC      = 1000,
  PSL_ERROR_couldNotOpenRegistryKey = PSL_ERROR_START_WIN_SPECIFIC,
  PSL_ERROR_couldNotIterateRegistryValues,
  PSL_ERROR_creatingDCBA,
  PSL_ERROR_setComState,

  PSL_ERROR_START_LINUX_SPECIFIC = 2000,
  PSL_ERROR_couldNotReadProcTtyFile = PSL_ERROR_START_LINUX_SPECIFIC,
  PSL_ERROR_couldParseProcTtyFile,
  PSL_ERROR_couldNotReadSysClassTty,


  PSL_ERROR_CODES_COUNT
}PSL_ErrorCodes_e;


/**
 * @brief  translates an error code in something printable
 * @param i_errorCode  the error code to translate
 * @returns pointer to a null-terminated string that gives a short
 *          description of the error
 * @ingroup PortUtility
 */
const char* PSerLib_getErrorMessage(PSL_ErrorCodes_e i_errorCode);


/**
 * @brief    returns a list with device names of available serial ports
 *
 * @param o_names          buffer for output\n
 *                         Each device name is quit a \\0 character.
 *                         An empty name indicates the end of the list.\n
 *                         Look at details - section for an example how to use the output.
 * @param i_maxlen         length of buffer
 * @param o_numPortsFound  pointer to variable that receives the number of ports found
 *
 * @returns PSL_ERROR_none if successful else another error code
 *
 * @details  Fiddeling out devices may be done in following fashion:
\code{.c}
  char buff[1000];
  char* iterator = buff;
  int n;
  PSerLib_getAvailablePorts(buff, sizeof(buff), &n);
  printf("found %i devices:\n", n);
  for( ;*iterator; iterator+=strlen(iterator)+1 )
  {
    printf("%s\n",iterator);
  }
\endcode
 * @ingroup PortUtility
 */
PSL_ErrorCodes_e PSerLib_getAvailablePorts(char* o_names, int i_maxlen, int* o_numPortsFound);


/**
 * tries to open the specified serial port
 * @param  i_portName  name of the port to open\n
 *                     These names are indeed some kind of platform specific - sorry.
 *                     But you may get a list of possible ports from \see PSerLib_getAvailablePorts.
 * @param  o_handle    pointer to a handle variable that receives the handle to the opened
 *                     port on success or PSL_NOPORT_HANDLE if opening failed
 *
 * @returns PSL_ERROR_none if successful else another error code
 * @ingroup PortHandling
 */
PSL_ErrorCodes_e PSerLib_open(const char* i_portName, PSerLibHandle_t* o_handle);


/**
 * @brief closes and deinitializes the port
 *
 * @param io_portH  pointer to port handle to close.
 *                  The handle will be invalidated.
 * @returns PSL_ERROR_none if successful else another error code
 * @ingroup PortHandling
 */
PSL_ErrorCodes_e PSerLib_close(PSerLibHandle_t* io_portH);



/**
 * @brief     sets the port to the given configuration
 * @attention currently not all baud rates are supported on
 *            linux (have to figure out how to set custom baud
 *            rates that are not defined in termios)
 *
 * @todo support all enumerated and custom baud rates
 *
 * @param io_port        handle of port to be configured
 * @param i_baudrate     baudrate if argument is less than PSL_BR_CONSTANTS_COUNT
 *                       it is interpreted as one of the enumerated constants.
 *                       If it is larger it is interpreted as baudrate.
 * @param i_bits         number of data bits
 * @param i_parity       parity
 * @param i_stopbits     number of stopbits
 * @param i_flowControl  flow control to use
 *
 * @returns PSL_ERROR_none if successful else another error code
 * @ingroup PortHandling
 */
PSL_ErrorCodes_e PSerLib_setParams(PSerLibHandle_t  io_port,
                                    PSL_Baudrates_e    i_baudrate,
                                    PSL_NumDataBits_e  i_bits,
                                    PSL_Parity_e       i_parity,
                                    PSL_StopBits_e     i_stopbits,
                                    PSL_FlowControl_e  i_flowControl
                                   );

/**
 * @todo specify
 * @ingroup PortHandling
 */
PSL_ErrorCodes_e PSerLib_setTimeOuts(PSerLibHandle_t  io_port);


/**
 * @brief  outputs the given character to the port
 *
 * @param io_port   handle of port to write to
 * @param c         character to send
 *
 * @returns PSL_ERROR_none if successful else another error code
 * @ingroup PortWrite
 */
PSL_ErrorCodes_e PSerLib_putc(PSerLibHandle_t io_port, char c);


/**
 * @brief  outputs the given string to the port
 *
 * @param io_port   handle of port to write to
 * @param i_str     null terminated string
 *
 * @returns PSL_ERROR_none if successful else another error code
 * @ingroup PortWrite
 */
PSL_ErrorCodes_e PSerLib_puts(PSerLibHandle_t io_port, const char* i_str);


/**
 * @brief  outputs the given data to the port
 *
 * @param io_port         handle of port to write to
 * @param i_data          pointer to data to output
 * @param i_dataLen       number of bytes to read from data pointer
 * @param o_bytesWritten  number of bytes actual written
 *                        (May differ because of time out or error)\n
 *                        Can be NULL if not required.
 *
 * @remark Sending/receiving of binary data may be incompatible with
 *         connections that base on XON/XOFF flow control
 *         because XON/XOFF are special bytes.
 *         If you use XON/XOFF flow control then ensure that these
 *         bytes cannot appear in i_data else your communication may
 *         screw up.
 *
 * @returns PSL_ERROR_none if successful else another error code
 * @ingroup PortWrite
 */
PSL_ErrorCodes_e PSerLib_writeBinaryData(       PSerLibHandle_t io_port,
                                           const uint8_t*          i_data,
                                                 int               i_dataLen,
                                                 int*              o_bytesWritten);



/**
 * @brief  reads data from the port
 *
 * @param io_port         handle of port to read from
 * @param o_string        pointer to buffer where the data should be stored.
 * @param i_bufferLength  number of bytes the buffer can take.
 *                        Remember that the buffer also have to take the end
 *                        character and terminating \\0 character
 * @param i_endLineChars  \\0 terminated string, that contains characters
 *                        that indicates line end.
 *                        The line end character will also be copied into
 *                        output buffer.
 *
 * @returns PSL_ERROR_none if successful else another error code
 * @ingroup PortRead
 */
PSL_ErrorCodes_e PSerLib_readLine( PSerLibHandle_t io_port,
                                    char*             o_string,
                                    int               i_bufferLength,
                                    const char*       i_endLineChars);


/**
 * @brief  reads data from the port
 *
 * @param io_port         handle of port to read from
 * @param o_data          pointer to buffer where the data should be stored.
 *                        Ensure that the buffer can take at least i_dataToRead
 *                        bytes.
 * @param i_dataToRead    number of bytes to read
 * @param o_bytesRead     number of bytes actual read
 *                        (May differ because of time out or error)\n
 *                        Can be NULL if not required.
 *
 * @remark see remarks of PSerLib_writeBinaryData
 *
 * @returns PSL_ERROR_none if successful else another error code
 * @ingroup PortRead
 */
PSL_ErrorCodes_e PSerLib_readBinaryData( PSerLibHandle_t io_port,
                                          uint8_t*          o_data,
                                          int               i_dataToRead,
                                          int*              o_bytesRead);



#ifdef __cplusplus
}
#endif

#endif /* _PortableSerialLib_h__3322127 */