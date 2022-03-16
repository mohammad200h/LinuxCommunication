from distutils.core import setup, Extension

setup(name='ClientMQPkg', version='1.0',  \
      ext_modules=[Extension(name='client',sources=['./src/client_com_wrapper.cpp',"./src/msg.cpp",'./src/Com.cpp'],
                    include_dirs = ['/usr/local/include','./include/'],
                    library_dirs = ['/usr/local/lib'],
                    extra_compile_args=['-std=c++17'],
                    libraries=['stdc++','-lrt'],
                 
                            )
        
      
                  ]
    )