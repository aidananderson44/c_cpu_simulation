# distutils: language = c++
from component cimport *
from libcpp.unordered_map cimport unordered_map
cdef class int_star:
   cdef int *p
   cdef int v
   def __cinit__(self, int v = 0):
      self.v = v
      self.p = &self.v
   cdef set_p(self, int *val):
      self.p = val
   def __call__(self):
      return self.p[0]




cdef class component:
   cdef c_component *c_comp
   cdef int *port
   
   def init_args(self, source_names = [], source_widths = [],port_names = [],port_args = [],int num_ports = 1,port_widths = [], int width = 8):
      source_names = [s.encode('utf-8') for s in source_names]
      port_names = [s.encode('utf-8') for s in port_names]
      return (source_names, source_widths, port_names, port_args, num_ports, port_widths, width)

   def __init__(self, **kwargs):
      args = self.init_args(**kwargs)
      self.c_comp = new c_component(args[0], args[1], args[2], args[3], args[4], args[5], args[6])
   def update(self):
      self.c_comp[0].update()
   def update_i(self):
      self.c_comp[0].update_i()
   def __getattr__(self, name):
      if(name[:4] == 'port'):
         b_str = name[5:].encode('utf-8')
         if self.c_comp[0].port.find(b_str) == self.c_comp[0].port.end():
            raise AttributeError('Attribute {} does not exist'.format(name))
         self.port = &(self.c_comp[0].port[b_str])
         ps = int_star()
         ps.set_p(self.port)
         self.port = NULL
         return ps

      else:
         return self.__dict__[name]
   def __setattr__(self, name, value):
      
      for c_str in self.c_comp[0].source_names:
         if c_str.decode() == name:
            if(type(value) is not int_star):
               raise TypeError('Value must be int_star')  
            self.c_comp[0].s[c_str] = (<int_star>value).p
            break
      else:
         self.__dict__[name] = value
   def __dealloc__(self):
      del self.c_comp

cdef class wire(component):
   def __init__(self, int_star source, length = 1, **kwargs):
      args = self.init_args(**kwargs)
      self.c_comp = new c_wire(source.p, length, args[0], args[1], args[2], args[3], args[4], args[5], args[6])

cdef class control(component):
   cdef unordered_map[int, int] c_lookup
   def __init__(self, dict lookup_table, **kwargs):
      args = self.init_args(**kwargs)
      for k in lookup_table:
         self.c_lookup[k] = lookup_table[k]
      self.c_comp = new c_control(self.c_lookup, args[0], args[1], args[2], args[3], args[4], args[5], args[6])

cdef class splitter(component):
   def __init__(self, **kwargs):
      args = self.init_args(**kwargs)
      self.c_comp = new c_splitter(args[0], args[1], args[2], args[3], args[4], args[5], args[6])

cdef class register(component):
   def __init__(self, **kwargs):
      args = self.init_args(**kwargs)
      self.c_comp = new c_register(args[0], args[1], args[2], args[3], args[4], args[5], args[6])

cdef class demux(component):
   def __init__(self, **kwargs):
      args = self.init_args(**kwargs)
      self.c_comp = new c_demux(args[0], args[1], args[2], args[3], args[4], args[5], args[6])

cdef class add(component):
   def __init__(self, **kwargs):
      args = self.init_args(**kwargs)
      self.c_comp = new c_add(args[0], args[1], args[2], args[3], args[4], args[5], args[6])

cdef class memory(component):
   def __init__(self,int_star write_addr_s = int_star(0),
                     int_star write_val_s = int_star(0),
                     int word_len = 8,
                     int addr_len = 8,
                     bool randomized = False,
                     **kwargs):
      args = self.init_args(**kwargs)
      self.c_comp = new c_memory(write_addr_s.p,write_val_s.p, word_len, randomized, addr_len, args[0], args[1], args[2], args[3], args[4], args[5], args[6])

def my_fun():
   
   
   cdef int length = 5
   port_star = int_star(3)
   port_star2 = int_star(4)
   


   w = wire(length = length, source = port_star)

   w2 = wire(source = w.port_0, length = length, port_names = ["new_port"])
   before = w.port_0()
   
   for i in range(length*2):
      w.update_i()
      w.update()
      w2.update_i()
      w2.update()
      after = w.port_0()
      after2 = w2.port_new_port()
      print('before', before, 'after', after, 'after2', after2)
