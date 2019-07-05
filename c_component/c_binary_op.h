#ifndef BINARY_OP_HEADER
#define BINARY_OP_HEADER
#include "c_component.h"
#define binary_op_class(name, op)   class name : public c_binary_op\
                                    {\
                                    public:\
                                        component_contructor(name, c_binary_op) {}\
                                        int binary_fun(int a, int b) {return a op b;}\
                                    }
class c_binary_op : public c_component
{
public:
    c_binary_op(std::vector<std::string> source_names,
            std::vector<int> source_widths,
            std::vector<std::string> port_names,
            std::vector<int> port_args,
            int num_ports,
            std::vector<int> port_widths,
            int width ) {}
    virtual int binary_fun(int, int) = 0;
    void update();
};

binary_op_class(c_add, +);
binary_op_class(c_sub, -);
binary_op_class(c_mul, *);
binary_op_class(c_div, /);
binary_op_class(c_or, |);
binary_op_class(c_and, &);
binary_op_class(c_xor, ^);
binary_op_class(c_mod, %);

#endif