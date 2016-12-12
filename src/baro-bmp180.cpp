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


Baro::Baro() : readingThread(&Baro::measureLoop, this) {

    measurementPeriod = 1000;

    fd = wiringPiI2CSetup(DEVICE_ADDRESS);

    if (fd <= 0) {
        log_error("Couldn't open I2C device");
        return;
    }

    for (int calibIndex = 0; calibIndex < 22; calibIndex++) {
    calibration[calibIndex] = wiringPiI2CReadReg8(fd, 0xAA + calibIndex);
    }
}

//==========================================

void Baro::measure() {

    wiringPiI2CWriteReg8(fd, 0xF4, 0x2e);

    cxxtools::Thread::sleep(53);

    rawTemp = (wiringPiI2CReadReg8(fd, 0xf6) << 8) + (wiringPiI2CReadReg8(fd, 0xf7) & 0xFF);

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

uint8_t Baro::getCalibration(int index) {
    return calibration[index];
}
int Baro::getCalibrationSize() {
    return sizeof(calibration);
}

int Baro::getRawTemp() {
    return rawTemp;
}

