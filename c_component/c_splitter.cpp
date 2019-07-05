#include "c_splitter.h"
using namespace std;

component_contructor(c_splitter::c_splitter, c_component) {}
void c_splitter::update()
{
    c_component::update();
    
    int s_width = this->source_widths[0];
    int shift = s_width; 
    for(int i = 0; i < this->num_ports; i++)
    {
        string port = this->port_names[i];
        int port_width = this->port_args[i];
        shift -= port_width;
        int val = *(this->s[this->source_names[0] + "_val"]);
        val = val >> shift;
        val = val & w_mask(port_width);
        this->port[port] = val;
    }
}