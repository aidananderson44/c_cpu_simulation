#ifndef DEMUX_HEADER
#define DEMUX_HEADER
#include "c_component.h"
class c_demux : public c_component
{
public:
    c_demux(std::vector<std::string> source_names,
        std::vector<int> source_widths,
        std::vector<std::string> port_names,
        std::vector<int> port_args,
        int num_ports,
        std::vector<int> port_widths,
        int width);
    void update();
};

#endif