#include "c_control.h"
#include <iostream>
using namespace std;

component_contructor(c_control::c_control, c_component, unordered_map<int, int> lookup_table,)
{
    this->lookup_table = unordered_map<int, int>(lookup_table);
}

void c_control::update()
{
    c_component::update();
    int control_arg = 0;
    int shift = 0; 
    int s_width = 0;
    for(int i = this->source_names.size() -1; 0 <= i ; i--)
    {
        string source_name = this->source_names[i];
        int width = this->source_widths[i];
        s_width += width;
        int val = *(this->s[source_name + "_val"]);
        val = val << shift;
        control_arg = control_arg | val;
        shift += width;
    }

    auto it = this->lookup_table.find(control_arg);
    if(it == this->lookup_table.end())
        cout << "Control did not match" << endl;
    int lt_val = it->second;
    shift = s_width; 
    for(int i = 0; i < this->num_ports; i++)
    {
        string port = this->port_names[i];
        int port_width = this->port_args[i];
        shift -= port_width;
        int val = lt_val;
        val = val >> shift;
        val = val & w_mask(port_width);
        this->port[port] = val;
    }
}