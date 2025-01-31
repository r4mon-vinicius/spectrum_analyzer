add_library(kiss_fft STATIC
    kiss_fft.c
    kiss_fftr.c
)

target_include_directories(kiss_fft PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
