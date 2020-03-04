#include <stdint.h>
#include "THPSensor.h"
#include "i2c_bus.h"
#include "timer.h"
#include "r_smc_entry.h"
//#include "smc_gen/Config_BLE_PROFILE/r_ble_THPSs.h"

typedef uint16_t ble_status_t;

extern "C" {
	ble_status_t R_BLE_THPSS_SetTemperature(const int16_t *p_value);
	ble_status_t R_BLE_THPSS_GetTemperature(int16_t *p_value);
	ble_status_t R_BLE_THPSS_NotifyTemperature(uint16_t conn_hdl, const int16_t *p_value);

	ble_status_t R_BLE_THPSS_SetHumidity(const uint16_t *p_value);
	ble_status_t R_BLE_THPSS_GetHumidity(uint16_t *p_value);
	ble_status_t R_BLE_THPSS_NotifyHumidity(uint16_t conn_hdl, const uint16_t *p_value);

	ble_status_t R_BLE_THPSS_SetPressure(const uint32_t *p_value);
	ble_status_t R_BLE_THPSS_GetPressure(uint32_t *p_value);
	ble_status_t R_BLE_THPSS_NotifyPressure(uint16_t conn_hdl, const uint32_t *p_value);
}


#define SENSOR_I2C_ADDRESS      (0x76)

// sensor commands
#define READ_ID             0xd0
#define READ_CALIBRATION_1  0x88
#define READ_CALIBRATION_2  0xE1
#define CTRL_HUM            0xF2
#define CTRL_MEASUREMENT    0xF4
#define REG_CONFIG          0xF5
#define RESET_REG           0xE0
#define READ_VALUES         0xF7

// Timeout values
// maximum allowed time for i2c transaction in milliseconds
#define I2C_TIMEOUT     3
#define SENSOR_POLL_INTERVAL    5000

static int32_t  t_fine;

static int16_t ReadTemperature(int32_t Tread, CALIBRATION Calibr)
{
    int32_t     var1, var2;
    var1 = ((((Tread >> 3) - ((int32_t)Calibr.dig_T1 << 1))) * ((int32_t)Calibr.dig_T2)) >> 11;
    var2 = (((((Tread >> 4) - ((int32_t)Calibr.dig_T1)) * ((Tread >> 4) - ((int32_t)Calibr.dig_T1))) >> 12) * ((int32_t)Calibr.dig_T3)) >> 14;
    int32_t t_fine = var1 + var2;
    int32_t T = (t_fine * 5 + 128) >> 8;
    return (int16_t)T;
}

static uint32_t ReadPressure(int32_t Pread, CALIBRATION Calibr)
{
    int64_t     var1, var2, p;
    int32_t     pr;

    pr = Pread;// >> 4;
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)Calibr.dig_P6;
    var2 = var2 + ((var1 * (int64_t)Calibr.dig_P5) << 17);
    var2 = var2 + (((int64_t)Calibr.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)Calibr.dig_P3) >> 8) + ((var1 * (int64_t)Calibr.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)Calibr.dig_P1) >> 33;
    if (var1 == 0) {
        return 0;
    }
    p = 1048576 - pr;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)Calibr.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)Calibr.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)Calibr.dig_P7) << 4);

    return (uint32_t)((10 * p)/256);
}

