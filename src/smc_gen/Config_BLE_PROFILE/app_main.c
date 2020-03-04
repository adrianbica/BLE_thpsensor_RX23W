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

/*******************************************************************************
* File Name    : app_main.c
* Device(s)    : RX23W
* Tool-Chain   : e2Studio, Renesas RX v2.08
* Description  : This is a application file for peripheral role.
*******************************************************************************/

/*****************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <string.h>
#include "r_ble_rx23w_if.h"

#include "gatt_db.h"
#include "abs/r_ble_abs_api.h"
#include "profile_cmn/r_ble_servs_if.h"
#include "profile_cmn/r_ble_servc_if.h"
#include "r_ble_THPSs.h"
#include "r_ble_relay_services.h"

/* TODO: Add include pass of app_lib. */
/* example: #include "timer/r_ble_timer.h" */

#define BLE_LOG_TAG "app_main"
#include "logger/r_ble_logger.h"

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
/* Internal functions */
static void gap_cb(uint16_t type, ble_status_t result, st_ble_evt_data_t *p_data);
static void gatts_cb(uint16_t type, ble_status_t result, st_ble_gatts_evt_data_t *p_data);
static void gattc_cb(uint16_t type, ble_status_t result, st_ble_gattc_evt_data_t *p_data);
static void vs_cb(uint16_t type, ble_status_t result, st_ble_vs_evt_data_t *p_data);

/* Advertising data */
static uint8_t gs_adv_data[] =
{
    /* TODO: Modify advertise data. Value of Data Flag is defined in https://www.bluetooth.com/specifications/assigned-numbers/generic-access-profile */

    /* Flag (mandatory) */
    2,          /**< Data Size */
    0x01,       /**< Data Type: Flag */
    (BLE_GAP_AD_FLAGS_LE_GEN_DISC_MODE | BLE_GAP_AD_FLAGS_BR_EDR_NOT_SUPPORTED),    /**< Data Value */

    /* Complete Local Name */
    9,         /**< Data Size */
    0x09,       /**< Data Type: Complete Local Name */
    'R', 'B', 'L', 'E', '-', 'D', 'E', 'V', /**< Data Value */
};

/* Scan response Data */
static uint8_t gs_sres_data[] =
{
    /* TODO: Modify scan response data. Value of Data Flag is defined in https://www.bluetooth.com/specifications/assigned-numbers/generic-access-profile */

    /* Complete Local Name */
    9,         /**< Data Size */
    0x09,       /**< Data Type: Complete Local Name */
    'R', 'B', 'L', 'E', '-', 'D', 'E', 'V', /**< Data Value */
};

/* Advertising parameters */
static st_ble_abs_legacy_adv_param_t gs_adv_param =
{
    /* TODO: Modify advertise parameters. */
    .slow_adv_intv   = 0x300,
    .slow_period     = 0,
    .p_adv_data      = gs_adv_data,
    .adv_data_length = ARRAY_SIZE(gs_adv_data),
    .p_sres_data     = gs_sres_data,
    .sres_data_length= ARRAY_SIZE(gs_sres_data),
    .adv_ch_map      = BLE_GAP_ADV_CH_ALL,
    .filter          = BLE_ABS_ADV_ALLOW_CONN_ANY,
    .o_addr_type     = BLE_GAP_ADDR_PUBLIC,
};

/* GATT server callback parameters */
static st_ble_abs_gatts_cb_param_t gs_abs_gatts_cb_param[] =
{
    {
        .cb       = gatts_cb,
        .priority = 1,
    },
    {
        .cb       = NULL,
    }
};

/* GATT server initialization parameters */
static st_ble_abs_gatts_init_param_t gs_abs_gatts_init_param =
{
    .p_cb_param = gs_abs_gatts_cb_param,
    .cb_num     = BLE_GATTS_MAX_CB,
};

/* GATT client callback parameters */
static st_ble_abs_gattc_cb_param_t gs_abs_gattc_cb_param[] =
{
    {
        .cb       = gattc_cb,
        .priority = 1,
    },
    {
        .cb       = NULL,
    }
};

/* GATT client initialization parameters */
static st_ble_abs_gattc_init_param_t gs_abs_gattc_init_param =
{
    .p_cb_param = gs_abs_gattc_cb_param,
    .cb_num     = BLE_GATTC_MAX_CB,
};

/* Pairing parameters */
static st_ble_abs_pairing_param_t gs_abs_pairing_param =
{
    .iocap         = BLE_GAP_IOCAP_NOINPUT_NOOUTPUT,
    .mitm          = BLE_GAP_SEC_MITM_BEST_EFFORT,
    .sec_conn_only = BLE_GAP_SC_BEST_EFFORT,
    .loc_key_dist  = BLE_GAP_KEY_DIST_ENCKEY,
    .rem_key_dist  = 0,
    .max_key_size  = 16,
};

