#include <malloc.h>
#include <stdio.h>
#include <CUnit/CUnit.h>

#include "ihex.h"

static uint8_t* g_memory;
static uint32_t g_oldaddress;
static uint32_t g_firstaddress;
static uint32_t g_lastaddress;

void test_write(uint32_t uAddress, uint8_t uByte)
{
    if (uAddress >= 0x000315C) {
        CU_FAIL("Address out of bounds!");
        return;
    }
    g_memory[uAddress] = uByte;
    if ((uAddress - g_oldaddress) != 1) {
        // not equal increasing addresses.
        CU_FAIL("Address not monotonous increasing!");
    } else {
        g_oldaddress = uAddress;
    }
    if (uAddress > g_lastaddress) g_lastaddress = uAddress;
    if (uAddress < g_firstaddress) g_firstaddress = uAddress;
}

void IHEXTEST_vTestFileRead(void)
{
    int32_t retval;
    uint32_t startaddress = 0;

    g_firstaddress = 0xFFFFFFFF; g_lastaddress = 0x00000000;
    // successfully read an ihex file without saving the data
    retval = ihex_read_file("testdata/ihex/data_normal.hex", &startaddress, &g_firstaddress, &g_lastaddress, NULL);
    CU_ASSERT(retval == e_ihex_success);
    CU_ASSERT(startaddress == 0x00000000);
    CU_ASSERT(g_firstaddress == 0x00000000);
    CU_ASSERT(g_lastaddress  == 0x0000315B);
}

void IHEXTEST_vTestFileReadToMemory(void)
{
    int32_t retval;

    // successfully read an ihex file while saving the data
    g_memory = malloc(0x0000315C * sizeof(uint8_t));
    g_oldaddress = 0xFFFFFFFF;
    g_firstaddress = 0xFFFFFFFF; g_lastaddress = 0x00000000;
    retval = ihex_read_file("testdata/ihex/data_normal.hex", NULL, NULL, NULL, test_write);
    CU_ASSERT(retval == e_ihex_success);
    CU_ASSERT(g_firstaddress == 0x00000000);
    CU_ASSERT(g_lastaddress  == 0x0000315B);
    free(g_memory);
}

void IHEXTEST_vTestFileWithSegmentOffset(void)
{
    int32_t retval;
    uint32_t startaddress = 0;

    g_firstaddress = 0xFFFFFFFF; g_lastaddress = 0x00000000;
    // successfully read an ihex file without saving the data
    retval = ihex_read_file("testdata/ihex/data_with_segment_offset.hex", &startaddress, &g_firstaddress, &g_lastaddress, NULL);
    CU_ASSERT(retval == e_ihex_success);
    CU_ASSERT(startaddress == 0x00000000);
    CU_ASSERT(g_firstaddress == 0x00030000);
    CU_ASSERT(g_lastaddress  == 0x000300FF);
}

void IHEXTEST_vTestFileWithSegmentStartAddress(void)
{
    int32_t retval;
    uint32_t startaddress = 0;

    g_firstaddress = 0xFFFFFFFF; g_lastaddress = 0x00000000;
    // successfully read an ihex file without saving the data
    retval = ihex_read_file("testdata/ihex/data_with_segment_startaddress.hex", &startaddress, &g_firstaddress, &g_lastaddress, NULL);
    CU_ASSERT(retval == e_ihex_success);
    CU_ASSERT(startaddress == 0x00030100);
    CU_ASSERT(g_firstaddress == 0x00030000);
    CU_ASSERT(g_lastaddress  == 0x000300FF);
}

void IHEXTEST_vTestFileWithLinearOffset(void)
{
    int32_t retval;
    uint32_t startaddress = 0;

    g_firstaddress = 0xFFFFFFFF; g_lastaddress = 0x00000000;
    // successfully read an ihex file without saving the data
    retval = ihex_read_file("testdata/ihex/data_with_linear_offset.hex", &startaddress, &g_firstaddress, &g_lastaddress, NULL);
    CU_ASSERT(retval == e_ihex_success);
    CU_ASSERT(startaddress == 0x00000000);
    CU_ASSERT(g_firstaddress == 0x30000000);
    CU_ASSERT(g_lastaddress  == 0x300000FF);
}

