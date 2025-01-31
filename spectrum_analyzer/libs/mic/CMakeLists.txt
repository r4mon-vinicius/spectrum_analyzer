add_library(mic mic.c)

# Adiciona o diretório da biblioteca como include para outros módulos
target_include_directories(mic PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# Linka com o Pico SDK e as outras bibliotecas
target_link_libraries(mic
    pico_stdlib
    hardware_adc
    hardware_dma
)
