
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <CUnit/CUnit.h>

#include "bus.h"
#include "bus_intern.h"

const uint8_t g_uLoggingOn = 0;

// Function stubs for bus_schedul.c and bus_transport.c
//-----------------------------------------------------------------------------

void    bus_phy_initialize         (sBusPhy_t*     psPhy,
                                     uint8_t        uUart)
{
}

BOOL    bus_phy_send               (sBusPhy_t*     psPhy,
                                     const uint8_t* puMsg,
                                     uint8_t        uLen)
{
    return TRUE;
}


BOOL    bus_phy_sending            (sBusPhy_t*     psPhy)
{
    return FALSE;
}

BOOL    bus_phy_data_received       (sBusPhy_t*     psPhy)
{
    return TRUE;
}

uint8_t bus_phy_read               (sBusPhy_t*     psPhy,
                                     uint8_t*       puInBuf)
{
    return 1;
}

BOOL    bus_phy_read_byte           (sBusPhy_t*     psPhy,
                                     uint8_t*       puByte)
{
    return TRUE;
}

void    bus_phy_flush              (sBusPhy_t* psPhy)
{
}

void    set_sleep_mode              (uint8_t        uMode)
{
}

void    sleep_mode                  (void)
{
}

void    _delay_ms                   (uint16_t       uMS)
{
}

BOOL    clk_timer_is_elapsed         (sClkTimer_t* psTimer)
{
    return TRUE;
}

BOOL    clk_timer_start             (sClkTimer_t* psTimer, uint16_t uTicks)
{
    return TRUE;
}

void    clk_control                (BOOL start)
{
}

uint16_t crc_calc16                (uint8_t* puData, uint8_t uLen)
{
    return 0x4242;
}

extern int8_t sched_get_next_node(sSched_t* psSched, BOOL bDiscovery);
extern BOOL sched_send_next_time_slot_token(sBus_t* psBus, sSched_t* psSched, BOOL bDiscovery);
extern void sched_set_node_state(sSched_t* psSched, uint8_t uNodeAddress, BOOL bOnline);
extern void sched_set_node_error(sSched_t* psSched, uint8_t uNodeAddress);

void set_all_nodes_present(sSched_t* psSched)
{
    int ii;
    for (ii=0; ii<sizeof(psSched->asNodeList.uAddress); ii++) {
        psSched->asNodeList.uAddress[ii] = 0b11111111;
    }
}

void set_all_nodes_missing(sSched_t* psSched)
{
    int ii;
    for (ii=0; ii<sizeof(psSched->asNodeList.uAddress); ii++) {
        psSched->asNodeList.uAddress[ii] = 0b00000000;
    }
}

void log_node_data(sSched_t* psSched, uint8_t uVal)
{
    if (g_uLoggingOn)
        printf("\nSCHED_GetNextNode retval = %02X, current-node = %02X, discovery-node = %02X, discovery-mode = %d",
            uVal, psSched->uCurrentNode, psSched->uDiscoverNode, psSched->bSchedDiscovery);
}

// Main test functions
//-----------------------------------------------------------------------------

/**
 * Test finding next nodes if all nodes are missing.
 */
