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
 * File Name: r_ble_THPSs.c
 * Version : 1.0
 * Description : The source file for THP Sensing Service service.
 **********************************************************************************************************************/

#include "r_ble_thpss.h"
#include "profile_cmn/r_ble_servs_if.h"
#include "gatt_db.h"

static st_ble_servs_info_t gs_servs_info;

/*----------------------------------------------------------------------------------------------------------------------
    Temperature Client Characteristic Configuration descriptor
----------------------------------------------------------------------------------------------------------------------*/

static const st_ble_servs_desc_info_t gs_temperature_cli_cnfg = {
    .attr_hdl = BLE_THPSS_TEMPERATURE_CLI_CNFG_DESC_HDL,
    .app_size = sizeof(uint16_t),
    .desc_idx = BLE_THPSS_TEMPERATURE_CLI_CNFG_IDX,
    .db_size  = BLE_THPSS_TEMPERATURE_CLI_CNFG_LEN,
    .decode   = (ble_servs_attr_decode_t)decode_uint16_t,
    .encode   = (ble_servs_attr_encode_t)encode_uint16_t,
};

ble_status_t R_BLE_THPSS_SetTemperatureCliCnfg(uint16_t conn_hdl, const uint16_t *p_value)
{
    return R_BLE_SERVS_SetDesc(&gs_temperature_cli_cnfg, conn_hdl, (const void *)p_value);
}

