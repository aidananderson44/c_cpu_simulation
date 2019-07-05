#ifndef WIRE_HEADER
#define WIRE_HEADER
#include "c_component.h"
class c_wire : public c_component
{
public:
    c_wire(int *source, 
        int length,
        std::vector<std::string> source_names,
        std::vector<int> source_widths,
        std::vector<std::string> port_names,
        std::vector<int> port_args,
        int num_ports,
        std::vector<int> port_widths,
        int width);
    ~c_wire(){}
    c_wire();
    void update();

private:
    std::vector<int> w;
    int length;
};

#endif