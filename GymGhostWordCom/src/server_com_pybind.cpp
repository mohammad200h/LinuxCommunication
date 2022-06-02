#include "Com/Com.h"

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/complex.h>
#include <pybind11/stl.h>
#include<map>
#include <pybind11/embed.h> // <= You need this header
//https://stackoverflow.com/questions/57021752/how-to-manipulate-pybind11dict-in-c
using namespace std;
namespace py=pybind11;



PYBIND11_MODULE(pyserverMQ,handle){
    handle.doc() = "This is Server_MQ for python";
    
    py::class_<Server_MQ>(handle,"serverMQ")
        .def(py::init<>())
        .def("init",&Server_MQ::init)

        .def("run",&Server_MQ::create_run_thread) 
  
        .def("getClientsInQueue",[](Server_MQ &self){
            vector<string> ids_str;
            vector <boost::uuids::uuid> ids = self.getClientsInQueue();
           for(boost::uuids::uuid id:ids){
               ids_str.push_back(uuid_s(id));
           }
            return ids_str;
        }) 
 
        .def("getGymStateforId",[](Server_MQ &self , string id_str){
            boost::uuids::uuid id  =boost::lexical_cast<boost::uuids::uuid >(id_str);
            return self.getGymStateforId(id);
        })


        .def("removeStateFromBuffer",[](Server_MQ &self,string id_str){
            boost::uuids::uuid id =  boost::lexical_cast<boost::uuids::uuid >(id_str);
            self.removeStateFromBuffer(id);
        })
     
        .def("remove_client_from_queue",[](Server_MQ &self,string id_str){
            boost::uuids::uuid id =  boost::lexical_cast<boost::uuids::uuid >(id_str);
            self.remove_client_from_queue(id);
        })
        .def("send_using_id",[](Server_MQ &self,string id_str,GhostWorldState ghostState){

            boost::uuids::uuid id =  boost::lexical_cast<boost::uuids::uuid >(id_str);
            return self.send(id,ghostState);
        })
      

    ;
    
    //********Dfining data type***********
    py::class_<GymworldState> gym_ws(handle,"GymworldState");
        gym_ws.def(py::init<>())
        // .def_readwrite("key", &GymworldState::key)
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

//******************casting*****************
//https://github.com/pybind/pybind11/issues/174
