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
| docs 	     | EtherCAT library help documentation (html format)         |
| drivers    | Driver/Firmware files for different products              |

# Cloning EtherCAT Microchip-MPLAB-Harmony github repo

To clone or download the repo from https://github.com/Microchip-MPLAB-Harmony/ethercat , go to the main page of this repository and then click **code->clone** button to **Clone with HTTPS** to clone this repo using a git client or download the repository as a zip file. The EtherCAT , CSP , MHC and Dev Pack module repositories can also be downloaded using content manager by following the instructions in the **MPLAB® Harmony Repository Download** section in the guide available at this link: https://github.com/Microchip-MPLAB-Harmony/ethercat/wiki/create-your-first-ethercat-application.

**NOTE** - Prior to using the EtherCAT demonstration, it is recommended to review the MPLAB® Harmony Release Notes (release_notes.md) for any known issues.

## EtherCAT Library and Demo Application

This distribution package contains MPLAB Harmony EtherCAT firmware project that demonstrates the capabilities of Process Data Objects (counter and Trigger objects) and FoE (File Over EtherCAT, Firmware upgrade feature) . The project describes its hardware setup and requirements.

The EVB-LAN9252_SAM D51 Evaluation Board is an EtherCAT Client controller with dual integrated Ethernet PHYs. Each Ethernet PHY contains a Full-Duplex 100BASE-TX transceiver and supports 100 Mbps (100BASE-TX) operation.

 * [EVB LAN9252 SAMD51 (LAN9252 - EtherCAT Client Controller evaluation kit with SAMD51 Microcontroller )](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/EV44C93A#:~:text=EVB%2DLAN9252%2DD51%20allows%20engineers,within%20the%20Microchip%20Harmony%20Framework)

All the EtherCAT applications are available in ethercat/apps folder.
All the drivers are located in the ethercat/drivers folder.
The distribution package also contains details about EtherCAT Beckhoff Slave stack code generation tool and EtherCAT manager (required to verify the operation of the demo).

| Name      | Description  |
| --------  | -----------  |
| [ethercat_library_lan925x](driver/readme.md) | This library explains about EtherCAT Client controller Interface layer w.r.t LAN9252/LAN9253/LAN9255 devices |
| [ethercat_counter_foe_app](apps/ethercat_counter_foe_app/readme.md)| This example application demonstrates the capable of doing firmware updates via FoE (File over EtherCAT).This also demonstrates  EtherCAT TwinCAT manager and  LAN925x Client communication with SAMD51 , SAME53 and LAN9255 micro-controller. |



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