void test_all_nodes_missing(void)
{
    sBus_t      sBus;
    sSched_t    sSched;
    uint8_t     ii, val;

    // initialize environment
    sSched.uDiscoverNode = BUS_LASTNODE;
    sSched.uCurrentNode  = BUS_FIRSTNODE;
    sSched.bSchedDiscovery = FALSE;
    set_all_nodes_missing(&sSched);

    // test first call
    val = sched_get_next_node(&sSched, FALSE);
    log_node_data(&sSched, val);
    CU_ASSERT_EQUAL(val, BUS_FIRSTNODE);                        // First node returned
    CU_ASSERT_EQUAL(sSched.uCurrentNode, BUS_FIRSTNODE);        // No current node
    CU_ASSERT_EQUAL(sSched.uDiscoverNode, BUS_FIRSTNODE);       // Returned node is discovery node
    CU_ASSERT_EQUAL(sSched.bSchedDiscovery, TRUE);

    // test second call
    val = sched_get_next_node(&sSched, FALSE);
    log_node_data(&sSched, val);
    CU_ASSERT_EQUAL(val, BUS_FIRSTNODE+1);                      // Second node returned
    CU_ASSERT_EQUAL(sSched.uCurrentNode, BUS_FIRSTNODE);        // No current node
    CU_ASSERT_EQUAL(sSched.uDiscoverNode, BUS_FIRSTNODE+1);     // Returned node is discovery node
    CU_ASSERT_EQUAL(sSched.bSchedDiscovery, TRUE);

    // iterate through all nodes in descoverymode until the node before the last node
    for (ii=BUS_FIRSTNODE+2; ii<=BUS_LASTNODE-1; ii++) {
        val = sched_get_next_node(&sSched, FALSE);
        log_node_data(&sSched, val);
    }

    // test last call
    val = sched_get_next_node(&sSched, FALSE);
    log_node_data(&sSched, val);
    CU_ASSERT_EQUAL(val, BUS_LASTNODE);                        // Last node returned
    CU_ASSERT_EQUAL(sSched.uCurrentNode, BUS_FIRSTNODE);       // No current node
    CU_ASSERT_EQUAL(sSched.uDiscoverNode, BUS_LASTNODE);       // Returned node is discovery node
    CU_ASSERT_EQUAL(sSched.bSchedDiscovery, TRUE);

    // test last call
    val = sched_get_next_node(&sSched, FALSE);
    log_node_data(&sSched, val);
    CU_ASSERT_EQUAL(val, BUS_FIRSTNODE);                       // First node returned
    CU_ASSERT_EQUAL(sSched.uCurrentNode, BUS_FIRSTNODE);       // No current node
    CU_ASSERT_EQUAL(sSched.uDiscoverNode, BUS_FIRSTNODE);      // Returned node is discovery node
    CU_ASSERT_EQUAL(sSched.bSchedDiscovery, TRUE);
}

/**
 * Test finding next nodes if all nodes are present.
 */
void test_all_nodes_present(void)
{
    sBus_t      sBus;
    sSched_t    sSched;
    uint8_t     ii, val;

    // initialize environment
    sSched.uDiscoverNode = BUS_FIRSTNODE;
    sSched.uCurrentNode  = BUS_LASTNODE;
    sSched.bSchedDiscovery = FALSE;
    set_all_nodes_present(&sSched);

    // test first call
    val = sched_get_next_node(&sSched, FALSE);
    log_node_data(&sSched, val);
    CU_ASSERT_EQUAL(val, BUS_FIRSTNODE);                        // First node returned
    CU_ASSERT_EQUAL(sSched.uCurrentNode, BUS_FIRSTNODE);        // Returned node is current node
    CU_ASSERT_EQUAL(sSched.uDiscoverNode, BUS_FIRSTNODE);
    CU_ASSERT_EQUAL(sSched.bSchedDiscovery, FALSE);             // not in discovery mode

    // test second call
    val = sched_get_next_node(&sSched, FALSE);
    log_node_data(&sSched, val);
    CU_ASSERT_EQUAL(val, BUS_FIRSTNODE+1);                      // Second node returned
    CU_ASSERT_EQUAL(sSched.uCurrentNode, BUS_FIRSTNODE+1);      // Returned node is current node
    CU_ASSERT_EQUAL(sSched.uDiscoverNode, BUS_FIRSTNODE);       //
    CU_ASSERT_EQUAL(sSched.bSchedDiscovery, FALSE);             // not in discovery mode

    // iterate through all nodes in descoverymode until the node before the last node
    for (ii=BUS_FIRSTNODE+2; ii<=BUS_LASTNODE-1; ii++) {
        val = sched_get_next_node(&sSched, FALSE);
        log_node_data(&sSched, val);
    }

    // test last call
    val = sched_get_next_node(&sSched, FALSE);
    log_node_data(&sSched, val);
    CU_ASSERT_EQUAL(val, BUS_LASTNODE);                        // Last node returned
    CU_ASSERT_EQUAL(sSched.uCurrentNode, BUS_LASTNODE);        // Returned node is current node
    CU_ASSERT_EQUAL(sSched.uDiscoverNode, BUS_FIRSTNODE);      //
    CU_ASSERT_EQUAL(sSched.bSchedDiscovery, FALSE);            // not in discovery mode

    // test last call + 1
    val = sched_get_next_node(&sSched, FALSE);
    log_node_data(&sSched, val);
    CU_ASSERT_EQUAL(val, BUS_FIRSTNODE);                       // First node returned
    CU_ASSERT_EQUAL(sSched.uCurrentNode, BUS_FIRSTNODE);       // Returned node is current node
    CU_ASSERT_EQUAL(sSched.uDiscoverNode, BUS_FIRSTNODE);      //
    CU_ASSERT_EQUAL(sSched.bSchedDiscovery, FALSE);            // not in discovery mode
}

