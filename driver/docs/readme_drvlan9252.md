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

Ethernet for Control Automation Technology (EtherCAT) was developed by Beckhoff. EtherCAT is a fast and deterministic network, and processes data using dedicated hardware and software. It uses a full duplex, master-slave configuration.

The LAN922 is a 2/3-port EtherCAT slave controller with dual integrated Ethernet PHYs which each contain a fullduplex 100BASE-TX transceiver and support 100Mbps (100BASE-TX) operation.

EtherCAT Slave Controller Interface layer is designed for Microchip MCUs to communicate with LAN9252 EtherCAT Slave controller. This EtherCAT framework is configured with the communication interfaces (QSPI (SPI mode), GPIO). This communication Interface layer which will acts like bridge between EtherCAT Slave Stack Code (SSC) and Host peripherals to communicate with LAN9252. SSC Stack (generated from standard SSC tool) have feature to transfer the File over EtherCAT (FoE) for MCU firmware upgrade from TwinCAT tool.

![Ethercat Technology](images/EtherCAT_module_diagram.png)

## Fatures Supported
* This library supports LAN 9252 slave device
* This library useful to the different 32 bit Microcontroller device
* LAN9252 doesn't support Run LED. So this feature has to be enabled by PDI SOC if it is needed. LAN9252 supports **Error LED**. EVB-LAN9252-SAMD51 supports ERROR LED indicator. For ELD status **ON** -
    * Indicates EEPROM loading error
    * Indicates the failure EtherCAT slave connectivity link
    * Application Layer Event (AL Event) error indication
    * EtherCAT State mode transition error indication
* Driver supports only SPI Indirect mode

    | Process Data Interface Mode | EEPROM Source Data Config |
    |:---------------------------:|:-------------------------:|
    | SPI Indirect Mode           |     0x80                  |

    The above **EEPROM Source Data config** details are provided to the **ESC CONFIG DATA** of the     
    **< ethercat repo > /apps/ethercat_counter_foe_app/firmware/src/slave_stack/lan9252/Microchip-SAMD51-LAN9252-SSC_Config.xml** file.  

* **FoE (File Over EtherCAT)** can be used to download and upload a file to an EtherCAT device. Bank Switch algorithm is used to switch the new firmware downloaded.

* The SPI Slave module provides a low pin count synchronous slave interface that facilitates communication between the device and a host system. The SPI Slave allows access to the System CSRs and internal FIFOs and memories.

    The following is an overview of the functions provided by the SPI/SQI Slave:

    * Serial Read: 4-wire (clock, select, data in and data out) reads Serial command, address and data. Single and multiple register reads with incrementing, decrementing or static addressing.

* The Beckhoff Slave Stack Code supports different modes of synchronization which are based on three physical signals: (PDI_)IRQ, Sync0 and Sync1.Both SM-Synchron and Distributed Clocks (DC) synchronization are supported.

## LAN 9252 Firmware Overview

* MHC generated drv_lan9252_ecat_util.c and drv_lan9252_ecat_util.h files are interfaced to Lan 9252 driver and these files are part of the **< ethercat repo >/apps/ethercat_counter_foe_app/firmware/src/config/sam_d51_lan9252_evb/driver/lan9252**
* The templates for both Lan9252 Slave driver and the software framework are part of the **< ethercat repo >/driver/lan9252/templates**

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

