cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE Debug)


link_libraries(${JSONCPP_LIBRARIES})

project(GymGhostWorldsCom VERSION 1.0.0)
add_compile_options(-std=c++17)

# set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

##################################Testing##################################

enable_testing()
add_subdirectory(unit_test)
add_subdirectory(googletest)
########################################USE#################################
#python binding
add_subdirectory(pybind11)

add_library(Com src/Com.cpp)
add_library(msg src/msg.cpp)

add_executable(server src/server.cpp)
add_executable(client src/client.cpp)


target_include_directories(server PUBLIC include)
target_include_directories(client PUBLIC include)


add_executable(server_MQ src/server_MQ.cpp)
add_executable(client_MQ src/client_MQ.cpp)


target_include_directories(server_MQ PUBLIC include)
target_include_directories(client_MQ PUBLIC include)


target_include_directories(Com PUBLIC
$<INSTALL_INTERFACE:include>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_include_directories(msg PUBLIC
$<INSTALL_INTERFACE:include>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)


target_link_libraries(Com jsoncpp msg ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(server  Com  -lrt)
target_link_libraries(client  Com  -lrt)
target_link_libraries(server_MQ  Com -lrt)
target_link_libraries(client_MQ  Com -lrt)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/Com/Com.h 
              ${CMAKE_CURRENT_SOURCE_DIR}/include/Com/msg.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Com
        )
install(TARGETS  Com msg EXPORT comExport DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/Com )


install(EXPORT comExport FILE com-config.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/Com)


#installing python extention

pybind11_add_module(pyclientMQ src/client_com_pybind.cpp src/Com.cpp)
target_link_libraries(pyclientMQ PRIVATE  Com msg -lrt)