/**
 * Test finding next nodes if all nodes 0x01, 0x04, 0x08, 0x10, 0x20, 0x21,
 * 0x22 and 0x7F are present.
 */
void test_some_nodes_present(void)
{
    sBus_t      sBus;
    sSched_t    sSched;
    uint16_t    ii;
    uint8_t     val;

    // initialize environment
    sSched.uDiscoverNode = BUS_FIRSTNODE;
    sSched.uCurrentNode  = BUS_FIRSTNODE;
    sSched.bSchedDiscovery = TRUE;
    set_all_nodes_missing(&sSched);
    sSched.asNodeList.uAddress[0] = 0b00010010; // node 0x01 and 0x04
    sSched.asNodeList.uAddress[1] = 0b00000001; // node 0x08
    sSched.asNodeList.uAddress[2] = 0b00000001; // node 0x10
    sSched.asNodeList.uAddress[4] = 0b00000111; // node 0x20
    sSched.asNodeList.uAddress[15] = 0b10000000; // node 0x7F

    // test first call
    val = sched_get_next_node(&sSched, FALSE);
    log_node_data(&sSched, val);
    CU_ASSERT_EQUAL(val, BUS_FIRSTNODE);                        // First discovery node returned
    CU_ASSERT_EQUAL(sSched.uCurrentNode, BUS_FIRSTNODE);        // Returned node is current node
    CU_ASSERT_EQUAL(sSched.uDiscoverNode, BUS_FIRSTNODE);
    CU_ASSERT_EQUAL(sSched.bSchedDiscovery, FALSE);             // in discovery mode

    // test second call
    val = sched_get_next_node(&sSched, FALSE);
    log_node_data(&sSched, val);
    CU_ASSERT_EQUAL(val, 4);                                    // First present node returned
    CU_ASSERT_EQUAL(sSched.uCurrentNode, 4);                    // Returned node is current node
    CU_ASSERT_EQUAL(sSched.uDiscoverNode, BUS_FIRSTNODE);       // unchanged
    CU_ASSERT_EQUAL(sSched.bSchedDiscovery, FALSE);             // not in discovery mode

    val = sched_get_next_node(&sSched, FALSE);
    log_node_data(&sSched, val);
    CU_ASSERT_EQUAL(val, 8);                                    // First present node returned
    CU_ASSERT_EQUAL(sSched.uCurrentNode, 8);                    // Returned node is current node
    CU_ASSERT_EQUAL(sSched.uDiscoverNode, BUS_FIRSTNODE);       // unchanged
    CU_ASSERT_EQUAL(sSched.bSchedDiscovery, FALSE);             // not in discovery mode

    val = sched_get_next_node(&sSched, FALSE);
    log_node_data(&sSched, val);
    CU_ASSERT_EQUAL(val, 0x10);                                 // First present node returned
    CU_ASSERT_EQUAL(sSched.uCurrentNode, 0x10);                 // Returned node is current node
    CU_ASSERT_EQUAL(sSched.uDiscoverNode, BUS_FIRSTNODE);       // unchanged
    CU_ASSERT_EQUAL(sSched.bSchedDiscovery, FALSE);             // not in discovery mode

    val = sched_get_next_node(&sSched, FALSE);
    log_node_data(&sSched, val);
    CU_ASSERT_EQUAL(val, 0x20);                                 // First present node returned
    CU_ASSERT_EQUAL(sSched.uCurrentNode, 0x20);                 // Returned node is current node
    CU_ASSERT_EQUAL(sSched.uDiscoverNode, BUS_FIRSTNODE);       // unchanged
    CU_ASSERT_EQUAL(sSched.bSchedDiscovery, FALSE);             // not in discovery mode

    val = sched_get_next_node(&sSched, FALSE);
    log_node_data(&sSched, val);
    CU_ASSERT_EQUAL(val, 0x21);                                 // First present node returned
    CU_ASSERT_EQUAL(sSched.uCurrentNode, 0x21);                 // Returned node is current node
    CU_ASSERT_EQUAL(sSched.uDiscoverNode, BUS_FIRSTNODE);       // unchanged
    CU_ASSERT_EQUAL(sSched.bSchedDiscovery, FALSE);             // not in discovery mode


    val = sched_get_next_node(&sSched, FALSE);
    log_node_data(&sSched, val);
    CU_ASSERT_EQUAL(val, 0x22);                                 // First present node returned
    CU_ASSERT_EQUAL(sSched.uCurrentNode, 0x22);                 // Returned node is current node
    CU_ASSERT_EQUAL(sSched.uDiscoverNode, BUS_FIRSTNODE);       // unchanged
    CU_ASSERT_EQUAL(sSched.bSchedDiscovery, FALSE);             // not in discovery mode

    val = sched_get_next_node(&sSched, FALSE);
    log_node_data(&sSched, val);
    CU_ASSERT_EQUAL(val, 0x7F);                                 // First present node returned
    CU_ASSERT_EQUAL(sSched.uCurrentNode, 0x7F);                 // Returned node is current node
    CU_ASSERT_EQUAL(sSched.uDiscoverNode, BUS_FIRSTNODE);       // unchanged
    CU_ASSERT_EQUAL(sSched.bSchedDiscovery, FALSE);             // not in discovery mode

    val = sched_get_next_node(&sSched, FALSE);
    log_node_data(&sSched, val);
    CU_ASSERT_EQUAL(val, BUS_FIRSTNODE+1);                      // First discovery node returned
    CU_ASSERT_EQUAL(sSched.uCurrentNode, BUS_FIRSTNODE);        // Returned node is current node
    CU_ASSERT_EQUAL(sSched.uDiscoverNode, BUS_FIRSTNODE+1);
    CU_ASSERT_EQUAL(sSched.bSchedDiscovery, TRUE);              // in discovery mode

    // iterate through all nodes in discoverymode until the node before the last node
    for (ii=0; ii<=118*9+6; ii++) {
        val = sched_get_next_node(&sSched, FALSE);
        log_node_data(&sSched, val);
    }

    // test last call
    val = sched_get_next_node(&sSched, FALSE);
    log_node_data(&sSched, val);
    CU_ASSERT_EQUAL(val, BUS_LASTNODE);                        // First node returned
    CU_ASSERT_EQUAL(sSched.uCurrentNode, BUS_LASTNODE);        // Returned node is current node
    CU_ASSERT_EQUAL(sSched.uDiscoverNode, BUS_LASTNODE-1);     //
    CU_ASSERT_EQUAL(sSched.bSchedDiscovery, FALSE);            // not in discovery mode

    // test last call
    val = sched_get_next_node(&sSched, FALSE);
    log_node_data(&sSched, val);
    CU_ASSERT_EQUAL(val, BUS_FIRSTNODE+1);                      // First node returned
    CU_ASSERT_EQUAL(sSched.uCurrentNode, BUS_FIRSTNODE);        // Returned node is current node
    CU_ASSERT_EQUAL(sSched.uDiscoverNode, BUS_FIRSTNODE+1);     //
    CU_ASSERT_EQUAL(sSched.bSchedDiscovery, TRUE);              // not in discovery mode

    // test last call
    val = sched_get_next_node(&sSched, FALSE);
    log_node_data(&sSched, val);
    CU_ASSERT_EQUAL(val, BUS_FIRSTNODE);                        // First node returned
    CU_ASSERT_EQUAL(sSched.uCurrentNode, BUS_FIRSTNODE);        // Returned node is current node
    CU_ASSERT_EQUAL(sSched.uDiscoverNode, BUS_FIRSTNODE+1);     //
    CU_ASSERT_EQUAL(sSched.bSchedDiscovery, FALSE);             // not in discovery mode
}

