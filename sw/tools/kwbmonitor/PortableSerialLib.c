/*********************************************************************************************//**
 *   @file       PortableSerialLib.c
 *
 *   @author     Copyright (c) 2012 René Staffen r.staffen@gmx.de
 *   @version    $Id: PortableSerialLib.c 12 2012-03-09 20:58:40Z Vlad $
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


#include "PortableSerialLib.h"

#include <string.h>

static const char* PSerLib_getPlatformSpecificErrorMessage(PSL_ErrorCodes_e i_errorCode);
static const char c_errMsgUnknown[] = "Unknown error code.";

const char* PSerLib_getErrorMessage(PSL_ErrorCodes_e i_errorCode)
{

  static const char* c_errMsg[]={
    /* [PSL_ERROR_none               ] */ "No error occurred - everything is fine",
    /* [PSL_ERROR_couldNotOpen       ] */ "Specified port could not be opened.",
    /* [PSL_ERROR_invalidHandle      ] */ "Got invalid Handle as input.",
    /* [PSL_ERROR_unsupportedBitrate ] */ "Selected bit rate is not supported.",
    /* [PSL_ERROR_unsupportedConfig  ] */ "Selected configuration parameters are not supported.",
    /* [PSL_ERROR_configurePort      ] */ "Could not configure port.",
    /* [PSL_ERROR_notYetImplemented  } */ "The desired feature is not implemented yet.",
    /* [PSL_ERROR_bufferToSmall      } */ "The output buffer is to small to receive complete available data.",
  };
  if(   (i_errorCode < 0)
     || i_errorCode >= sizeof(c_errMsg)/sizeof(*c_errMsg) )
  {
    return PSerLib_getPlatformSpecificErrorMessage(i_errorCode);
  }
  return c_errMsg[i_errorCode];
}


/* ------------------------------------------------------------------------- */
/* - writing functions                                                     - */
/* ------------------------------------------------------------------------- */

PSL_ErrorCodes_e PSerLib_putc(PSerLibHandle_t io_port, char c)
{
  return PSerLib_writeBinaryData(io_port, (uint8_t*)&c, sizeof(c), NULL);
}



PSL_ErrorCodes_e PSerLib_puts(PSerLibHandle_t io_port, const char* i_str)
{
  const int size = strlen(i_str);
  return PSerLib_writeBinaryData(io_port, (uint8_t*)i_str, size, NULL);
}


/* ------------------------------------------------------------------------- */
/* - reading functions                                                     - */
/* ------------------------------------------------------------------------- */

PSL_ErrorCodes_e PSerLib_readLine( PSerLibHandle_t io_port, 
                                    char*             o_string,
                                    int               i_bufferLength,
                                    const char*       i_endLineChars)
{
  char c;
  int  endLineCharsCount = strlen(i_endLineChars);
  int  endlineFound = 0;
  PSL_ErrorCodes_e ret;
  int i = 0;
  do{
    ret = PSerLib_readBinaryData(io_port, (uint8_t*)(&c), 1,NULL);
    if(ret != PSL_ERROR_none){
      return ret;
    }
    o_string[i] = c;
    endlineFound = NULL != memchr(i_endLineChars, c, endLineCharsCount);
    ++i;
  }while(    (!endlineFound)
          && (i  < i_bufferLength-1));
  o_string[i] = '\0';
  if(!endlineFound){
    return PSL_ERROR_bufferToSmall;
  }else{
    return PSL_ERROR_none;
  }
}



#define PSL_DIRECTCOMPILATIONPROTECTION
#if   defined _WIN32
#  include "PortableSerialLibWindows.c"
#elif defined __linux
#  include "PortableSerialLibLinux.c"
#else
#  error PortableSerialLib does not support your os yet
#endif


