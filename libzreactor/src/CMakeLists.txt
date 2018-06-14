add_subdirectory(util)
add_subdirectory(eventloop)
aux_source_directory(. DIRSRCS)
add_library(zreactor SHARED ${DIRSRCS})
target_link_libraries(zreactor eventloop util zmq)

