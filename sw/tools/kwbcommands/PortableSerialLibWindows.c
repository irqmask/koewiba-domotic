/*********************************************************************************************//**
 *   @file       PortableSerialLibWindows.c
 *
 *   @author     Copyright (c) 2012 René Staffen r.staffen@gmx.de
 *   @version    $Id: PortableSerialLibWindows.c 17 2012-03-10 22:36:04Z Vlad $
 *  
 *   @copyright  This program is free software; you can redistribute it
 *               and/or modify it under the terms of the GNU General Public 
 *               License as published by the Free Software Foundation; 
 *               either version 2 of the License, or (at your option) any
 *               later version.
 *               For a full text of the license look at the enclosed gpl2.txt
 *               or at http://www.gnu.org/licenses/gpl-2.0.html
 *
 *//*********************************************************************************************/

/* code goes here to be protected from accidently be directly compiled 
   (this file has to be included from PortableSerialLib.c)
 */
#ifdef PSL_DIRECTCOMPILATIONPROTECTION
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

/* ========================================================================= */
/* = internal port structure                                               = */
/* ========================================================================= */
struct PSerLibHandleInternal_t
{
  HANDLE portHandle;
  DCB    portSettings;
};



/* ========================================================================= */
/* = some look up tables for mapping params to platform dependent constants= */
/* ========================================================================= */

static const int c_baudrates[] ={
50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800, 2400, 4800, 9600, 14400,
19200, 28800, 38400, 56000, 57600, 115200, 128000, 230400, 256000, 460800,
500000, 576000, 921600, 1000000, 1152000, 1500000, 2000000, 2500000, 3000000,
3500000, 4000000, 
};

static const char c_parity[] = {'n','o','e','m','s',};
static const char* const c_stopbits[] ={"1", "1.5", "2"};
static const char c_databits[] = {'5','6','7','8'};

static const char* const c_flowcontrol[]={
  /* PSL_FC_none              */ "xon=off odsr=off octs=off dtr=off rts=off idsr=off",
  /* PSL_FC_rts_cts           */ "xon=off odsr=off octs=on dtr=off rts=hs idsr=off",
  /* PSL_FC_xon_xoff          */ "xon=on odsr=off octs=off dtr=off rts=off idsr=off",
  /* PSL_FC_rts_cts__xon_xoff */ "xon=on odsr=off octs=on dtr=off rts=hs idsr=off"
};


/* ========================================================================= */
/* = helper functions                                                      = */
/* ========================================================================= */

static const char* PSerLib_getPlatformSpecificErrorMessage(PSL_ErrorCodes_e i_errorCode)
{
  static const char* c_errMsg[]={
    /* [PSL_ERROR_couldNotOpenRegistryKey      ] */ "Error opening registry key.",
    /* [PSL_ERROR_couldNotIterateRegistryValues] */ "Error iterating registry values.",
    /* [PSL_ERROR_creatingDCBA                 ] */ "Error creating DCBA structure.",
    /* [PSL_ERROR_setComState                  ] */ "Error setting configuring port (SetComState).",
  };
  int ec = i_errorCode - PSL_ERROR_START_WIN_SPECIFIC;
  if(    (ec <  0)
      || (ec >= sizeof(c_errMsg)/sizeof(*c_errMsg) ))
  {
    return c_errMsgUnknown;
  }
  return c_errMsg[ec];
}


/* ========================================================================= */
/* = interface implementation                                              = */
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
/* - port handling                                                         - */
/* ------------------------------------------------------------------------- */


