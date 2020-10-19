---
grand_parent: EtherCAT Library and Applications
parent: EtherCAT LAN925x Library
title: EtherCAT LAN9253 Library
has_children: false
has_toc: false
nav_order: 2
---

[![Microchip logo](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# EtherCAT LAN9253 Library

Ethernet for Control Automation Technology (EtherCAT) was developed by Beckhoff. EtherCAT is a fast and deterministic network, It processes data using dedicated hardware and software. It is based on a full duplex, master-slave configuration.

The LAN9253 is a 2/3-port EtherCAT slave controller with dual integrated Ethernet PHYs which each contain a fullduplex 100BASE-TX transceiver and support 100Mbps (100BASE-TX) operation.

The EtherCAT LAN9253 Library contains a EtherCAT Slave Controller Interface layer that enables Microchip MCUs to communicate with the LAN9253 EtherCAT Slave controller. This interface layer supports different communication interfaces (QSPI (SPI mode), GPIO). The interface layer also serves as a Host peripheral bridge between the EtherCAT Slave Stack Code (SSC) and the LAN9253. The SSC Stack (generated from standard SSC tool) contains features such as File over EtherCAT (FoE) for MCU firmware upgrade from TwinCAT tool.

![Ethercat Technology](images/EtherCAT_module_lan9253_diagram.png)

## Fatures Supported
* Supports the LAN9253 EtherCAT Slave Controller device
* Enables interfacing with different 32-bit microcontroller devices
* Contains File over EtherCAT and Trigger & Counter demonstration applications.
* Demonstration applications are targeted for EVB-LAN9253-SAMD51 Evaluation Board.
* Demonstration applications are configured to use the SSC Error LED feature. This feature is implemented through the ERROR LED indicator on the board. When the ERRRO LED is ON, this indicates either of the following conditions:
    * EEPROM loading error
    * EtherCAT slave connectivity link failure
    * Application Layer Event (AL Event) error indication
    * EtherCAT State mode transition error indication
* The demonstration applications do not use the SSC Run LED feature.
* Driver supports SPI Indirect, SPI Direct and Beckhoff SPI mode of Process Data Interface(PDI) selection.

    | Process Data Interface Mode | EEPROM Source Data Config |
    |:---------------------------:|:-------------------------:|
    | SPI Indirect Mode           |     0x80    |
    | SPI Direct Mode             |     0x82    |
    | SPI Beckhoff Mode           |     0x05    |

    The **EEPROM Source Data config** data in the above table should be specified in the **ESC CONFIG DATA** attribute in the     
    **Microchip-SAMD51-EtherCAT-Slave_SSC_Config_SPI_XXX_mode.xml** file.

* **FoE (File Over EtherCAT)** can be used to download and upload a file to an EtherCAT device. Bank Switch algorithm is used to switch the new firmware downloaded.

*  The LAN9253 SPI Slave module provides a low pin count synchronous slave interface that facilitates communication between the LAN9253 and the microcontroller. The SPI Slave provides the microcontroller access to the LAN9253 System Control and Status Registers, internal FIFOs and memories.

    The following is an overview of the functions provided by the LAN9253 SPI/SQI Slave:
    * Serial Read: 4-wire (clock, select, data in and data out) reads at up to 30 MHz Serial command, address and data. Single and multiple register reads with incrementing, decrementing or static addressing.

    * Fast Read: 4-wire (clock, select, data in and data out) reads at up to 50 MHz Serial command, address and data. Dummy Byte(s) for first access. Single and multiple register reads with incrementing, decrementing or static addressing.

* The Beckhoff Slave Stack Code supports different modes of synchronization which are based on three physical signals: (PDI_)IRQ, Sync0 and Sync1.Both SM-Synchron and Distributed Clocks (DC) synchronization are supported.

## LAN 9253 Firmware Overview

* drv_lan9253_ecat_util.c and drv_lan9253_ecat_util.h files are interfaced to Lan 9253 driver.

|Name	|Description|
|:------:|:----------:|
|ECAT_Initialization|	Initialize the hardware interface (SPI) used to communicate with Slave Controller|
|ECAT_Lan9253_IsPDIFunctional	|Verifies the hardware interface (SPI) is functional or not |
|ECAT_Lan925x_SPIWrite |	Initiates an *Indirect mode* write transfer to Slave Controller via selected PDI|
|ECAT_Lan925x_SPIRead |	Initiates an *Indirect mode* read transfer to Slave Controller via selected PDI |
|ECAT_Lan925x_SPIFastRead |	Initiates an *Indirect mode* fast-read transfer to Slave Controller via selected PDI |
|ECAT_Lan925x_SPIReadPDRAM |	Initiates an *Indirect mode* read process data RAM transfer to Slave Controller via selected PDI |
|ECAT_Lan925x_SPIFastReadPDRAM |	Initiates an *Indirect mode* fast-read process data RAM transfer to Slave Controller via selected PDI |
|ECAT_Lan925x_SPIWritePDRAM |	Initiates an *Indirect mode* write process data RAM transfer to Slave Controller via selected PDI |
|ECAT_Lan9253_SPIWrite |	Initiates a *direct mode* write transfer to Slave Controller via selected PDI|
|ECAT_Lan9253_SPIRead |	Initiates a *direct mode* read transfer to Slave Controller via selected PDI |
|ECAT_Lan9253_SPIFastRead |	Initiates a *direct mode* fast-read transfer to Slave Controller via selected PDI |
|ECAT_Lan9253_Beckhoff_SPIWrite |	Initiates a *Beckhoff mode* write transfer to Slave Controller via selected PDI|
|ECAT_Lan9253_Beckhoff_SPIRead |	Initiates a *Beckhoff mode* read transfer to Slave Controller via selected PDI |
|ECAT_ESCIRQInitialization| Interrupt service routine initialization for the interrupt from EtherCAT Slave Controller|
|ECAT_SyncInterruptsInitialization| Sync0 and Sync1 Interrupt service routine Initialization|
