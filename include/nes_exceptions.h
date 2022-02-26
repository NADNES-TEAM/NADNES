#pragma once

#include <stdexcept>
#include <string>

namespace NES {

struct CartridgeError : std::runtime_error {
    [[nodiscard]] explicit CartridgeError(const std::string &msg) : std::runtime_error(msg) {}
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
        : CartridgeError("Address '" + std::to_string(address) + "' can't be mapped to " + type +
                         " address space") {}
};

struct WritingToRomError : CartridgeError {
    [[nodiscard]] WritingToRomError() : CartridgeError("Writing to ROM is denied") {}
};

}  // namespace NES
