# osc_button
ESP32 button that sends a message over OSC

## Hardware

[Waveshare ESP32-S3-ETH](https://www.waveshare.com/wiki/ESP32-S3-ETH#Other_resource_link)  
[Schematics](https://files.waveshare.com/wiki/ESP32-S3-ETH/ESP32-S3-ETH-Schematic.pdf)

<img src="https://www.waveshare.com/w/upload/e/e0/ESP32-S3-ETH-details-15.jpg" width=400/>

## Configuration

Configuration file can be found in include/config.h.  

## Upload

    # Compile and upload over USB
    pio run --target upload

    # Override ADDR_BASE build flag by specifying an 'env' (see: platformio.ini)
    pio run --target upload -e box0
    pio run --target upload -e box1

## Debugging

https://github.com/72nd/osc-utility

    # Use 'osc-utility' to listen for OSC messages
    osc-utility server --port 53000 --host 0.0.0.0
