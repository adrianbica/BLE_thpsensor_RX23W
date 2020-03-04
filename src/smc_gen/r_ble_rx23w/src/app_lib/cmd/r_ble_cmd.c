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
#include "r_ble_cmd.h"
#include "cli/r_ble_cli.h"

#if (BLE_CFG_CMD_LINE_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0)

void R_BLE_CMD_ParseValues(char *p_str, uint8_t *p_buffer, uint16_t *p_length)
{
    *p_length = 0;

    char *p_tp = strtok(p_str, ",");

    while ((p_tp != NULL) && (*p_length < 50))
    {
        if (p_tp != NULL)
        {
            p_buffer[*p_length] = (uint8_t)strtol(p_tp, NULL, 0);
            *p_length += 1;
        }

        p_tp = strtok(NULL, ",");
    }
}

void R_BLE_CMD_PrintValues(uint8_t *p_buffer, uint16_t length)
{
    for (int i = 0; i < length; i++)
    {
        R_BLE_CLI_Printf("0x%02x", p_buffer[i]);

        if (i != length - 1)
        {
            R_BLE_CLI_Printf(",", p_buffer[i]);
        }
    }
}

void R_BLE_CMD_ParseAddr(char *p_str, uint8_t *p_addr)
{
    int p = 5;
    char *p_tp = strtok(p_str, ":");

    while ((p_tp != NULL) && (p >= 0))
    {
        if (p_tp != NULL)
        {
            p_addr[p] = (uint8_t)strtol(p_tp, NULL, 16);
            p--;
        }

        p_tp = strtok(NULL, ":");
    }
}

void R_BLE_CMD_PrintAddr(uint8_t *p_addr)
{
    for (uint16_t i = 0; i < 6; i++)
    {
        R_BLE_CLI_Printf("%02x", p_addr[5-i]);
        if (i != 5)
        {
            R_BLE_CLI_Printf(":");
        }
    }
    R_BLE_CLI_Printf("\n");
}

#else /* (BLE_CFG_CMD_LINE_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0) */

void R_BLE_CMD_ParseValues(char *p_str, uint8_t *p_buffer, uint16_t *p_length)
{
    (void)p_str;
    (void)p_buffer;
    (void)p_length;
}

void R_BLE_CMD_PrintValues(uint8_t *p_buffer, uint16_t length)
{
    (void)p_buffer;
    (void)length;
}

void R_BLE_CMD_ParseAddr(char *p_str, uint8_t *p_addr)
{
    (void)p_str;
    (void)p_addr;
}

void R_BLE_CMD_PrintAddr(uint8_t *p_addr)
{
    (void)p_addr;
}

#endif /* (BLE_CFG_CMD_LINE_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0) */