static uint16_t ReadHumidity(int32_t Hread, CALIBRATION Calibr)
{
    int64_t v_x1_u32r;

    v_x1_u32r = (t_fine - ((int32_t)76800));

    v_x1_u32r = (((((Hread << 14) - (((int32_t)Calibr.dig_H4) << 20) - (((int32_t)Calibr.dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
                 (((((((v_x1_u32r * ((int32_t)Calibr.dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t)Calibr.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
                    ((int32_t)2097152)) * ((int32_t)Calibr.dig_H2) + 8192) >> 14));

    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
                               ((int32_t)Calibr.dig_H1)) >> 4));

    v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
    v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;

    return(uint16_t)((100 * v_x1_u32r) >> 22);
}


THPSensor::THPSensor(void) {
    i2c_bus::GetInstance().init(SENSOR_I2C_ADDRESS);
    state = STATE_INIT;
    sub_state = 0;

    setTemperatureNotifyStatus(0);
    setPressureNotifyStatus(0);
    setHumidityNotifyStatus(0);
}

void THPSensor::checkAnswerTimeout(void) {
    if (Timer::GetInstance().IsTimerElapsed(SENSOR_TIMER, I2C_TIMEOUT)) {
        state = STATE_ERROR;
        sub_state = 0;
    }
}

void THPSensor::init(void) {
    static uint8_t  	cmd[2];
    MD_STATUS 			status;

    switch (sub_state) {
    case 0:
        cmd[0] = READ_ID;
        status = i2c_bus::GetInstance().write(cmd, 1);
        sub_state = 1;
        Timer::GetInstance().ArmTimer(SENSOR_TIMER);
        break;
    case 1:
        if (i2c_bus::GetInstance().transfer_ready()) {
        	status = i2c_bus::GetInstance().read(cmd, 1);
            sub_state = 2;
            Timer::GetInstance().ArmTimer(SENSOR_TIMER);
        } else {
            checkAnswerTimeout();
        }
        break;
    case 2:
        if (i2c_bus::GetInstance().transfer_ready()) {
            state = STATE_READ_CALIBR;
            sub_state = 0;
            Timer::GetInstance().ArmTimer(SENSOR_TIMER);
        } else {
            checkAnswerTimeout();
        }
        break;
    }
    return;
}

void THPSensor::calibrate(void) {
    static uint8_t  cmd[2];
    static uint8_t  buffer[32];

    switch (sub_state) {
    case 0:
        cmd[0] = READ_CALIBRATION_1;
        i2c_bus::GetInstance().write(cmd, 1);
        sub_state = 1;
        Timer::GetInstance().ArmTimer(SENSOR_TIMER);
        break;
    case 1:
        if (i2c_bus::GetInstance().transfer_ready()) {
            i2c_bus::GetInstance().read(buffer, 26);
            sub_state = 2;
            Timer::GetInstance().ArmTimer(SENSOR_TIMER);
        } else {
            checkAnswerTimeout();
        }
        break;
    case 2:
        if (i2c_bus::GetInstance().transfer_ready()) {
            Calibr.dig_T1 = (uint16_t)((buffer[1] << 8) + buffer[0]);
            Calibr.dig_T2 = (int16_t)((buffer[3] << 8) + buffer[2]);
            Calibr.dig_T3 = (int16_t)((buffer[5] << 8) + buffer[4]);
            Calibr.dig_P1 = (uint16_t)((buffer[7] << 8) + buffer[6]);
            Calibr.dig_P2 = (int16_t)((buffer[9] << 8) + buffer[8]);
            Calibr.dig_P3 = (int16_t)((buffer[11] << 8) + buffer[10]);
            Calibr.dig_P4 = (int16_t)((buffer[13] << 8) + buffer[12]);
            Calibr.dig_P5 = (int16_t)((buffer[15] << 8) + buffer[14]);
            Calibr.dig_P6 = (int16_t)((buffer[17] << 8) + buffer[16]);
            Calibr.dig_P7 = (int16_t)((buffer[19] << 8) + buffer[18]);
            Calibr.dig_P8 = (int16_t)((buffer[21] << 8) + buffer[20]);
            Calibr.dig_P9 = (int16_t)((buffer[23] << 8) + buffer[22]);
            Calibr.dig_H1 = buffer[25];
            cmd[0] = READ_CALIBRATION_2;
            i2c_bus::GetInstance().write(cmd, 1);
            sub_state = 3;
            Timer::GetInstance().ArmTimer(SENSOR_TIMER);
        } else {
            checkAnswerTimeout();
        }
        break;
    case 3:
        if (i2c_bus::GetInstance().transfer_ready()) {
            i2c_bus::GetInstance().read(buffer, 7);
            sub_state = 4;
            Timer::GetInstance().ArmTimer(SENSOR_TIMER);
        } else {
            checkAnswerTimeout();
        }
        break;
    case 4:
        if (i2c_bus::GetInstance().transfer_ready()) {
            Calibr.dig_H2 = (int16_t)((buffer[1] << 8) + buffer[0]);
            Calibr.dig_H3 = buffer[2];
            Calibr.dig_H4 = (int16_t)((buffer[3] << 4) + (buffer[4] & 0x07));
            Calibr.dig_H5 = (int16_t)((buffer[5] << 4) + ((buffer[4] >> 4) & 0x07));
            Calibr.dig_H6 = buffer[6];
            state = STATE_CONFIGURE;
            sub_state = 0;
            Timer::GetInstance().ArmTimer(SENSOR_TIMER);
        } else {
            checkAnswerTimeout();
        }
        break;
    }
    return;
}

void THPSensor::configure(void) {
    static uint8_t  cmd[2];

    switch (sub_state) {
    case 0:
        cmd[0] = RESET_REG;
        cmd[1] = 0xB6;
        i2c_bus::GetInstance().write(cmd, 2);
        sub_state = 1;
        Timer::GetInstance().ArmTimer(SENSOR_TIMER);
        break;
    case 1:
        if (i2c_bus::GetInstance().transfer_ready()) {
            cmd[0] = CTRL_HUM;
            cmd[1] = 0x01; // h oversampling x1
            i2c_bus::GetInstance().write(cmd, 2);
            sub_state = 2;
            Timer::GetInstance().ArmTimer(SENSOR_TIMER);
        } else {
            checkAnswerTimeout();
        }
        break;
    case 2:
        if (i2c_bus::GetInstance().transfer_ready()) {
            cmd[0] = CTRL_MEASUREMENT;
            cmd[1] = 0xAB; // p oversampling x16, t oversampling x2, normal mode
            i2c_bus::GetInstance().write(cmd, 2);
            sub_state = 3;
            Timer::GetInstance().ArmTimer(SENSOR_TIMER);
        } else {
            checkAnswerTimeout();
        }
        break;
    case 3:
        if (i2c_bus::GetInstance().transfer_ready()) {
            cmd[0] = REG_CONFIG;
            cmd[1] = 0x30; // p oversampling x16, t oversampling x2, normal mode
            i2c_bus::GetInstance().write(cmd, 2);
            sub_state = 4;
            Timer::GetInstance().ArmTimer(SENSOR_TIMER);
        } else {
            checkAnswerTimeout();
        }
        break;
    case 4:
        if (i2c_bus::GetInstance().transfer_ready()) {
            state = STATE_IDLE;
            sub_state = 0;
            Timer::GetInstance().ArmTimer(SENSOR_TIMER);
            //NRF_LOG_INFO("Configured");
        } else {
            checkAnswerTimeout();
        }
        break;
    }
    return;
}


extern uint16_t g_conn_hdl;

void THPSensor::measure(void) {
    static uint8_t  cmd[2];
    static uint8_t  buffer[32];
    int32_t         Tread;
    int32_t         Pread;
    int32_t         Hread;

    switch (sub_state) {
    case 0:
        if (Timer::GetInstance().IsTimerElapsed(SENSOR_TIMER, SENSOR_POLL_INTERVAL)) {
            Timer::GetInstance().ArmTimer(SENSOR_TIMER);
            cmd[0] = READ_VALUES;
            i2c_bus::GetInstance().write(cmd, 1);
            sub_state = 1;
        }
        break;
    case 1:
        if (i2c_bus::GetInstance().transfer_ready()) {
            i2c_bus::GetInstance().read(buffer, 8);
            sub_state = 2;
            Timer::GetInstance().ArmTimer(SENSOR_TIMER);
        } else {
            checkAnswerTimeout();
        }
        break;
    case 2:
        if (i2c_bus::GetInstance().transfer_ready()) {
            Tread = ((uint32_t)buffer[5] & 0x0f) + ((uint32_t)buffer[4] << 4) + ((uint32_t)buffer[3] << 12);
            Pread = ((uint32_t)buffer[2] & 0x0f) + ((uint32_t)buffer[1] << 4) + ((uint32_t)buffer[0] << 12);
            Hread = (uint32_t)buffer[7] + ((uint32_t)buffer[6] << 8);
            Temperature = ReadTemperature(Tread, Calibr);
            Pressure = ReadPressure(Pread, Calibr);
            Humidity = ReadHumidity(Hread, Calibr);

            // update BLE database
            R_BLE_THPSS_SetTemperature(&Temperature);
            R_BLE_THPSS_NotifyTemperature(g_conn_hdl, &Temperature);
            R_BLE_THPSS_SetHumidity(&Humidity);
            R_BLE_THPSS_NotifyHumidity(g_conn_hdl, &Humidity);
            R_BLE_THPSS_SetPressure(&Pressure);
            R_BLE_THPSS_NotifyPressure(g_conn_hdl, &Pressure);

            sub_state = 0;
            Timer::GetInstance().ArmTimer(SENSOR_TIMER);
        } else {
            checkAnswerTimeout();
        }
        break;
    }
    return;
}

void THPSensor::task(void) {
    switch (state) {
    case STATE_INIT:
        // detect sensor by reading ID
        init();
        break;
    case STATE_READ_CALIBR:
        calibrate();
        break;
    case STATE_CONFIGURE:
        configure();
        break;
    case STATE_IDLE:
        measure();
        break;
    case STATE_ERROR:
        break;
    default:
        break;
    }
}
