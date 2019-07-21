# distutils: language = c++
from component_wrapper cimport *
from libcpp.unordered_map cimport unordered_map
import numpy as np
def to_dec(b):
    width = len(b)
    return int(np.sum(b * 2**((width - 1) - np.arange(width) )))
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
   
   def init_args(self, source_names = [], source_widths = [],port_names = [],port_args = [],int num_ports = 1,port_widths = [], int width = 8, **kwargs):
      source_names = [s.encode('utf-8') for s in source_names]
      port_names = [s.encode('utf-8') for s in port_names]
      return [source_names, source_widths, port_names, port_args, num_ports, port_widths, width]

   def __init__(self, **kwargs):
      args = self.init_args(**kwargs)
      self.c_comp = new c_component(args[0], args[1], args[2], args[3], args[4], args[5], args[6])
   cpdef update(self):
      self.c_comp[0].update()
   cpdef update_i(self):
      self.c_comp[0].update_i()
   cpdef cycle(self):
      self.c_comp.cycle()
   def __getattr__(self, name):
      for c_str in self.c_comp.port_names:
         if c_str.decode() == name:   
            self.port = &(self.c_comp[0].port[c_str])
            ps = int_star()
            ps.set_p(self.port)
            self.port = NULL
            return ps
      else:
         raise AttributeError('Attribute {} does not exist'.format(name))
       
   @property
   def source_names(self):
      return [s.decode() for s in self.c_comp.source_names]
   @property
   def port_names(self):
      return [s.decode() for s in self.c_comp.port_names]
   @property
   def port_args(self):
      return list (self.c_comp.port_args)
   @property
   def port_widths(self):
      return list(self.c_comp.port_widths)
   @property
   def source_widths(self):
      return list(self.c_comp.source_widths)
   @property
   def num_ports(self):
      return self.c_comp.num_ports

   def __setattr__(self, name, value):
      for c_str in self.c_comp[0].source_names:
         if c_str.decode() == name:
            if(type(value) is not int_star):
               raise TypeError('Value must be int_star')  
            self.c_comp[0].s[c_str] = (<int_star>value).p
            break
      else:
         if(name[:4] == 'port'):
            b_str = name[5:].encode('utf-8')
            if self.c_comp[0].port.find(b_str) != self.c_comp[0].port.end():   
               raise AttributeError('Cannot set attribute {}'.format(name))
         else:
            raise AttributeError('Attribute {} does not exist'.format(name))
   def __dealloc__(self):
      del self.c_comp

cdef class wire(component):
   def __init__(self, int_star source = int_star(), length = 1, **kwargs):
      args = self.init_args(**kwargs)

      self.c_comp = new c_wire(source.p, length, args[0], args[1], args[2], args[3], args[4], args[5], args[6])

cdef class control(component):
   cdef unordered_map[int, int] c_lookup
   def __init__(self, dict lookup_table, port_ranges = None, table_len = 8, **kwargs):
      args = self.init_args(**kwargs)
      for k in lookup_table:
         if isinstance(k, int):
            key = k
         elif isinstance(eval(k, {}, {}), list) and all(isinstance(x, int) for x in eval(k, {}, {})):
            key = to_dec(eval(k, {}, {}))
         else:
            raise ValueError('Cannot interpret key {} as integer'.format(k))
         if isinstance(lookup_table[k], int):
            value = lookup_table[k]

         elif isinstance(lookup_table[k], list) and all(isinstance(x, list) for x in lookup_table[k]):
            value = []
            port_ranges = []
            last_upper = 0
            for i in lookup_table[k]:
               p_range = (last_upper, last_upper + len(i))
               last_upper += len(i)
               port_ranges += [p_range]
               if not all(isinstance(x, int) for x in i):
                  raise ValueError('Cannot interpret value {} as integer'.format(i))
               value += i
            table_len = last_upper
            value = to_dec(value)

         else:
            raise ValueError('Cannot interpret value {} as integer'.format(lookup_table[k]))

         self.c_lookup[key] = value
      args[4] = len(port_ranges)
      self.c_comp = new c_control(self.c_lookup, port_ranges, table_len, args[0], args[1], args[2], args[3], args[4], args[5], args[6])
   @property
   def lookup_table(self):
      return dict((<c_control *> self.c_comp).lookup_table)
   @property
   def port_ranges(self):
      return list((<c_splitter *>self.c_comp).port_ranges)

cdef class splitter(component):
   def __init__(self, **kwargs):
      args = self.init_args(**kwargs)
      self.c_comp = new c_splitter(kwargs['port_ranges'], args[0], args[1], args[2], args[3], args[4], args[5], args[6])
   @property
   def port_ranges(self):
      return list((<c_splitter *>self.c_comp).port_ranges)

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
   def load_memory(self, vals, start_addr = 0):
      (<c_memory *>self.c_comp).load_memory(vals, start_addr)



   @property
   def mem(self):
      return (<c_memory *> self.c_comp).mem


cdef class machine():
   def __init__(self, num_ticks):
      self.num_ticks = num_ticks
      self.component_attributes = None

   def update_components(self):
      for attr in dir(self): 
            f = getattr(self, attr)
            if issubclass(type(f), component):
               self.component_attributes += [f]

   cdef _pseudo_update(self, fun):
      if self.component_attributes:
         for attr in self.component_attributes:
            getattr(attr, fun)()

      else:
         self.component_attributes = [] 
         self.component_class = [] 

         for attr in dir(self): 
               f = getattr(self, attr)
               if issubclass(type(f), component):
                  getattr(f, fun)()
                  self.component_attributes += [f]
       
   cdef _update(self):
      
      self._pseudo_update('update_i')
      self._pseudo_update('update')
   
   def cycle(self):
      cdef int i
      for i in range(self.num_ticks):
         self._update()


      self._pseudo_update('cycle')
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