/**
 * Test setting nodes active and inactive.
 */
void test_set_node_state(void)
{
    sSched_t sSched;

    // initialize test data
    set_all_nodes_missing(&sSched);

    // run test
    sched_set_node_state(&sSched, BUS_FIRSTNODE, TRUE);
    CU_ASSERT_EQUAL(sSched.asNodeList.uAddress[0], 0b00000010);
    sched_set_node_state(&sSched, BUS_FIRSTNODE, FALSE);
    CU_ASSERT_EQUAL(sSched.asNodeList.uAddress[0], 0b00000000);
    sched_set_node_state(&sSched, BUS_LASTNODE, TRUE);
    CU_ASSERT_EQUAL(sSched.asNodeList.uAddress[15], 0b10000000);
    sched_set_node_state(&sSched, BUS_LASTNODE, FALSE);
    CU_ASSERT_EQUAL(sSched.asNodeList.uAddress[15], 0b00000000);
}

void test_set_node_error(void)
{
    sSched_t sSched;

    // initialize test data
    set_all_nodes_present(&sSched);
    sSched.uErrorNode = 0;
    sSched.uErrorCount = 0;

    // run test
    // first error of node, only error counted
    sched_set_node_error(&sSched, 1);
    CU_ASSERT_EQUAL(sSched.asNodeList.uAddress[0], 0b11111111);
    CU_ASSERT_EQUAL(sSched.uErrorNode, 1);
    CU_ASSERT_EQUAL(sSched.uErrorCount, 0);

    // second error of node, only error counted
    sched_set_node_error(&sSched, 1);
    CU_ASSERT_EQUAL(sSched.asNodeList.uAddress[0], 0b11111111);
    CU_ASSERT_EQUAL(sSched.uErrorNode, 1);
    CU_ASSERT_EQUAL(sSched.uErrorCount, 1);

    // third error of node, only error counted
    sched_set_node_error(&sSched, 1);
    CU_ASSERT_EQUAL(sSched.asNodeList.uAddress[0], 0b11111111);
    CU_ASSERT_EQUAL(sSched.uErrorNode, 1);
    CU_ASSERT_EQUAL(sSched.uErrorCount, 2);

    // error of another node does not affect current error counter
    sched_set_node_error(&sSched, 2);
    CU_ASSERT_EQUAL(sSched.asNodeList.uAddress[0], 0b11111111);
    CU_ASSERT_EQUAL(sSched.uErrorNode, 1);
    CU_ASSERT_EQUAL(sSched.uErrorCount, 2);

    // forth error of node, only error counted
    sched_set_node_error(&sSched, 1);
    CU_ASSERT_EQUAL(sSched.asNodeList.uAddress[0], 0b11111111);
    CU_ASSERT_EQUAL(sSched.uErrorNode, 1);
    CU_ASSERT_EQUAL(sSched.uErrorCount, 3);

    // fifth error of node, only error counted
    sched_set_node_error(&sSched, 1);
    CU_ASSERT_EQUAL(sSched.asNodeList.uAddress[0], 0b11111111);
    CU_ASSERT_EQUAL(sSched.uErrorNode, 1);
    CU_ASSERT_EQUAL(sSched.uErrorCount, 4);

    // sixth error of node, only error counted
    sched_set_node_error(&sSched, 1);
    CU_ASSERT_EQUAL(sSched.asNodeList.uAddress[0], 0b11111101);
    CU_ASSERT_EQUAL(sSched.uErrorNode, 0);
    CU_ASSERT_EQUAL(sSched.uErrorCount, 0);
}

/**
 * Test suite of bus-schudler tests.
 */
CU_TestInfo TESTBUSSCHED_asTests[] = {
    {"SCHED_GetNextNode: all nodes missing               ",  test_all_nodes_missing  },
    {"SCHED_GetNextNode: all nodes present               ",  test_all_nodes_present  },
    {"SCHED_GetNextNode: some nodes present              ",  test_some_nodes_present },
    {"SCHED_SetNodeState: set and reset nodes            ",  test_set_node_state     },
    {"SCHED_vSetNodeError: test node going to error state",  test_set_node_error     },
    CU_TEST_INFO_NULL,
};
