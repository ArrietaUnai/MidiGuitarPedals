# MidiPedals #

This project have been developed using the [JUCE](https://juce.com/) framework.

For now the EmpressReverb guitar pedal have been implemented. You can connect a midi usb device such as a iRig Midi 2 and with a MIDI - jack adapter you can control the pedal's inputs.

## Windows ##

The only dependency for windows is Cmake (>3.16) and Visual Studio Compilers (Visual Studio 16 2019 tested).

## Linux ##

See build-path/\_deps/juce-src/README.md for more information

### Installing dependencies ###

Taken from build-path/\_deps/juce-src/docs/README.md

The full command is as follows:

    sudo apt update
    sudo apt install libasound2-dev libjack-jackd2-dev \
        libcurl4-openssl-dev  \
        libfreetype6-dev \
        libx11-dev libxcomposite-dev libxcursor-dev libxcursor-dev libxext-dev libxinerama-dev libxrandr-dev libxrender-dev \
        libwebkit2gtk-4.0-dev \
        libglu1-mesa-dev mesa-common-dev


## MacOS ##

The only dependency for MacOS is CMake (>3.16) and clang (9.1.0) have been tested on MacOS 10.13.3 (High Sierra).
