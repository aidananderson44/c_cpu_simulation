#ifndef SPLITTER_HEADER
#define SPLITTER_HEADER
#include "c_component.h"
class c_splitter : public c_component
{
public:
    c_splitter(std::vector<std::string> source_names,
        std::vector<int> source_widths,
        std::vector<std::string> port_names,
        std::vector<int> port_args,
        int num_ports,
        std::vector<int> port_widths,
        int width );
    void update();
};

#endif