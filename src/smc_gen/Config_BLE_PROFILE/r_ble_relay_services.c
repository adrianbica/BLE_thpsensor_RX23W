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
/***********************************************************************************************************************
 * File Name: r_ble_relay_services.c
 * Version : 1.0
 * Description : The source file for Relay Service service.
 **********************************************************************************************************************/

#include "r_ble_relay_services.h"
#include "profile_cmn/r_ble_servs_if.h"
#include "gatt_db.h"

static st_ble_servs_info_t gs_servs_info;

/*----------------------------------------------------------------------------------------------------------------------
    Digital 1 Client Characteristic Configuration descriptor
----------------------------------------------------------------------------------------------------------------------*/

static const st_ble_servs_desc_info_t gs_digital_1_cli_cnfg = {
    .attr_hdl = BLE_RELAY_SERVICES_DIGITAL_1_CLI_CNFG_DESC_HDL,
    .app_size = sizeof(uint16_t),
    .desc_idx = BLE_RELAY_SERVICES_DIGITAL_1_CLI_CNFG_IDX,
    .db_size  = BLE_RELAY_SERVICES_DIGITAL_1_CLI_CNFG_LEN,
    .decode   = (ble_servs_attr_decode_t)decode_uint16_t,
    .encode   = (ble_servs_attr_encode_t)encode_uint16_t,
};

ble_status_t R_BLE_RELAY_SERVICES_SetDigital1CliCnfg(uint16_t conn_hdl, const uint16_t *p_value)
{
    return R_BLE_SERVS_SetDesc(&gs_digital_1_cli_cnfg, conn_hdl, (const void *)p_value);
}

ble_status_t R_BLE_RELAY_SERVICES_GetDigital1CliCnfg(uint16_t conn_hdl, uint16_t *p_value)
{
    return R_BLE_SERVS_GetDesc(&gs_digital_1_cli_cnfg, conn_hdl, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Digital 1 characteristic
----------------------------------------------------------------------------------------------------------------------*/

/* Digital 1 characteristic descriptor definition */
static const st_ble_servs_desc_info_t *gspp_digital_1_descs[] = {
    &gs_digital_1_cli_cnfg,
};

/* Digital 1 characteristic definition */
static const st_ble_servs_char_info_t gs_digital_1_char = {
    .start_hdl    = BLE_RELAY_SERVICES_DIGITAL_1_DECL_HDL,
    .end_hdl      = BLE_RELAY_SERVICES_DIGITAL_1_CLI_CNFG_DESC_HDL,
    .char_idx     = BLE_RELAY_SERVICES_DIGITAL_1_IDX,
    .app_size     = sizeof(uint16_t),
    .db_size      = BLE_RELAY_SERVICES_DIGITAL_1_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_uint16_t,
    .encode       = (ble_servs_attr_encode_t)encode_uint16_t,
    .pp_descs     = gspp_digital_1_descs,
    .num_of_descs = ARRAY_SIZE(gspp_digital_1_descs),
};

ble_status_t R_BLE_RELAY_SERVICES_SetDigital1(const uint16_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_digital_1_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_RELAY_SERVICES_GetDigital1(uint16_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_digital_1_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

ble_status_t R_BLE_RELAY_SERVICES_NotifyDigital1(uint16_t conn_hdl, const uint16_t *p_value)
{
    return R_BLE_SERVS_SendHdlVal(&gs_digital_1_char, conn_hdl, (const void *)p_value, true);
}

/*----------------------------------------------------------------------------------------------------------------------
    Relay Service server
----------------------------------------------------------------------------------------------------------------------*/

/* Relay Service characteristics definition */
static const st_ble_servs_char_info_t *gspp_chars[] = {
    &gs_digital_1_char,
};

/* Relay Service service definition */
static st_ble_servs_info_t gs_servs_info = {
    .pp_chars     = gspp_chars,
    .num_of_chars = ARRAY_SIZE(gspp_chars),
};

ble_status_t R_BLE_RELAY_SERVICES_Init(ble_servs_app_cb_t cb)
{
    if (NULL == cb)
    {
        return BLE_ERR_INVALID_PTR;
    }

    gs_servs_info.cb = cb;

    return R_BLE_SERVS_RegisterServer(&gs_servs_info);
}
