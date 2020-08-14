---
title: EtherCAT Library and Applications
has_children: true
has_toc: false
nav_order: 1
---
![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# MPLAB® Harmony 3 EtherCAT

MPLAB® Harmony 3 is an extension of the MPLAB® ecosystem for creating
embedded firmware solutions for Microchip 32-bit SAM and PIC® microcontroller
and microprocessor devices.  Refer to the following links for more information.

- [Microchip 32-bit MCUs](https://www.microchip.com/design-centers/32-bit)
- [Microchip 32-bit MPUs](https://www.microchip.com/design-centers/32-bit-mpus)
- [Microchip MPLAB X IDE](https://www.microchip.com/mplab/mplab-x-ide)
- [Microchip MPLAB® Harmony](https://www.microchip.com/mplab/mplab-harmony)
- [Microchip MPLAB® Harmony Pages](https://microchip-mplab-harmony.github.io/)

This repository contains the MPLAB® Harmony 3 EtherCAT solutions and example applications.
Quickly incorporate connectivity to your designs with EtherCAT ICs, modules,
software and development kits that make connecting effortless for your customers.
Our comprehensive EtherCAT portfolio has the technology to meet your range,
data rate, interoperability, frequency and topology needs. Refer to
the following links for release notes, training materials, and interface
reference information.

- [Release Notes](./release_notes.md)
- [MPLAB® Harmony License](mplab_harmony_license.md)
- [MPLAB® Harmony 3 EtherCAT Wiki](https://github.com/Microchip-MPLAB-Harmony/EtherCAT/wiki)
- [MPLAB® Harmony 3 EtherCAT API Help](https://microchip-mplab-harmony.github.io/EtherCAT)

# Contents Summary

| Folder     | Description                                               |
| ---        | ---                                                       |
| apps       | Example applications for EtherCAT library components      |
| config     | EtherCAT module configuration file                        |
| doc        | EtherCAT library help documentation(chm format)           |
| docs 	     | EtherCAT library help documentation (html format)         |
| drivers    | Driver/Firmware files for different products              |

# Cloning EtherCAT Microchip-MPLAB-Harmony github repo

To clone or download the repo from https://github.com/Microchip-MPLAB-Harmony/ethercat , go to the main page of this repository and then click Clone button to clone this repo or download as zip file. This content can also be download using content manager by following https://github.com/Microchip-MPLAB-Harmony/EtherCAT/wiki.

**NOTE** - Prior to using the EtherCAT demonstration, it is recommended to review the MPLAB® Harmony Release Notes (release_notes.md) for any known issues. It is located in the ethercat folder.

## EtherCAT Library and Demo Application

This distribution package contains MPLAB Harmony EtherCAT firmware project that demonstrate the capabilities of Process Data Objects( counter and Trigger objects) and FoE( File Over EtherCAT ,Firmware upgrade feature) . The project describes its hardware setup and requirements.

All the EtherCAT applications are available in ethercat/apps folder.
All the drivers are located in the ethercat/drivers folder.
The distribution package also contains details about EtherCAT Slave stack code generation tool and also it contains details of EtherCAT manager ,where the demo is verified with TwinCAT manager.

| Name      | Description  |
| --------  | -----------  |
| [ethercat_library_lan925x](driver/readme.md) | This library explains about EtherCAT slave controller Interface layer w.r.t LAN9252 device |
| [ethercat_counter_foe_app](apps/ethercat_counter_foe_app/readme.md)| This example application demonstrates the capable of doing firmware updates via FoE (File over EtherCAT) and EtherCAT Master slave communication with SAMD51 micro-controller. |



____

[![License](https://img.shields.io/badge/license-Harmony%20license-orange.svg)](https://github.com/Microchip-MPLAB-Harmony/ethercat/blob/master/mplab_harmony_license.md)
[![Latest release](https://img.shields.io/github/release/Microchip-MPLAB-Harmony/ethercat.svg)](https://github.com/Microchip-MPLAB-Harmony/ethercat/releases/latest)
[![Latest release date](https://img.shields.io/github/release-date/Microchip-MPLAB-Harmony/ethercat.svg)](https://github.com/Microchip-MPLAB-Harmony/ethercat/releases/latest)
[![Commit activity](https://img.shields.io/github/commit-activity/y/Microchip-MPLAB-Harmony/ethercat.svg)](https://github.com/Microchip-MPLAB-Harmony/ethercat/graphs/commit-activity)
[![Contributors](https://img.shields.io/github/contributors-anon/Microchip-MPLAB-Harmony/ethercat.svg)]()

____

[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/user/MicrochipTechnology)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/microchip-technology)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/microchiptechnology/)
[![Follow us on Twitter](https://img.shields.io/twitter/follow/MicrochipTech.svg?style=social)](https://twitter.com/MicrochipTech)

[![](https://img.shields.io/github/stars/Microchip-MPLAB-Harmony/ethercat.svg?style=social)]()
[![](https://img.shields.io/github/watchers/Microchip-MPLAB-Harmony/ethercat.svg?style=social)]()
