# Spectrum Analyzer  

This project captures audio signals from the BitDogLab microphone and uses the Fast Fourier Transform (FFT) to extract frequency components, displaying the results on the OLED screen.  

## How It Works  

- The microphone captures ambient sound samples via the ADC.  
- The values are stored in a buffer and processed.  
- The FFT is computed using the **kiss_fftr** library, generating a frequency spectrum.  
- The results are converted into vertical bars and displayed on the OLED.  

## Installation and Usage  

1. Clone this repository and open the files in VS Code. To clone the repository, use the command below:  
    ```bash  
    git clone https://github.com/r4mon-vinicius/spectrum_analyzer.git  
    ```  

2. Connect the BitDogLab to the computer via USB.  
3. Press the **BOOTSEL** button and then the **RESET** button. The board will enter **BOOT mode**. If the board has serial communication enabled, you can use the following command:  
    ```bash  
    picotool reboot -f -u  
    ```  

4. Compile and upload the code to the BitDogLab. The OLED display should show the frequency spectrum captured by the microphone.  

## Future Improvements  

- Implementation of a numerical frequency scale on the display.  
- Optimization of the display rendering algorithm.  
- Application of digital filters for noise reduction.  

## Links  

This project is based on the following repositories:  

- [BitDogLab C](https://github.com/BitDogLab/BitDogLab-C.git)  
- [KISS FFT](https://github.com/mborgerding/kissfft.git)  

