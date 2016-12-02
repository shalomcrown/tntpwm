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
#include <tnt/tntnet.h>
#include "baro-bmp180.hpp"

#define DEVICE_ADDRESS 0x77

log_define("baro")


Baro::Baro() {
    fd = wiringPiI2CSetup(DEVICE_ADDRESS);

    if (fd <= 0) {
        log_error("Couldn't open I2C device");
	return;
    }

    for (int calibIndex = 0; calibIndex < 22; calibIndex++) {
	calibration[calibIndex] = wiringPiI2CReadReg8(fd, 0xAA + calibIndex);
    }

}
