#include "c_ALU.h"
#include <math.h>
using namespace std;
component_contructor(c_ALU::c_ALU, c_component){}
void c_ALU::update()
{
    
    c_component::update();
    for(int i = 0; i < this->num_ports; i++)
    {
        string port = this->port_names[i];
        int arg = this->port_args[i];

        int control = *(this->s["csource_val"]);
        int control_extra = *(this->s["csource_extra_val"]);
        int acc = *(this->s["acc_source_val"]);
        int data = *(this->s["data_source_val"]);
        if(arg == 0)
        switch (arg)
        {
            case 0:
                switch (control_extra)
                {
                    case 0:
                        switch (control)
                        {
                            case 0:
                                this->port[port] = acc + data;
                                break;
                            case 1:
                                this->port[port] = acc - data;
                                break;
                            case 2:
                                this->port[port] = acc * data;
                                break;
                            case 3:
                                this->port[port] = acc % data;
                                break;
                            case 4:
                                if(data > 7)
                                    this->port[port] = acc >>(256 - data);
                                else
                                    this->port[port] = acc << data;
                                break;
                            case 5:
                                this->port[port] = acc & data;
                                break;
                            case 6:
                            case 7:
                                this->port[port] = acc;
                                break;                             
                        }
                        break;
                    case 1:
                        this->port[port] = ((int) acc / data);
                        break;
                    case 2:
                        this->port[port] = (int) pow(acc, ((double)data)/10.0);
                        break;
                    default:
                        printf("invalid control_extra");
                }

                break;
            case 1:
                if(control_extra)
                    this->port[port] = 0;
                else
                {
                    switch (control)
                    {
                        case 6:
                            if(data == 0)
                            {
                                this->port[port] = (acc==0);
                            }else if(data < 128)
                            {
                                this->port[port] = (acc <= 0xf && acc > 0);
                            }else if(data >= 128)
                            {
                                this->port[port] = (acc <= 0xff && acc > 0);
                            }
                            break;
                        case 7:
                            this->port[port] = data;
                            break;
                    }
                }
                break;
            default:
                printf("invalid control in ALU\n");
        }

    }
}