#include<vector>
#include<unordered_map>
#include<string>
#include<iostream>
#include<tuple>
#include <cstdio>
#include <ctime>
#include <math.h>

using namespace std;
#define component_contructor(inherited, base, ...) inherited(__VA_ARGS__  vector<string> source_names = {},\
                                            vector<int> source_widths = {},\
                                            vector<string> port_names = {},\
                                            vector<int> port_args = {},\
                                            int num_ports = 1,\
                                            vector<int> port_widths = {},\
                                            int width = 8) : base(source_names, source_widths, port_names, port_args, num_ports, port_widths, width)

#define binary_op_class(name, op)   class name : public binary_op\
                                    {\
                                    public:\
                                        component_contructor(name, binary_op) {}\
                                        int binary_fun(int a, int b) {return a op b;}\
                                    }

#define w_mask(n) ((1 << n) - 1)
#define NUM_THREADS 4
#define default_args {},{},{},{},1,{},8
int ZERO = 0;
typedef unordered_map<string, void *> cpu_dict; 


class component
{
public:
    component() : component(default_args){}
    component(vector<string> source_names,
            vector<int> source_widths = {},
            vector<string> port_names = {},
            vector<int> port_args = {},
            int num_ports = 1,
            vector<int> port_widths = {},
            int width = 8)
    { 
        this->width = width;
        if(!source_names.size())
            this->source_names = vector<string>({"source"}); 
        else    
            this->source_names = vector<string>(source_names);
        if(!source_widths.size())
            this->source_widths = vector<int>(this->source_names.size(), this->width);
        else
            this->source_widths = vector<int>(source_widths);
        
        
        this->sources = vector<int *>(this->source_names.size(), NULL);

        this->sources_val = vector<int>(this->sources.size(), 0);
        this->sources_val_i = vector<int>(this->sources.size(), 0);
        
        this->s = unordered_map<string, int *>();
        for(unsigned int i = 0; i < this->source_names.size(); i++)
        {
            (this->s)[(this->source_names)[i]] = (this->sources[i]);
            (this->s)[(this->source_names)[i] + "_val"] = &(this->sources_val[i]);
            (this->s)[(this->source_names)[i] + "_val_i"] = &(this->sources_val_i[i]);
        }

        if(port_names.size())
        {

            this->port_names = vector<string>(port_names);
            this->port_args = vector<int>(this->port_names.size());
            this->num_ports = this->port_names.size();
            for(unsigned int i = 0; i < this->port_names.size(); i++)
                ((this->port_args))[i] = i;            
        }
        else if(port_args.size())
        {
            
            this->port_args = vector<int>(port_args);
            this->port_names = vector<string>(this->port_names.size());
            this->num_ports = this->port_names.size();
            for(unsigned int i = 0; i < this->port_args.size(); i++)
                ((this->port_names))[i] = to_string(i);
        }
        else
        {
            
            this->num_ports = num_ports;

            this->port_names = vector<string>(num_ports);
            this->port_args =  vector<int>(num_ports);
            for(int i = 0; i < this->num_ports; i++)
            {
                ((this->port_names))[i] = to_string(i);
                ((this->port_args))[i] = i;
            }

        }

        if(!port_widths.size())    
            this->port_widths = vector<int>(this->num_ports, 0);
        else
            this->port_widths = vector<int>(port_widths);
            
        
        if(this->port_names.size() != this->port_widths.size())
            throw "Invalid port_widths";
        
        for(int i = 0; i < this->num_ports; i++)
        {
            string name = (this->port_names)[i];
            this->port[name] = 0;
        }     
    }
    void update_i()
    {

        for(auto &attr_name : this->source_names)
        {
        //    cout << this->s[attr_name] << endl;
            int *target = this->s[attr_name + "_val_i"];
            int *source = this->s[attr_name];
            *target = *source; 
        }
    }

    virtual void update()
    {
        int i = 0;
        for(auto &attr_name : this->source_names)
        {
            int *target = this->s[attr_name + "_val"];
            int *source = this->s[attr_name + "_val_i"];
            *target = (*source ) & w_mask(this->source_widths[i]);
            
            i++;
        }
    }
    virtual void cycle() {};
    unordered_map<string, int *> s;
    vector<string> source_names;
    vector<string> port_names;
    unordered_map<string, int> port;
protected:
    vector<int> port_widths;
    int width = 8;
    vector<int *> sources;
    vector<int> sources_val;
    vector<int> sources_val_i;
    int num_ports = 1;
    vector<int> source_widths;
    vector<int> port_args;
    
};

class wire : public component
{
public:
    component_contructor(wire, component, int *source, int length = 1,)
    {

        this->s[this->source_names[0]] = source;
        this->length = length;
        w = vector<int>(this->length);
    }
    wire() : wire(NULL, 1, default_args){}
    virtual void update()
    {
        component::update();
       // cout << &w << endl;
        for(int i = 0; i < this->length - 1; i ++)
            this->w[i] = this->w[i + 1];
        this->w[this->length - 1] = *(this->s[this->source_names[0] + "_val"]);
        for(auto &port_name : this->port_names)
        {    
            (this->port[port_name]) = w[0];
        }
    }
    vector<int> w;
    int length;

};

class control : public component
{
public:
    component_contructor(control, component, unordered_map<int, int> lookup_table,)
    {
        this->lookup_table = unordered_map<int, int>(lookup_table);
    }
    virtual void update()
    {
        component::update();
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
    unordered_map<int, int> lookup_table;
};

class splitter : public component
{
public:
    component_contructor(splitter, component) {}
    void update()
    {
        component::update();
        
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
};

class Register : public component
{
public:
    component_contructor(Register, component)
    {
        this->pc = 0;
    }
    void update_i(){}
    void update()
    {
        for(int i = 0; i < this->num_ports; i++)
        {
            string port = this->port_names[i];
            this->port[port] = pc;
        }
    }
    void cycle()
    {
        this->pc = *(this->s[this->source_names[0]]) & w_mask(this->width);
    }
    int pc;
};
class demux : public component
{
public:
    component_contructor(demux, component) {}
    void update()
    {
        component::update();
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
};
class binary_op : public component
{
public:
    component_contructor(binary_op, component) {}
    virtual int binary_fun(int, int) = 0;
    void update()
    {
        component::update();
        int s1 = *(this->s[this->source_names[0]]);
        int s2 = *(this->s[this->source_names[1]]);
        for(int i = 0; i < this->num_ports; i++)
        {
            string port = this->port_names[i];
            this->port[port] = this->binary_fun(s1, s2);
        }
    }
};
binary_op_class(add, +);
binary_op_class(sub, -);
binary_op_class(mul, *);
binary_op_class(div, /);
binary_op_class(Or, |);
binary_op_class(And, &);
binary_op_class(Xor, ^);
binary_op_class(mod, %);


class memory : public component
{
public:
    component_contructor(memory, component, 
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
    void cycle()
    {
        int addr = *(this->s["write_addr_s"]);
        int val = *(this->s["write_val_s"]);
        if(addr == 0) return;
        this->mem[addr] = val;
    }
    void update()
    {
        component::update();
        for(int i = 0; i < this->num_ports; i++)
        {
            string port = this->port_names[i];
            int s = *(this->s[this->source_names[i] + "_val"]);
            this->port[port] = this->mem[s];
        }
    }
    vector<int> mem;
};
class ALU : public component
{
public:
    component_contructor(ALU, component){}
    void update()
    {
        component::update();
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
};
