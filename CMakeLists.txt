cmake_minimum_required(VERSION 2.6)
project(knotdisentangler2)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(KnotDisentangler2_SRCS main-knot.cpp GiecieWezla.cpp funkcjezew.cpp Losowania.cpp Knot3D.cpp KnotPliker.cpp Histogram.cpp KnotSimData.cpp  KnotSimulator.cpp)

qt4_automoc(${KnotDisentangler2_SRCS})
add_executable(knotdisentangler2 ${KnotDisentangler2_SRCS})
target_link_libraries(knotdisentangler2 ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} GL GLU glut glui)

install(TARGETS knotdisentangler2 RUNTIME DESTINATION bin)
