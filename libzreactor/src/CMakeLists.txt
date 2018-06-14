include_directories(./util ./eventloop)
add_subdirectory(util)
aux_source_directory(. DIRSRCS)
add_library(zreactor SHARED ${DIRSRCS})
target_link_libraries(zreactor util zmq)

