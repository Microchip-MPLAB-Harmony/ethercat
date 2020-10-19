---
grand_parent: EtherCAT Library and Applications
parent: EtherCAT LAN925x Library
title: EtherCAT LAN9252 Library
has_children: false
has_toc: false
nav_order: 1
---

[![Microchip logo](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# EtherCAT LAN9252 Library

Ethernet for Control Automation Technology (EtherCAT) was developed by Beckhoff. EtherCAT is a fast and deterministic network, It processes data using dedicated hardware and software. It is based on a full duplex, master-slave configuration.

The LAN9252 is a 2/3-port EtherCAT slave controller with dual integrated Ethernet PHYs which each contain a fullduplex 100BASE-TX transceiver and support 100Mbps (100BASE-TX) operation.

The EtherCAT LAN9252 Library contains a EtherCAT Slave Controller Interface layer that enables Microchip MCUs to communicate with the LAN9252 EtherCAT Slave controller. This interface layer supports different communication interfaces (QSPI (SPI mode), GPIO). The interface layer also serves as a Host peripheral bridge between the EtherCAT Slave Stack Code (SSC) and the LAN9252. The SSC Stack (generated from standard SSC tool) contains features such as File over EtherCAT (FoE) for MCU firmware upgrade from TwinCAT tool.

![Ethercat Technology](images/EtherCAT_module_diagram.png)

## Library Features
* Supports the LAN9252 EtherCAT Slave Controller device
* Enables interfacing with different 32-bit microcontroller devices
* Contains File over EtherCAT and Trigger & Counter demonstration applications.
* Demonstration applications are targeted for EVB-LAN9252-SAMD51 Evaluation Board.
* Demonstration applications are configured to use the SSC Error LED feature. This feature is implemented through the ERROR LED indicator on the board. When the ERRRO LED is ON, this indicates either of the following conditions:
    * EEPROM loading error
    * EtherCAT slave connectivity link failure
    * Application Layer Event (AL Event) error indication
    * EtherCAT State mode transition error indication
* The demonstration applications do not use the SSC Run LED feature.
* Demonstration application supports LAN9252 Indirect mode with SPI communication

    | Process Data Interface Mode | EEPROM Source Data Config |
    |:---------------------------:|:-------------------------:|
    | SPI Indirect Mode           |     0x80                  |

    The **EEPROM Source Data config** value in the above table is specified in the **ESC CONFIG DATA** attribute in the     
    **< ethercat repo > /apps/ethercat_counter_foe_app/firmware/src/slave_stack/lan9252/Microchip-SAMD51-LAN9252-SSC_Config.xml** file.  

* The **FoE (File Over EtherCAT)** feature can be used to download and upload a file to an EtherCAT device. The FoE demonstration application employs a Bank Switch algorithm to switch the updated downloaded firmware.

* The LAN9252 SPI Slave module provides a low pin count synchronous slave interface that facilitates communication between the LAN9252 and the microcontroller. The SPI Slave provides the microcontroller access to the LAN9252 System Control and Status Registers, internal FIFOs and memories.

    The following is an overview of the functions provided by the LAN9252 SPI/SQI Slave:

    * Serial Read: 4-wire (clock, select, data in and data out) reads Serial command, address and data. Single and multiple register reads with incrementing, decrementing or static addressing.

* The Beckhoff Slave Stack Code supports different modes of synchronization which are based on three physical signals: (PDI_)IRQ, Sync0 and Sync1.Both SM-Synchron and Distributed Clocks (DC) synchronization are supported.

## LAN9252 Firmware Overview

* MHC generated drv_lan9252_ecat_util.c and drv_lan9252_ecat_util.h files are interfaced to Lan 9252 driver and these files are contained in the **< ethercat repo >/apps/ethercat_counter_foe_app/firmware/src/config/sam_d51_lan9252_evb/driver/lan9252** folder.
* The templates for both LAN9252 Slave driver and the software framework are contained in  the **< ethercat repo >/driver/lan9252/templates** folder.

|Name	|Description|
|:------:|:----------:|
|ECAT_Initialization| Initialize the QSPI call back registration for SPI mode and LAN 9252 driver Initialization|
|ECAT_Util_Initialize|	Initialize the hardware interface (SPI)  and Timer peripheral used for Slave Controller|
|ECAT_Lan9252_SPIWrite |  Initiates a write transfer to Slave Controller via selected PDI|
|ECAT_Lan9252_SPIRead |	Initiates a fast-read transfer to Slave Controller via selected PDI |
|ECAT_Lan9252_SPIReadPDRAM|	Initiates a read process data RAM transfer to Slave Controller via selected PDI |
|ECAT_Lan9252_SPIWritePDRAM |	Initiates a write process data RAM transfer to Slave Controller via selected PDI |
|ECAT_ESCIRQInitialization| Interrupt service routine initialization for the interrupt from EtherCAT Slave Controller|
|ECAT_SyncInterruptsInitialization| Sync0 and Sync1 Interrupt service routine Initialization|
