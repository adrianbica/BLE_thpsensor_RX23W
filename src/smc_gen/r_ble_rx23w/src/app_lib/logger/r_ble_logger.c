/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
#include <string.h>
#include "r_ble_rx23w_if.h"
#include "r_ble_logger.h"

#if (BLE_DEFAULT_LOG_LEVEL != 0) && (BLE_CFG_HCI_MODE_EN == 0)

char *BLE_BD_ADDR_STR(uint8_t *p_addr, uint8_t addr_type)
{
    static char addr_str[24];

    memset(addr_str, '\0', 24);

    if (NULL != p_addr)
    {
        sprintf(addr_str, "%02X:%02X:%02X:%02X:%02X:%02X %s",
                p_addr[5], p_addr[4], p_addr[3], p_addr[2], p_addr[1], p_addr[0],
                (addr_type == BLE_GAP_ADDR_PUBLIC) ? "pub" : "rnd");
    }

    return addr_str;
}

char *BLE_UUID_STR(uint8_t *p_uuid, uint8_t uuid_type)
{
    static char uuid_str[37];

    memset(uuid_str, '\0', 37);

    if (NULL != p_uuid)
    {
        if (uuid_type == 0)
        {
            /* For 16bit, ex: 0xE29B */
            sprintf(uuid_str, "0x%02X%02X", p_uuid[1], p_uuid[0]);
        }
        else if (uuid_type == 1)
        {
            /* For 128bit, ex: 550E8400-E29B-41D4-A716-446655440000 */
            sprintf(uuid_str, "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
                    p_uuid[15], p_uuid[14], p_uuid[13], p_uuid[12],
                    p_uuid[11], p_uuid[10], p_uuid[ 9], p_uuid[ 8],
                    p_uuid[ 7], p_uuid[ 6], p_uuid[ 5], p_uuid[ 4],
                    p_uuid[ 3], p_uuid[ 2], p_uuid[ 1], p_uuid[ 0]);
        }
        else
        {
            /* Do nothing */
        }
    }

    return uuid_str;
}

#else /* (BLE_DEFAULT_LOG_LEVEL != 0) && (BLE_CFG_HCI_MODE_EN == 0) */
char *BLE_BD_ADDR_STR(uint8_t *p_addr, uint8_t addr_type)
{
    static char addr = '\n';
    (void)p_addr;
    (void)addr_type;
    return &addr;
}

char *BLE_UUID_STR(uint8_t *p_uuid, uint8_t uuid_type)
{
    static char uuid = '\n';
    (void)p_uuid;
    (void)uuid_type;
    return &uuid;
}

#endif /* (BLE_DEFAULT_LOG_LEVEL != 0) && (BLE_CFG_HCI_MODE_EN == 0) */
