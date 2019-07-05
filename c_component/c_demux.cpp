#include "c_demux.h"
using namespace std;

component_contructor(c_demux::c_demux, c_component) {}

void c_demux::update()
{
    c_component::update();
    int control = *(this->s["csource_val"]);
    int data = *(this->s["dsource_val"]);
    for(int i = 0; i < this->num_ports; i++)
    {
        string port = this->port_names[i];
        int arg = this-> port_args[i];
        if(control == arg)
            this->port[port] = data;
        else
            this->port[port] = 0;
            
    }
}

