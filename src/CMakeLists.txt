set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "--std=gnu++17 -fpermissive ${CMAKE_CXX_FLAGS}")

find_package(Boost REQUIRED COMPONENTS program_options)
find_package(laterographics)

set(studio_SRCS
    mainwindow.h
    mainwindow.cpp
    main.cpp
    managerwidget.h
    managerwidget.cpp)

add_executable(latero-studio ${studio_SRCS})
target_link_libraries(latero-studio ${LATERO_GRAPHICS_LIBRARIES} laterographics::laterographics Boost::program_options)
install_programs(/bin latero-studio)

target_compile_definitions(latero-studio PRIVATE GDK_DISABLE_DEPRECATED GTK_DISABLE_DEPRECATED GTKMM_DISABLE_DEPRECATED)
