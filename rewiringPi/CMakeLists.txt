add_library (rewiringPi
    rwPi.c
    globals.c
    rpiModel.c
    softPwm.c
    softTone.c
)
set_property(TARGET rewiringPi PROPERTY C_STANDARD 99)

target_include_directories (rewiringPi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
