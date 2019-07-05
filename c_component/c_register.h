#ifndef REGISTER_HEADER
#define REGISTER_HEADER
#include "c_component.h"
class c_register : public c_component
{
public:
    c_register(std::vector<std::string> source_names,
        std::vector<int> source_widths,
        std::vector<std::string> port_names,
        std::vector<int> port_args,
        int num_ports,
        std::vector<int> port_widths,
        int width );
    void update_i(){}
    void update();
    void cycle();
private:
    int pc;
};

#endif