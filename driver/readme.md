---
parent: EtherCAT Library and Applications
title: EtherCAT LAN925x Library
has_children: true
has_toc: false
nav_order: 1
---

[![Microchip logo](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# EtherCAT LAN925x Library

EtherCAT LAN925x slave device software utility framework is a layered software framework that enables other PIC and Cortex Microcontroller peripherals to work together.

* **LAN925x EtherCAT Framework**
    | EtherCAT Slave Framework    |Description                |
    |:---------------------------:|:-------------------------:|
    | [LAN9252 EtherCAT Framework](docs/readme_drvlan9252.md)   |    The Framework explains about EtherCAT slave controller Interface layer w.r.t LAN9252 device    |
    | [LAN9253 EtherCAT Framework](docs/readme_drvlan9253.md)   |     The Framework explains about EtherCAT slave controller Interface layer w.r.t LAN9253 device     |
    
Ethernet for Control Automation Technology (EtherCAT) was developed by Beckhoff. EtherCAT is a fast and deterministic network, and processes data using dedicated hardware and software. It uses a full duplex, master-slave configuration.

The LAN925x is a 2/3-port EtherCAT slave controller with dual integrated Ethernet PHYs which each contain a fullduplex 100BASE-TX transceiver and support 100Mbps (100BASE-TX) operation.

EtherCAT Slave Controller Interface layer is designed for Microchip MCUs to communicate with LAN925x EtherCAT Slave controller. This EtherCAT framework is configured with the communication interfaces (QSPI (SPI mode), GPIO). This communication Interface layer which will act like bridge between EtherCAT Slave Stack Code (SSC) and Host peripherals to communicate with LAN925x. SSC Stack (generated from standard SSC tool) have feature to transfer the File over EtherCAT (FoE) for MCU firmware upgrade from TwinCAT tool.

![Ethercat Technology](docs/images/EtherCAT_module_diagram.png)

Use of the Microchip EVB-LAN925x (and similar EtherCAT interface devices) requires use of the Beckhoff EtherCAT Slave Stack Code (SSC) and its associated configuration and code generation tool.

* The **interrupts** have to be configured during hardware initialization.
    *   **PDI ( Process Data Interface ) Interrupt** -
    The programmable system interrupts are generated internally by the various device sub-modules and can be configured to generate a single external host interrupt via the IRQ interrupt output pin. That is TwinCAT manager with  enabled **Synchron Synchronization** mode.

    * **DC - SYNC0 AND SYNC1** -
    If the application running on the SOC requires Distributed Clock, then SYNC0 and SYNC1 should be connected to the microcontroller’s interrupts lines. Refer to LAN9252 datasheet for configuration of SYNC0 and SYNC1.

    * **TIMER Interupt** -
    SSC has a variable which will count every millisecond, which can be implemented either timer interrupt or polling method. The interrupt/polling mode can be selected in the SSC Tool before creating the slave stack code.
    SSC will access EtherCAT core registers from both interrupt context and polling mode. So, the ECAT_CSR_CMD and ECAT_CSR_DATA registers has to be protected against simultaneous access which can corrupt the state machine inside the slave stack code.

* **Configure of Library**
   
    Configure the peripheral library using the MHC.
    EtherCAT Slave Device Indicators
    This section describes the LAN925x driver and EtherCAT trigger and counter variables are used to support visual inspection and troubleshooting of the driver and networks.

    1. **PDO ( Process Data Object Trigger and Counter )** -
    The PDO protocol is used for communication with External interrupt IRQ.

        | Index  |  Object | Type           | Direction |
        |:------:|:-------:|:--------------:|:---------:|
        |1       |0x6000   |32bit Counter   |TX         |

    2. **Receive PDO Mapping ( Outputs )** -
    The PDO protocol is used for communication with External interrupt IRQ.
        
        | Index  |  Object | Type           | Direction |
        |:------:|:-------:|:--------------:|:---------:|
        |1       |0x7010   |32bit Counter   |RX         |

    3. Synchronization -
    The Beckhoff Slave Stack Code supports different modes of synchronization which are based on three physical signals: (PDI_)IRQ, Sync0 and Sync1
    Microchip EtherCAT device also supports different modes.
        * Master and Slaves for synchronization: Free run The Master cycle time and Slave cycle time are independent. 

        * IRQ interrupt event is triggered from Master for SM-Synchron Synchronization.

        * Both IRQ and SYNC0 event occurs for the DC-Synchron operation mode and SYNC0 and SYNC1 unit cycle time is configured 1000µs.


* **Using Library**

    File over EtherCAT (FoE) -
    Architecture – Host/Slave interaction states for firmware update. The Dual Bank feature enables a firmware to execute from the NVM and at the same time the program to the flash with a new version of itself.After programming is completed the APP_BankSwitch() application function is used to swap the banks and to reset the device.

    ![library usage](docs/images/Firmware_upgrade.png)

    FOE demostartion with respect to ATSAMD51J19A device -

    ![library usage](docs/images/D51Bankdetails.png)

    * Master changes from INIT to BOOT, then download of a file initiated. When state changes from INIT to BOOT, slave or the FoE application is ready to write the new FW or bin file at the BankB location( 0x40000 Bank B location for ATSAMD51j19A device ). Flag gFirmwareDownload_Started set to 1.

    * Master initiates Download command with default password value as **0x11223344** and bin file (this file is generated from the existing D51 foe studio project). In the slave FOE_FILEDOWNLOAD_PASSWORD is set to 0x11223344 and can be modified from EtherCAT MHC componet configuration. FoE_Write() checks file name size ( limited to 16) and checks password value and then proceed to FoE_WriteData().

    * Once Master completes bin file transfer, then Master need to change the state from BOOT to INIT. In the FW application or slave code, APP_FW_GetDownloadStateFinished() changes state to gFirmwareDownload_Started = 0 and APP_FW_StateBOOTtoINIT() changes gFirmwareDownload_Finished = 1.

    * Once State changes from BOOT to INIT, Slave application initiates bank switch application(APP_BankSwitch()) and software reset(run_application()). Then Bank A moves to BANK B location and the new application stored in BANK B , moved to BANK A with flash location 0x0. The new application starts running from flash location 0x0 and will be ready with new application.

    SSC tool FoE configuration -

    ![library usage](docs/images/SSCtool_FoE_configuration.png)


