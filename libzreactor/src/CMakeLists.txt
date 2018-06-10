include_directories(/usr/local/include)
link_directories(/usr/local/lib)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(../include . ./util)
add_subdirectory(util)
aux_source_directory(. DIRSRCS)
add_library(zreactor SHARED ${DIRSRCS})
target_link_libraries(zreactor util zmq)

