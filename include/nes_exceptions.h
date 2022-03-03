#ifndef NADNES_CARTRIDGE_EXCEPTIONS_H
#define NADNES_CARTRIDGE_EXCEPTIONS_H

#include <stdexcept>
#include <string>


namespace NES {
    struct InvalidMapperConfiguration : std::runtime_error {
        InvalidMapperConfiguration() : std::runtime_error("Invalid mapper configuration") {};
    };

    struct UnableToOpenFile : std::runtime_error {
        explicit UnableToOpenFile(const std::string &filename)
                : runtime_error("Unable to open file '" + filename + "'") {}
    };

    struct InvalidHeaderFormat : std::runtime_error {
        InvalidHeaderFormat()
                : std::runtime_error("Header format doesn't match iNES format") {
        }
    };

    struct UnknownMapperType : std::runtime_error {
        explicit UnknownMapperType(uint8_t type)
                : std::runtime_error("Unknown mapper type: " + std::to_string(type)) {
        }
    };


    struct AddressOutOfBounds : std::runtime_error {
        explicit AddressOutOfBounds(uint16_t address, const std::string &type)
                : std::runtime_error("Address '" + std::to_string(address) + "' can't be mapped to " + type + "_ROM") {
        }
    };
}

#endif //NADNES_CARTRIDGE_EXCEPTIONS_H
