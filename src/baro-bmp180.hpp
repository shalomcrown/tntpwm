/*
 * baro-bmp180.h
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

#include <stdint.h>
#include <thread>
#include <cxxtools/thread.h>
#include <cxxtools/jsonserializer.h>

#if ! defined(BARO_BMP180_INCLUDED)
#define BARO_BMP180_INCLUDED

class Baro {
    public:
        double pressure;
        double temperature;
        int measurementPeriod;


        Baro();
        void measure();

        int getFd();
        uint16_t getCalibration(int index);
        int getCalibrationSize();
        int getRawTemp();

        int fd;
        uint16_t calibration[11];
        std::thread readingThread;
        int rawTemp;

        int16_t AC1;
        int16_t AC2;
        int16_t AC3;
        uint16_t AC4;
        uint16_t AC5;
        uint16_t AC6;
        int16_t B1;
        int16_t B2;
        int16_t MB;
        int16_t MC;
        int16_t MD;

        void measureLoop();
};


extern void operator<<= (cxxtools::SerializationInfo& si, const Baro& config);

#endif // BARO_BMP180_INCLUDED
