#include "c_register.h"
using namespace std;

component_contructor(c_register::c_register, c_component)
{
    this->pc = 0;
}

void c_register::update()
{
    for(int i = 0; i < this->num_ports; i++)
    {
        string port = this->port_names[i];
        this->port[port] = pc;
    }
}
void c_register::cycle()
{
    this->pc = *(this->s[this->source_names[0]]) & w_mask(this->width);
}