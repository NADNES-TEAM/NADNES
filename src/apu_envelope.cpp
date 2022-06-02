#include "apu_envelope.h"

ApuEnvelope::ApuEnvelope(ApuChannelType channel_type, ApuContainer *apu_container)
    : ApuLengthCounter(channel_type, apu_container) {

}
