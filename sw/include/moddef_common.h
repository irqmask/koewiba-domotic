/**
 * @addtogroup MODDEF_COMMON
 * @brief Project global layout of the internal configuration storage.
 * The location of all general settings which apply to every module are defined
 * here. Mostly, depending on the processor, this configuration area resides in
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

#define MOD_LEN_APPSIZE         2
#define MOD_LEN_APPCRC          2

#define MOD_LEN_CONTROLLERID    4
#define MOD_LEN_BOARDID         2
#define MOD_LEN_BOARDREV        1
#define MOD_LEN_APPID           2
#define MOD_LEN_APPVER          2

// --- Type definitions --------------------------------------------------------

//! Common register storage layout which is used by all applications.
typedef enum {
    MOD_eReg_Undefined,
    MOD_eReg_ModuleID,                  //!< Module ID as it appears on the bus.
    MOD_eReg_BldFlag,                   //!< Storage for bootloader flags for
                                        //!< next reboot will be reset by the
                                        //!< bootloader.

    MOD_eReg_FirstAppSpecific   = 16,
    MOD_eReg_LastAppSpecific    = 229,
    MOD_eReg_DeviceSignature0   = 230,  //!< 1st byte of device signature.
    MOD_eReg_DeviceSignature1   = 231,  //!< 2nd byte of device signature.
    MOD_eReg_DeviceSignature2   = 232,  //!< 3rd byte of device signature.
    MOD_eReg_BoardID            = 233,  //!< Board ID.
    MOD_eReg_AppID              = 234,  //!< Application ID.
    MOD_eReg_AppVersionMajor    = 235,  //!< Application version major.
    MOD_eReg_AppVersionMinor    = 236,  //!< Application version minor.
    MOD_eReg_AppVersionBugfix   = 237   //!< Application version bug-fix.
} MOD_eRegCommon_t;

//! Common configuration storage layout which is used by all applications.
//! Mostly, depending on the processor, this configuration area resides in the
//! controller's internal eeprom.
typedef enum {
    MOD_eCfg_ModuleID,                  //!< Module ID as it appears on the bus.
    MOD_eCfg_ModuleIDLo = MOD_eCfg_ModuleID,  //!< Module ID low-byte as it
                                        //!< appears on the bus.
    MOD_eCfg_ModuleIDHigh,              //!< Module high-byte ID as it appears
                                        //!< on the bus.
    MOD_eCfg_BldFlag,                   //!< Storage for bootloader flags for
                                        //!< next reboot will be reset by the
                                        //!< bootloader.
    MOD_eCfg_FirstAppSpecific   = 16,
} MOD_eCfgCommon_t;

//! Layout of external EEProm
typedef enum {
    MOD_eExtEEPAddr_AppSize     = 0,
    MOD_eExtEEPAddr_AppCrc      = MOD_eExtEEPAddr_AppSize + MOD_LEN_APPSIZE,
    MOD_eExtEEPAddr_AppStart    = MOD_eExtEEPAddr_AppCrc + MOD_LEN_APPCRC,
} MOD_eExtEEPAddr_t;

//! Flags of bootloader as they appear in MOD_eCfg_BldFlag.
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
