#pragma once

// TODO: Can add enum NSTC/PAM/Famicon/whatever

enum class Frame {
    NONE,
    QUARTER,
    HALF,
};

enum class StepMode {
    FOUR_STEP = 0,
    FIVE_STEP = 1,
};

enum class ApuChannelType {
    PULSE1,
    PULSE2,
    NOISE,
    TRIANGLE,
    DMC
};