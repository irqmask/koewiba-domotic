/**
 * @addtogroup KWB_DEFINES
 * @brief KoeWiBa global definitions.
 *
 * This module contains the definition global to the KoeWiBa project.
 *
 * @{
 * @file    kwb_defines.h
 * @brief   KoeWiBa global definitions.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2017  christian <irqmask@gmx.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _KWB_DEFINES_H_
#define _KWB_DEFINES_H_

// --- Include section ---------------------------------------------------------

// --- Definitions -------------------------------------------------------------

// common used log masks
#define KWB_LOG_ERROR       LOG_ERROR       //!< Logmask for errors
#define KWB_LOG_WARNING     LOG_WARNING     //!< Logmask for warnings
#define KWB_LOG_INFO        LOG_INFO        //!< Logmask for information
#define KWB_LOG_STATUS      LOG_STATUS      //!< Logmask for status changes
#define KWB_LOG_INTERCOMM   LOG_USERDEFINED //!< Logmask for interface communication 
#define KWB_LOG_VERBOSE1    LOG_VERBOSE1    //!< Logmask for verbosity level 1
#define KWB_LOG_VERBOSE2    LOG_VERBOSE2    //!< Logmask for verbosity level 2
#define KWB_LOG_VERBOSE3    LOG_VERBOSE3    //!< Logmask for verbosity level 3
#define KWB_LOG_TRACE       LOG_VERBOSE4    //!< Logmask for function tracing

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _KWB_DEFINES_H_ */
/** @} */
