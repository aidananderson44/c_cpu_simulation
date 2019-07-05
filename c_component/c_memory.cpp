#include "c_memory.h"
using namespace std;
component_contructor(c_memory::c_memory, c_component, 
                    int *write_addr_s = NULL, 
                    int *write_val_s = NULL,
                    int word_len = 8,
                    int addr_len = 8,
                    bool randomized = false,)
{
    if(!write_addr_s)
        this->s["write_addr_s"] = &ZERO;
    else
        this->s["write_addr_s"] = write_addr_s;
    
    if(!write_val_s)
        this->s["write_val_s"] = &ZERO;
    else
        this->s["write_val_s"] = write_val_s;
    this->mem = vector<int>(1 << addr_len, 0);

}
void c_memory::cycle()
{
    int addr = *(this->s["write_addr_s"]);
    int val = *(this->s["write_val_s"]);
    if(addr == 0) return;
    this->mem[addr] = val;
}
void c_memory::update()
{
    c_component::update();
    for(int i = 0; i < this->num_ports; i++)
    {
        string port = this->port_names[i];
        int s = *(this->s[this->source_names[i] + "_val"]);
        this->port[port] = this->mem[s];
    }
}