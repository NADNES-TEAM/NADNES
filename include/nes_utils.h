#pragma once
#include<QObject>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
namespace utils {

std::string to_hex8(uint8_t v);
std::string to_hex_addr(uint16_t addr);

template <typename T>
void free_qptr(T **ptr) {
    if(*ptr != nullptr) {
        (*ptr)->close();
        (*ptr)->deleteLater();
        (*ptr) = nullptr;
    }
}
}
