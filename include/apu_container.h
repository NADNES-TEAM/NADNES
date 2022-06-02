#pragma once

#include "apu_fwd.h"

class ApuContainer {
public:
    Apu *get_apu();
    ApuMixer *get_mixer();
};