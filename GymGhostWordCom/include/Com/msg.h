
//UUID
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/lexical_cast.hpp>         //used for type casting
#include <chrono>
#include <iostream>


using namespace std;
using namespace std::chrono;

struct service_keys{
    boost::uuids::uuid id;
    int socket;//This is used for socket communication
    string mqName; //This is used for message Queue communication
    milliseconds timestep;

};

struct position{
    double x;
    double y;
    double z;
};

struct orientation{
    double r;
    double p;
    double y;
};

struct objState{
    position pos;
    orientation orn;
};


struct GymworldState{
    service_keys key;

    objState ff;
    objState mf;
    objState rf;
    objState th;
    objState cube;
};

struct GhostWorldState{
    service_keys key;

    bool ff;
    bool mf;
    bool rf;
    bool th;
};

extern const char* GymStateFormat;

// const char* GymStateFormat  = "{%f,%f,%f},{%f,%f,%f}\n";
extern const char* GhostSateFormat ;