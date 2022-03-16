#include "Com/Com.h"
#include <iostream>
#include <Python.h>
#include <cstdio>
#define PY_SSIZE_T_CLEAN
typedef struct{
    PyObject_HEAD Client_MQ *ptrObj;
}PyClient;

static PyModuleDef clientmodule = {
    PyModuleDef_HEAD_INIT,
    "client",
    "Example module that wrapped a C++ object",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

static int constructor(PyClient *self,PyObject *args,PyObject *kwds){
    //intialize PyClient_MQ object


    self->ptrObj = new Client_MQ();

    return 0;
}
static void PyClient_MQ_dealloc(PyClient * self)
// destruct the object
{
    delete self->ptrObj;
    Py_TYPE(self)->tp_free(self);
}

static PyObject * PyClient_MQ_init(PyClient *self,PyObject *args){
    (self->ptrObj)->init();
}


static PyObject * PyClient_MQ_create_run_thread(PyClient *self,PyObject *args){
    (self->ptrObj)->create_run_thread();
}

static PyObject * PyClient_MQ_send(PyClient *self,PyObject *args){

    /********************************
    gymState will be represented by a dictionary in python
    it will be converted to struct 
    ********************************/
    PyObject *gymState_dict = NULL; //This is used for convertion from python dic to struct
    
    GymworldState gymState;//This input to C++ function
    bool sucess;//This is return value

    if (! PyArg_ParseTuple(args, "O!",&gymState_dict )){
         return NULL;
    }
    else{
        //parsing cube
        gymState.cube.pos.x = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "cube_pos_x"));
        gymState.cube.pos.y = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "cube_pos_y"));
        gymState.cube.pos.z = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "cube_pos_z"));
        gymState.cube.orn.r = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "cube_orn_x"));
        gymState.cube.orn.p = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "cube_orn_y"));
        gymState.cube.orn.y = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "cube_orn_z"));

        //parsing hand
        gymState.ff.pos.x = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "ff_pos_x"));
        gymState.ff.pos.y = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "ff_pos_y"));
        gymState.ff.pos.z = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "ff_pos_z"));
        gymState.ff.orn.r = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "ff_orn_x"));
        gymState.ff.orn.p = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "ff_orn_y"));
        gymState.ff.orn.y = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "ff_orn_z"));
        
        gymState.mf.pos.x = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "mf_pos_x"));
        gymState.mf.pos.y = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "mf_pos_y"));
        gymState.mf.pos.z = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "mf_pos_z"));
        gymState.mf.orn.r = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "mf_orn_x"));
        gymState.mf.orn.p = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "mf_orn_y"));
        gymState.mf.orn.y = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "mf_orn_z"));

        gymState.rf.pos.x = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "rf_pos_x"));
        gymState.rf.pos.y = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "rf_pos_y"));
        gymState.rf.pos.z = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "rf_pos_z"));
        gymState.rf.orn.r = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "rf_orn_x"));
        gymState.rf.orn.p = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "rf_orn_y"));
        gymState.rf.orn.y = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "rf_orn_z"));

        gymState.th.pos.x = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "th_pos_x"));
        gymState.th.pos.y = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "th_pos_y"));
        gymState.th.pos.z = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "th_pos_z"));
        gymState.th.orn.r = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "th_orn_x"));
        gymState.th.orn.p = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "th_orn_y"));
        gymState.th.orn.y = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "th_orn_z"));

        //parsing key
        string uuid         = PyBytes_AsString(PyDict_GetItemString( gymState_dict, "key_id"));
        gymState.key.socket     = PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "key_socket"));
        gymState.key.mqName     = PyBytes_AsString(PyDict_GetItemString( gymState_dict, "key_mqName"));
        gymState.key.timestep   =std::chrono::milliseconds ((int)PyFloat_AsDouble(PyDict_GetItemString( gymState_dict, "key_timestep")));
    }
       
    sucess = (self->ptrObj)->send(gymState);

    return Py_BuildValue("p",sucess);//build a boolean python value
}

static PyObject * PyClient_MQ_getGhostStateforClient(PyClient *self,PyObject *args){

    GhostWorldState ghost_state;//This is return value

    ghost_state =(self->ptrObj)->getGhostStateforClient();
 
    return Py_BuildValue("{s:p,s:p,s:p,s:p}",
                          "ff",ghost_state.ff,
                          "mf",ghost_state.mf,
                          "rf",ghost_state.rf,
                          "th",ghost_state.th);//build a dic python value
}

static PyMethodDef PyClientMQ_methods[] = {
    { "filter", (PyCFunction)PyClient_MQ_init,    METH_NOARGS,       "Intialize the client" },
    {NULL}  /* Sentinel */
};

static PyTypeObject PyClientType = { PyVarObject_HEAD_INIT(NULL, 0)
                                    "client.Client_MQ"   /* tp_name */
                                };


PyMODINIT_FUNC PyInit_client(void)
// create the module
{
    PyObject* m;

    PyClientType.tp_new = PyType_GenericNew;
    PyClientType.tp_basicsize=sizeof(PyClient);
    PyClientType.tp_dealloc=(destructor) PyClient_MQ_dealloc;
    PyClientType.tp_flags=Py_TPFLAGS_DEFAULT;
    PyClientType.tp_doc="Client_MQ objects";
    PyClientType.tp_methods=PyClientMQ_methods;
    //~ PyClientType.tp_members=Noddy_members;
    PyClientType.tp_init=(initproc)constructor;

    if (PyType_Ready(&PyClientType) < 0)
        return NULL;

    m = PyModule_Create(&clientmodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&PyClientType);
    PyModule_AddObject(m, "Client_MQ", (PyObject *)&PyClientType); // Add Voice object to the module
    return m;
}