ble_status_t R_BLE_THPSS_GetTemperatureCliCnfg(uint16_t conn_hdl, uint16_t *p_value)
{
    return R_BLE_SERVS_GetDesc(&gs_temperature_cli_cnfg, conn_hdl, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Temperature characteristic
----------------------------------------------------------------------------------------------------------------------*/

/* Temperature characteristic descriptor definition */
static const st_ble_servs_desc_info_t *gspp_temperature_descs[] = {
    &gs_temperature_cli_cnfg,
};

/* Temperature characteristic definition */
static const st_ble_servs_char_info_t gs_temperature_char = {
    .start_hdl    = BLE_THPSS_TEMPERATURE_DECL_HDL,
    .end_hdl      = BLE_THPSS_TEMPERATURE_CLI_CNFG_DESC_HDL,
    .char_idx     = BLE_THPSS_TEMPERATURE_IDX,
    .app_size     = sizeof(int16_t),
    .db_size      = BLE_THPSS_TEMPERATURE_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_int16_t,
    .encode       = (ble_servs_attr_encode_t)encode_int16_t,
    .pp_descs     = gspp_temperature_descs,
    .num_of_descs = ARRAY_SIZE(gspp_temperature_descs),
};

ble_status_t R_BLE_THPSS_SetTemperature(const int16_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_temperature_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_THPSS_GetTemperature(int16_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_temperature_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

ble_status_t R_BLE_THPSS_NotifyTemperature(uint16_t conn_hdl, const int16_t *p_value)
{
    return R_BLE_SERVS_SendHdlVal(&gs_temperature_char, conn_hdl, (const void *)p_value, true);
}

/*----------------------------------------------------------------------------------------------------------------------
    Humidity Client Characteristic Configuration descriptor
----------------------------------------------------------------------------------------------------------------------*/

static const st_ble_servs_desc_info_t gs_humidity_cli_cnfg = {
    .attr_hdl = BLE_THPSS_HUMIDITY_CLI_CNFG_DESC_HDL,
    .app_size = sizeof(uint16_t),
    .desc_idx = BLE_THPSS_HUMIDITY_CLI_CNFG_IDX,
    .db_size  = BLE_THPSS_HUMIDITY_CLI_CNFG_LEN,
    .decode   = (ble_servs_attr_decode_t)decode_uint16_t,
    .encode   = (ble_servs_attr_encode_t)encode_uint16_t,
};

ble_status_t R_BLE_THPSS_SetHumidityCliCnfg(uint16_t conn_hdl, const uint16_t *p_value)
{
    return R_BLE_SERVS_SetDesc(&gs_humidity_cli_cnfg, conn_hdl, (const void *)p_value);
}

ble_status_t R_BLE_THPSS_GetHumidityCliCnfg(uint16_t conn_hdl, uint16_t *p_value)
{
    return R_BLE_SERVS_GetDesc(&gs_humidity_cli_cnfg, conn_hdl, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Humidity characteristic
----------------------------------------------------------------------------------------------------------------------*/

/* Humidity characteristic descriptor definition */
static const st_ble_servs_desc_info_t *gspp_humidity_descs[] = {
    &gs_humidity_cli_cnfg,
};

/* Humidity characteristic definition */
static const st_ble_servs_char_info_t gs_humidity_char = {
    .start_hdl    = BLE_THPSS_HUMIDITY_DECL_HDL,
    .end_hdl      = BLE_THPSS_HUMIDITY_CLI_CNFG_DESC_HDL,
    .char_idx     = BLE_THPSS_HUMIDITY_IDX,
    .app_size     = sizeof(uint16_t),
    .db_size      = BLE_THPSS_HUMIDITY_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_uint16_t,
    .encode       = (ble_servs_attr_encode_t)encode_uint16_t,
    .pp_descs     = gspp_humidity_descs,
    .num_of_descs = ARRAY_SIZE(gspp_humidity_descs),
};

ble_status_t R_BLE_THPSS_SetHumidity(const uint16_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_humidity_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_THPSS_GetHumidity(uint16_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_humidity_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

ble_status_t R_BLE_THPSS_NotifyHumidity(uint16_t conn_hdl, const uint16_t *p_value)
{
    return R_BLE_SERVS_SendHdlVal(&gs_humidity_char, conn_hdl, (const void *)p_value, true);
}

/*----------------------------------------------------------------------------------------------------------------------
    Pressure Client Characteristic Configuration descriptor
----------------------------------------------------------------------------------------------------------------------*/

static const st_ble_servs_desc_info_t gs_pressure_cli_cnfg = {
    .attr_hdl = BLE_THPSS_PRESSURE_CLI_CNFG_DESC_HDL,
    .app_size = sizeof(uint16_t),
    .desc_idx = BLE_THPSS_PRESSURE_CLI_CNFG_IDX,
    .db_size  = BLE_THPSS_PRESSURE_CLI_CNFG_LEN,
    .decode   = (ble_servs_attr_decode_t)decode_uint16_t,
    .encode   = (ble_servs_attr_encode_t)encode_uint16_t,
};

ble_status_t R_BLE_THPSS_SetPressureCliCnfg(uint16_t conn_hdl, const uint16_t *p_value)
{
    return R_BLE_SERVS_SetDesc(&gs_pressure_cli_cnfg, conn_hdl, (const void *)p_value);
}

ble_status_t R_BLE_THPSS_GetPressureCliCnfg(uint16_t conn_hdl, uint16_t *p_value)
{
    return R_BLE_SERVS_GetDesc(&gs_pressure_cli_cnfg, conn_hdl, (void *)p_value);
}

/*----------------------------------------------------------------------------------------------------------------------
    Pressure characteristic
----------------------------------------------------------------------------------------------------------------------*/

/* Pressure characteristic descriptor definition */
static const st_ble_servs_desc_info_t *gspp_pressure_descs[] = {
    &gs_pressure_cli_cnfg,
};

/* Pressure characteristic definition */
static const st_ble_servs_char_info_t gs_pressure_char = {
    .start_hdl    = BLE_THPSS_PRESSURE_DECL_HDL,
    .end_hdl      = BLE_THPSS_PRESSURE_CLI_CNFG_DESC_HDL,
    .char_idx     = BLE_THPSS_PRESSURE_IDX,
    .app_size     = sizeof(uint32_t),
    .db_size      = BLE_THPSS_PRESSURE_LEN,
    .decode       = (ble_servs_attr_decode_t)decode_uint32_t,
    .encode       = (ble_servs_attr_encode_t)encode_uint32_t,
    .pp_descs     = gspp_pressure_descs,
    .num_of_descs = ARRAY_SIZE(gspp_pressure_descs),
};

ble_status_t R_BLE_THPSS_SetPressure(const uint32_t *p_value)
{
    return R_BLE_SERVS_SetChar(&gs_pressure_char, BLE_GAP_INVALID_CONN_HDL, (const void *)p_value);
}

ble_status_t R_BLE_THPSS_GetPressure(uint32_t *p_value)
{
    return R_BLE_SERVS_GetChar(&gs_pressure_char, BLE_GAP_INVALID_CONN_HDL, (void *)p_value);
}

ble_status_t R_BLE_THPSS_NotifyPressure(uint16_t conn_hdl, const uint32_t *p_value)
{
    return R_BLE_SERVS_SendHdlVal(&gs_pressure_char, conn_hdl, (const void *)p_value, true);
}

/*----------------------------------------------------------------------------------------------------------------------
    THP Sensing Service server
----------------------------------------------------------------------------------------------------------------------*/

/* THP Sensing Service characteristics definition */
static const st_ble_servs_char_info_t *gspp_chars[] = {
    &gs_temperature_char,
    &gs_humidity_char,
    &gs_pressure_char,
};

/* THP Sensing Service service definition */
static st_ble_servs_info_t gs_servs_info = {
    .pp_chars     = gspp_chars,
    .num_of_chars = ARRAY_SIZE(gspp_chars),
};

ble_status_t R_BLE_THPSS_Init(ble_servs_app_cb_t cb)
{
    if (NULL == cb)
    {
        return BLE_ERR_INVALID_PTR;
    }

    gs_servs_info.cb = cb;

    return R_BLE_SERVS_RegisterServer(&gs_servs_info);
}