void IHEXTEST_vTestFileWithLinearStartAddress(void)
{
    int32_t retval;
    uint32_t startaddress = 0;

    g_firstaddress = 0xFFFFFFFF; g_lastaddress = 0x00000000;
    // successfully read an ihex file without saving the data
    retval = ihex_read_file("testdata/ihex/data_with_linear_startaddress.hex", &startaddress, &g_firstaddress, &g_lastaddress, NULL);
    CU_ASSERT(retval == e_ihex_success);
    CU_ASSERT(startaddress == 0x30000100);
    CU_ASSERT(g_firstaddress == 0x30000000);
    CU_ASSERT(g_lastaddress  == 0x300000FF);
}

void IHEXTEST_vTestFileNotExisting(void)
{
    int32_t retval;
    retval = ihex_read_file("filenotexisting.hex", NULL, NULL, NULL, NULL);
    CU_ASSERT(retval == e_ihex_error_file_open);
}

void IHEXTEST_vTestUnexpectedChar(void)
{
    int32_t retval;
    retval = ihex_read_file("testdata/ihex/unexpected_char.hex", NULL, NULL, NULL, NULL);
    CU_ASSERT(retval == e_ihex_error_unexpected_char);
}

void IHEXTEST_vTestUnexpectedEndOfLine(void)
{
    int32_t retval;
    retval = ihex_read_file("testdata/ihex/unexpected_eol.hex", NULL, NULL, NULL, NULL);
    CU_ASSERT(retval == e_ihex_error_unexpected_eol);
}

void IHEXTEST_vTestUnexpectedEndOfFile(void)
{
    int32_t retval;
    retval = ihex_read_file("testdata/ihex/unexpected_eof.hex", NULL, NULL, NULL, NULL);
    CU_ASSERT(retval == e_ihex_error_unexpected_eof);
}

void IHEXTEST_vTestInvalidLineMark(void)
{
    int32_t retval;
    retval = ihex_read_file("testdata/ihex/invalidlinemark.hex", NULL, NULL, NULL, NULL);
    CU_ASSERT(retval == e_ihex_error_line_mark);
}

void IHEXTEST_vTestUnexpectedCharAtEndOfLine(void)
{
    int32_t retval;
    retval = ihex_read_file("testdata/ihex/charsateol.hex", NULL, NULL, NULL, NULL);
    CU_ASSERT(retval == e_ihex_error_chars_at_eol);
}

void IHEXTEST_vTestInvalidRecordType(void)
{
    int32_t retval;
    retval = ihex_read_file("testdata/ihex/invalidrectype.hex", NULL, NULL, NULL, NULL);
    CU_ASSERT(retval == e_ihex_error_invalid_rectype);
}

void IHEXTEST_vTestInvalidChecksum(void)
{
    int32_t retval;
    retval = ihex_read_file("testdata/ihex/invalidchecksum.hex", NULL, NULL, NULL, NULL);
    CU_ASSERT(retval == e_ihex_error_checksum);
}

CU_TestInfo TESTIHEX_asTests[] = {
    {"Reading a file without a writer function ",  IHEXTEST_vTestFileRead                   },
    {"Reading a file with a writer function    ",  IHEXTEST_vTestFileReadToMemory           },
    {"Reading a file with segment offset       ",  IHEXTEST_vTestFileWithSegmentOffset      },
    {"Reading a file with segment start address",  IHEXTEST_vTestFileWithSegmentStartAddress},
    {"Reading a file with linear offset        ",  IHEXTEST_vTestFileWithLinearOffset       },
    {"Reading a file with linear start address ",  IHEXTEST_vTestFileWithLinearStartAddress },

    {"Error case: Opening not existing file    ",  IHEXTEST_vTestFileNotExisting            },
    {"Error case: Unexpected character         ",  IHEXTEST_vTestUnexpectedChar             },
    {"Error case: Unexpected end of line       ",  IHEXTEST_vTestUnexpectedEndOfLine        },
    {"Error case: Unexpected end of file       ",  IHEXTEST_vTestUnexpectedEndOfFile        },
    {"Error case: Invalid line mark            ",  IHEXTEST_vTestInvalidLineMark            },
    {"Error case: Unexpected chars at eol      ",  IHEXTEST_vTestUnexpectedCharAtEndOfLine  },
    {"Error case: Invalid record type          ",  IHEXTEST_vTestInvalidRecordType          },
    {"Error case: Invalid checksum             ",  IHEXTEST_vTestInvalidChecksum            },

    CU_TEST_INFO_NULL,
};
