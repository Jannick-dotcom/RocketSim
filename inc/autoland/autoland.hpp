#ifndef autoland_hpp
#define autoland_hpp

#include <math.h>
#include <stdint.h>
#include "mainGame.hpp"
// #include "vectors.hpp"
#include "helpers.hpp"

double calculate_d_suicide(struct vals *values, uint8_t ct_engines);
void autoland(struct vals *values);

#endif