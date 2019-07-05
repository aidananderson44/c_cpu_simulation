#include "c_wire.h"
using namespace std;
component_contructor(c_wire::c_wire, c_component, int *source, int length = 1,)
{
    this->s[this->source_names[0]] = source;
    this->length = length;
    w = vector<int>(this->length);
}
c_wire::c_wire() : c_wire(NULL, 1, default_args){}
void c_wire::update()
{
    c_component::update();
    // cout << &w << endl;
    for(int i = 0; i < this->length - 1; i ++)
        this->w[i] = this->w[i + 1];
    this->w[this->length - 1] = *(this->s[this->source_names[0] + "_val"]);
    for(auto &port_name : this->port_names)
    {    
        (this->port[port_name]) = w[0];
    }
}