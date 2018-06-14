include_directories(../src) #include internal headers for unit tests 
add_executable(zreactor-example main.cpp)
target_link_libraries(zreactor-example zreactor pthread)