PSL_ErrorCodes_e PSerLib_open(const char* i_portName, PSerLibHandle_t* o_handle)
{
  PSerLibHandle_t pspH;
  HANDLE fileHandle;
  COMMTIMEOUTS cmt;
  char nameBuffer[50];
  if( NULL == strchr(i_portName, '\\')){  /* seems not to be a devicepath (some kind like \\.\COM1 */
    sprintf_s(nameBuffer, sizeof(nameBuffer), "\\\\.\\%s", i_portName);
  }else{
    strcpy_s(nameBuffer, sizeof(nameBuffer), i_portName);
  }
  
  fileHandle = CreateFile( nameBuffer, 
                            GENERIC_READ | GENERIC_WRITE,
                            0,             //sharing mode ports CANNOT be shared
                            NULL,          //security attributes --> file handle cannot be inherited
                            OPEN_EXISTING, //we cannot create serial ports
                            FILE_FLAG_OVERLAPPED,// to allow reading and writing from diffrent threads
                            NULL);         //no template

  if (fileHandle == INVALID_HANDLE_VALUE) {
    *o_handle = PSL_NOPORT_HANDLE;
    /** @todo some more differentiation about errors reason */
    return PSL_ERROR_couldNotOpen;
  }

  cmt.ReadIntervalTimeout = 0x7fFFffF0;
  cmt.ReadTotalTimeoutConstant = 0x7fFFffF0;
  cmt.ReadTotalTimeoutMultiplier = 0x7fFFffF0;
  cmt.WriteTotalTimeoutConstant = 0x7fFFffF0;
  cmt.WriteTotalTimeoutMultiplier = 0x7fFFffF0;
  if(!SetCommTimeouts(fileHandle, &cmt)) {
    return PSL_ERROR_couldNotOpen;
  }
  pspH = malloc(sizeof(*pspH));
  pspH->portHandle = fileHandle;

  *o_handle = pspH;
  return PSL_ERROR_none;
}


PSL_ErrorCodes_e PSerLib_close(PSerLibHandle_t* io_portH)
{
  if(io_portH && (*io_portH)){
    CloseHandle((*io_portH)->portHandle);
    free(*io_portH);
    (*io_portH) = PSL_NOPORT_HANDLE;
  }
  return PSL_ERROR_none;
}


