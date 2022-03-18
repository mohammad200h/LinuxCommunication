#include "Com/Com.h"

#include <iostream>
#include <pybind11/pybind11.h>

namespace py=pybind11;



PYBIND11_MODULE(pyclientMQ,handle){
    handle.doc() = "This is client_MQ for python";
    
    py::class_<Client_MQ>(handle,"clientMQ")
        .def(py::init<>())
        .def("init",&Client_MQ::init)

        .def("run",&Client_MQ::create_run_thread)
        .def("send",&Client_MQ::send)
        .def("getGhostState",&Client_MQ::getGhostStateforClient)
    ;
    
    //********Dfining data type***********
    py::class_<GymworldState> gym_ws(handle,"GymworldState");
        gym_ws.def(py::init<>())
        .def_readwrite("ff", &GymworldState::ff) 
        .def_readwrite("mf", &GymworldState::mf) 
        .def_readwrite("rf", &GymworldState::rf) 
        .def_readwrite("th", &GymworldState::th) 
        .def_readwrite("cube", &GymworldState::cube)  
        ;



    py::class_<GhostWorldState> ghost_ws(handle,"GhostWorldState");
        ghost_ws.def(py::init<>())
        .def_readwrite("ff", &GhostWorldState::ff) 
        .def_readwrite("mf", &GhostWorldState::mf) 
        .def_readwrite("rf", &GhostWorldState::rf) 
        .def_readwrite("th", &GhostWorldState::th) 
   
        ;


    py::class_<objState> obj_state(gym_ws, "objState");
       obj_state.def(py::init<>())
       .def_readwrite("pos", &objState::pos)  
                 .def_readwrite("orn", &objState::orn)  
        ;

    py::class_<position> pos(obj_state,"position");
        pos.def(py::init<>())
        .def_readwrite("x", &position::x) 
           .def_readwrite("y", &position::y) 
           .def_readwrite("z", &position::z)  
        ;

    py::class_<orientation> orn(obj_state,"orientation");
        orn.def(py::init<>())
        .def_readwrite("r", &orientation::r) 
        .def_readwrite("p", &orientation::p) 
        .def_readwrite("y", &orientation::y)  
        ;


}