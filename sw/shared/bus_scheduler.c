/**
 * @addtogroup BUS
 * @addtogroup BUS_SCHEDUL
 * @brief Message scheduler of bus messages. Used by the master on the bus.
 *
 * @{
 * @file    bus_schedul.c
 * @brief   Bus message scheduler.
 * @author  Christian Verhalen, Robert Mueller
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#ifdef PRJCONF_UC_AVR
# include <avr/io.h>
# include <avr/sleep.h>
# include <util/delay.h>
#endif

#include "bus.h"
#include "bus_scheduler.h"
#include "bus_intern.h"
#include "led_debug.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Resets the error-counter for uNodeAddress.
 *
 * @param[in] psSched       Handle of the scheduler-struct.
 * @param[in] uNodeAddress  Node address.
 */
void sched_reset_node_error(sSched_t* psSched, uint8_t uNodeAddress)
{
    if (uNodeAddress == psSched->uErrorNode) {    // error-counter is counting for uNodeAddress
        psSched->uErrorNode  = 0;
        psSched->uErrorCount = 0;
    }
}


/**
 * Provides the next node-address to be scheduled.
 *
 * @param[in] psSched       Handle of the scheduler-struct.
 * @returns                 Next node-address.
 */
 uint8_t sched_get_next_node(sSched_t* psSched)
 {
     uint8_t nodebit, nodebyte, mask;
     uint8_t foundnode = 0,
             startnode = 0;
     bool    secondrun = false, discover_round_complete = false;

     // loop until at least one present node or one missing node have been found.
     while (foundnode == 0) {
         // search first present node, beginning at last present node + 1
         startnode = psSched->uCurrentNode;
         do {
             // was last node not a discovery node?
             if (!psSched->bSchedDiscovery && !secondrun) {
                 psSched->uCurrentNode++;
                 if (psSched->uCurrentNode > BUS_LASTNODE) {
                     psSched->uCurrentNode = BUS_FIRSTNODE;
                     break; // now a new node shall be discovered
                 }
             }
             // test if node is available
             nodebyte = psSched->asNodeList.uAddress[psSched->uCurrentNode >> 3];
             nodebit  = psSched->uCurrentNode % 8;
             mask = (1<<nodebit);
             if ((nodebyte & mask) != 0) {
                 foundnode = psSched->uCurrentNode;
                 psSched->bSchedDiscovery = false;
                 break;
             }
         } while (psSched->uCurrentNode != startnode);

         // search first missing node, beginning at last missing node + 1
         if (foundnode == 0) {
             startnode = psSched->uDiscoverNode;
             do {
                 psSched->uDiscoverNode++;
                 if (psSched->uDiscoverNode > BUS_LASTNODE) {
                     psSched->uDiscoverNode = BUS_FIRSTNODE;
                     discover_round_complete = true;
                 }
                 // test if node is NOT available
                 nodebyte = psSched->asNodeList.uAddress[psSched->uDiscoverNode >> 3];
                 nodebit  = psSched->uDiscoverNode % 8;
                 mask = (1<<nodebit);
                 if ((nodebyte & mask) == 0) {
                     foundnode = psSched->uDiscoverNode;
                     psSched->bSchedDiscovery = true;
                     break;
                 }
             } while (psSched->uDiscoverNode != startnode);
         }
         secondrun = true;
     }
     // one discovery round no activity on bus
     if (discover_round_complete && psSched->uSleepLoopCnt) {
         psSched->uSleepLoopCnt--;
     }
     return foundnode;
}


/**
 * Add/Remove node to/from scheduler list.
 *
 * @param[in] psSched       Handle of the scheduler-struct.
 * @param[in] uNodeAddress  Node-address.
 * @param[in] bOnline       Node-state.
 */
void sched_set_node_state(sSched_t* psSched, uint8_t uNodeAddress, bool bOnline)
{
    uint8_t     *ubyte = &psSched->asNodeList.uAddress[uNodeAddress/8];

    if (bOnline) *ubyte |=  (1<<uNodeAddress%8);
    else         *ubyte &= ~(1<<uNodeAddress%8);
}


