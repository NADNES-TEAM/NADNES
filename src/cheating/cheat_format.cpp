#include "cheating/cheat_format.h"

namespace NES::Cheating {

SearchItem SearchItem::getNext() const {
    SearchItem searchItem(*this);
    searchItem.addr++;
    return searchItem;
}

void SearchItem::set_const_mem(std::vector<std::uint8_t> &vec) {
    const_mem = &vec[0];
    const_mem_size = vec.size();
}

void SearchItem::set_volatile_mem(std::array<uint8_t, VOLATILE_MEM_SIZE> &arr) {
    volatile_mem = &arr[0];
}

uint16_t SearchItem::getValue() const {
    uint8_t *data = (inConstMem ? const_mem : volatile_mem);
    std::size_t data_size = (inConstMem ? const_mem_size : VOLATILE_MEM_SIZE);
    if (data == nullptr || addr + byteCount - 1 > data_size) {
        throw std::runtime_error("address is too big: " + std::to_string(addr) + " + " +
                                 std::to_string(byteCount) +
                                 " - 1 = " + std::to_string(addr + byteCount - 1) +
                                 " > data_size = " + std::to_string(data_size));
    }
    uint16_t ans = data[addr];
    if (byteCount == 2) {
        ans += (data[addr + 1] << 8);
    }
    return ans;
}
}  // namespace NES::Cheating