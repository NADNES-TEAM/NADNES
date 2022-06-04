#pragma once

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include "utils.h"
namespace NES {

struct NesError : std::runtime_error {
    using std::runtime_error::runtime_error;
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
        : CartridgeError("Address '" + Utils::to_hex_addr(address) + "' can't be mapped to " + type +
                         " address space") {}
};

struct WritingToRomError : CartridgeError {
    [[nodiscard]] WritingToRomError() : CartridgeError("Writing to ROM is denied") {}
};

struct ControllerWriteError : NesError {
    [[nodiscard]] ControllerWriteError(uint16_t addr, uint8_t value)
        : NesError(std::string("Invalid Controller write: address: ") + Utils::to_hex_addr(addr) +
                   ", value: " + Utils::to_hex8(value)) {}
};

struct IncorrectOpcodeError : NesError {
    [[nodiscard]] explicit IncorrectOpcodeError(uint8_t opcode)
        : NesError("Incorrect opcode! Number: " + Utils::to_hex8(opcode)) {}
};

struct UninitializedController1InterfaceError : NesError {
    [[nodiscard]] UninitializedController1InterfaceError()
        : NesError("Uninitialized Controller1 interface!") {}
};

}  // namespace NES
