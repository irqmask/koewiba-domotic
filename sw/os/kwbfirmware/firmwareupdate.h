/**
 * @addtogroup FIRMWAREUPDATE
 * @brief Process firmware update of a module.
 *
 * @{
 * @file    firmwareupdate.h
 * @brief   Process firmware update of a module.
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
#ifndef _FIRMWAREUPDATE_H_
#define _FIRMWAREUPDATE_H_

// --- Include section ---------------------------------------------------------

#include "connection.h"
#include "systime.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

/**
 * States of the firmware update
 */
typedef enum fwu_states {
    eFWU_IDLE,
    eFWU_START,
    eFWU_DATA,
    eFWU_WAIT_ACK,
    eFWU_END,
    eFWU_CRC_INFO,
    eFWU_RESET_NODE,
    eFWU_WAIT_AFTER_RESET,
    eFWU_BLDFLAGS,
    eFWU_WAIT_BLDFLAGS,
    eFWU_ERROR_ABORT,
    eFWU_LAST
} fwu_states_t;

/**
 * Callback type to inform outer application about firmware update progress.
 */
typedef void (*fwu_progress_func_t)(uint8_t progress, void *arg);

/**
 * Firmware update process.
 */
class FirmwareUpdate
{
public:
    FirmwareUpdate();
    FirmwareUpdate(std::shared_ptr<Connection> connection);
    ~FirmwareUpdate();

    int start(const char *filename, uint16_t module_address);

    int run();

    void registerProgressFunc(fwu_progress_func_t func, void *arg);

    void setProgressThd(uint8_t thd);

    uint8_t getBldFlags();
    uint16_t getNodeCrcCalculated();
    uint16_t getNodeCrcExpected();

protected:
    //! Current state of firmware-update statemachine.
    fwu_states_t    curr_state;
    //! Handle of the connection.
    std::shared_ptr<Connection> connection;
    //! Path and filename to the IHEX file.
    char            filename[256];
    //! target module address.
    uint16_t        module_address;

    //! Pointer to the memory block wich contains the target module's firmware.
    uint8_t        *fw_memory;
    //! First (lowest) address of the target module's firmware.
    uint32_t        fw_firstaddress;
    //! Program entry point of the target's firmware.
    uint32_t        fw_startaddress;
    //! Size on bytes of the firmware.
    uint32_t        fw_size;

    //! Current offset in fw_memory during transmission.
    uint32_t        curr_offset;
    //! Last offset in fw_memory during transmission.
    uint32_t        last_offset;
    //! Last offset which has been acknowledged by node.
    uint32_t        last_node_offset;
    //! Last used address in node.
    uint32_t        last_node_address;
    //! Checksum expected by node.
    uint16_t        node_crc_expected;
    //! Checksum calculated by node.
    uint16_t        node_crc_calculated;
    //! Flag if block_info message has been received. Reset by block_data and
    //! block_end message. Set by block_info message.
    uint8_t         block_info_received;
    //! Flag, if the target not shall be reset automatically after firmware
    //! upload.
    uint8_t         reset_target_node;
    //! timestamp of start of waiting for a response.
    sys_time_t      wait_start;
    //! Bootloader flags, which inform about the upload success.
    uint8_t         bldflags;
    //! Flag, if the requested bootloader flags have been received yet.
    uint8_t         bldflags_received;
    //! last progress
    uint8_t         last_progress;
    //! threshold of progress update.
    uint8_t         progress_thd;
    //! progress update function.
    fwu_progress_func_t progress_func;
    //! optional arguments for progress_func;
    void           *progress_arg;

    uint16_t calculateCRC16();
    void parseSetReg8bitMsg(const msg_t &message);
    void parseBlockInfoMsg(const msg_t &message);
    void parseBlockAckMsg(const msg_t &message);
    void parseBlockNakMsg(const msg_t &message);
    void handleIncomingMessage(const msg_t &message, void *reference);

    int sendBlockStartMessage();
    int sendBlockDataMessage();
    int sendBlockEndMessage();
    int sendResetMessage();
    int sendRequestBldflagsMessage();
    int receiveAndCheckBldflags();
};

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _FIRMWAREUPDATE_H_
/** @} */
