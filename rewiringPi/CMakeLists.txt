add_library (rewiringPi SHARED
    rwPi.c
    globals.c
    piHiPri.c
    rpiModel.c
    softPwm.c
    softTone.c
)
set(RWPI_VERSION_MAJOR 2)
set(RWPI_VERSION_MINOR 32)
set(RWPI_VERSION_PATCH 1)

set(RWPI_VERSION_STRING ${RWPI_VERSION_MAJOR}.${RWPI_VERSION_MINOR}.${RWPI_VERSION_PATCH})

set_target_properties(rewiringPi PROPERTIES VERSION ${RWPI_VERSION_STRING})

set_property(TARGET rewiringPi PROPERTY C_STANDARD 99)

target_include_directories (rewiringPi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(rewiringPi pthread)
