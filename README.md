# osc_button
ESP32 button that sends a message over OSC

## Hardware

[Waveshare ESP32-S3-ETH](https://www.waveshare.com/wiki/ESP32-S3-ETH#Other_resource_link)  
[Schematics](https://files.waveshare.com/wiki/ESP32-S3-ETH/ESP32-S3-ETH-Schematic.pdf)

<img src="https://www.waveshare.com/w/upload/e/e0/ESP32-S3-ETH-details-15.jpg" width=400/>


## Upload

    # Compile and upload over USB
    pio run --target upload

## Debugging

    # Use **osc-utility** to listen for OSC messages
    ./osc-utility_0.2.1_linux_amd64 server --port 5555 --host 0.0.0.0
