from libcpp.vector cimport vector
from libcpp.pair cimport pair
from libcpp.unordered_map cimport unordered_map
from libcpp.string cimport string
from libcpp cimport bool
cdef extern from "c_component/c_component.cpp":
    pass
cdef extern from "c_component/c_component.h":
    cdef cppclass c_component:
        c_component(vector[string] source_names,
                    vector[int] source_widths,
                    vector[string] port_names,
                    vector[int] port_args,
                    int num_ports,
                    vector[int] port_widths,
                    int width)
        void update_i()
        void update()
        void cycle()
        unordered_map[string, int *] s
        vector[string] source_names
        vector[string] port_names
        unordered_map[string, int] port
        vector[int] port_args;
        vector[int] port_widths;
        int num_ports;
        vector[int] source_widths;
        
cdef extern from "c_component/c_wire.cpp":
    pass
cdef extern from "c_component/c_wire.h":
    cdef cppclass c_wire(c_component):
        c_wire(int *source,
            int length, 
            vector[string] source_names,
            vector[int] source_widths,
            vector[string] port_names,
            vector[int] port_args,
            int num_ports,
            vector[int] port_widths,
            int width)
cdef extern from "c_component/c_control.cpp":
    pass
cdef extern from "c_component/c_control.h":
    cdef cppclass c_control(c_component):
        c_control(unordered_map[int, int] lookup_table,
                vector[pair[int, int] ] port_ranges,
                int table_len,
                vector[string] source_names,
                vector[int] source_widths,
                vector[string] port_names,
                vector[int] port_args,
                int num_ports,
                vector[int] port_widths,
                int width)
        unordered_map[int, int] lookup_table
        vector[pair[int, int] ] port_ranges
cdef extern from "c_component/c_splitter.cpp":
    pass
cdef extern from "c_component/c_splitter.h":
    cdef cppclass c_splitter(c_component):
        c_splitter(vector[pair[int, int] ] port_ranges,
                    vector[string] source_names,
                    vector[int] source_widths,
                    vector[string] port_names,
                    vector[int] port_args,
                    int num_ports,
                    vector[int] port_widths,
                    int width)
        vector[pair[int, int] ] port_ranges
cdef extern from "c_component/c_register.cpp":
    pass
cdef extern from "c_component/c_register.h":
    cdef cppclass c_register(c_component):
        c_register(vector[string] source_names,
                    vector[int] source_widths,
                    vector[string] port_names,
                    vector[int] port_args,
                    int num_ports,
                    vector[int] port_widths,
                    int width)
cdef extern from "c_component/c_demux.cpp":
    pass
cdef extern from "c_component/c_demux.h":                
    cdef cppclass c_demux(c_component):
        c_demux(vector[string] source_names,
                    vector[int] source_widths,
                    vector[string] port_names,
                    vector[int] port_args,
                    int num_ports,
                    vector[int] port_widths,
                    int width)
cdef extern from "c_component/c_binary_op.cpp":
    pass
cdef extern from "c_component/c_binary_op.h": 
    cdef cppclass c_add(c_component):
        c_add(vector[string] source_names,
                    vector[int] source_widths,
                    vector[string] port_names,
                    vector[int] port_args,
                    int num_ports,
                    vector[int] port_widths,
                    int width)
cdef extern from "c_component/c_memory.cpp":
    pass
cdef extern from "c_component/c_memory.h": 
    cdef cppclass c_memory(c_component):
        c_memory(int * write_addr_s,
                    int *write_val_s,
                    int word_len,
                    int addr_len,
                    bool randomized,
                    vector[string] source_names,
                    vector[int] source_widths,
                    vector[string] port_names,
                    vector[int] port_args,
                    int num_ports,
                    vector[int] port_widths,
                    int width)
        void load_memory(vector[int], int)
        vector[int] mem
cdef extern from "c_component/c_ALU.cpp":
    pass
cdef extern from "c_component/c_ALU.h": 
    cdef cppclass c_ALU(c_component):
        c_ALU(vector[string] source_names,
                    vector[int] source_widths,
                    vector[string] port_names,
                    vector[int] port_args,
                    int num_ports,
                    vector[int] port_widths,
                    int width)

