#!/usr/bin/bash
# Install stuff needed to run the project
git clone https://github.com/leodido99/zephyr.git ../zephyr_fork
git clone https://github.com/hallard/RPI-Lora-Gateway.git ../packet_forwarder
make
sudo cp bin/bossac_1.7.0 /opt
echo "Install Android Studios to compile mobile application"
echo "Compilation of gateway software should be done on gateway itself or investigate for RPI crosscompile (didnt manage to make it work)"
echo "Firmware compilation procedure:"
echo "- Install arm eabi toolchain in ~/toolchains"
echo "- Source src/RaceSensor/zephyr_fork"
echo "- mkdir src/RaceSensor/build"
echo "- cd src/RaceSensor/build"
echo "- cmake .."
echo "- make -j"
echo "- Connect sensor on USB and double press quickly reset button on the board to enter bootloader mode"
echo "- make flash (Requires bossac)"
echo "- Connect FTDI and do ./sodaq_terminal.sh"
