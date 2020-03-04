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
 * File Name: r_ble_relay_services.h
 * Version : 1.0
 * Description : The header file for Relay Service service.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 31.12.2999 1.00 First Release
 ***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @file
 * @defgroup relay_services Relay Service Service
 * @{
 * @ingroup profile
 * @brief   
 **********************************************************************************************************************/
#include "r_ble_rx23w_if.h"

#include "profile_cmn/r_ble_servs_if.h"
#include "gatt_db.h"

#ifndef R_BLE_RELAY_SERVICES_H
#define R_BLE_RELAY_SERVICES_H

/*----------------------------------------------------------------------------------------------------------------------
    Digital 1 Characteristic
----------------------------------------------------------------------------------------------------------------------*/

/***************************************************************************//**
 * @brief Digital 1 Digital 1 enumeration.
*******************************************************************************/
typedef enum {
    BLE_RELAY_SERVICES_DIGITAL_1_DIGITAL_1_INACTIVE = 0, /**< Inactive */
    BLE_RELAY_SERVICES_DIGITAL_1_DIGITAL_1_ACTIVE = 1, /**< Active */
    BLE_RELAY_SERVICES_DIGITAL_1_DIGITAL_1_TRI_STATE = 2, /**< Tri-state */
    BLE_RELAY_SERVICES_DIGITAL_1_DIGITAL_1_OUTPUT_STATE = 3, /**< Output-state */
} e_ble_digital_1_digital_1_t;

/***************************************************************************//**
 * @brief     Set Digital 1 characteristic value to the local GATT database.
 * @param[in] p_value  Characteristic value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_RELAY_SERVICES_SetDigital1(const uint16_t *p_value);

/***************************************************************************//**
 * @brief     Get Digital 1 characteristic value from the local GATT database.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_RELAY_SERVICES_GetDigital1(uint16_t *p_value);

/***************************************************************************//**
 * @brief     Send notification of  Digital 1 characteristic value to the remote device.
 * @param[in] conn_hdl Connection handle.
 * @param[in] p_value  Characteristic value to send.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_RELAY_SERVICES_NotifyDigital1(uint16_t conn_hdl, const uint16_t *p_value);

/***************************************************************************//**
 * @brief     Set Digital 1 cli cnfg descriptor value to the local GATT database.
 * @param[in] conn_hdl Connection handle.
 * @param[in] p_value  Descriptor value to set.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_RELAY_SERVICES_SetDigital1CliCnfg(uint16_t conn_hdl, const uint16_t *p_value);

/***************************************************************************//**
 * @brief     Get Digital 1 cli cnfg descriptor value from the local GATT database.
 * @param[in] conn_hdl Connection handle.
 * @param[out] p_value  Output location for the acquired descriptor value.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_RELAY_SERVICES_GetDigital1CliCnfg(uint16_t conn_hdl, uint16_t *p_value);

/*----------------------------------------------------------------------------------------------------------------------
    Relay Service Service
----------------------------------------------------------------------------------------------------------------------*/

/***************************************************************************//**
 * @brief Relay Service characteristic Index.
*******************************************************************************/
typedef enum {
    BLE_RELAY_SERVICES_DIGITAL_1_IDX,
    BLE_RELAY_SERVICES_DIGITAL_1_CLI_CNFG_IDX,
} e_ble_relay_services_char_idx_t;

/***************************************************************************//**
 * @brief Relay Service event type.
*******************************************************************************/
typedef enum {
    /* Digital 1 */
    BLE_RELAY_SERVICES_EVENT_DIGITAL_1_WRITE_REQ = BLE_SERVS_ATTR_EVENT(BLE_RELAY_SERVICES_DIGITAL_1_IDX, BLE_SERVS_WRITE_REQ),
    BLE_RELAY_SERVICES_EVENT_DIGITAL_1_WRITE_COMP = BLE_SERVS_ATTR_EVENT(BLE_RELAY_SERVICES_DIGITAL_1_IDX, BLE_SERVS_WRITE_COMP),
    BLE_RELAY_SERVICES_EVENT_DIGITAL_1_WRITE_CMD = BLE_SERVS_ATTR_EVENT(BLE_RELAY_SERVICES_DIGITAL_1_IDX, BLE_SERVS_WRITE_CMD),
    BLE_RELAY_SERVICES_EVENT_DIGITAL_1_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_RELAY_SERVICES_DIGITAL_1_IDX, BLE_SERVS_READ_REQ),
    BLE_RELAY_SERVICES_EVENT_DIGITAL_1_CLI_CNFG_WRITE_REQ = BLE_SERVS_ATTR_EVENT(BLE_RELAY_SERVICES_DIGITAL_1_CLI_CNFG_IDX, BLE_SERVS_WRITE_REQ),
    BLE_RELAY_SERVICES_EVENT_DIGITAL_1_CLI_CNFG_WRITE_COMP = BLE_SERVS_ATTR_EVENT(BLE_RELAY_SERVICES_DIGITAL_1_CLI_CNFG_IDX, BLE_SERVS_WRITE_COMP),
    BLE_RELAY_SERVICES_EVENT_DIGITAL_1_CLI_CNFG_READ_REQ = BLE_SERVS_ATTR_EVENT(BLE_RELAY_SERVICES_DIGITAL_1_CLI_CNFG_IDX, BLE_SERVS_READ_REQ),
} e_ble_relay_services_event_t;

/***************************************************************************//**
 * @brief     Initialize Relay Service service.
 * @param[in] cb Service callback.
 * @return    @ref ble_status_t
*******************************************************************************/
ble_status_t R_BLE_RELAY_SERVICES_Init(ble_servs_app_cb_t cb);

#endif /* R_BLE_RELAY_SERVICES_H */

/** @} */
