#ifndef BUS_H
#define BUS_H
namespace NES {
    struct bus_interface {

        virtual void mem_write(uint_16 addr, uint_8 data) = 0;

        virtual uint_8 mem_read(uint_16 addr) = 0;
    };

    class bus : bus_interface {

    };
}
#endif //BUS_H
