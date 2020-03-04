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
#include <stdlib.h>
#include "r_ble_rx23w_if.h"

#ifndef R_BLE_CMD_H
#define R_BLE_CMD_H

/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 23.08.2019 1.00    First Release
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Parse comma separated hex values.
 * @param[in] p_str    Comma separated string.
 * @param[in] p_buffer uint8_t array to store values.
 * @param[in] p_length The length of values.
 **********************************************************************************************************************/
void R_BLE_CMD_ParseValues(char *p_str, uint8_t *p_buffer, uint16_t *p_length);

/*******************************************************************************************************************//**
 * @brief Print with format like printf.
 * @param[in] p_bufffer uint8_t array to print.
 * @param[in] length    The length of values.
 **********************************************************************************************************************/
void R_BLE_CMD_PrintValues(uint8_t *p_buffer, uint16_t length);

/*******************************************************************************************************************//**
 * @brief Parse bluetooth device address, the format is 00:11:22:33:44:55.
 * @param[in] p_str  uint8_t array to parse.
 * @param[in] p_addr uint8_t array to store address.
 **********************************************************************************************************************/
void R_BLE_CMD_ParseAddr(char *p_str, uint8_t *p_addr);

/*******************************************************************************************************************//**
 * @brief Print bluetooth device address, the format is 00:11:22:33:44:55.
 * @param[in] p_addr uint8_t array to print in bluetooth device address format.
 **********************************************************************************************************************/
void R_BLE_CMD_PrintAddr(uint8_t *p_addr);

#endif /* R_BLE_CMD_H */
