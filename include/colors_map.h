#pragma once

#include <map>
#include <vector>
#include "interfaces/screen_interface.h"

extern const std::vector<NES::Color> colors_to_bytes;
extern const std::map<NES::Color, uint8_t> bytes_to_colors;
