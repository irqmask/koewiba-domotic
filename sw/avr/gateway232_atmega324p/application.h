/**
 * @addtogroup BUS_GATEWAY
 * @addtogroup APPLICATION
 * @brief Interface of the business logic of the application taster8.
 *
 * @{
 * @file    application.h
 * @brief   Interface of the business logic of the application taster8.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "register.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

sBus_t  app_bus;

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

void        app_initialize_modes    (void);

// --- Global functions --------------------------------------------------------

void        app_initialize          (void);

void        app_check_keys          (void);

bool        app_register_get        (uint8_t                uRegNo,
                                     eRegType_t*            peRegType,
                                     void*                  pvValue);

void        app_register_set        (uint8_t                uRegNo,
                                     uint32_t               uValue);

/** @} */
