cmake_minimum_required(VERSION 2.8.11)
project(knotdisentangler2)

set (CMAKE_PREFIX_PATH "/home/space/Projects/Programming/Qt5.5/5.5/gcc_64")
find_package(Qt5Widgets REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_AUTOMOC ON)

set(KnotDisentangler2_SRCS main-knot.cpp GiecieWezla.cpp funkcjezew.cpp Losowania.cpp Knot3D.cpp KnotPliker.cpp Histogram.cpp KnotSimData.cpp  KnotSimulator.cpp)

add_executable(knotdisentangler2 ${KnotDisentangler2_SRCS})
target_link_libraries(knotdisentangler2 ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} GL GLU glut glui)

install(TARGETS knotdisentangler2 RUNTIME DESTINATION bin)
