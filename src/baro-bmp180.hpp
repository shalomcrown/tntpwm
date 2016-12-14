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

    private:
        int fd;
        uint16_t calibration[22];
        std::thread readingThread;
        int rawTemp;

        void measureLoop();
};

#endif // BARO_BMP180_INCLUDED