PSL_ErrorCodes_e PSerLib_getAvailablePorts(char* o_names, int i_maxlen, int* o_numPortsFound)
{
  PSL_ErrorCodes_e result = PSL_ERROR_none;
  HKEY keySerialCom;
  *o_names = '\0';
   --i_maxlen;  // -1 becasue we need space for one additional \0 character
  if(o_numPortsFound){
    *o_numPortsFound = 0;
  }

  if( ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM", 0, KEY_QUERY_VALUE, &keySerialCom))
  { 
     char  valueNameBuffer[200];
    DWORD valueNameBufferSize = sizeof(valueNameBuffer);
    BYTE  valueValueBuffer[50];
    DWORD valueValueBufferSize = sizeof(valueValueBuffer);
    DWORD valueIndex=0;
    DWORD valType;
    LONG enumValRet;
    do{
       enumValRet = RegEnumValue( keySerialCom, valueIndex   // iterate over serialcom key
                                , valueNameBuffer, &valueNameBufferSize  // name of variable is not interesting to us
                                , NULL
                                ,&valType
                                , valueValueBuffer
                                , &valueValueBufferSize);
       if( ERROR_SUCCESS == enumValRet)
       {
          int len = valueValueBufferSize-1; // valueValueBufferSize seems to be stringlen+1
#         if (PSL_REPORT_FULL_DEVICE_NAME!=0)
            len += 4;
#           define NAMEADDON "\\\\.\\"
#         else
#           define NAMEADDON ""
#         endif
          if(i_maxlen > len){
            sprintf_s(o_names, i_maxlen, NAMEADDON"%s", valueValueBuffer);
            i_maxlen -= len + 1;
            o_names  += len + 1;
          }else{
            result = PSL_ERROR_bufferToSmall;
          }
          valueNameBufferSize  = sizeof(valueNameBuffer);
          valueValueBufferSize = sizeof(valueValueBuffer);
          ++valueIndex;
       }
    }while(ERROR_SUCCESS==enumValRet);
    *o_names = '\0';
    if(o_numPortsFound){
      *o_numPortsFound = valueIndex;
    }
    if(   (ERROR_SUCCESS       != enumValRet)
       && (ERROR_NO_MORE_ITEMS != enumValRet))
    {
      return PSL_ERROR_couldNotIterateRegistryValues;
    }
    return result;
  }else{
    return PSL_ERROR_couldNotOpenRegistryKey;
  }
}

/* ------------------------------------------------------------------------- */

PSL_ErrorCodes_e PSerLib_setParams(PSerLibHandle_t  io_port,
                                    PSL_Baudrates_e    i_baudrate, 
                                    PSL_NumDataBits_e  i_bits,
                                    PSL_Parity_e       i_parity,
                                    PSL_StopBits_e     i_stopbits,
                                    PSL_FlowControl_e  i_flowControl
                                   )
{
  char modeBuffer[100];
  int baudrate =   (i_baudrate<PSL_BR_CONSTANTS_COUNT)
                 ? c_baudrates[i_baudrate]
                 :(int)i_baudrate;
  if(io_port == PSL_NOPORT_HANDLE){
    return PSL_ERROR_invalidHandle;
  }

  if(   (i_bits        >= PSL_DB_CONSTANTS_COUNT)
     || (i_stopbits    >= PSL_SB_CONSTANTS_COUNT)
     || (i_parity      >= PSL_P_CONSTANTS_COUNT)
     || (i_flowControl >= PSL_FC_CONSTANTS_COUNT)
     )
  {
    return PSL_ERROR_unsupportedConfig;
  }


  sprintf_s(modeBuffer, sizeof(modeBuffer)
            , "baud=%d parity=%c data=%c stop=%s %s"
            , baudrate
            , c_parity[i_parity]
            , c_databits[i_bits]
            , c_stopbits[i_stopbits]
            , c_flowcontrol[i_flowControl]);

  memset(&(io_port->portSettings), 0, sizeof(io_port->portSettings));  /* clear the new struct  */
  io_port->portSettings.DCBlength = sizeof(io_port->portSettings);

  if(!BuildCommDCBA(modeBuffer, &(io_port->portSettings)))
  {
    return PSL_ERROR_creatingDCBA;
  }

  if(!SetCommState(io_port->portHandle, &(io_port->portSettings)))
  {
    return PSL_ERROR_setComState;
  }
  return PSL_ERROR_none;

}

/* ------------------------------------------------------------------------- */
/* - writing functions                                                     - */
/* ------------------------------------------------------------------------- */


PSL_ErrorCodes_e PSerLib_writeBinaryData(       PSerLibHandle_t io_port, 
                                           const uint8_t*          i_data,
                                                 int               i_dataLen,
                                                 int*              o_bytesWritten)
{
  DWORD written = 0;
  OVERLAPPED osWrite = {0};
  PSL_ErrorCodes_e fRes = PSL_ERROR_none;
  const DWORD size = i_dataLen;
  if(io_port == PSL_NOPORT_HANDLE){
    return PSL_ERROR_invalidHandle;
  }
  if(o_bytesWritten){
    *o_bytesWritten = 0;
  }

  //WriteFile(io_port->portHandle, i_data, size, &written, NULL);
  //ret = written == size;
  osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  if (osWrite.hEvent == NULL){
    // Error creating overlapped event handle.
    return PSL_ERROR_writeDataFailed;
  }

  // Issue write.
  if (!WriteFile(io_port->portHandle, i_data, size, &written, &osWrite)) {
    if (GetLastError() != ERROR_IO_PENDING) { 
      // WriteFile failed, but it isn't delayed. Report error and abort.
      fRes = PSL_ERROR_writeDataFailed;
    }
    else {
      // Write is pending.
      if (!GetOverlappedResult(io_port->portHandle, &osWrite, &written, TRUE)){
        fRes = PSL_ERROR_writeDataFailed;
      }else{
        // Write operation completed successfully.
        fRes = PSL_ERROR_none;
      }
    }
  }else{
    // WriteFile completed immediately.
    fRes = PSL_ERROR_none;
  }

  CloseHandle(osWrite.hEvent);

  if(o_bytesWritten){
    *o_bytesWritten = written;
  }

  return fRes;
}


/* ------------------------------------------------------------------------- */
/* - reading functions                                                     - */
/* ------------------------------------------------------------------------- */



PSL_ErrorCodes_e PSerLib_readBinaryData( PSerLibHandle_t io_port, 
                                          uint8_t*          o_data,
                                          int               i_dataToRead,
                                          int*              o_bytesRead)
{
  PSL_ErrorCodes_e fRes = PSL_ERROR_none;
  DWORD dwRead = 0;
  OVERLAPPED osReader = {0};

  // Create the overlapped event. Must be closed before exiting
  // to avoid a handle leak.
  osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

  if (osReader.hEvent == NULL){
    // Error creating overlapped event; abort.
    fRes = PSL_ERROR_readDataFailed;
  }else{
    if (!ReadFile(io_port->portHandle, o_data, i_dataToRead, &dwRead, &osReader)) {
      if( ERROR_IO_PENDING == GetLastError()){
        if (!GetOverlappedResult(io_port->portHandle, &osReader, &dwRead, TRUE))
        {
          fRes = PSL_ERROR_readDataFailed;
        }
      }else{
        fRes = PSL_ERROR_readDataFailed;
      }

    }else {    
    }
  }
  if( o_bytesRead ){
    *o_bytesRead = dwRead;
  }
  return fRes;
}


#endif



