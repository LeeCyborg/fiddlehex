# DMX Lights

The DMX controls are based on [DMX Library for Arduino](https://sourceforge.net/projects/dmxlibraryforar/)and uses the DMX Shield for Arduino.

![DMX Shield](assets/ctc-dra-10-1.jpg)

## Setup

PlatformIO project was initialized with

    pio project init --ide vscode -b uno

Awesome doxygen was installed

    cd doc
    git submodule add https://github.com/jothepro/doxygen-awesome-css.git
    cd doxygen-awesome-css
    git checkout v2.2.0

## Documentation

### Prerequisites

    sudo apt install -y cmake doxygen graphviz python3
    git submodule update --init

### Build Documentation

    cmake -S . -B build
    cmake --build build --target FiddleHexDmxDocs
    python3 -m http.server 9050 --directory build/doc/html
