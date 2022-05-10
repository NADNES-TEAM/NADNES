#include "nes.h"
#include "reset_token.h"
#include<iostream>
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

Nes::Nes(const std::string &filename, ScreenInterface *screen_, KeyboardInterface *keyboard_)
    : cartridge(filename), bus() {
    ppu.connect(screen_, ConnectToken());
    ppu.connect(&cartridge, ConnectToken());
    ppu.connect(&cpu, ConnectToken());

    bus.connect(&ppu, ConnectToken());
    bus.connect(&cartridge, ConnectToken());
    bus.connect(&controller, ConnectToken());

    controller.connect(keyboard_, ConnectToken());

    cpu.connect(&bus, ConnectToken());
    cpu.reset(ResetToken());
    search({{Search::Input::raw_bytes,Search::Place::RAM,{0}},1,Search::Action::all});
}

void Nes::reset() {
    ppu.reset(ResetToken());
    cpu.reset(ResetToken());
    std::vector<Search::ResultRaw> kek = search({{Search::Input::raw_bytes,Search::Place::RAM,{0}},0,Search::Action::encrease});
    std::cout<<"########################\n";
    for(int i=0;i<kek.size();i++){
        std::cout<<std::hex<<kek[i].address<<"   ";
        for(int j=0;j<kek[i].old_data.size();j++){
            std::cout<<std::fixed<<(int)kek[i].old_data[j];
        }
        std::cout<<"   ";
        for(int j=0;j<kek[i].cur_data.size();j++){
            std::cout<<std::fixed<<(int)kek[i].cur_data[j];
        }
        std::cout<<'\n';
    }
    std::cout<<kek.size();
}

void Nes::get_memory_p(uint8_t *&memory_p, size_t &size_of_memory, const Search::Params &params) {
    switch (params.place) {
        case Search::Place::RAM:
            memory_p = bus.get_RAM();
            size_of_memory = 1 << 11;
            break;
        case Search::Place::ROM:
            memory_p = cartridge.get_ROM();
            size_of_memory = cartridge.get_ROM_size();
            break;
    }
}

std::vector<Search::ResultRaw> Nes::search(const Search::ParamsOfSearch &params) {
    uint8_t *memory_p;
    size_t size_of_memory;
    get_memory_p(memory_p, size_of_memory, params);
    if (params.is_initial) {
        result_data.clear();
        size_t size_of_element = params.data_in.size();
        for (size_t i = 0; i < size_of_memory - size_of_element + 1; i++) {
            auto p_l = memory_p + i;
            auto p_r = p_l + size_of_element;
            std::vector<uint8_t> elem(p_l, p_r);
            Search::ResultRaw raw = {i, elem, elem};
            if (params.check_coincidence(raw)) {
                result_data.push_back(raw);
            }
        }
    } else {
        std::vector<Search::ResultRaw> temp_result_data;
        for (auto &raw : result_data) {
            raw.old_data = raw.cur_data;
            for (size_t i = 0; i < params.data_in.size(); i++) {
                raw.cur_data[i] = *(memory_p + raw.address + i);
            }
            if (params.check_coincidence(raw)) {
                temp_result_data.push_back(raw);
            }
        }
        result_data = temp_result_data;
    }
    return result_data;
}
bool Nes::change_memory_value(const Search::ParamsOfChange &params) {
    try {
        uint8_t *memory_p;
        size_t size_of_memory;
        get_memory_p(memory_p, size_of_memory, params);
        if (params.index + params.data_in.size() > size_of_memory)
            return false;
        for (size_t i = 0; i < params.data_in.size(); i++) {
            *(memory_p + params.index + i) = params.data_in[i];
        }
        return true;
    } catch (...) {
        return false;
    }
}

}  // namespace NES
