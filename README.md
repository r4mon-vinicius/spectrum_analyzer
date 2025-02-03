# Spectrum Analyzer

Este projeto captura sinais de aúdio do microfone da BitDogLab e utiliza a Transformada Rápida de Fourier (FFT) para extrair as componentes de frequência, exibindo os resultados no display OLED.

## Funcionamento 

- O microfone captura amostras do som ambiente via ADC.
- Os valores são armazenados em um buffer e passam por um processamento.
- A FFT é calculada usando a biblioteca kiss_fftr, gerando um espectro de frequência.
- Os resultados são convertidos em barras verticais e exibidos no OLED.

## Instalação e Uso

1. Clone este repositório e abra os arquivos no VS Code. Para clonar o repositório, use o comando abaixo:
    ```bash 
    git clone https://github.com/r4mon-vinicius/spectrum_analyzer.git 
    ```

2. Conecte a BitDogLab ao computador via USB.
3. Pressione o botão BOOTSEL e em seguida o botão RESET. A placa entrará em modo de BOOT. Caso a placa esteja com a comunicação serial habilitada, pode-se utilizar o seguinte comando: 
    ```bash
    picotool reboot -f -u
    ```

4. Compile e carregue o código na BitDogLab. O display OLED deverá exibir o espectro de frequências capturado pelo microfone.
