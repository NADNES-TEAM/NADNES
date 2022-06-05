#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace NES::Cheating {

enum class Action {
    save,
    encrease,
    decrease,
    encrease_or_save,
    changed,
    decrease_or_save,
    all,
    eq_num,
    g_num,
    geq_num,
    l_num,
    leq_num
};

struct Place {
    int id{};  // 0 1 2
    static size_t rom_size;
    static uint8_t *ram_mem;
    static uint8_t *rom_mem;
    [[nodiscard]] size_t get_size() const;
    [[nodiscard]] uint8_t *get_mem() const;
    static Place RAM, ROM, RAM_AND_ROM;
};

enum class ByteCount : std::size_t { ONE = 1, TWO = 2 };
struct ResultRaw {
    Place place{};
    std::size_t address{};
    long long old_value{}, cur_value{};
    static long long get_value(uint8_t *first, ByteCount byteCount);
};

struct Params {
    Place place{};
    ByteCount byteCount{};
    long long data_in{};
};

struct ParamsOfSearch : Params {
    bool is_initial{true};
    Action event{};
    bool check_coincidence(ResultRaw &raw) const;
};

struct ParamsOfChange : Params {
    std::size_t index{};
};

}  // namespace NES::Cheating
