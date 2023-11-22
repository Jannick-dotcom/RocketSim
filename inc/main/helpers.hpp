#pragma once

#include <math.h>
#include <stdint.h>
#include "mainGame.hpp"

double findGAtAlt(struct vals *temp, double alt);

double rad2deg(double rad);

double deg2rad(double deg);

extern const std::string OutputPath;