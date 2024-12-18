﻿/*
 * Volcanic Skies
 * Copyright (C) 2024 P. Cilliers Pretorius, University of Cape Town
 *
 * This file is part of the Volcanic Skies project.
 *
 * Volcanic Skies is free software: you can redistribute it and/or modify it under the terms 
 * of the GNU General Public License (GPL) as published by the Free Software 
 * Foundation, either version 2 of the License, or (at your discretion) any later version.
 *
 * Volcanic Skies is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with 
 * Volcanic Skies in the LICENSE file. If not, see <https://www.gnu.org/licenses/>.
 *
 * Additional information and disclaimers regarding liability and third-party 
 * components can be found in the NOTICE file included with this project.
 *
 */
#include <cmath>
#include "position.h"

namespace PWM{
    namespace PWMDataStructure{
        Position::Position(double X, double Y, double Z) : x(X), y(Y), z(Z){

        }

        const double Position::getX() const{
            return x;
        }

        const double Position::getY() const{
            return y;
        }

        const double Position::getZ() const{
            return z;
        }

        const double Position::getDistanceTo(Position& other) const{
            return sqrt(pow(getX() - other.getX(), 2) + pow(getY() - other.getY(), 2) + pow(getZ() - other.getZ(), 2));
        }

        void Position::move(double newX, double newY, double newZ){
            x = newX;
            y = newY;
            z = newZ;
        }

        void Position::move(std::tuple<double, double, double>& distance){
            x += std::get<0>(distance);
            y += std::get<1>(distance);
            z += std::get<2>(distance);
        }

        void Position::move(std::tuple<double, double, double>& velocity, double dt){
            x += std::get<0>(velocity) * dt;
            y += std::get<1>(velocity) * dt;
            z += std::get<2>(velocity) * dt;
        }

        void Position::move(Position& other){
            x = other.getX();
            y = other.getY();
            z = other.getZ();
        }

        bool Position::operator==(Position& other){
            if (getX() != other.getX())
                return false;
            if (getY() != other.getY())
                return false;
            if (getZ() != other.getZ())
                return false;
            return true;
        }
        
        bool Position::operator!=(Position& other){
            return !(*this == other);
        }
    }
}