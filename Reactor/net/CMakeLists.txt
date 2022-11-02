add_library(reactor_net Channel Eventloop Poll TimerQueue Timer)
install(TARGETS reactor_net DESTINATION lib)
file(GLOB Cpp ".h")
install(FILES ${Cpp} DESTINATION include/net)

add_executable(test00 test.cc)
target_link_libraries(test00 reactor_net)