/**
 * Counts errors for one node till it gets reset or counter reaches max.
 *
 * @param[in] psSched       Handle of the scheduler-struct.
 * @param[in] uNodeAddress  Node-address.
 */
void sched_set_node_error(sSched_t* psSched, uint8_t uNodeAddress)
{
    if (0==psSched->uErrorNode) psSched->uErrorNode = uNodeAddress; // error-counter is free
    else if (uNodeAddress==psSched->uErrorNode) {    // error-counter is counting for uNodeAddress
        psSched->uErrorCount++;
        if (psSched->uErrorCount >= BUS_MAX_ANSWERFAILS) { // max errors counted ...
            sched_set_node_state(psSched, uNodeAddress, false); // set node-address offline
            // If there is no other node online set current-node 0
            psSched->uErrorNode  = 0;
            psSched->uErrorCount = 0;
        }
    }
}


/**
 * Start sending of next token.
 *
 * @param[in] psBus         Handle of the bus-struct.
 * @param[in] psSched       Handle of the scheduler-struct.
 * @returns                 true if token has been successfully sent, otherwise false.
 */
bool sched_send_next_timeslot_token(sBus_t* psBus, sSched_t* psSched)
{
    uint8_t node;

    if ((node = sched_get_next_node(psSched)) == 0) {
        return false;
    }
    psSched->auTokenMsg[1] = node | 0x80;
    return bus_phy_send(&psBus->sPhy, psSched->auTokenMsg, BUS_TOKEN_MSG_LEN);
}


/**
 * Check if current node is the scheduler node.
 *
 * @param[in] psBus         Handle of the bus-struct.
 * @param[in] psSched       Handle of the scheduler-struct.
 * @returns                 true if token matches scheduler-address, otherwise false.
 */
