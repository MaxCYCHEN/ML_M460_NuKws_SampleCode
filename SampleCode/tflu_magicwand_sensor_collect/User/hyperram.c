//
// Includes
//
#include "hyperram.h"

//
// Defines
//
#define HYPERRAM_TOTAL_SIZE     0x800000    // 8MB = 64Mb
#define TEST_PATTERN            0x5A5A5A5A
//
// Prototype
//
static int32_t Clear4Bytes(uint32_t u32StartAddr);
static int32_t ClearHyperRAM(uint32_t u32StartAddr, uint32_t u32EndAddr);

//
// Functions
//

//
// hyperram_low_level_init
//
void hyperram_low_level_init(void)
{
    SET_HBI_D0_PJ6();
    SET_HBI_D1_PJ5();
    SET_HBI_D2_PJ4();
    SET_HBI_D3_PJ3();
    SET_HBI_D4_PH15();
    SET_HBI_D5_PD7();
    SET_HBI_D6_PD6();
    SET_HBI_D7_PD5();

    SET_HBI_RWDS_PH14();
    SET_HBI_nRESET_PJ2();
    SET_HBI_nCS_PJ7();
    SET_HBI_CK_PH13();
    SET_HBI_nCK_PH12();
    
    HBI->CONFIG |= 0x00008000;
}

//
// hyperram_rw_test
//
int8_t hyperram_rw_test(void)
{
    uint32_t i;
    uint32_t u32StartAddr, u32EndAddr;
    uint32_t r_data, w_data;
    
    u32StartAddr = HYPER_RAM_MEM_MAP;
    u32EndAddr = u32StartAddr + HYPERRAM_TOTAL_SIZE;
    
    //
    // Clear
    //
    printf("Clear HyperRAM...");
    if (ClearHyperRAM(u32StartAddr, u32EndAddr) < 0) {
        return -1;
    }
    printf("<done>\n");
    
    //
    // Write
    //
    printf("Write HyperRAM...");
    w_data = TEST_PATTERN;
    for(i = u32StartAddr; i < u32EndAddr; i+=4) {
        outp32(i, w_data);
    }
    printf("<done>\n");
    
    //
    // Verify
    //
    printf("Verify HyperRAM...");
    for(i = u32StartAddr; i < u32EndAddr; i+=4) {
        r_data = inp32(i);
        if(r_data != TEST_PATTERN) {
            printf("<fail>\n");
            return -1;
        }
    }    
    printf("<done>\n");

    return 0;
}


//
// Clear4Bytes
// 
static int32_t Clear4Bytes(uint32_t u32StartAddr)
{
    outp32(u32StartAddr, 0);
    return 0;
}

//
// ClearHyperRAM
//
static int32_t ClearHyperRAM(uint32_t u32StartAddr, uint32_t u32EndAddr)
{
    uint32_t u32Data, i;

    for(i = u32StartAddr; i < u32EndAddr; i+=4) {
        if( Clear4Bytes(i) < 0 ) {
            return -1;
        }
        
        u32Data = inp32(i);
        if(u32Data != 0) {
            printf("ClearHyperRAM fail!! Read address:0x%08x  data::0x%08x  expect: 0\n",  i, u32Data);
            return -1;
        }
    }
    return 0;
}
