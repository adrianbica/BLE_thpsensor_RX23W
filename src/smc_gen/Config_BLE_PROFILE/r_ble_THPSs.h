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
 * File Name: r_ble_THPSs.h
 * Version : 1.0
 * Description : The header file for THP Sensing Service service.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 31.12.2999 1.00 First Release
 ***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @file
 * @defgroup THPSs THP Sensing Service Service
 * @{
 * @ingroup profile
 * @brief   
 **********************************************************************************************************************/
#include "r_ble_rx23w_if.h"

#include "profile_cmn/r_ble_servs_if.h"
#include "gatt_db.h"

#ifndef R_BLE_THPSS_H
#define R_BLE_THPSS_H

/*----------------------------------------------------------------------------------------------------------------------
    Temperature Characteristic
----------------------------------------------------------------------------------------------------------------------*/

/***************************************************************************//**
 * @brief     Set Temperature characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_THPSS_SetTemperature(const int16_t *p_value);

/***************************************************************************//**
 * @brief     Get Temperature characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_THPSS_GetTemperature(int16_t *p_value);

/***************************************************************************//**
 * @brief     Send notification of  Temperature characteristic value to the remote device.
 * @param[in] conn_hdl Connection handle.
 * @param[in] p_value  Characteristic value to send.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_THPSS_NotifyTemperature(uint16_t conn_hdl, const int16_t *p_value);

/***************************************************************************//**
 * @brief     Set Temperature cli cnfg descriptor value to the local GATT database.
 * @param[in] conn_hdl Connection handle.
 * @param[in] p_value  Descriptor value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_THPSS_SetTemperatureCliCnfg(uint16_t conn_hdl, const uint16_t *p_value);

/***************************************************************************//**
 * @brief     Get Temperature cli cnfg descriptor value from the local GATT database.
 * @param[in] conn_hdl Connection handle.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_THPSS_GetTemperatureCliCnfg(uint16_t conn_hdl, uint16_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Humidity Characteristic
----------------------------------------------------------------------------------------------------------------------*/

/***************************************************************************//**
 * @brief     Set Humidity characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_THPSS_SetHumidity(const uint16_t *p_value);

/***************************************************************************//**
 * @brief     Get Humidity characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_THPSS_GetHumidity(uint16_t *p_value);

/***************************************************************************//**
 * @brief     Send notification of  Humidity characteristic value to the remote device.
 * @param[in] conn_hdl Connection handle.
 * @param[in] p_value  Characteristic value to send.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_THPSS_NotifyHumidity(uint16_t conn_hdl, const uint16_t *p_value);

/***************************************************************************//**
 * @brief     Set Humidity cli cnfg descriptor value to the local GATT database.
 * @param[in] conn_hdl Connection handle.
 * @param[in] p_value  Descriptor value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_THPSS_SetHumidityCliCnfg(uint16_t conn_hdl, const uint16_t *p_value);

/***************************************************************************//**
 * @brief     Get Humidity cli cnfg descriptor value from the local GATT database.
 * @param[in] conn_hdl Connection handle.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_THPSS_GetHumidityCliCnfg(uint16_t conn_hdl, uint16_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Pressure Characteristic
----------------------------------------------------------------------------------------------------------------------*/

/***************************************************************************//**
 * @brief     Set Pressure characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_THPSS_SetPressure(const uint32_t *p_value);

/***************************************************************************//**
 * @brief     Get Pressure characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_THPSS_GetPressure(uint32_t *p_value);

/***************************************************************************//**
 * @brief     Send notification of  Pressure characteristic value to the remote device.
 * @param[in] conn_hdl Connection handle.
 * @param[in] p_value  Characteristic value to send.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_THPSS_NotifyPressure(uint16_t conn_hdl, const uint32_t *p_value);

/***************************************************************************//**
 * @brief     Set Pressure cli cnfg descriptor value to the local GATT database.
 * @param[in] conn_hdl Connection handle.
 * @param[in] p_value  Descriptor value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_THPSS_SetPressureCliCnfg(uint16_t conn_hdl, const uint16_t *p_value);

/***************************************************************************//**
 * @brief     Get Pressure cli cnfg descriptor value from the local GATT database.
 * @param[in] conn_hdl Connection handle.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_THPSS_GetPressureCliCnfg(uint16_t conn_hdl, uint16_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    THP Sensing Service Service
----------------------------------------------------------------------------------------------------------------------*/

/***************************************************************************//**
 * @brief THP Sensing Service characteristic Index.
*******************************************************************************/
typedef enum {
    BLE_THPSS_TEMPERATURE_IDX,
    BLE_THPSS_TEMPERATURE_CLI_CNFG_IDX,
    BLE_THPSS_HUMIDITY_IDX,
    BLE_THPSS_HUMIDITY_CLI_CNFG_IDX,
    BLE_THPSS_PRESSURE_IDX,
    BLE_THPSS_PRESSURE_CLI_CNFG_IDX,
} e_ble_thpss_char_idx_t;

/***************************************************************************//**
 * @brief THP Sensing Service event type.
*******************************************************************************/
typedef enum {
    /* Temperature */
    BLE_THPSS_EVENT_TEMPERATURE_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_THPSS_TEMPERATURE_IDX, BLE_SERVS_READ_REQ),
    BLE_THPSS_EVENT_TEMPERATURE_CLI_CNFG_WRITE_REQ = BLE_SERVS_ATTR_EVENT(BLE_THPSS_TEMPERATURE_CLI_CNFG_IDX, BLE_SERVS_WRITE_REQ),
    BLE_THPSS_EVENT_TEMPERATURE_CLI_CNFG_WRITE_COMP = BLE_SERVS_ATTR_EVENT(BLE_THPSS_TEMPERATURE_CLI_CNFG_IDX, BLE_SERVS_WRITE_COMP),
    BLE_THPSS_EVENT_TEMPERATURE_CLI_CNFG_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_THPSS_TEMPERATURE_CLI_CNFG_IDX, BLE_SERVS_READ_REQ),
    /* Humidity */
    BLE_THPSS_EVENT_HUMIDITY_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_THPSS_HUMIDITY_IDX, BLE_SERVS_READ_REQ),
    BLE_THPSS_EVENT_HUMIDITY_CLI_CNFG_WRITE_REQ = BLE_SERVS_ATTR_EVENT(BLE_THPSS_HUMIDITY_CLI_CNFG_IDX, BLE_SERVS_WRITE_REQ),
    BLE_THPSS_EVENT_HUMIDITY_CLI_CNFG_WRITE_COMP = BLE_SERVS_ATTR_EVENT(BLE_THPSS_HUMIDITY_CLI_CNFG_IDX, BLE_SERVS_WRITE_COMP),
    BLE_THPSS_EVENT_HUMIDITY_CLI_CNFG_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_THPSS_HUMIDITY_CLI_CNFG_IDX, BLE_SERVS_READ_REQ),
    /* Pressure */
    BLE_THPSS_EVENT_PRESSURE_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_THPSS_PRESSURE_IDX, BLE_SERVS_READ_REQ),
    BLE_THPSS_EVENT_PRESSURE_CLI_CNFG_WRITE_REQ = BLE_SERVS_ATTR_EVENT(BLE_THPSS_PRESSURE_CLI_CNFG_IDX, BLE_SERVS_WRITE_REQ),
    BLE_THPSS_EVENT_PRESSURE_CLI_CNFG_WRITE_COMP = BLE_SERVS_ATTR_EVENT(BLE_THPSS_PRESSURE_CLI_CNFG_IDX, BLE_SERVS_WRITE_COMP),
    BLE_THPSS_EVENT_PRESSURE_CLI_CNFG_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_THPSS_PRESSURE_CLI_CNFG_IDX, BLE_SERVS_READ_REQ),
} e_ble_thpss_event_t;

/***************************************************************************//**
 * @brief     Initialize THP Sensing Service service.
 * @param[in] cb Service callback.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_THPSS_Init(ble_servs_app_cb_t cb);

#endif /* R_BLE_THPSS_H */

/** @} */
