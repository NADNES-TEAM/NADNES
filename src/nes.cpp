#include "nes.h"
#include <fstream>
#include "nes_exceptions.h"
#include <iostream>
#include "reset_token.h"
namespace NES {

void Nes::tick() {
    bool frame_rendered = false;
    int64_t frame_counter = 0;
    while (!frame_rendered) {
        if (frame_counter % 3 == 0) {
            cpu.tick(frame_counter % 2 == 0);
        }
        frame_rendered = ppu.tick();
        frame_counter += 1;
    };
}

Nes::Nes(const std::string &filename,
         ScreenInterface *screen_,
         KeyboardInterface *keyboard_1_,
         KeyboardInterface *keyboard_2_)
    : cartridge(filename), bus() {
    ppu.connect(screen_, ConnectToken());
    ppu.connect(&cartridge, ConnectToken());
    ppu.connect(&cpu, ConnectToken());

    bus.connect(&ppu, ConnectToken());
    bus.connect(&cartridge, ConnectToken());
    bus.connect(&controller, ConnectToken());

    controller.connect_to_port1(keyboard_1_, ConnectToken());
    controller.connect_to_port2(keyboard_2_, ConnectToken());

    cpu.connect(&bus, ConnectToken());
    cpu.reset(ResetToken());
}

void Nes::reset() {
    ppu.reset(ResetToken());
    cartridge.reset(ResetToken());
    cpu.reset(ResetToken());
}

namespace {
void print_counter(uint8_t *data, size_t len) {
    std::unordered_map<uint8_t, int> mp;
    for (int i = 0; i < len; ++i) {
        mp[data[i]]++;
    }
    std::cout << "{\n";
    for (const auto &p : mp) {
        std::cout << "\t" << p.first << " " << p.second << "\n";
    }
    std::cout << "}\n";
}
}

std::vector<Cheating::ResultRaw> Nes::search(
    const Cheating::ParamsOfSearch &params,
    const std::vector<Cheating::ResultRaw> &old_result_data) {
    Cheating::Place::rom_size = cartridge.get_ROM_size();
    Cheating::Place::ram_mem = bus.get_RAM();
    Cheating::Place::rom_mem = cartridge.get_ROM();
//    print_counter(bus.get_RAM(), (1 << 11));
//    print_counter(cartridge.get_ROM(), cartridge.get_ROM_size());
    std::vector<Cheating::ResultRaw> new_result_data;
    if (params.is_initial) {
        for (int where = 0; where < 2; ++where) {
            if (params.place.id != Cheating::Place::RAM_AND_ROM.id && params.place.id != where) {
                continue;
            }
            for (size_t i = 0; i <= Cheating::Place{where}.get_size() - size_t(params.byteCount);
                 i++) {
                auto p_l = Cheating::Place{where}.get_mem() + i;
                long long value = Cheating::ResultRaw::get_value(p_l, params.byteCount);
                Cheating::ResultRaw raw = {Cheating::Place{where}, i, value, value};
                if (params.check_coincidence(raw)) {
                    new_result_data.push_back(raw);
                }
            }
        }
    } else {
        for (auto raw : old_result_data) {
            raw.old_value = raw.cur_value;
            raw.cur_value = Cheating::ResultRaw::get_value(raw.place.get_mem() + raw.address, params.byteCount);
            if (params.check_coincidence(raw)) {
                new_result_data.push_back(raw);
            }
        }
    }
    return new_result_data;
}
bool Nes::change_memory_value(const Cheating::ParamsOfChange &params) {
    uint8_t *memory_p = params.place.get_mem();
    size_t size_of_memory = params.place.get_size();
    if (params.index + size_t(params.byteCount) > size_of_memory)
        return false;
    for (size_t i = 0, d = params.data_in; i < size_t(params.byteCount); i++) {
        *(memory_p + params.index + i) = d % (1 << 8);
        d >>= 8;
    }
    return true;
}

void Nes::save(const std::string &filename) {
    auto file = std::ofstream(filename, std::ios::binary);
    if (!file) {
        throw UnableToOpenFileError(filename);
    }
    cpu.save(file);
    ppu.save(file);
    cartridge.save(file);
    bus.save(file);
    file.close();
}

void Nes::load(const std::string &filename) {
    auto file = std::ifstream(filename, std::ios::binary);
    if (!file) {
        throw UnableToOpenFileError(filename);
    }
    cpu.load(file);
    ppu.load(file);
    cartridge.load(file);
    bus.load(file);
    file.close();
}

}  // namespace NES
