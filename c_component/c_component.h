#ifndef COMPONENT_HEADER
#define COMPONENT_HEADER
#define component_contructor(inherited, base, ...) inherited(__VA_ARGS__  std::vector<std::string> source_names = {},\
                                            std::vector<int> source_widths = {},\
                                            std::vector<std::string> port_names = {},\
                                            std::vector<int> port_args = {},\
                                            int num_ports = 1,\
                                            std::vector<int> port_widths = {},\
                                            int width = 8) : base(source_names, source_widths, port_names, port_args, num_ports, port_widths, width)
#define w_mask(n) ((1 << n) - 1)
#define default_args {},{},{},{},1,{},8
#include<vector>
#include<unordered_map>
#include<string>
class c_component
{
public:
    c_component(std::vector<std::string> source_names,
        std::vector<int> source_widths,
        std::vector<std::string> port_names,
        std::vector<int> port_args,
        int num_ports,
        std::vector<int> port_widths,
        int width);
    c_component();
    ~c_component(){}
    virtual void update_i();
    virtual void update();
    virtual void cycle() {};

    std::unordered_map<std::string, int *> s;
    std::vector<std::string> source_names;
    std::vector<std::string> port_names;
    std::unordered_map<std::string, int> port;
    std::vector<int> port_args;
    std::vector<int> port_widths;
    int num_ports = 1;
    std::vector<int> source_widths;
protected:
    
    int width = 8;
    std::vector<int *> sources;
    std::vector<int> sources_val;
    std::vector<int> sources_val_i;

    
};


#endif