/* Host stack initialization parameters */
static st_ble_abs_init_param_t gs_abs_init_param =
{
    .gap_cb          = gap_cb,
    .p_gatts_cbs     = &gs_abs_gatts_init_param,
    .p_gattc_cbs     = &gs_abs_gattc_init_param,
    .vs_cb           = vs_cb,
    .p_pairing_param = &gs_abs_pairing_param,
};

/* Connection handle */
uint16_t g_conn_hdl;



/******************************************************************************
 * Function Name: ble_app_gapcb
 * Description  : Internal function called by GAP API callback function.
 *              : Implement the code specific to the application.
 * Arguments    : uint16_t type -
 *                  Event type of GAP API.
 *              : ble_status_t result -
 *                  Event result of GAP API.
 *              : st_ble_vs_evt_data_t *p_data - 
 *                  Event parameters of GAP API.
 * Return Value : none
 ******************************************************************************/
static void ble_app_gapcb(uint16_t type, ble_status_t result, st_ble_evt_data_t *p_data)
{
    switch (type)
    {
        case BLE_GAP_EVENT_STACK_ON:
        {
            R_BLE_ABS_StartLegacyAdv(&gs_adv_param);
        } break;

        case BLE_GAP_EVENT_CONN_IND:
        {
            if (BLE_SUCCESS == result)
            {
                st_ble_gap_conn_evt_t *p_gap_conn_evt_param =
                    (st_ble_gap_conn_evt_t *)p_data->p_param;
                g_conn_hdl = p_gap_conn_evt_param->conn_hdl;
            }
        } break;

        case BLE_GAP_EVENT_DISCONN_IND:
        {
            g_conn_hdl = BLE_GAP_INVALID_CONN_HDL;
            R_BLE_ABS_StartLegacyAdv(&gs_adv_param);
        } break;

        case BLE_GAP_EVENT_CONN_PARAM_UPD_REQ:
        {
            st_ble_gap_conn_upd_req_evt_t *p_conn_upd_req_evt_param = (st_ble_gap_conn_upd_req_evt_t *)p_data->p_param;

            st_ble_gap_conn_param_t conn_updt_param = {
                .conn_intv_min = p_conn_upd_req_evt_param->conn_intv_min,
                .conn_intv_max = p_conn_upd_req_evt_param->conn_intv_max,
                .conn_latency  = p_conn_upd_req_evt_param->conn_latency,
                .sup_to        = p_conn_upd_req_evt_param->sup_to,
            };

            R_BLE_GAP_UpdConn(p_conn_upd_req_evt_param->conn_hdl,
                              BLE_GAP_CONN_UPD_MODE_RSP,
                              BLE_GAP_CONN_UPD_ACCEPT,
                              &conn_updt_param);
        } break;

        case BLE_GAP_EVENT_DATA_LEN_CHG:
        case BLE_GAP_EVENT_PHY_UPD:
        case BLE_GAP_EVENT_CONN_PARAM_UPD_COMP:
        {
            /* Do nothing. */
        } break;

        default:
        {
            /* Do nothing. */
        } break;
    }
}

/******************************************************************************
 * Function Name: gap_cb
 * Description  : Callback function for GAP API.
 * Arguments    : uint16_t type -
 *                  Event type of GAP API.
 *              : ble_status_t result -
 *                  Event result of GAP API.
 *              : st_ble_vs_evt_data_t *p_data - 
 *                  Event parameters of GAP API.
 * Return Value : none
 ******************************************************************************/
static void gap_cb(uint16_t type, ble_status_t result, st_ble_evt_data_t *p_data)
{
    ble_app_gapcb(type, result, p_data);

    switch(type)
    {
        /* TODO: Set callback events of GAP. Check BLE API reference for events. */

        default:
        {
            /* Do nothing. */
        } break;
    }
}

/******************************************************************************
 * Function Name: gatts_cb
 * Description  : Callback function for GATT Server API.
 * Arguments    : uint16_t type -
 *                  Event type of GATT Server API.
 *              : ble_status_t result -
 *                  Event result of GATT Server API.
 *              : st_ble_gatts_evt_data_t *p_data - 
 *                  Event parameters of GATT Server API.
 * Return Value : none
 ******************************************************************************/
static void gatts_cb(uint16_t type, ble_status_t result, st_ble_gatts_evt_data_t *p_data)
{
    R_BLE_SERVS_GattsCb(type, result, p_data);

    switch(type)
    {
        /* TODO: Set callback events of GATTS. Check BLE API reference for events. */

        default:
        {
            /* Do nothing. */
        } break;
    }
}

/******************************************************************************
 * Function Name: gattc_cb
 * Description  : Callback function for GATT Client API.
 * Arguments    : uint16_t type -
 *                  Event type of GATT Client API.
 *              : ble_status_t result -
 *                  Event result of GATT Client API.
 *              : st_ble_gattc_evt_data_t *p_data - 
 *                  Event parameters of GATT Client API.
 * Return Value : none
 ******************************************************************************/
