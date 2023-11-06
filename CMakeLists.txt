project(VPBaum)

set(SOURCES
KArray.cc KArray.hh                                  
Mass.cc Mass.hh
Merkmal.cc Merkmal.hh
MerkmalsMenge.cc MerkmalsMenge.hh

#Seite.cc
Seite.hh
VPBaum.cc VPBaum.hh
Makefile
)

add_library(VPBaum ${SOURCES})
target_include_directories(VPBaum PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(VPBaum PUBLIC spdlog::spdlog)

add_executable(VPgenerate VPgenerate.cc)
target_link_libraries(VPgenerate PRIVATE VPBaum)

add_executable(VPinfo VPinfo.cc)
target_link_libraries(VPinfo PRIVATE VPBaum)

add_executable(VPksearch VPksearch.cc)
target_link_libraries(VPksearch PRIVATE VPBaum)

add_executable(VPsearch VPsearch.cc)
target_link_libraries(VPsearch PRIVATE VPBaum)

add_executable(testset testset.cc)
target_link_libraries(testset PRIVATE VPBaum)

add_executable(serie serie.c)
target_link_libraries(serie PRIVATE VPBaum)

add_executable(positionstest positionstest.c)
target_link_libraries(positionstest  PRIVATE VPBaum)

add_executable(NNsearch NNsearch.cc)
target_link_libraries(NNsearch PRIVATE VPBaum)

