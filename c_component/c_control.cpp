#include "c_control.h"
#include <iostream>
using namespace std;

component_contructor(c_control::c_control, c_component, 
                    unordered_map<int, int> lookup_table,
                    std::vector<std::pair<int, int> > port_ranges,
                    int table_len,)
{
    this->lookup_table = unordered_map<int, int>(lookup_table);
    this->port_ranges = std::vector<std::pair<int, int> >(port_ranges);
    this->table_len = table_len;
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



    for(int i = 0; i < this->num_ports; i++)
    {
        string port = this->port_names[i];
        std::pair<int, int> range = this->port_ranges[i];
        int lower = this->table_len - range.first;
        int upper = this->table_len - range.second;
        int val = lt_val;
        val >>= upper;
        val = val & w_mask(lower - upper);
        this->port[port] = val;
    }


}