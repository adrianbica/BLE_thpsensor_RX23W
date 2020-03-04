/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : Config_RIIC0_user.c
* Version      : 1.8.0
* Device(s)    : R5F523W8AxNG
* Description  : This file implements device driver for Config_RIIC0.
* Creation Date: 2020-03-02
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "Config_RIIC0.h"
/* Start user code for include. Do not edit comment generated here */
#include "interface.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t    g_riic0_mode_flag;               /* RIIC0 master transmit receive flag */
extern volatile uint8_t    g_riic0_state;                   /* RIIC0 master state */
extern volatile uint16_t   g_riic0_slave_address;           /* RIIC0 slave address */
extern volatile uint8_t   *gp_riic0_tx_address;             /* RIIC0 transmit buffer address */
extern volatile uint16_t   g_riic0_tx_count;                /* RIIC0 transmit data number */
extern volatile uint8_t   *gp_riic0_rx_address;             /* RIIC0 receive buffer address */
extern volatile uint16_t   g_riic0_rx_count;                /* RIIC0 receive data number */
extern volatile uint16_t   g_riic0_rx_length;               /* RIIC0 receive data length */
extern volatile uint8_t    g_riic0_stop_generation;         /* RIIC0 stop condition generation flag */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_RIIC0_Create_UserInit
* Description  : This function adds user code after initializing the RIIC0 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_RIIC0_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_RIIC0_transmit_interrupt
* Description  : This function is TXI0 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_RIIC0_TXI0
#pragma interrupt r_Config_RIIC0_transmit_interrupt(vect=VECT(RIIC0,TXI0),fint)
#else
#pragma interrupt r_Config_RIIC0_transmit_interrupt(vect=VECT(RIIC0,TXI0))
#endif
static void r_Config_RIIC0_transmit_interrupt(void)
{
    if (_0D_IIC_MASTER_TRANSMIT == g_riic0_mode_flag)
    {
        if (_01_IIC_MASTER_SENDS_ADR_7_W == g_riic0_state)
        {
            RIIC0.ICDRT = (uint8_t)(g_riic0_slave_address << 1U);
            g_riic0_state = _05_IIC_MASTER_SENDS_DATA;
        }
        else if (_02_IIC_MASTER_SENDS_ADR_10A_W == g_riic0_state)
        {
            RIIC0.ICDRT = (uint8_t)(((g_riic0_slave_address & 0x0300U) >> 7U) | 0x00F0U);
            g_riic0_state = _04_IIC_MASTER_SENDS_ADR_10B;
        }
        else if (_04_IIC_MASTER_SENDS_ADR_10B == g_riic0_state)
        {
            RIIC0.ICDRT = (uint8_t)(g_riic0_slave_address & 0x00FFU);
            g_riic0_state = _05_IIC_MASTER_SENDS_DATA;
        }
        else if (_05_IIC_MASTER_SENDS_DATA == g_riic0_state)
        {
            if (0U < g_riic0_tx_count)
            {
                RIIC0.ICDRT = *gp_riic0_tx_address;
                gp_riic0_tx_address++;
                g_riic0_tx_count--;
            }
            else
            {
                g_riic0_state = _06_IIC_MASTER_SENDS_END;
            }
        }
        else
        {
             /* Do nothing */
        }
    }
    else if (_0C_IIC_MASTER_RECEIVE == g_riic0_mode_flag)
    {
        if (_00_IIC_MASTER_SENDS_ADR_7_R == g_riic0_state)
        {
            RIIC0.ICDRT = (uint8_t)((g_riic0_slave_address << 1U) | 0x0001U);
            g_riic0_state = _08_IIC_MASTER_RECEIVES_START;
        }
        else if (_02_IIC_MASTER_SENDS_ADR_10A_W == g_riic0_state)
        {
            RIIC0.ICDRT = (uint8_t)(((g_riic0_slave_address & 0x0300U) >> 7U) | 0x00F0U);
            g_riic0_state = _04_IIC_MASTER_SENDS_ADR_10B;
        }
        else if (_04_IIC_MASTER_SENDS_ADR_10B == g_riic0_state)
        {
            RIIC0.ICDRT = (uint8_t)(g_riic0_slave_address & 0x00FFU);
            g_riic0_state = _0E_IIC_MASTER_RECEIVES_RESTART;
        }
        else if (_0E_IIC_MASTER_RECEIVES_RESTART == g_riic0_state)
        {
            RIIC0.ICSR2.BIT.START = 0U;
            RIIC0.ICIER.BIT.STIE = 1U;
            RIIC0.ICCR2.BIT.RS = 1U;    /* Set restart condition flag */
        }
        else if (_03_IIC_MASTER_SENDS_ADR_10A_R == g_riic0_state)
        {
            RIIC0.ICDRT = (uint8_t)(((g_riic0_slave_address & 0x0300U) >> 7U) | 0x00F1U);
            g_riic0_state = _08_IIC_MASTER_RECEIVES_START;
        }
        else
        {
             /* Do nothing */
        }
    }
    else
    {
         /* Do nothing */
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_RIIC0_transmitend_interrupt
* Description  : This function is TEI0 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_RIIC0_TEI0
#pragma interrupt r_Config_RIIC0_transmitend_interrupt(vect=VECT(RIIC0,TEI0),fint)
#else
#pragma interrupt r_Config_RIIC0_transmitend_interrupt(vect=VECT(RIIC0,TEI0))
#endif
static void r_Config_RIIC0_transmitend_interrupt(void)
{
    if (_06_IIC_MASTER_SENDS_END == g_riic0_state)
    {
        if (1U == g_riic0_stop_generation)
        {
            RIIC0.ICSR2.BIT.STOP = 0U;
            RIIC0.ICCR2.BIT.SP = 1U;

            g_riic0_state = _07_IIC_MASTER_SENDS_STOP;
        }
        else
        {
            RIIC0.ICSR2.BIT.TEND = 0U;
            r_Config_RIIC0_callback_transmitend();
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_RIIC0_receive_interrupt
* Description  : This function is RXI0 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_RIIC0_RXI0
#pragma interrupt r_Config_RIIC0_receive_interrupt(vect=VECT(RIIC0,RXI0),fint)
#else
#pragma interrupt r_Config_RIIC0_receive_interrupt(vect=VECT(RIIC0,RXI0))
#endif
static void r_Config_RIIC0_receive_interrupt(void)
{
    volatile uint8_t dummy;

    if (_08_IIC_MASTER_RECEIVES_START == g_riic0_state)
    {
        if ((2U == g_riic0_rx_length) || (1U == g_riic0_rx_length))
        {
            RIIC0.ICMR3.BIT.WAIT = 1U;
        }

        if (1U == g_riic0_rx_length)
        {
            RIIC0.ICMR3.BIT.RDRFS = 1U;
            RIIC0.ICMR3.BIT.ACKWP = 1U;
            RIIC0.ICMR3.BIT.ACKBT = 1U;
        }

        /* Dummy read to release SCL */
        dummy = RIIC0.ICDRR;

        g_riic0_state = _09_IIC_MASTER_RECEIVES_DATA;

        if (1U == g_riic0_rx_length)
        {
            g_riic0_state = _0A_IIC_MASTER_RECEIVES_STOPPING;
        }
    }
    else if (_09_IIC_MASTER_RECEIVES_DATA == g_riic0_state)
    {
        if (g_riic0_rx_count < g_riic0_rx_length)
        {
            if (g_riic0_rx_count == (g_riic0_rx_length - 3))
            {
                RIIC0.ICMR3.BIT.WAIT = 1U;

                *gp_riic0_rx_address = RIIC0.ICDRR;
                gp_riic0_rx_address++;
                g_riic0_rx_count++;
            }
            else if (g_riic0_rx_count == (g_riic0_rx_length - 2))
            {
                RIIC0.ICMR3.BIT.RDRFS = 1U;
                RIIC0.ICMR3.BIT.ACKWP = 1U;
                RIIC0.ICMR3.BIT.ACKBT = 1U;

                *gp_riic0_rx_address = RIIC0.ICDRR;
                gp_riic0_rx_address++;
                g_riic0_rx_count++;

                g_riic0_state = _0A_IIC_MASTER_RECEIVES_STOPPING;
            }
            else
            {
                *gp_riic0_rx_address = RIIC0.ICDRR;
                gp_riic0_rx_address++;
                g_riic0_rx_count++;
            }
        }
    }
    else if (_0A_IIC_MASTER_RECEIVES_STOPPING == g_riic0_state)
    {
        RIIC0.ICSR2.BIT.STOP = 0U;
        RIIC0.ICCR2.BIT.SP = 1U;

        *gp_riic0_rx_address = RIIC0.ICDRR;
        gp_riic0_rx_address++;
        g_riic0_rx_count++;

        RIIC0.ICMR3.BIT.ACKWP = 1U;
        RIIC0.ICMR3.BIT.ACKBT = 1U;
        RIIC0.ICMR3.BIT.WAIT = 0U;

        g_riic0_state = _0B_IIC_MASTER_RECEIVES_STOP;
    }
    else
    {
         /* Do nothing */
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_RIIC0_error_interrupt
* Description  : This function is EEI0 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_RIIC0_EEI0
#pragma interrupt r_Config_RIIC0_error_interrupt(vect=VECT(RIIC0,EEI0),fint)
#else
#pragma interrupt r_Config_RIIC0_error_interrupt(vect=VECT(RIIC0,EEI0))
#endif
static void r_Config_RIIC0_error_interrupt(void)
{
    volatile uint8_t dummy;

    if ((1U == RIIC0.ICIER.BIT.ALIE) && (1U == RIIC0.ICSR2.BIT.AL))
    {
        r_Config_RIIC0_callback_receiveerror(MD_ERROR1);
    }
    else if ((1U == RIIC0.ICIER.BIT.TMOIE) && (1U == RIIC0.ICSR2.BIT.TMOF))
    {
        r_Config_RIIC0_callback_receiveerror(MD_ERROR2);
    }
    else if ((1U == RIIC0.ICIER.BIT.NAKIE) && (1U == RIIC0.ICSR2.BIT.NACKF))
    {
        if (_0D_IIC_MASTER_TRANSMIT == g_riic0_mode_flag)
        {
            RIIC0.ICSR2.BIT.STOP = 0U;
            RIIC0.ICCR2.BIT.SP = 1U;
            while (1U != RIIC0.ICSR2.BIT.STOP)
            {
                nop();
            }
            RIIC0.ICSR2.BIT.NACKF = 0U;
            RIIC0.ICSR2.BIT.STOP = 0U;
        }
        else if (_0C_IIC_MASTER_RECEIVE == g_riic0_mode_flag)
        {
            RIIC0.ICSR2.BIT.STOP = 0U;
            RIIC0.ICCR2.BIT.SP = 1U;

            /* Dummy read the ICDRR register */
            dummy = RIIC0.ICDRR;
            while (1U != RIIC0.ICSR2.BIT.STOP)
            {
                nop();
            }
            RIIC0.ICSR2.BIT.NACKF = 0U;
            RIIC0.ICSR2.BIT.STOP = 0U;
        }

        r_Config_RIIC0_callback_receiveerror(MD_ERROR3);
    }
    else if (_0D_IIC_MASTER_TRANSMIT == g_riic0_mode_flag)
    {
        if ((_01_IIC_MASTER_SENDS_ADR_7_W == g_riic0_state) || (_02_IIC_MASTER_SENDS_ADR_10A_W == g_riic0_state))
        {
            RIIC0.ICSR2.BIT.START = 0U;
            RIIC0.ICIER.BIT.STIE = 0U;
            RIIC0.ICIER.BIT.SPIE = 1U;
        }
        else if (_07_IIC_MASTER_SENDS_STOP == g_riic0_state)
        {
            RIIC0.ICSR2.BIT.NACKF = 0U;
            RIIC0.ICSR2.BIT.STOP = 0U;
            RIIC0.ICIER.BIT.SPIE = 0U;
            RIIC0.ICIER.BIT.STIE = 1U;

            r_Config_RIIC0_callback_transmitend();
        }
        else
        {
             /* Do nothing */
        }
    }
    else if (_0C_IIC_MASTER_RECEIVE == g_riic0_mode_flag)
    {
        if ((_00_IIC_MASTER_SENDS_ADR_7_R == g_riic0_state) || (_02_IIC_MASTER_SENDS_ADR_10A_W == g_riic0_state))
        {
            RIIC0.ICSR2.BIT.START = 0U;
            RIIC0.ICIER.BIT.STIE = 0U;
            RIIC0.ICIER.BIT.SPIE = 1U;
        }
        else if (_0E_IIC_MASTER_RECEIVES_RESTART == g_riic0_state)
        {
            RIIC0.ICSR2.BIT.START = 0U;
            RIIC0.ICIER.BIT.STIE = 0U;
            g_riic0_state = _03_IIC_MASTER_SENDS_ADR_10A_R;
        }
        else if (_0B_IIC_MASTER_RECEIVES_STOP == g_riic0_state)
        {
            RIIC0.ICMR3.BIT.RDRFS = 0U;
            RIIC0.ICMR3.BIT.ACKWP = 1U;
            RIIC0.ICMR3.BIT.ACKBT = 0U;
            RIIC0.ICSR2.BIT.NACKF = 0U;
            RIIC0.ICSR2.BIT.STOP = 0U;
            RIIC0.ICIER.BIT.SPIE = 0U;
            RIIC0.ICIER.BIT.STIE = 1U;

            r_Config_RIIC0_callback_receiveend();
        }
        else
        {
             /* Do nothing */
        }
    }
    else
    {
         /* Do nothing */
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_RIIC0_callback_transmitend
* Description  : This function is a callback function when RIIC0 finishes transmission
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_RIIC0_callback_transmitend(void)
{
    /* Start user code for r_Config_RIIC0_callback_transmitend. Do not edit comment generated here */
	CompleteI2CTransfer();
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_RIIC0_callback_receiveend
* Description  : This function is a callback function when RIIC0 finishes reception
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_RIIC0_callback_receiveend(void)
{
    /* Start user code for r_Config_RIIC0_callback_receiveend. Do not edit comment generated here */
	CompleteI2CTransfer();
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_RIIC0_callback_receiveerror
* Description  : This function is a callback function when RIIC0 encounters error while receiving
* Arguments    : status -
*                    MD_OK or MD_ARGERROR
* Return Value : None
***********************************************************************************************************************/

static void r_Config_RIIC0_callback_receiveerror(MD_STATUS status)
{
    /* Start user code for r_Config_RIIC0_callback_receiveerror. Do not edit comment generated here */
	CompleteI2CTransfer();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */   



