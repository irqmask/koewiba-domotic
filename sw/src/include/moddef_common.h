/**
 * @addtogroup MODDEF_COMMON
 * @brief Project global layout of the internal configuration storage.
 * The location of all general settings which apply to every module are defined
 * here. Mosty, depending on the procesor, this configuration area resides in
 * the controller's internal eeprom.
 *
 * @{
 * @file    moddef_common.h
 * @brief   Project global layout of the internal configuration storage.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _MODDEF_COMMON_H_
#define _MODDEF_COMMON_H_

// --- Include section ---------------------------------------------------------

// --- Definitions -------------------------------------------------------------

#define MOD_LEN_APPSIZE         4
#define MOD_LEN_APPCRC          2

// --- Type definitions --------------------------------------------------------

//! Common configuration storage layout which is used by all applications.
//! Mostly, depending on the processor, this configuration area resides in the
//! controller's internal eeprom.
typedef enum {
    MOD_eCfg_ModuleID,                  //!< Module ID as it appears on the bus.
    MOD_eCfg_BldFlag,                   //!< Storage for bootloader flags for
                                        //!< next reboot will be reset by the
                                        //!< bootloader.

    MOD_eCfg_FirstAppSpecific   = 16
} MOD_eCfgCommon_t;

typedef enum {
    MOD_eExtEEPAddr_AppSize     = 0,
    MOD_eExtEEPAddr_AppCrc      = MOD_eExtEEPAddr_AppSize + MOD_LEN_APPSIZE,
    MOD_eExtEEPAddr_AppStart    = MOD_eExtEEPAddr_AppCrc + MOD_LEN_APPCRC,
} MOD_eExtEEPAddr_t;

typedef enum {
    eBldFlagNewSWProgrammed,
    eBldFlagCRCMismatch,
    eBldFlagControllerTypeMismatch,
    eBldFlagBoardTypeMismatch,
    eBldFlagAppIDChanged,
    eBldFlagAppVersionChanged,
    eBldFlagUnused,
    eBldFlagAppProgram
} eBldStatusFlags_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _MODDEF_COMMON_H_ */
/** @} */
