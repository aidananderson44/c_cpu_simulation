
from Cython.Build import cythonize
from setuptools import setup, find_packages, Extension

extensions = [
    Extension(
        'c_cpu_component',
        ['component_wrapper.pyx'],
        extra_compile_args=['-std=c++11']
    )
]

setup(name = 'component_wrapper', 
      ext_modules = cythonize(extensions))