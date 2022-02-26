#include "ApuExceptions.h"

namespace NADNESS::APU {

std::string get_hex(std::uint16_t addr) {
    std::stringstream ss;
    ss << "$" << std::hex << addr;
    return ss.str();
}

BadApuRead::BadApuRead(uint16_t addr)
    : std::runtime_error("Apu unavailable read address: " + get_hex(addr)) {
}

BadApuWrite::BadApuWrite(uint16_t addr) :
std::runtime_error("Apu unavailable write address: " + get_hex(addr)) {
}

}  // namespace NADNESS::APU
