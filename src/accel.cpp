/*
 * accel.cpp
 *
 * Copyright 2017  <shalomcrown@gmail.com>
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

#include <stdint.h>
#include <thread>
#include <cxxtools/thread.h>
#include <cxxtools/jsonserializer.h>
#include <cxxtools/log.h>


#include "accel.hpp"

int8_t accelAddress = 0x1d;

log_define("accumulator")

Accel::Accel() {

    fd = wiringPiI2CSetup(accelAddress);

    if (fd <= 0) {
        log_error("Couldn't open I2C device");
        return;
    }


}


void operator<<= (cxxtools::SerializationInfo& si, const Accel& config)
{
  si.addMember("fd") <<= config.fd;
  si.addMember("xField") <<= config.xField;
  si.addMember("yField") <<= config.yField;
  si.addMember("zField") <<= config.zField;

  si.addMember("xAccel") <<= config.xAccel;
  si.addMember("yAccel") <<= config.yAccel;
  si.addMember("zAccel") <<= config.zAccel;
}
