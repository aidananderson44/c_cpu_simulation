#ifndef MEMORY_HEADER
#define MEMORY_HEADER
#include "c_component.h"
class c_memory : public c_component
{
public:
    c_memory(int *write_addr_s, 
        int *write_val_s,
        int word_len,
        int addr_len,
        bool randomized,
        std::vector<std::string> source_names,
        std::vector<int> source_widths,
        std::vector<std::string> port_names,
        std::vector<int> port_args,
        int num_ports,
        std::vector<int> port_widths,
        int width);
    void cycle();
    void update();
    std::vector<int> mem;
private:
    int ZERO = 0;
};
#endif