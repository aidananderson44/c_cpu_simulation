#ifndef CONTROL_HEADER
#define CONTROL_HEADER
#include "c_component.h"
class c_control : public c_component
{
public:
    c_control(std::unordered_map<int, int> lookup_table,
        std::vector<std::string> source_names,
        std::vector<int> source_widths,
        std::vector<std::string> port_names,
        std::vector<int> port_args,
        int num_ports,
        std::vector<int> port_widths,
        int width );
    void update();
protected:
    std::unordered_map<int, int> lookup_table;
};
#endif