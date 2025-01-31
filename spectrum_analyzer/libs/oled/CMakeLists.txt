add_library(oled ssd1306_i2c.c)

# Adiciona o diretório da biblioteca como include para outros módulos
target_include_directories(oled PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# Linka com o Pico SDK e as outras bibliotecas
target_link_libraries(oled
    pico_stdlib
    pico_binary_info
    hardware_i2c
)
