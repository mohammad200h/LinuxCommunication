
cmake_minimum_required(VERSION  2.8.3)
set(CMAKE_BUILD_TYPE Debug)
find_package(Boost COMPONENTS system )

add_compile_options(-std=c++17)


include_directories( ${Boost_INCLUDE_DIRS})
link_directories( ${Boost_LIBRARY_DIRS})




add_executable(Test_MQ unitTest_MQ.cpp)

target_include_directories(Test_MQ PUBLIC ../include)



target_link_libraries(Test_MQ  Com stdc++fs ${Boost_LIBRARIES}  -lrt)




add_test(
    NAME Test_MQ
    COMMAND Test_MQ
)