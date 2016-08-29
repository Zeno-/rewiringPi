add_library (rewiringPi
    wiringPi.c
    wiringPi.h
    rpiModel.c
    rpiModel.h
    softPwm.c
    softPwm.h
    softTone.c
    softTone.h
)

target_include_directories (rewiringPi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
