/*
 * baro-bmp180.cpp
 *
 * Copyright 2016  <shalomcrown@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */


#include <iostream>
#include <wiringPiI2C.h>
#include <cxxtools/log.h>
#include <cxxtools/thread.h>
#include <tnt/tntnet.h>
#include <thread>
#include "baro-bmp180.hpp"


#define DEVICE_ADDRESS 0x77
#define RAW_TEMP_MSB 0xF6
#define RAW_TEMP_LSB 0xF7
#define RAW_TEMP_XL SB 0xF8



log_define("baro")


static int baroLookupTimes[] = {5, 8, 14, 26};

//==========================================

uint16_t readI2CShortValue(int fd, uint8_t address) {
    uint8_t ms = wiringPiI2CReadReg8(fd, address);
    uint8_t ls = wiringPiI2CReadReg8(fd, address + 1);

    uint16_t value = (((uint16_t)ms & 0xFF) << 8) + ((uint16_t)ls & 0xFF);
    return value;
}



uint32_t readI2CLongValueThreeRegs(int fd, uint8_t address) {
    uint8_t ms = wiringPiI2CReadReg8(fd, address);
    uint8_t ls = wiringPiI2CReadReg8(fd, address + 1);
    uint8_t xs = wiringPiI2CReadReg8(fd, address + 2);

    uint32_t value = (((uint32_t)ms & 0xFF) << 13) + (((uint32_t)ls & 0xFF) << 5) + ((((uint32_t)xs & 0xFF) >> 3));
    return value;
}



//==========================================

Baro::Baro() : readingThread(&Baro::measureLoop, this) {

    measurementPeriod = 1000;
    oversampling = 2;

    fd = wiringPiI2CSetup(DEVICE_ADDRESS);

    if (fd <= 0) {
        log_error("Couldn't open I2C device");
        return;
    }

    for (uint8_t calibIndex = 0; calibIndex < 11; calibIndex++) {
        calibration[calibIndex] = readI2CShortValue(fd, 0xAA + calibIndex * 2);
    }


    AC1 = calibration[0];
    AC2 = calibration[1];
    AC3 = calibration[2];
    AC4 = calibration[3];
    AC5 = calibration[4];
    AC6 = calibration[5];

    B1 = calibration[6];
    B2 = calibration[7];
    MB = calibration[8];
    MC = calibration[9];
    MD = calibration[10];
}

//==========================================

void Baro::measure() {

    wiringPiI2CWriteReg8(fd, 0xF4, 0x2e);

    cxxtools::Thread::sleep(53);

    rawTemp = readI2CShortValue(fd, 0xf6);

    double x1 = (double)(rawTemp - AC6) * AC5 / 32768.0;
    double x2 = MC * 2048.0 / (x1 + MD);
    double b5 = x1 + x2;
    temperature = (b5 + 8) / 16.0 / 10.0;

    oversampling = oversampling & 0x3;

    wiringPiI2CWriteReg8(fd, 0xF4, 0x34 | (oversampling << 6));

    cxxtools::Thread::sleep(baroLookupTimes[oversampling]);

    rawPressure = readI2CLongValueThreeRegs(fd, 0xf6) >> (8 - oversampling);

    double b6 = b5 - 4000;
    x1 = (B2 * (b6 * b6 / 4096.0)) / 2048.0;
    x2 = AC2 * b6 / 2048.0;
    double x3 = x1 + x2;
    double b3 = (((AC1 * 4.0 + x3) * pow(2, oversampling)) + 2.0) / 4.0;
    x1 = AC3 * b6 /  8192.0;
    x2 = (B1 * (b6 * b6 / 4096.0)) / 65536.;
    x3 = ((x1 + x2) + 2.0) / 4.0;
    double b4 = AC4 * (x3 + 32768.0) / 32768.0;
    double b7 = ((rawPressure - b3)) * (50000 >> oversampling);

    double p;
    p = (b7 * 2.0) / b4;

    //if (b7 < 0X80000000) {
    //    p = (b7 * 2.) / b4;
    //} else {
    //    p = (b7 / b4) * 2.;
    //}

    x1 = (p / 256.0) * (p / 256.);
    x1 = (x1 * 3038.0) / 65536.0;
    x2 = -7357.0 * p / 65536.0;
    pressure = p + (x1 + x2 + 3791.0) / 16.0;

    cxxtools::Thread::sleep(measurementPeriod);
}


void Baro::measureLoop() {
    cxxtools::Thread::sleep(53);

    while (true) {
        measure();
        cxxtools::Thread::sleep(measurementPeriod);
    }
}

//==========================================

int Baro::getFd() {
    return fd;
}

uint16_t Baro::getCalibration(int index) {
    return calibration[index];
}
int Baro::getCalibrationSize() {
    return sizeof(calibration) / sizeof(uint16_t);
}

int Baro::getRawTemp() {
    return rawTemp;
}


void operator<<= (cxxtools::SerializationInfo& si, const Baro& config)
{
  si.addMember("fd") <<= config.fd;
  si.addMember("rawTemp") <<= config.rawTemp;
  si.addMember("temperature") <<= config.temperature;
  si.addMember("pressure") <<= config.pressure;
  si.addMember("rawPressure") <<= config.rawPressure;


  si.addMember("AC1") <<= config.AC1;
  si.addMember("AC2") <<= config.AC2;
  si.addMember("AC3") <<= config.AC3;
  si.addMember("AC4") <<= config.AC4;
  si.addMember("AC5") <<= config.AC5;
  si.addMember("AC6") <<= config.AC6;
  si.addMember("B1") <<= config.B1;
  si.addMember("B2") <<= config.B2;

  si.addMember("MB") <<= config.MB;
  si.addMember("MC") <<= config.MC;
  si.addMember("MD") <<= config.MD;
}