static void gattc_cb(uint16_t type, ble_status_t result, st_ble_gattc_evt_data_t *p_data)
{
    R_BLE_SERVC_GattcCb(type, result, p_data);

    switch(type)
    {
        /* TODO: Set callback events of GATTC. Check BLE API reference for events. */

        default:
        {
            /* Do nothing. */
        } break;
    }
}

/******************************************************************************
 * Function Name: vs_cb
 * Description  : Callback function for Vendor Specific API.
 * Arguments    : uint16_t type -
 *                  Event type of Vendor Specific API.
 *              : ble_status_t result -
 *                  Event result of Vendor Specific API.
 *              : st_ble_vs_evt_data_t *p_data - 
 *                  Event parameters of Vendor Specific API.
 * Return Value : none
 ******************************************************************************/
static void vs_cb(uint16_t type, ble_status_t result, st_ble_vs_evt_data_t *p_data)
{
    R_BLE_SERVS_VsCb(type, result, p_data);

    switch(type)
    {
        /* TODO: Set callback events of VS. Check BLE API reference for events. */

        default:
        {
            /* Do nothing. */
        } break;
    }
}

/******************************************************************************
 * Function Name: THPSs_cb
 * Description  : Callback function for THP Sensing Service server feature.
 * Arguments    : uint16_t type -
 *                  Event type of THP Sensing Service server feature.
 *              : ble_status_t result -
 *                  Event result of THP Sensing Service server feature.
 *              : st_ble_servs_evt_data_t *p_data - 
 *                  Event parameters of THP Sensing Service server feature.
 * Return Value : none
 ******************************************************************************/
static void THPSs_cb(uint16_t type, ble_status_t result, st_ble_servs_evt_data_t *p_data)
{
    switch(type)
    {
        /* TODO: Set callback events of THPSs. Check BLE API reference or e_ble_THPSs_event_t for events. */

        default:
        {
            /* Do nothing. */
        } break;
    }    
}

/******************************************************************************
 * Function Name: relay_services_cb
 * Description  : Callback function for Relay Service server feature.
 * Arguments    : uint16_t type -
 *                  Event type of Relay Service server feature.
 *              : ble_status_t result -
 *                  Event result of Relay Service server feature.
 *              : st_ble_servs_evt_data_t *p_data - 
 *                  Event parameters of Relay Service server feature.
 * Return Value : none
 ******************************************************************************/
static void relay_services_cb(uint16_t type, ble_status_t result, st_ble_servs_evt_data_t *p_data)
{
    switch(type)
    {
        /* TODO: Set callback events of relay_services. Check BLE API reference or e_ble_relay_services_event_t for events. */

        default:
        {
            /* Do nothing. */
        } break;
    }    
}
/******************************************************************************
 * Function Name: ble_app_init
 * Description  : Initialize host stack and profiles.
 * Arguments    : none
 * Return Value : BLR_SUCCESS - SUCCESS
 *                BLE_ERR_INVALID_OPERATION -
 *                    Failed to initialize host stack or profiles.
 ******************************************************************************/
static ble_status_t ble_app_init(void)
{
    ble_status_t status;

    /* TODO: Add Init function of app_lib. */
    /* example: R_BLE_LPC_Init(); */

    /* Initialize host stack */
    status = R_BLE_ABS_Init(&gs_abs_init_param);
    if (BLE_SUCCESS != status)
    {
        return BLE_ERR_INVALID_OPERATION;
    }

    /* Initialize GATT Database */
    status = R_BLE_GATTS_SetDbInst(&g_gatt_db_table);
    if (BLE_SUCCESS != status)
    {
        return BLE_ERR_INVALID_OPERATION;
    }

    /* Initialize GATT server */
    status = R_BLE_SERVS_Init();
    if (BLE_SUCCESS != status)
    {
        return BLE_ERR_INVALID_OPERATION;
    }

    /* Initialize GATT client */
    status = R_BLE_SERVC_Init();
    if (BLE_SUCCESS != status)
    {
        return BLE_ERR_INVALID_OPERATION;
    }

    /* Initialize THP Sensing Service server feature */
    status = R_BLE_THPSS_Init(THPSs_cb);
    if (BLE_SUCCESS != status)
    {
        return BLE_ERR_INVALID_OPERATION;
    }

    /* Initialize Relay Service server feature */
    status = R_BLE_RELAY_SERVICES_Init(relay_services_cb);
    if (BLE_SUCCESS != status)
    {
        return BLE_ERR_INVALID_OPERATION;
    }

    return status;
}

/******************************************************************************
 * Function Name: main
 * Description  : The main loop
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void app_main(void)
{
    /* Initialize BLE */
    R_BLE_Open();

    /* Initialize BLE host stack and profiles */
    ble_app_init();

    /* main loop */
    while (1)
    {
        /* Process Event */
        R_BLE_Execute();

        /* TODO: Add function of app_lib. */
        /* example: R_BLE_LPC_EnterLowPowerMode(); */
    }

    /* TODO: Add Termination processing. */

    /* Terminate BLE */
    R_BLE_Close();
}