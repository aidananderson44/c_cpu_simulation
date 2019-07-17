#include "c_splitter.h"
using namespace std;

component_contructor(c_splitter::c_splitter, c_component, std::vector<std::pair<int, int> > port_ranges,) 
{
    this->port_ranges = std::vector<std::pair<int, int> >(port_ranges);
}
void c_splitter::update()
{
    c_component::update();
    
    int source = 0;
    int shift = 0; 
    int s_width = 0;
    for(int i = this->source_names.size() -1; 0 <= i ; i--)
    {
        string source_name = this->source_names[i];
        int width = this->source_widths[i];
        s_width += width;
        int val = *(this->s[source_name + "_val"]);
        val = val << shift;
        source = source | val;
        shift += width;
    }

    for(int i = 0; i < this->num_ports; i++)
    {
        string port = this->port_names[i];
        std::pair<int, int> range = this->port_ranges[i];
        int lower = s_width - range.first;
        int upper = s_width - range.second;
        int val = source;
        val >>= upper;
        val = val & w_mask(lower - upper);
        this->port[port] = val;
    }


}