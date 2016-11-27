# Building the demo project

This document describes the steps needed to build and deploy the demo project
to a XMEGA-B1 Xplained board.

The following steps assume an Ubuntu based distro. For other distros some of the
commands or packages used may be different.

## Setting up the tools

To install the build toolchain use the following command:
```
sudo apt-get install gcc-avr avr-libc avrdude
```
The GCC available through the software channels is usually an older version
which might have some quirks, so it may be a good idea to install the latest
version of the toolchain. This is available from the following PPA (this must
be executed before the above command):
```
sudo apt-add-repository ppa:pmjdebruijn/gcc-avr-release
sudo apt-get update
```

## Connecting the Xplained board to the PC

The easiest way to connect the board to the host PC for programming purposes is
using the factory preloaded bootloader in DFU mode using the FLIP protocol.
The microcontroller aboard the Xplained board comes preprogrammed with this
bootloader, and unless it was programmed by other means it is available to
download new application images by connecting through the mini-USB port to a
host PC.

The `avrdude` utility is able to upload the application image using the FLIP
protocol through this bootloader. To be able to use it however adequate
permissions are needed for the user.

This can be achieved by creating a corresponding udev rule, which will allow
the user to access the Atmel USB device. Here is an example rule:

```
# rules for AVR programming tools
# the programmers are added to the plugdev group, so plain users members of this
# group can access them

SUBSYSTEM!="usb-device", ACTION!="add", GOTO="avr_rules_end"

# Xplained
ATTRS{idVendor}=="03eb", ATTRS{idProduct}=="2fea", GROUP="plugdev", MODE="0660"

LABEL="avr_rules_end"
```

This have to saved into `/etc/udev/rules.d/` folder having a `.rules` extension,
for example `65-avr.rules`.
