#pragma once

#include "interfaces/screen_interface.h"
#include <vector>
#include <map>

extern const std::vector<NES::Color> colors_to_bytes;
extern const std::map<NES::Color, uint8_t> bytes_to_colors;
