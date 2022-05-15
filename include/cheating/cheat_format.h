#ifndef NADNES_CHEAT_FORMAT_H
#define NADNES_CHEAT_FORMAT_H

#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace NES::Cheating {
struct SearchItem {
    static uint8_t *const_mem;
    static std::size_t const_mem_size;
    static uint8_t *volatile_mem;
    static constexpr std::size_t VOLATILE_MEM_SIZE = 1 << 11;
    static void set_const_mem(std::vector<std::uint8_t> &vec);
    static void set_volatile_mem(std::array<uint8_t, VOLATILE_MEM_SIZE> &arr);
    uint16_t addr{};
    bool inConstMem{};
    int byteCount = 1;  // 1 or 2
    [[nodiscard]] uint16_t getValue() const;
    [[nodiscard]] SearchItem getNext() const;
};

uint8_t *SearchItem::const_mem{};
std::size_t SearchItem::const_mem_size{};
uint8_t *SearchItem::volatile_mem{};

struct Cheat {
    std::vector<SearchItem> addrs;
    int byteCount = 1;  // 1 or 2
    uint16_t setValue{};
};

enum class CompareType { LE, LEEQ, EQ, GREQ, GR, NEQ };

struct Filter {
    int byteCount = 1;
    CompareType compareType = CompareType::EQ;
    uint16_t compareWith{};
};

struct SearchState {
    std::vector<SearchItem> addrs;
    Filter filter;
};

}  // namespace NES::Cheating

#endif  // NADNES_CHEAT_FORMAT_H
