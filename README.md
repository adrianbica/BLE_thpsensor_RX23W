# BLE_thpsensor_RX23W
BLE peripheral device based on Renesas RX23W Target board

This is a custom profile with 2 services:
1. THP Sensing Service has three characteristics:
   a. Humidity characteristic
   b. Temperature characteristic
   c. Pressure characteristic
  The service is build around a BME280 sensor connected via I2C to microcontroller
2. Relay service has one characteristic:
   a. Digital characteristic
  It is used to control a board with four relays 

It is a project for Renesas E2Studio using RX C compiler.
The for relay command pins are connected to RX pins P41, P45, P46 and P47. For BME280 sensor, SCL is connected at P16 and SDA at P17
