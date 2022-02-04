#ifndef BUS_H
#define BUS_H
namespace NES {
    class bus_interface {

        void mem_write(uint_16 addr, uint_8 data);

        uint_8 mem_read(uint_16 addr);
    };

    class bus : bus_interface {

    };
}
#endif //BUS_H