bool sched_current_node_is_me(sBus_t* psBus, sSched_t* psSched)
{
    uint8_t node;

    node = psSched->auTokenMsg[1] & 0x7F;

    if (node == psBus->sCfg.uOwnNodeAddress) return true;
    return false;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------


/**
 * Initialize scheduler functions.
 *
 * @param[in] psSched       Handle of the scheduler-struct.
 */
void bus_scheduler_initialize (sBus_t* psBus, sSched_t* psSched, uint8_t uUart)
{
    uint8_t ii;

    bus_initialize(psBus, uUart);
    // no eBus_InitWait state for scheduler, because it has immediately to
    // start scheduling
    psBus->eState = eBus_Idle;
    psBus->eModuleState = eMod_Running;

    // initialize scheduler data
    psSched->uCurrentNode  = BUS_FIRSTNODE;
    psSched->uDiscoverNode = BUS_FIRSTNODE;
    for (ii=0; ii<BUS_LASTNODE/8; ii++) {
        psSched->asNodeList.uAddress[ii] = 0;
    }
    psSched->auTokenMsg[0] = BUS_SYNCBYTE;
    psSched->auTokenMsg[1] = 0;
    psSched->bSchedDiscovery = true;
    psSched->bSchedWaitingForAnswer = false;
    psSched->uSleepLoopCnt = BUS_LOOPS_TILL_SLEEP;
}

/**
 * Schedule nodes on bus.
 *
 * @param[in] psBus         Handle of the bus.
 * @param[in] psSched       Handle of the scheduler-struct.
 * @returns                 true if message was received, otherwise false.
 */
bool bus_schedule_and_get_message (sBus_t* psBus, sSched_t* psSched )
{
    bool msg_received = false;

    switch (psBus->eState) {

    case eBus_Idle:
        // send token
        if (sched_send_next_timeslot_token(psBus, psSched)) {
            timer_start(&psSched->sNodeAnsTimeout, TIMER_MS_2_TICKS(BUS_MESSAGE_TIMEOUT));
            psBus->eState = eBus_SendingToken;
        }
        break;

    case eBus_SendingToken:
        // wait for finished token sending
        if (!bus_phy_sending(&psBus->sPhy)) {
            if (sched_current_node_is_me(psBus, psSched)) {
                // The scheduler cannot receive its own message,
                // so we set the state to eBus_GotToken "manually".
                sched_set_node_state(psSched, psBus->sCfg.uOwnNodeAddress, true);
                psBus->eState = eBus_GotToken;
            }
            else {
                // Token has been sent, now wait for a message.
                psSched->bSchedWaitingForAnswer = true;
                psBus->eState = eBus_ReceivingWait;
            }
        }
        else if (timer_is_elapsed(&psSched->sNodeAnsTimeout)) {
                sched_set_node_error(psSched, psSched->auTokenMsg[1] & 0x7F);
                // return to IDLE state
                psBus->eState = eBus_Idle;
                LED_ERROR_ON;
        }
        break;

    default:
        break;
    }

    msg_received = bus_trp_send_and_receive(psBus);

    // scheduler send ACK bytes for broadcast messages
    if (msg_received == true &&
        psBus->sRecvMsg.uReceiver == BUS_BRDCSTADR) {
        if (psSched->broadcast_acknowledged == false) {
            psSched->broadcast_acknowledged = true;
            bus_trp_send_ackbyte(psBus);
        }
    } else {
        psSched->broadcast_acknowledged = false;
    }

    if (psSched->bSchedWaitingForAnswer) {
        bool receive_end = false;

        if (psBus->msg_receive_state > eBUS_RECV_NOTHING) {
            if (psBus->msg_receive_state > eBUS_RECV_EMPTY_MESSAGE) psSched->uSleepLoopCnt = BUS_LOOPS_TILL_SLEEP;
            receive_end = true;
            if (true == psSched->bSchedDiscovery) {
                // node is now active
                sched_set_node_state(psSched, psSched->auTokenMsg[1] & 0x7F, true);
            }
            else sched_reset_node_error(psSched, psSched->auTokenMsg[1] & 0x7F);

        }
        else if (timer_is_elapsed(&psSched->sNodeAnsTimeout)) {
            receive_end = true;
            // this node has an error, if we are not in discovery-mode, because
            // it did not answer the token
            if (false == psSched->bSchedDiscovery) {
                sched_set_node_error(psSched, psSched->auTokenMsg[1] & 0x7F);
            }
            if (0 == psSched->uSleepLoopCnt) { // if schedulerloopcount has reached zero ...
                psSched->uSleepLoopCnt = BUS_LOOPS_TILL_SLEEP;
                psBus->eModuleState = eMod_Sleeping; // ... initiate sleep-mode
            }
        }
        if (receive_end) { // receiving finished
            // only reset message state if message is not to be read by
            // bus_read_message()
            if (psBus->msg_receive_state != eBUS_RECV_MESSAGE) {
                psBus->msg_receive_state = eBUS_RECV_NOTHING;
            }
            psSched->bSchedWaitingForAnswer = false;
            if (eBus_AckWaitReceiving != psBus->eState) {
                // return to IDLE state
                bus_trp_reset(psBus);
            }
        }
    }
    return msg_received;
}


/**
 * Check if schedule is in sleep mode and send sleep command to slaves.
 *
 * @param[in] psBus     Handle of the bus.
 */
void bus_schedule_check_and_set_sleep (sBus_t* psBus)
{
    if (eMod_Sleeping == psBus->eModuleState) {
        if (bus_trp_send_sleepcmd(psBus)) {
            timer_control(false); // disable clock-timer, otherwise
                                  // IRQ will cause immediate wake-up.

            // sleep till byte is received.
#ifdef PRJCONF_UC_AVR
            set_sleep_mode(SLEEP_MODE_IDLE);
            sleep_mode();

            _delay_ms(1); // wait for sys-clock becoming stable
#endif
            bus_flush_bus(psBus); // Clean bus-buffer
            psBus->eModuleState = eMod_Running;
            timer_control(true); // enable clock-timer
        }
    }
}
/** @} */
