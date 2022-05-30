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

        .def("getKeyId",[](Server_MQ &self, string id_str){
            boost::uuids::uuid id  =boost::lexical_cast<boost::uuids::uuid >(id_str);
            service_keys key =  self.getKeyId(id);
            // py::scoped_interpreter guard{}; // <= Initialize the interpreter
            py::dict dict;
            dict["id"] = uuid_s(key.id);
            dict["request_timestep"] = key.request_timestep;
            dict["served_timestep"]  = key.served_timestep;
            return dict;
        })

        .def("removeStateFromBuffer",&Server_MQ::removeStateFromBuffer)
    

        .def("clean_after_serving_client",[](Server_MQ &self,string id_str){
             boost::uuids::uuid id =  boost::lexical_cast<boost::uuids::uuid >(id_str);
             self.clean_after_serving_client(id);

        })
 
        .def("send_using_id",[](Server_MQ &self,string id_str,GhostWorldState ghostState){

            boost::uuids::uuid id =  boost::lexical_cast<boost::uuids::uuid >(id_str);
            return self.send(id,ghostState);
        })
      
        .def("send_using_key",[](Server_MQ &self,py::dict key_dic,GhostWorldState ghostState){
            //https://stackoverflow.com/questions/67798290/how-to-access-pybind11dict-by-key
            service_keys service_key;
            for (std::pair<py::handle, py::handle> item : key_dic)
            {
                auto key = item.first.cast<std::string>();
                auto value = item.second.cast<string>();
                if (key =="id")service_key.id = boost::lexical_cast<boost::uuids::uuid >(value);
                if (key=="request_timestep") service_key.request_timestep= value;
                if (key=="served_timestep") service_key.served_timestep= value;
                
                
                cout << key << " : " << value<<endl;
              
            }
         
           return self.send(service_key,ghostState);
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

