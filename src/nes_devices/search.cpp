#include "search.h"
#include <cassert>
#include "algorithm"

namespace NES::Cheating {

Place Place::RAM{0}, Place::ROM{1}, Place::RAM_AND_ROM{2};
bool ParamsOfSearch::check_coincidence(ResultRaw &raw) const {
    long long num_old = raw.old_value;
    long long num_new = raw.cur_value;
    long long num_in = data_in;
    switch (event) {
        case Action::all: return true;
        case Action::save: return num_new == num_old;
        case Action::changed: return num_new != num_old;
        case Action::encrease: return num_new > num_old;
        case Action::decrease: return num_new < num_old;
        case Action::encrease_or_save: return num_new >= num_old;
        case Action::decrease_or_save: return num_new <= num_old;
        case Action::eq_num: return num_new == num_in;
        case Action::geq_num: return num_new >= num_in;
        case Action::leq_num: return num_new <= num_in;
        case Action::g_num: return num_new > num_in;
        case Action::l_num: return num_new < num_in;
        default: assert(false);
    }
}

size_t Place::rom_size{};
size_t Place::get_size() const {
    assert(id == 0 || id == 1);
    if (id == 0) {
        return (1 << 11);
    } else {
        assert(rom_size != 0);
        return rom_size;
    }
}
uint8_t *Place::ram_mem{};
uint8_t *Place::rom_mem{};
uint8_t *Place::get_mem() const {
    assert(id == 0 || id == 1);
    if (id == 0) {
        assert(ram_mem != nullptr);
        return ram_mem;
    } else {
        assert(rom_mem != nullptr);
        return rom_mem;
    }
}
long long ResultRaw::get_value(uint8_t *first, ByteCount byteCount) {
    if (byteCount == ByteCount::ONE) {
        return *first;
    } else if (byteCount == ByteCount::TWO) {
        return *first + (1 << 8) * *std::next(first);
    } else {
        assert(false);
    }
}
}  // namespace NES::Cheating
