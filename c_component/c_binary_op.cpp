#include "c_binary_op.h"
using namespace std;
void c_binary_op::update()
{
    c_binary_op::update();
    int s1 = *(this->s[this->source_names[0]]);
    int s2 = *(this->s[this->source_names[1]]);
    for(int i = 0; i < this->num_ports; i++)
    {
        string port = this->port_names[i];
        this->port[port] = this->binary_fun(s1, s2);
    }
}