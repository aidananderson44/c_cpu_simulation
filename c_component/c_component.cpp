#include "c_component.h"
using namespace std;
c_component::c_component() : c_component(default_args){}
c_component::c_component(vector<string> source_names,
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
void c_component::update_i()
{
    for(auto &attr_name : this->source_names)
    {
    //    cout << this->s[attr_name] << endl;
        int *target = this->s[attr_name + "_val_i"];
        int *source = this->s[attr_name];
        *target = *source; 
    }  
}
void c_component::update()
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