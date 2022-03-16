#pragma once


#include <stdexcept>
#include <string>
#include <sstream>
#include <iomanip>

namespace NES {

struct NesError : std::runtime_error {
    using std::runtime_error::runtime_error;
    static std::string to_hex8(uint8_t v) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(2) << v;
        return ss.str();
    }
    static std::string to_hex_addr(uint16_t addr) {
        std::stringstream ss;
        ss << "$" << std::hex << std::setfill('0') << std::setw(4) << addr;
        return ss.str();
    }
};

struct CartridgeError : NesError {
    [[nodiscard]] explicit CartridgeError(const std::string &msg) : NesError(msg) {}
};

struct InvalidMapperConfigurationError : CartridgeError {
    [[nodiscard]] InvalidMapperConfigurationError()
        : CartridgeError("Invalid mapper configuration"){};
};

struct UnableToOpenFileError : CartridgeError {
    [[nodiscard]] explicit UnableToOpenFileError(const std::string &filename)
        : CartridgeError("Unable to open file '" + filename + "'") {}
};

struct InvalidHeaderFormatError : CartridgeError {
    [[nodiscard]] InvalidHeaderFormatError()
        : CartridgeError("Header format doesn't match iNES format") {}
};

struct UnknownMapperTypeError : CartridgeError {
    [[nodiscard]] explicit UnknownMapperTypeError(uint16_t type)
        : CartridgeError("Unknown mapper type: " + std::to_string(type)) {}
};

struct AddressOutOfBoundsError : CartridgeError {
    [[nodiscard]] explicit AddressOutOfBoundsError(uint16_t address, const std::string &type)
        : CartridgeError("Address '" + to_hex_addr(address) + "' can't be mapped to " + type +
                         " address space") {}
};

struct WritingToRomError : CartridgeError {
    [[nodiscard]] WritingToRomError() : CartridgeError("Writing to ROM is denied") {}
};

struct ControllerWriteError : NesError {
    [[nodiscard]] ControllerWriteError(uint16_t addr, uint8_t value)
        : NesError(std::string("Invalid Controller write: address: ") + to_hex_addr(addr) +
                             ", value: " + to_hex8(value)) {}
};

}  // namespace NES
