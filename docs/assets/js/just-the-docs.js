var myVariable = `
{"0": {
    "doc": "EtherCAT Counter FoE Application",
    "title": "EtherCAT Counter FoE Application",
    "content": "This EtherCAT example application demonstrates the capable of doing firmware updates via FoE (File over EtherCAT).The communication between EtherCAT Manager (TwinCAT Manager) and Client (EtherCAT LAN925x) communication with EVB_LAN925x_SAMD51 micro-controller. Microchip’s EtherCAT provides the information about how to configure and run the application on different EtherCAT platform. | Target Development board | Description | . | EVB_LAN9252_SAMD51 | ETherCAT evaluation board with LAN9252 | . | EVB_LAN9253_SAMD51 | ETherCAT SAMD51 evaluation board with LAN9253 SPI Communication | . | EVB_LAN9253_SAMD51_SQI | ETherCAT SAMD51 evaluation board with LAN9253 SQI Communication | . | EVB_LAN9255_SAME53_SPI | ETherCAT SAME53 evaluation board with LAN9255 SPI Communication | . | EVB_LAN9255_SAME53_SQI | ETherCAT SAME53 evaluation board with LAN9255 SQI Communication | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/readme.html#ethercat-counter-foe-application",
    "relUrl": "/apps/ethercat_counter_foe_app/readme.html#ethercat-counter-foe-application"
  },"1": {
    "doc": "EtherCAT Counter FoE Application",
    "title": "EtherCAT Counter FoE Application",
    "content": ". ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/readme.html",
    "relUrl": "/apps/ethercat_counter_foe_app/readme.html"
  },"2": {
    "doc": "EtherCAT LAN925x Library",
    "title": "EtherCAT LAN925x Library",
    "content": "EtherCAT LAN925x Client device software utility framework is a layered software framework designed to operate with different PIC and Cortex Microcontroller peripherals. | LAN925x EtherCAT Framework . | EtherCAT Client Framework | Description | . | LAN9252 EtherCAT Framework | This Framework implements a EtherCAT Client Controller Interface layer for the LAN9252 device | . | LAN9253 EtherCAT Framework | This Framework implements a EtherCAT Client Controller Interface layer for the LAN9253 device | . | LAN9255 EtherCAT Framework | This Framework implements a EtherCAT Client Controller Interface layer for the LAN9255 device | . | . Ethernet for Control Automation Technology (EtherCAT) was developed by Beckhoff. EtherCAT is a fast and deterministic network. It processes data using dedicated hardware and software. It is based on a full duplex, manager(TwinCAT Manager)-client(LAN925x Client) configuration. The LAN925x is a 2/3-port EtherCAT client controller with dual integrated Ethernet PHYs which each contain a fullduplex 100BASE-TX transceiver and support 100Mbps (100BASE-TX) operation. Use of the Microchip EVB-LAN925x (and similar EtherCAT interface devices) requires use of the Beckhoff EtherCAT Slave Stack Code (SSC) and its associated configuration and code generation tool. | The interrupts have to be configured during hardware initialization. | PDI ( Process Data Interface ) Interrupt - This is a programmable interrupt. It is triggered via the TwinCAT manager by enabling the Synchron Synchronization mode. There are various operational modes in the LAN925x device that can cause this interrupt. | DC - SYNC0 AND SYNC1 - If the application running on the micrcontroller requires Distributed Clock, then LAN925x SYNC0 and SYNC1 pins should be connected to the microcontroller’s external interrupt lines. Refer to LAN9252 datasheet for configuration of SYNC0 and SYNC1. | TIMER Interupt - The SSC employs a variable that counts up every one millisecond by default. This variable can be accessed via an interrupt or a polling method. If accessed via an interrupt, the interrupt occurs every one millisecond. The interrupt/polling mode can be selected in the SSC Tool before generating the Beckhoff slave stack code. It should be noted that the SSC will access EtherCAT ECAT_CSR_CMD and ECAT_CSR_DATA core registers from both interrupt context and polling mode. This can corrupt the SSC state machine. These registers should therefore be protected against simultaneous access. | . | SSC Library Configuration . The SSC provides a set of EtherCAT Client Device Indicators. These indicators are composed of an Ethercat Trigger and counter variable. These indicators support visual inspection and troubleshooting of the driver and the Ethercat Network. This section contains additional details about these indicators. | PDO ( Process Data Object Trigger and Counter ) - The PDO protocol is used for communication with External interrupt IRQ. | Index | Object | Type | Direction | . | 1 | 0x6000 | 32bit Counter | TX | . | Receive PDO Mapping ( Outputs ) - The PDO protocol is used for communication with External interrupt IRQ. | Index | Object | Type | Direction | . | 1 | 0x7010 | 32bit Counter | RX | . | . | Using the Library . This section provides details about the File Over EtherCAT (FoE) protocol and EEPROM Emulation Configuration in this library. The below flowchart depicts the Host/Client interaction during a Firmware Update operation. The microcontroller Flash Dual Bank feature enables a firmware to execute from one bank while the other Flash bank is programmed with a Firmware Update. The APP_BankSwitch() application function is called to swap the banks and to reset the device when the programming is complete. The below figure shows the Dual Bank organization used by FOE demonstration on a ATSAMD51J19A device. The firmware upgrade is performed as follows: . | Selecting the Bootstrap option in the TwinCAT manager changes the state machine state from INIT to BOOT and initiates a file download. The SSC code receives intimation to change mode from INIT to BOOT and gets ready to write the new firmware to Bank B location (0x40000 on the ATSAMD51j19A device ). The gFirmwareDownload_Started flag set to 1. | The TwinCAT manager downloads the password (default value as 0x11223344) and the firrmware binary file. This file is generated from the existing ATSAMD51J19A FoE ATMEL Studio IDE project. In the SSC stack, the FOE_FILEDOWNLOAD_PASSWORD configuration constant is set to 0x11223344. This constant can be modified through the EtherCAT component in MPLAB Harmony Configurator (MHC). The FoE_Write() function checks the file name size (this is limited to 16 characters), verifies the password and then proceeds to call the FoE_WriteData() function. | After completing the file transfer, the TwinCAT manager state is changed from BOOT to INIT. This triggers the APP_FW_GetDownloadStateFinished() function in the client firmware changes. The gFirmwareDownload_Started flag is cleared and the APP_FW_StateBOOTtoINIT() function sets the gFirmwareDownload_Finished flag to 1. | After the state change from BOOT to INIT is complete, the Client application initiates a bank switch ( by calling the APP_BankSwitch() function) and then performs a software reset(by calling the run_application() function). This causes the execution of the application stored in BANK B location. The Bank B location is now treated as Bank A and the start location of Bank B is treated as location 0x0 of Bank A. The execution of the downloaded updated application will start. | . SSC tool FoE configuration - . | For SSC tool EEPROM emulation ,select Hardware in Slave Project Navigation Window and make sure ESC_EEPROM_SIZE is 0x800, ESC_EEPROM_EMULATION is 1 &amp; CREATE_EEPROM_CONTENT is 0. | . SSC tool EEPROM Configuration - . | . ",
    "url": "http://localhost:4000/ethercat/driver/readme.html#ethercat-lan925x-library",
    "relUrl": "/driver/readme.html#ethercat-lan925x-library"
  },"3": {
    "doc": "EtherCAT LAN925x Library",
    "title": "EtherCAT LAN925x Library",
    "content": ". ",
    "url": "http://localhost:4000/ethercat/driver/readme.html",
    "relUrl": "/driver/readme.html"
  },"4": {
    "doc": "EtherCAT LAN9252 Library",
    "title": "EtherCAT LAN9252 Library",
    "content": "Ethernet for Control Automation Technology (EtherCAT) was developed by Beckhoff. EtherCAT is a fast and deterministic network, It processes data using dedicated hardware and software. It is based on a full duplex, manager(TwinCAT Manager)-client(LAN9252 Client) configuration. The LAN9252 is a 2/3-port EtherCAT Client controller with dual integrated Ethernet PHYs which each contain a fullduplex 100BASE-TX transceiver and support 100Mbps (100BASE-TX) operation. The EtherCAT LAN9252 Library contains a EtherCAT Client Controller Interface layer that enables Microchip MCUs to communicate with the LAN9252 EtherCAT Client controller. This interface layer supports different communication interfaces (QSPI (SPI mode), GPIO). The interface layer also serves as a Host peripheral bridge between the EtherCAT Beckhoff Slave Stack Code (SSC) and the LAN9252. The SSC Stack (generated from standard SSC tool) contains features such as File over EtherCAT (FoE) for MCU firmware upgrade from TwinCAT tool. ",
    "url": "http://localhost:4000/ethercat/driver/docs/readme_drvlan9252.html#ethercat-lan9252-library",
    "relUrl": "/driver/docs/readme_drvlan9252.html#ethercat-lan9252-library"
  },"5": {
    "doc": "EtherCAT LAN9252 Library",
    "title": "Library Features",
    "content": ". | Supports the LAN9252 EtherCAT Client Controller device | Enables interfacing with different 32-bit microcontroller devices | Contains File over EtherCAT and Trigger &amp; Counter demonstration applications. | Demonstration applications are targeted for EVB-LAN9252-SAMD51 Evaluation Board. | Demonstration applications are configured to use the SSC Error LED feature. This feature is implemented through the ERROR LED indicator on the board. When the ERRRO LED is ON, this indicates either of the following conditions: . | EEPROM loading error | EtherCAT Client connectivity link failure | Application Layer Event (AL Event) error indication | EtherCAT State mode transition error indication | . | The demonstration applications do not use the SSC Run LED feature. | Demonstration application supports LAN9252 Indirect mode with SPI communication . | Process Data Interface Mode | EEPROM Source Data Config | . | SPI Indirect Mode | 0x80 | . The EEPROM Source Data config value in the above table is specified in the ESC CONFIG DATA attribute in the &lt; ethercat repo &gt; /apps/ethercat_counter_foe_app/firmware/src/slave_stack/lan9252/Microchip-SAMD51-LAN9252-SSC_Config.xml file. | The FoE (File Over EtherCAT) feature can be used to download and upload a file to an EtherCAT device. The FoE demonstration application employs a Bank Switch algorithm to switch the updated downloaded firmware. | The LAN9252 SPI Client module provides a low pin count synchronous client interface that facilitates communication between the LAN9252 and the microcontroller. The SPI Client provides the microcontroller access to the LAN9252 System Control and Status Registers, internal FIFOs and memories. The following is an overview of the functions provided by the LAN9252 SPI/SQI Client: . | Serial Read: 4-wire (clock, select, data in and data out) reads Serial command, address and data. Single and multiple register reads with incrementing, decrementing or static addressing. | . | The Beckhoff Slave Stack Code supports different modes of synchronization which are based on three physical signals: (PDI_)IRQ, Sync0 and Sync1.Both SM-Synchron and Distributed Clocks (DC) synchronization are supported. | . ",
    "url": "http://localhost:4000/ethercat/driver/docs/readme_drvlan9252.html#library-features",
    "relUrl": "/driver/docs/readme_drvlan9252.html#library-features"
  },"6": {
    "doc": "EtherCAT LAN9252 Library",
    "title": "LAN9252 Firmware Overview",
    "content": ". | MHC generated drv_lan9252_ecat_util.c and drv_lan9252_ecat_util.h files are interfaced to Lan 9252 driver and these files are contained in the &lt; ethercat repo &gt;/apps/ethercat_counter_foe_app/firmware/src/config/sam_d51_lan9252_evb/driver/lan9252 folder. | The templates for both LAN9252 Client driver and the software framework are contained in the &lt; ethercat repo &gt;/driver/lan9252/templates folder. | . | Name | Description | . | ECAT_Initialization | Initialize the QSPI call back registration for SPI mode and LAN 9252 driver Initialization | . | ECAT_Util_Initialize | Initialize the hardware interface (SPI) and Timer peripheral used for Client Controller | . | ECAT_Lan9252_SPIWrite | Initiates a write transfer to Client Controller via selected PDI | . | ECAT_Lan9252_SPIRead | Initiates a fast-read transfer to Client Controller via selected PDI | . | ECAT_Lan9252_SPIReadPDRAM | Initiates a read process data RAM transfer to Client Controller via selected PDI | . | ECAT_Lan9252_SPIWritePDRAM | Initiates a write process data RAM transfer to Client Controller via selected PDI | . | ECAT_ESCIRQInitialization | Interrupt service routine initialization for the interrupt from EtherCAT Client Controller | . | ECAT_SyncInterruptsInitialization | Sync0 and Sync1 Interrupt service routine Initialization | . ",
    "url": "http://localhost:4000/ethercat/driver/docs/readme_drvlan9252.html#lan9252-firmware-overview",
    "relUrl": "/driver/docs/readme_drvlan9252.html#lan9252-firmware-overview"
  },"7": {
    "doc": "EtherCAT LAN9252 Library",
    "title": "EtherCAT LAN9252 Library",
    "content": ". ",
    "url": "http://localhost:4000/ethercat/driver/docs/readme_drvlan9252.html",
    "relUrl": "/driver/docs/readme_drvlan9252.html"
  },"8": {
    "doc": "EtherCAT LAN9253 Library",
    "title": "EtherCAT LAN9253 Library",
    "content": "Ethernet for Control Automation Technology (EtherCAT) was developed by Beckhoff. EtherCAT is a fast and deterministic network, It processes data using dedicated hardware and software. It is based on a full duplex, manager(TwinCAT Manager)-client(LAN9253 Client) configuration. The LAN9253 is a 2/3-port EtherCAT Client controller with dual integrated Ethernet PHYs which each contain a fullduplex 100BASE-TX transceiver and support 100Mbps (100BASE-TX) operation. The EtherCAT LAN9253 Library contains a EtherCAT Client Controller Interface layer that enables Microchip MCUs to communicate with the LAN9253 EtherCAT Client controller. This interface layer supports different communication interfaces (QSPI (SPI mode), GPIO). The interface layer also serves as a Host peripheral bridge between the EtherCAT Beckhoff Slave Stack Code (SSC) and the LAN9253. The SSC Stack (generated from standard SSC tool) contains features such as File over EtherCAT (FoE) for MCU firmware upgrade from TwinCAT tool. ",
    "url": "http://localhost:4000/ethercat/driver/docs/readme_drvlan9253.html#ethercat-lan9253-library",
    "relUrl": "/driver/docs/readme_drvlan9253.html#ethercat-lan9253-library"
  },"9": {
    "doc": "EtherCAT LAN9253 Library",
    "title": "Fatures Supported",
    "content": ". | Supports the LAN9253 EtherCAT Client Controller device | Enables interfacing with different 32-bit microcontroller devices | Contains File over EtherCAT and Trigger &amp; Counter demonstration applications. | Demonstration applications are targeted for EVB-LAN9253-SAMD51 Evaluation Board. | Demonstration applications are configured to use the SSC Error LED feature. This feature is implemented through the ERROR LED indicator on the board. When the ERRRO LED is ON, this indicates either of the following conditions: . | EEPROM loading error | EtherCAT client connectivity link failure | Application Layer Event (AL Event) error indication | EtherCAT State mode transition error indication | . | The demonstration applications do not use the SSC Run LED feature. | Driver supports SPI Indirect, SPI Direct and Beckhoff SPI mode of Process Data Interface(PDI) selection. | Process Data Interface Mode | EEPROM Source Data Config | . | SPI Indirect Mode | 0x80 | . | SPI Direct Mode | 0x82 | . | SPI Beckhoff Mode | 0x05 | . The EEPROM Source Data config data in the above table should be specified in the ESC CONFIG DATA attribute in the Microchip-SAMD51-EtherCAT-Slave_SSC_Config_SPI_XXX_mode.xml file. | FoE (File Over EtherCAT) can be used to download and upload a file to an EtherCAT device. Bank Switch algorithm is used to switch the new firmware downloaded. | The LAN9253 SPI Client module provides a low pin count synchronous client interface that facilitates communication between the LAN9253 and the microcontroller. The SPI client provides the microcontroller access to the LAN9253 System Control and Status Registers, internal FIFOs and memories. The following is an overview of the functions provided by the LAN9253 SPI/SQI Client: . | Serial Read: 4-wire (clock, select, data in and data out) reads at up to 30 MHz Serial command, address and data. Single and multiple register reads with incrementing, decrementing or static addressing. | Fast Read: 4-wire (clock, select, data in and data out) reads at up to 50 MHz Serial command, address and data. Dummy Byte(s) for first access. Single and multiple register reads with incrementing, decrementing or static addressing. | . | The Beckhoff Slave Stack Code supports different modes of synchronization which are based on three physical signals: (PDI_)IRQ, Sync0 and Sync1.Both SM-Synchron and Distributed Clocks (DC) synchronization are supported. | . ",
    "url": "http://localhost:4000/ethercat/driver/docs/readme_drvlan9253.html#fatures-supported",
    "relUrl": "/driver/docs/readme_drvlan9253.html#fatures-supported"
  },"10": {
    "doc": "EtherCAT LAN9253 Library",
    "title": "LAN 9253 Firmware Overview",
    "content": ". | drv_lan9253_ecat_util.c and drv_lan9253_ecat_util.h files are interfaced to Lan 9253 driver. | . | Name | Description | . | ECAT_Initialization | Initialize the hardware interface (SPI) used to communicate with Client Controller | . | ECAT_Lan9253_IsPDIFunctional | Verifies the hardware interface (SPI) is functional or not | . | ECAT_Lan925x_SPIWrite | Initiates an Indirect mode write transfer to Client Controller via selected PDI | . | ECAT_Lan925x_SPIRead | Initiates an Indirect mode read transfer to Client Controller via selected PDI | . | ECAT_Lan925x_SPIFastRead | Initiates an Indirect mode fast-read transfer to Client Controller via selected PDI | . | ECAT_Lan925x_SPIReadPDRAM | Initiates an Indirect mode read process data RAM transfer to Client Controller via selected PDI | . | ECAT_Lan925x_SPIFastReadPDRAM | Initiates an Indirect mode fast-read process data RAM transfer to Client Controller via selected PDI | . | ECAT_Lan925x_SPIWritePDRAM | Initiates an Indirect mode write process data RAM transfer to Client Controller via selected PDI | . | ECAT_Lan9253_SPIWrite | Initiates a direct mode write transfer to client Controller via selected PDI | . | ECAT_Lan9253_SPIRead | Initiates a direct mode read transfer to Client Controller via selected PDI | . | ECAT_Lan9253_SPIFastRead | Initiates a direct mode fast-read transfer to Client Controller via selected PDI | . | ECAT_Lan9253_Beckhoff_SPIWrite | Initiates a Beckhoff mode write transfer to Client Controller via selected PDI | . | ECAT_Lan9253_Beckhoff_SPIRead | Initiates a Beckhoff mode read transfer to Client Controller via selected PDI | . | ECAT_ESCIRQInitialization | Interrupt service routine initialization for the interrupt from EtherCAT Client Controller | . | ECAT_SyncInterruptsInitialization | Sync0 and Sync1 Interrupt service routine Initialization | . ",
    "url": "http://localhost:4000/ethercat/driver/docs/readme_drvlan9253.html#lan-9253-firmware-overview",
    "relUrl": "/driver/docs/readme_drvlan9253.html#lan-9253-firmware-overview"
  },"11": {
    "doc": "EtherCAT LAN9253 Library",
    "title": "EtherCAT LAN9253 Library",
    "content": ". ",
    "url": "http://localhost:4000/ethercat/driver/docs/readme_drvlan9253.html",
    "relUrl": "/driver/docs/readme_drvlan9253.html"
  },"12": {
    "doc": "EtherCAT LAN9255 Library",
    "title": "EtherCAT LAN9255 Library",
    "content": "Ethernet for Control Automation Technology (EtherCAT) was developed by Beckhoff. EtherCAT is a fast and deterministic network, It processes data using dedicated hardware and software. It is based on a full duplex, manager(TwinCAT Manager)-client(LAN9255 Client) configuration. The LAN9255 is a 2/3-port EtherCAT Client controller with dual integrated Ethernet PHYs which each contain a fullduplex 100BASE-TX transceiver and support 100Mbps (100BASE-TX) operation. The EtherCAT LAN9255 Library contains a EtherCAT Client Controller Interface layer that enables Microchip MCUs to communicate with the LAN9255 EtherCAT Client controller. This interface layer supports different communication interfaces (QSPI (SPI/SQI mode), GPIO). The interface layer also serves as a Host peripheral bridge between the EtherCAT Beckhoff Slave Stack Code (SSC) and the LAN9255. The SSC Stack (generated from standard SSC tool) contains features such as File over EtherCAT (FoE) and EEPROM Emulation for MCU firmware upgrade from TwinCAT tool. ",
    "url": "http://localhost:4000/ethercat/driver/docs/readme_drvlan9255.html#ethercat-lan9255-library",
    "relUrl": "/driver/docs/readme_drvlan9255.html#ethercat-lan9255-library"
  },"13": {
    "doc": "EtherCAT LAN9255 Library",
    "title": "EEPROM Emulation",
    "content": "In some system configurations the PDI (Processor Data Interface) is disabled, since much of the device is configured by the EEPROM contents. When EEPROM Emulation mode is used, the PDI configuration must be selected, as the EEPROM configuration data will be stored on the microcontroller’s internal memory. The microcontroller will configure the LAN9255 over the PDI interface and no external EEPROM device will be required to be connected to the LAN9255. | Hardware Configuration:- . | EE_EMUL[2:0] | PDI_SELECT | . | 000 | 0x80 (SPI) | . | 001 | 0x95 (HBI Indexed 16bit EtherCAT Direct Mapped) | . | 010 | 0x91 (HBI Multiplexed 1 Phase 16-bit EtherCAT Direct Mapped) | . | 011 | 0x93 (HBI Multiplexed 2 Phase 16-bit EtherCAT Direct Mapped) | . | 100 | 0x82 (SPI EtherCAT Direct Mode) | . | 101 | 0x05 (Beckhoff SPI Mode) | . | 110 | Unused | . | 111 | EEPROM is enabled | . | Software Configuration:- . Microchip provides ESF Demo code, which includes application files that support EEPROM Emulation, Configuration file (.xml) and Application file (.xlsx) are also provided. From the EtherCAT master’s point of view, there is no difference between EEPROM Emulation mode and I2C EEPROM, the master just issues EEPROM commands and wait until the EEPROM interface is no longer busy. Normally the ESI file that is loaded on the EEPROM, is generated using the SSC tool from Beckhoff. The EEPROM Emulation mode of the SSC tool must be configured for ESC EEPROM Emulation mode. | EEPROM Emulation configurations parameter | Description | . | ESC_EEPROM_EMULATION | If set to 1, the EEPROM emulation is enabled and EEPROM commands are handled by the SSC. | . | CREATE_EEPROM_CONTENT | If set to 1, then a header file including the EEPROM data according to the slave configuration will be created during the slave file generation process. | . | ESC_EEPROM_SIZE | Available EEPROM buffer size in Bytes. | . | EEPROM_READ_SIZE | Number of Bytes the ESC can handle on a single read access. For LAN9255, this value is 0x08 | . | EEPROM_WRITE_SIZE | Number of Bytes the ESC can handle on a single write access. This value is always 2. | . For SSC tool EEPROM emulation ,select Hardware in Slave Project Navigation Window and make sure ESC_EEPROM_SIZE is 0x800, ESC_EEPROM_EMULATION is 1 &amp; CREATE_EEPROM_CONTENT is 0. SSC tool EEPROM Configuration - . | EtherCAT EEPROM Register Address | Register Name | . | 0x140 | PDI Control Register | . | 0x141 | ESC Configuration Register | . | 0x150 | PDI Configuration Register | . | 0x151 | Sync/Latch PDI Configuration Register | . | 0x982:0x983 | Pulse Length of SyncSignals Register | . | 0x152:0x153 | PDI Extended Configuration Register | . | 0x0012:0x0013 | Configured Station Alias | . | 0x0510:0x0511 | MII Management Control/Status Register | . | 0x0142:0x0143 | ASIC Configuration | . | Reserved 0x144:0x145 | Reserved | . | . ",
    "url": "http://localhost:4000/ethercat/driver/docs/readme_drvlan9255.html#eeprom-emulation",
    "relUrl": "/driver/docs/readme_drvlan9255.html#eeprom-emulation"
  },"14": {
    "doc": "EtherCAT LAN9255 Library",
    "title": "Fatures Supported",
    "content": ". | Supports the app_lan9255 EtherCAT Client Controller device | Supports EEPROM Emulation | Enables interfacing with different 32-bit microcontroller devices | Contains File over EtherCAT and Trigger &amp; Counter demonstration applications. | Demonstration applications are targeted for EVB-LAN9255-SAME53 Evaluation Board. | Demonstration applications are configured to use the SSC Error LED feature. This feature is implemented through the ERROR LED indicator on the board. When the ERRRO LED is ON, this indicates either of the following conditions: . | EEPROM loading error | EtherCAT client connectivity link failure | Application Layer Event (AL Event) error indication | EtherCAT State mode transition error indication | . | The demonstration applications do not use the SSC Run LED feature. | Driver supports SPI Indirect, SPI Direct and Beckhoff SPI mode of Process Data Interface(PDI) selection. | Process Data Interface Mode | EEPROM Source Data Config | . | SPI/SQI Indirect Mode | 0x80 | . | SPI/SQI Direct Mode | 0x82 | . The EEPROM Source Data config data in the above table should be specified in the ESC CONFIG DATA attribute in the Microchip-SAME53-LAN9255-EtherCAT-Slave_SSC_Config_SPI_SQI_XXX_mode.xml file. | FoE (File Over EtherCAT) can be used to download and upload a file to an EtherCAT device. Bank Switch algorithm is used to switch the new firmware downloaded. | The LAN9255 SPI or SQI Client module provides a low pin count synchronous client interface that facilitates communication between the LAN9255 and the microcontroller. The SPI client provides the microcontroller access to the LAN9255 System Control and Status Registers, internal FIFOs and memories. The following is an overview of the functions provided by the LAN9255 SPI/SQI Client: . | Serial Read: 4-wire (clock, select, data in and data out) reads at up to 30 MHz Serial command, address and data. Single and multiple register reads with incrementing, decrementing or static addressing. | Fast Read: 4-wire (clock, select, data in and data out) reads at up to 50 MHz Serial command, address and data. Dummy Byte(s) for first access. Single and multiple register reads with incrementing, decrementing or static addressing. | . | The Beckhoff Slave Stack Code supports different modes of synchronization which are based on three physical signals: (PDI_)IRQ, Sync0 and Sync1.Both SM-Synchron and Distributed Clocks (DC) synchronization are supported. | . ",
    "url": "http://localhost:4000/ethercat/driver/docs/readme_drvlan9255.html#fatures-supported",
    "relUrl": "/driver/docs/readme_drvlan9255.html#fatures-supported"
  },"15": {
    "doc": "EtherCAT LAN9255 Library",
    "title": "LAN 9255 Firmware Overview",
    "content": ". | drv_lan9255_ecat_util.c and drv_lan9255_ecat_util.h files are interfaced to Lan 9255 driver. | . | Name | SPI Different Mode API Description | . | ECAT_Initialization | Initialize the hardware interface (SPI) used to communicate with Client Controller | . | ECAT_Lan9255_IsPDIFunctional | Verifies the hardware interface (SPI) is functional or not | . | ECAT_Lan925x_SPIWrite | Initiates an Indirect mode write transfer to Client Controller via selected PDI | . | ECAT_Lan925x_SPIRead | Initiates an Indirect mode read transfer to Client Controller via selected PDI | . | ECAT_Lan925x_SPIFastRead | Initiates an Indirect mode fast-read transfer to Client Controller via selected PDI | . | ECAT_Lan925x_SPIReadPDRAM | Initiates an Indirect mode read process data RAM transfer to Client Controller via selected PDI | . | ECAT_Lan925x_SPIFastReadPDRAM | Initiates an Indirect mode fast-read process data RAM transfer to Client Controller via selected PDI | . | ECAT_Lan925x_SPIWritePDRAM | Initiates an Indirect mode write process data RAM transfer to Client Controller via selected PDI | . | ECAT_Lan9255_SPIWrite | Initiates a direct mode write transfer to client Controller via selected PDI | . | ECAT_Lan9255_SPIRead | Initiates a direct mode read transfer to Client Controller via selected PDI | . | ECAT_Lan9255_SPIFastRead | Initiates a direct mode fast-read transfer to Client Controller via selected PDI | . | ECAT_Lan9255_Beckhoff_SPIWrite | Initiates a Beckhoff mode write transfer to Client Controller via selected PDI | . | ECAT_Lan9255_Beckhoff_SPIRead | Initiates a Beckhoff mode read transfer to Client Controller via selected PDI | . | ECAT_ESCIRQInitialization | Interrupt service routine initialization for the interrupt from EtherCAT Client Controller | . | ECAT_SyncInterruptsInitialization | Sync0 and Sync1 Interrupt service routine Initialization | . | Name | SQI Different Mode API Description | . | ECAT_Lan925x_SQIWrite | Initiates an Indirect mode or Direct Mode SQI write transfer to Client Controller via selected PDI | . | ECAT_Lan925x_SQIRead | Initiates an Indirect mode or Direct Mode SQI read transfer to Client Controller via selected PDI | . | ECAT_LAN925x_SQIWritePDRAM | Initiates an Indirect mode SQI write process to Client Controller via selected PDI | . | ECAT_Lan925x_SQIReadPDRAM | Initiates an Indirect mode SQI read process data RAM transfer to Client Controller via selected PDI | . ",
    "url": "http://localhost:4000/ethercat/driver/docs/readme_drvlan9255.html#lan-9255-firmware-overview",
    "relUrl": "/driver/docs/readme_drvlan9255.html#lan-9255-firmware-overview"
  },"16": {
    "doc": "EtherCAT LAN9255 Library",
    "title": "EtherCAT LAN9255 Library",
    "content": ". ",
    "url": "http://localhost:4000/ethercat/driver/docs/readme_drvlan9255.html",
    "relUrl": "/driver/docs/readme_drvlan9255.html"
  },"17": {
    "doc": "EVB LAN9252 SAMD51 Application",
    "title": "EtherCAT Trigger &amp; Counter and FoE Application for EVM_LAN9252_SAMD51",
    "content": "This EtherCAT example application demonstrates Firmware Update over EtherCAT capability.The Counter and Trigger parameters are configured in the &lt; ethercat repo &gt;/apps/ethercat_counter_foe_app/firmware/src/slave_stack/lan9252 and demonstrates the communication between the EtherCAT Manager (TwinCAT Manager) and the EtherCAT Client (EtherCAT LAN9252). The firmware update is performed by FoE (File over EtherCAT) protocol. It is triggered by the EtherCAT Manager (TwinCAT Manager) which then download the firmware onto the EtherCAT Client (LAN9252) on the EVB_LAN9252_SAMD51 board. Note : The EtherCAT Library can also be configured to execute on other EthertCAT development boards available from Microchip. Additional instruction at available in the Create your first EtherCAT Application section. This demonstration help document contains the following sections: . | MPLAB® Harmony Software Setup | Hardware Setup | Beckhoff Slave Stack Code (SSC) Generation | MPLAB® Harmony Project Configuration | Completing The EtherCAT Application | Running The Application . | TwinCAT Manager and Microchip EtherCAT Client communication . | EtherCAT interface detect and EEPROM Programming | Trigger and Counter Demonstration | File over EtherCAT communication | . | . | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9252_samd51.html#ethercat-trigger--counter-and-foe-application-for-evm_lan9252_samd51",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9252_samd51.html#ethercat-trigger--counter-and-foe-application-for-evm_lan9252_samd51"
  },"18": {
    "doc": "EVB LAN9252 SAMD51 Application",
    "title": "1. MPLAB® Harmony Software Setup",
    "content": "The following MPLAB® software components are a prerequisite for the subsequent steps in this demonstration. Please follow the download and installation instructions available at below links. | MPLAB® X Integrated Development Environment | MPLAB® XC32/32++ C Compiler | MPLAB® Harmony Configurator | On the management PC, download and install the TwinCAT 3 Engineering Full Setup at https://www.beckhoff.com/english.asp?download/tc3-download-xae.htm. Select the latest TwinCAT 3.1 Version and click on the link. Note the dialog box that shows the TwinCAT tool to be installed TC31-Full-Setup.3.1.XXXX.XX and click on “Start Download”. Follow instructions to download. | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9252_samd51.html#1-mplab-harmony-software-setup",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9252_samd51.html#1-mplab-harmony-software-setup"
  },"19": {
    "doc": "EVB LAN9252 SAMD51 Application",
    "title": "2. Hardware Setup",
    "content": "The following tools will be used to program and debug the application on the target hardware. | MPLAB® ICD4 + ICD4/PICKIT 3 Target Adapter Board using JTAG interface. | . The following development board will be used to develop and execute the EtherCAT application. | LAN9252 - EtherCAT Client Controller evaluation kit with SAMD51 Microcontroller . The instructions in this guide are also applicable to other development boards with LAN9252 EtherCAT client device. Hardware settings are board dependent and may vary between boards. | Connect a micro USB cable to port J8 to power the board.. | For programming, connect a ICD4 JTAG cable to port J10 of the EVB_LAN9252_SAMD51 board. | Connect RJ45 connector J1 to the TwinCAT Manager. | Block diagram of the EVB_LAN9252_SAMD51 board - . | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9252_samd51.html#2-hardware-setup",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9252_samd51.html#2-hardware-setup"
  },"20": {
    "doc": "EVB LAN9252 SAMD51 Application",
    "title": "3. Beckhoff Slave Stack Code (SSC) Generation",
    "content": "Follow instruction at this link to generate Beckhoff Slave Stack Code: Steps to generate Beckhoff Slave Stack Code . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9252_samd51.html#3-beckhoff-slave-stack-code-ssc-generation",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9252_samd51.html#3-beckhoff-slave-stack-code-ssc-generation"
  },"21": {
    "doc": "EVB LAN9252 SAMD51 Application",
    "title": "MPLAB® Harmony Project Configuration",
    "content": "There are two options available for downloading/installing the MPLAB® Harmony Software Repositories from github &amp; gitee. The required repositories can be cloned from the github (or gitee) by using a local git client (such git bash). The MPLAB® Harmony Repositories are available at the following links: . https://github.com/Microchip-MPLAB-Harmony/ . https://gitee.com/Microchip-MPLAB-Harmony/ . The csp, dev_packs, mhc and ethercat repositories should be cloned. The required repositories can also be cloned (downloaded) or previously downloaded repositories can also be updated by using the MPLAB® Harmony 3 Content Manager. The following sections provide details on using the MPLAB® Harmony 3 Content Manager to download the repositories. | Refer to the EtherCAT MPLAB® Harmony Software Setup https://github.com/Microchip-MPLAB-Harmony/ethercat/wiki/create-your-first-ethercat-application for details about creating an EtherCAT project. | The following Project Graph diagram shows the MPLAB® Harmony Library components that are required by the FoE EtherCAT application on the EVB_LAN9252_SAMD51 board. | Click on the EtherCAT Stack in the Project Graph window. In the Configuration window, . | The Slave Stack source directory path should point to the folder that contains the files generated by the SSC tool. | The Enable FoE checkbox enables File over EtherCAT feature. | . | Click on LAN9252 component from the project graph. | EtherCAT Interrupt Priority Level :- This defines a interrupt priority range. All application interrupts whose priority is more than or equal to this level will be disabled during an EtherCAT interrupt service routine execution. | Following table maps EtherCAT interrupt name with respective peripheral channel selection. EIC interrupt handler and the SPI chip select Configuration for EVB_LAN9252_SAMD51 . | Interrupt Name | EIC Channel | . | SYNC0 Interrupt | EIC_EXTINT0 | . | SYNC1 Interrupt | EIC_EXTINT1 | . | IRQ Interrupt | EIC_EXTINT7 | . | Other Usage | Port Pin | . | SPI Chip Select | PORT RB11 | . | Error Select Pin | PORT RB31 | . | . NOTE - EIC/GPIO/PIO pins for External event registration and event handler processing can be selected based on the micrcontroller and LAN9252 interrupt support. | EIC, QSPI and TC0 are configured as per the application requirement. | The below PINs are configured for the application . | QSPI PIN Configuration for EVB_LAN9252_SAMD51 . | QSPI Custom Name | Function | PORT PIN ID | . | QSPI_MOSI | QSPI_DATA0 | PA08 | . | QSPI_MISO | QSPI_DATA1 | PA09 | . | QSPI_SCK | QSPI_SCK | PB10 | . | SPI_CS | GPIO | PB11 | . | PIN Configuration for EtherCAT External Interrupt Pins for EVB_LAN9252_SAMD51 . | EIC Custom Name | EIC Channel | Port Pin | . | EIC_EXTINT0 | EIC_EXTINT0 | PORT PB16 | . | EIC_EXTINT1 | EIC_EXTINT1 | PORT PB17 | . | EIC_EXTINT7 | EIC_EXTINT7 | PORT PB23 | . | . | Open NVIC configuration window from MHC→Tools. Set the QSPI Interrupt Priority Level to 2. This interrupt priority level is selected to be less than value of the EtherCAT Interrupt Priority Level configuration option in the LAN9252 component. | The application will use the default clock options. No changes are required in clock settings. | The FoE application uses a customized linker file. The ROM_LENGTH attribute in the linker file is modified to 0x40000 for ATSAMD51J19A. Bank A memory range is configured between 0x00000 to 0x3FFFF. Bank B memory range is configured between 0x40000 to 0x7FFFF. The Dual Bank feature enables the FoE firmware to be executed from one bank while an updated version of the firmware is programmed into the other bank. The APP_BankSwitch() application function is called to swap the banks and to reset the device when the programming is complete. The APP_RunApplication() function is called to execute the new firmware. To add a customized linker file to the project, uncheck the Add linker file to project option. Navigate to System&gt;Project Configuration&gt;Tool Chain Selections&gt;Add linker file in the MHC Project graph to add the customized linked script file. See the below screen capture: . | Generate the code by clicking the Generate Code button (marked in red). | The following diagram shows the generated EtherCAT project. This contains the sample_app.c file. It does not contain the Beckhoff Slave stack code. The Beckhoff Slave Stack Code should be generated using the SSC tool with the required configuration and the Microchip-SAMD51-EtherCAT-Slave_SSC_Config.xml file. The folder view on the right side in the below figure shows the EtherCAT project with SSC stack files added to the folder. | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9252_samd51.html#mplab-harmony-project-configuration",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9252_samd51.html#mplab-harmony-project-configuration"
  },"22": {
    "doc": "EVB LAN9252 SAMD51 Application",
    "title": "4. Completing The EtherCAT Application",
    "content": "At this point, the FoE application MPLAB® X IDE project is ready along with the SSC Tool generated code. The following section describes the steps to be followed to update the FoE application files. The below table lists the MPLAB® X projects available for the demonstration. The MPLAB® X project mentioned in this table is available at &lt; install-dir &gt;/ethercat/apps/ethercat_counter_foe_app/firmware. | MPLAB® X Project table . | Project Name | Target Device | Target Development board | Description | . | sam_d51_lan9252_evb.X | ATSAMD51J19A | EVB-LAN9252_SAMD51 | ETherCAT evaluation board with LAN9252 | . | The project contains the MPLAB® Harmony components that are required for an EtherCAT application. The application files need to be added to the project. | . The application source files to be added to the project, the ethercat_counter_foe_app demo, are available in Harmony_Repo_Path/h3/ethercat/apps/ethercat_counter_foe_app/firmware/src folder. | The app.c, app.h, main.c files are updated to demonstrate the application. | The apps/ethercat_counter_foe_app/firmware/src/config/sam_d51_lan9252_evb/ethercat_foe.ld linker file is a modified linker file to be used in the FOE application. | The sample application files sample_app.c, sample_app.h and sample_appObjects.h, generated by SSC tool, are updated for the application requirement. The SSC tool generated sample application files, highlighted in the above screen capture above, are updated for Counter, Trigger peripheral interface and for the FoE read/write operations. These are available in the apps/ethercat_counter_foe_app/firmware/src/slave_stack folder. | Verify the XC32 Compiler Toolchain version and set the Connected Hardware Tool to ICD4, if this is connected to J10, or PICkit 3. Press Apply button and then press OK button. | Build the application by clicking on the Build Main Project. | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9252_samd51.html#4-completing-the-ethercat-application",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9252_samd51.html#4-completing-the-ethercat-application"
  },"23": {
    "doc": "EVB LAN9252 SAMD51 Application",
    "title": "5. Running The Application",
    "content": "1. TwinCAT Manager and Microchip EtherCAT Client communication . 1. EtherCAT interface detect and EEPROM Programming . | Upon successful installation of the TwinCAT Manager, the network adapter will be moved to Installed and ready to use devices section as shown in the following figure. Copy the available SAMD51_EtherCAT_Slave.xml file from &lt; harmony-repo &gt;app/ethercat/slave_stack/lan9252/ directory to the TwinCAT\\3.1\\Config\\Io\\EtherCAT directory. | In TwinCAT XAE, create a New Project (File=&gt;New=&gt;Project). In the New Project Window, select the TwinCAT Projects option and then click OK . | Connect port zero (J1 connector) of the EVB_LAN9252_SAMD51 board to the TwinCAT Server using a RJ45 Ethernet cable, and then power up the board. The Link/Act LED should be ON at Port zero when the cable is connected. If the Link/Act LED is not ON, then this indicates that there is an issue with the connection or the cable. | Expand the IO option in the TwinCAT XAE project window and right click on Devices. Select Scan. | Click OK if the TwinCAT network interface is selected and continue scanning as shown in the below image. | Click OK to continue scanning and Click Yes in the Scan for boxes prompt. | EEPROM Programming After a successful scan, click on Device 2 (EtherCAT) in the solution explorer window of the TwinCAT tool and Click Online in the TwinCAT project window. Once this is done, highlight the Device. This should read OP. | . 2. Trigger and Counter Demonstration . This section describes the Tigger ( Output value for the TwinCAT manager and Input value to the LAN9253 EtherCAT device ) parameter and the Counter (Input value to the TwinCAT manager and Output value for the LAn9253 EtherCAT device ) parameter configuration. Counter parameter value is determined based on Trigger parameter value. If Trigger parameter value is 0, then Counter parameter is cleared. Trigger parameter value is used to determine value of Counter parameter. | Input Trigger configuration - From I/O level on the Solution Explorer (left window), go to Devices=&gt;Device # (EtherCAT)=&gt;Box 1 (SAMD51 EtherCAT Slave)=&gt;Outputs process data mapping=&gt;Trigger. | In the top center window, select Online tab. Click Write and then enter 1 in Decimal: field and click OK. | Output Counter Observation - From I/O level on the Solution Explorer (left window), go to Devices=&gt;Device # (EtherCAT)=&gt;Box 1 (SAMD51 EtherCAT Slave)=&gt;Inputs process data mapping=&gt;Counter. | In the top center window, select Online tab. The counter value should be incrementing. | . 3. File over EtherCAT communication . | FoE (File over EtherCAT) Test Update - Click on Box1, Select “Online” tab. Before FoE test Curent state and Requested state should be in OP mode. | For file download, change the mode from INIT to BOOT mode. | After clicking Download button , select BIN file that needs to be downloaded and configure the Password as per the EtherCAT component password configuration. The sample BIN files present in the &lt; harmony-repo &gt;/ethercat/apps/ethercat_counter_foe_app/firmware/src/FoE_Bin_imagefiles/lan9252_foe_binfiles folder can be used to verify the operation of the FoE application. NOTE - A BIN file can be generated from the generated EtheCAT HEX image after configuring the Project Properties-&gt;Conf:-&gt;Building-&gt;Execute This Line After Build as such: \${MP_CC_DIR}/xc32-objcopy” -I ihex -O binary “\${DISTDIR}/\${PROJECTNAME}.\${IMAGE_TYPE}.hex” “\${DISTDIR}/\${PROJECTNAME}.\${IMAGE_TYPE}.bin . NOTE - The Execute This Line After Build option should be unchecked in debug mode. | The download process can be tracked as shown in the figure below- . | After successful download, change the state from BOOT to INIT mode. This helps the application switch the bank from BANK B to BANK A and the newly downloaded firmware will be executed from BANK A. The new application starts running from flash location 0x0. | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9252_samd51.html#5-running-the-application",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9252_samd51.html#5-running-the-application"
  },"24": {
    "doc": "EVB LAN9252 SAMD51 Application",
    "title": "EVB LAN9252 SAMD51 Application",
    "content": ". ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9252_samd51.html",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9252_samd51.html"
  },"25": {
    "doc": "EVB LAN9253 SAMD51 Application",
    "title": "EtherCAT Counter-Trigger and FoE Application for EVM_LAN9253_SAMD51",
    "content": "This EtherCAT example application demonstrates Counter-Trigger and Firmware Update over EtherCAT capability. The Counter and Trigger parameters are configured in the &lt; ethercat repo &gt;/apps/ethercat_counter_foe_app/firmware/src/slave_stack/lan9253 and demonstrates the communication between the EtherCAT Manager (TwinCAT Manager) and the EtherCAT Client (EtherCAT LAN9253).The firmware update is performed by FoE (File over EtherCAT) protocol. It is triggered by the EtherCAT Manager (TwinCAT Manager) which then download the firmware onto the EtherCAT Client (EtherCAT LAN9253) communication with EVB_LAN9253_SAMD51 board. Note : The EtherCAT Library can also be configured to execute on other EthertCAT development boards available from Microchip. Additional instruction at available in the Create your first EtherCAT Application section. This demonstration help document contains the following sections: . | MPLAB® Harmony Software Setup | Hardware Setup | Beckhoff Slave Stack Code (SSC) Generation | MPLAB® Harmony Project configuration | Completing The EtherCAT Application | Running The Application . | TwinCAT Manager and Microchip EtherCAT Client communication . | EtherCAT interface detect and EEPROM Programming | Trigger and Counter Demonstration | File over EtherCAT communication | . | . | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51.html#ethercat-counter-trigger-and-foe-application-for-evm_lan9253_samd51",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51.html#ethercat-counter-trigger-and-foe-application-for-evm_lan9253_samd51"
  },"26": {
    "doc": "EVB LAN9253 SAMD51 Application",
    "title": "1. MPLAB® Harmony Software Setup",
    "content": "The following MPLAB® software components are a prerequisite for the rest of the steps in this demonstration. Please follow the download and installation instructions available at below links. | MPLAB® X Integrated Development Environment | MPLAB® XC32/32++ C Compiler | MPLAB® Harmony Configurator | On the management PC, download and Install on the TwinCAT 3 Engineering Full Setup at https://www.beckhoff.com/english.asp?download/tc3-download-xae.htm. Select the latest TwinCAT 3.1 Version and click on the link. Note the dialog box shows the TwinCAT tool that will be installed TC31-Full-Setup.3.1.XXXX.XX and click on “Start Download”. Follow instructions to download. | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51.html#1-mplab-harmony-software-setup",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51.html#1-mplab-harmony-software-setup"
  },"27": {
    "doc": "EVB LAN9253 SAMD51 Application",
    "title": "2. Hardware Setup",
    "content": "The following tools will be used to program and debug the application on the target hardware. | MPLAB® ICD4 + ICD4/PICKIT 3 Target Adapter Board using JTAG interface. | . The following development board will be used for EtherCAT application development and run the application. | LAN9253 - EtherCAT Client Controller evaluation kit with SAMD51 Microcontroller . The instructions in this guide are also applicable to other development boards with LAN9253 EtherCAT Client device. Hardware settings are board dependent and may vary between boards. | Connect a micro USB cable to J8 port for power source. | For programming, Connect a ICD4 JTAG cable to the J10 port of the EVB_LAN9253_SAMD51 board. | Connect RJ45 connector J1 to the TwinCAT Manager. | Block diagram of the EVB_LAN9253_SAMD51 board - . | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51.html#2-hardware-setup",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51.html#2-hardware-setup"
  },"28": {
    "doc": "EVB LAN9253 SAMD51 Application",
    "title": "3. Beckhoff Slave Stack Code (SSC) Generation",
    "content": "Follow instruction at this link to generate Beckhoff Slave Stack Code: Steps to generate Beckhoff Slave Stack Code . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51.html#3-beckhoff-slave-stack-code-ssc-generation",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51.html#3-beckhoff-slave-stack-code-ssc-generation"
  },"29": {
    "doc": "EVB LAN9253 SAMD51 Application",
    "title": "MPLAB® Harmony configuration",
    "content": "There are two options available for downloading/installing the MPLAB® Harmony Software Repositories from github &amp; gitee. The required repositories can be cloned from the github (or gitee) by using a local git client (such git bash). The MPLAB® Harmony Repositories are available at the following links: . https://github.com/Microchip-MPLAB-Harmony/ . https://gitee.com/Microchip-MPLAB-Harmony/ . The csp, dev_packs, mhc and ethercat repositories should be cloned. The required repositories can also be cloned (downloaded) or previously downloaded repositories can be updated by using the MPLAB® Harmony 3 Content Manager. The following sections provide details on using the MPLAB® Harmony 3 Content Manager to download the repositories. | Refer to the EtherCAT MPLAB® Harmony Software Setup https://github.com/Microchip-MPLAB-Harmony/ethercat/wiki/create-your-first-ethercat-application details to create a EtherCAT project. | The following Project Graph diagram shows the required Harmony components those are included for the present EtherCAT application for the EVB_LAN9253_SAMD51 board. | Click on the EtherCAT Stack in the Project Graph window. In the Configuration window, . | The Beckhoff Slave Stack source directory path should point to the folder that contains the files generated by the SSC tool. | Enable FoE checkbox enables File over EtherCAT feature. | . | Click on LAN9253 component from the project graph. | EtherCAT Interrupt Priority Level :- This defines a interrupt priority range. All application interrupts whose priority is more than or equal to this level will be disabled during an EtherCAT interrupt service routine execution. | Following table maps EtherCAT interrupt name with respective peripheral channel selection. EIC interrupt handler and the SPI chip select Configuration for EVB_LAN9253_SAMD51 . | Interrupt Name | EIC Channel | . | SYNC0 Interrupt | EIC_EXTINT0 | . | SYNC1 Interrupt | EIC_EXTINT1 | . | IRQ Interrupt | EIC_EXTINT7 | . | Other Usage | Port Pin | . | SPI Chip Select | PORT RB11 | . | Error Select Pin | PORT RB31 | . | . NOTE - , EIC/GPIO/PIO pins for external event registration and event handler processing can be selected based on the microcontroller and LAN9253 interrupt support . | SPI Cofiguration Mode Supported | Description | . | ETHERCAT_SPI_INDIRECT_MODE_ACCESS | SPI Indirect Mode | . | ETHERCAT_SPI_DIRECT_MODE_ACCESS | SPI Direct Mode | . | ETHERCAT_SPI_BECKHOFF_MODE_ACCESS | SPI Beckhoff Mode | . | EIC, QSPI and TC0 are configured as per the application requirement. | These are the below PINs configured for the application . | QSPI PIN Configuration for EVB_LAN9253_SAMD51 . | QSPI Custom Name | Function | PORT PIN ID | . | QSPI_MOSI | QSPI_DATA0 | PA08 | . | QSPI_MISO | QSPI_DATA1 | PA09 | . | QSPI_SCK | QSPI_SCK | PB10 | . | SPI_CS | GPIO | PB11 | . | PIN Configuration for EtherCAT External Interrupt Pins for EVB_LAN9253_SAMD51 . | EIC Custom Name | EIC Channel | Port Pin | . | EIC_EXTINT0 | EIC_EXTINT0 | PORT PB16 | . | EIC_EXTINT1 | EIC_EXTINT1 | PORT PB17 | . | EIC_EXTINT7 | EIC_EXTINT7 | PORT PB23 | . | . | Open NVIC configuration window from MHC→Tools. Set the QSPI Interrupt Priority Level to 2. This interrupt priority level is selected to be less than value of the EtherCAT Interrupt Priority Level configuration option in the LAN9253 component . | The application will use the default clock options. No changes are required in clock settings. | The FoE application uses a customized linker file. The ROM_LENGTH attribute in the linker file is modified to 0x40000 for ATSAMD51J19A. Bank A memory range is configured from 0x00000 to 0x3FFFF. Bank B memory range is configured from 0x40000 to 0x7FFFF. The Dual Bank feature enables the FoE firmware to be executed from one bank while an updated version of the firmware is programmed into the other bank. The APP_BankSwitch() application function is called to swap the banks and to reset the device when the programming is complete. The APP_RunApplication() function is called to execute the new firmware. To add a customized linker file to the project, uncheck the Add linker file to project option. Navigate to System&gt;Project Configuration&gt;Tool Chain Selections&gt;Add linker file in the MHC Project graph to add the customized linked script file. See the below screen shots: . | Generate the code by clicking the Generate Code button (marked in red). | The following diagram shows the generated EtherCAT project. This contains the sample_app.c file. It does not contain Beckhoff Slave stack code. The Beckhoff Slave Stack Code should be generated using the SSC tool with the required configuration and the Microchip-SAMD51-EtherCAT-Slave_SSC_Config_SPI_&lt; direct/indirect/beckhoff &gt;_mode.xml file. The folder view on the right side in the below figure shows the EtherCAT project with SSC stack files added to the folder. | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51.html#mplab-harmony-configuration",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51.html#mplab-harmony-configuration"
  },"30": {
    "doc": "EVB LAN9253 SAMD51 Application",
    "title": "4. Completing The EtherCAT Application",
    "content": "This section demonstrates the MPLAB® X IDE projects for the ethercat_counter_foe_app application. The following table list the MPLAB® X projects available for the demonstration. These projects are available inside &lt; install-dir &gt;/ethercat/apps/ethercat_counter_foe_app/firmware . | MPLAB® X project table . | Project Name | Target Device | Target Development board | Description | . | samd51_lan9253_evb.X | ATSAMD51J19A | EVB-LAN9253_SAMD51 | ETherCAT evaluation board with LAN9253 | . | The project contains the MPLAB® Harmony components that are required for an EtherCAT application. The application files need to be added to the project. The application source files for the ethercat_counter_foe_app which are available in Harmony_Repo_Path/h3/ethercat/apps/ethercat_counter_foe_app/firmware/src. | The app_lan9253.c, app.h, main.c files are updated to demonstrate the application. | The apps/ethercat_counter_foe_app/firmware/src/config/samd51_lan9253_evb/ethercat_foe.ld linker file is a modified linker file which is used for FOE application. | The sample application files sample_app.c, sample_app.h and sample_appObjects.h, generated by SSC tool, are updated for the application requirement. The SSC tool generated sample application files, highlighted in the above screen capture above, are updated for Counter, Trigger peripheral interface and for the FoE read/write operations. These are available in the apps/ethercat_counter_foe_app/firmware/src/slave_stack folder. | Verify the XC32 Compiler Toolchain version and set the Connected Hardware Tool to ICD4 or PICkit 3. Press Apply button and then press OK button. | Build the application by clicking on the Build Main Project. | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51.html#4-completing-the-ethercat-application",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51.html#4-completing-the-ethercat-application"
  },"31": {
    "doc": "EVB LAN9253 SAMD51 Application",
    "title": "5. Running The Application",
    "content": "1. TwinCAT Manager and Microchip EtherCAT Client communication . 1. EtherCAT interface detect and EEPROM Programming . | Upon successful installation of the TwinCAT Manager, the network adapter will be moved to Installed and ready to use devices section as shown in the following figure. Please find the below table for the available Client configuration files which are generated by SSC tool. | Client Configuration File | Mode | . | SAMD51_SPI_Indirect_Mode.xml | SPI Indirect mode | . | SAMD51_SPI_Direct_mode.xml | SPI Direct mode | . | MSAMD51_SPI_Beckhoff_mode.xml | SPI Beckhoff mode | . Copy the required available configuration file from &lt; harmony-repo &gt;/ethercat/slave_stack/lan9253/ directory to the TwinCAT\\3.1\\Config\\Io\\EtherCAT directory. | In TwinCAT XAE, create a New Project (File=&gt;New=&gt;Project). In the New Project Window, select the TwinCAT Projects option and then click OK . | Connect port zero (J1 connector) of the EVB_LAN9253_SAMD51 board to the TwinCAT Manager using a RJ45 Ethernet cable, and then power up the board. The Link/Act LED should be ON at Port zero when the cable is connected. If the Link/Act LED is not ON, then this indicates that there is an issue with the connection or the cable. | Expand the IO option in the TwinCAT XAE project window and right click on Devices. Select Scan. | Click OK if the TwinCAT network interface is selected and continue scanning as shown in the below image. | Click OK to continue scanning and Click Yes in the Scan for boxes prompt. | EEPROM Programming After a successful scan, click on Device 2 (EtherCAT) in the solution explorer window of the TwinCAT tool and Click Online in the TwinCAT project window. Once this is done, highlight the Device. This should read OP. | . 2. Trigger and Counter Demonstration . This section describes the Tigger ( Output value for the TwinCAT manager and Input value to the LAN9253 EtherCAT device ) parameter and the Counter (Input value to the TwinCAT manager and Output value for the LAn9253 EtherCAT device ) parameter configuration. Counter parameter value is determined based on Trigger parameter value. If Trigger parameter value is 0, then Counter parameter is cleared. Trigger parameter value is used to determine value of Counter parameter. | Input Trigger configuration - From I/O level on the Solution Explorer (left window), go to Devices=&gt;Device # (EtherCAT)=&gt;Box 1 (SAMD51 EtherCAT Slave)=&gt;Outputs process data mapping=&gt;Trigger. | To change the Trigger parameter value, click on the Trigger which is available under Output process data mapping in the Search Solution Explorer window. In the top center window, select Online tab. Click Write and then enter 1 in Decimal: field and click OK.( Trigger parameter value can be anything. The default Trigger parameter value is used here .) . | Output Counter Observation - From I/O level on the Solution Explorer (left window), go to Devices=&gt;Device # (EtherCAT)=&gt;Box 1 (SAMD51 EtherCAT Slave)=&gt;Inputs process data mapping=&gt;Counter. | The Counter parameter value will be incremented based on the Trigger value. ( The Counter value will not be a stable value and will keep incrementing based on the Trigger value. ) In the top center window, select Online tab. | . 3. File over EtherCAT communication . | FoE (File over EtherCAT) Test Update - Click on Box1, Select “Online” tab. Before FoE test Curent state and Requested state should be in OP mode. | For file download, change the mode from INIT to BOOT mode. | After clicking Download button , select BIN file that need to be downloaded and configure the Password as per the EtherCAT component password configuration. The sample BIN files are present in &lt; harmony-repo &gt;/ethercat/apps/ethercat_counter_foe_app/firmware/src/FoE_Bin_imagefiles/lan9252_foe_binfiles folder can be used to verify the operation of the FoE application. NOTE - A BIN file can be generated from the generated EtheCAT HEX image after configuring the Project Properties-&gt;Conf:-&gt;Building-&gt;Execute This Line After Build \${MP_CC_DIR}/xc32-objcopy” -I ihex -O binary “\${DISTDIR}/\${PROJECTNAME}.\${IMAGE_TYPE}.hex” “\${DISTDIR}/\${PROJECTNAME}.\${IMAGE_TYPE}.bin . NOTE - The Execute This Line After Build option should be unchecked in debug mode. | The download process can be tracked as shown in the figure below- . | After successful download, change the state from BOOT to INIT mode. This helps the application switch the bank from BANK B to BANK A and the newly downloaded firmware will be executed from BANK A. The new application starts running from flash location 0x0. | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51.html#5-running-the-application",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51.html#5-running-the-application"
  },"32": {
    "doc": "EVB LAN9253 SAMD51 Application",
    "title": "EVB LAN9253 SAMD51 Application",
    "content": ". ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51.html",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51.html"
  },"33": {
    "doc": "EVB LAN9253 SAMD51 Application SQI Communication",
    "title": "EtherCAT Counter-Trigger and FoE Application for EVM_LAN9253_SAMD51 SQI Communication",
    "content": "This EtherCAT example application demonstrates Counter-Trigger and Firmware Update over EtherCAT capability with SQI communication. The Counter and Trigger parameters are configured in the &lt; ethercat repo &gt;/apps/ethercat_counter_foe_app/firmware/src/slave_stack/lan9253 and demonstrates the communication between the EtherCAT Manager (TwinCAT Manager) and the EtherCAT Client (EtherCAT LAN9253).The firmware update is performed by FoE (File over EtherCAT) protocol. It is triggered by the EtherCAT Manager (TwinCAT Manager) which then download the firmware onto the EtherCAT Client (EtherCAT LAN9253) communication with EVB_LAN9253_SAMD51 board. Note : The EtherCAT Library can also be configured to execute on other EthertCAT development boards available from Microchip. Additional instruction at available in the Create your first EtherCAT Application section. This demonstration help document contains the following sections: . | MPLAB® Harmony Software Setup | Hardware Setup | Beckhoff Slave Stack Code (SSC) Generation | MPLAB® Harmony Project configuration | Completing The EtherCAT Application | Running The Application . | TwinCAT Manager and Microchip EtherCAT Client communication . | EtherCAT interface detect and EEPROM Programming | Trigger and Counter Demonstration | File over EtherCAT communication | . | . | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51_sqi.html#ethercat-counter-trigger-and-foe-application-for-evm_lan9253_samd51-sqi-communication",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51_sqi.html#ethercat-counter-trigger-and-foe-application-for-evm_lan9253_samd51-sqi-communication"
  },"34": {
    "doc": "EVB LAN9253 SAMD51 Application SQI Communication",
    "title": "1. MPLAB® Harmony Software Setup",
    "content": "The following MPLAB® software components are a prerequisite for the rest of the steps in this demonstration. Please follow the download and installation instructions available at below links. | MPLAB® X Integrated Development Environment | MPLAB® XC32/32++ C Compiler | MPLAB® Harmony Configurator | On the management PC, download and Install on the TwinCAT 3 Engineering Full Setup at https://www.beckhoff.com/english.asp?download/tc3-download-xae.htm. Select the latest TwinCAT 3.1 Version and click on the link. Note the dialog box shows the TwinCAT tool that will be installed TC31-Full-Setup.3.1.XXXX.XX and click on “Start Download”. Follow instructions to download. | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51_sqi.html#1-mplab-harmony-software-setup",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51_sqi.html#1-mplab-harmony-software-setup"
  },"35": {
    "doc": "EVB LAN9253 SAMD51 Application SQI Communication",
    "title": "2. Hardware Setup",
    "content": "The following tools will be used to program and debug the application on the target hardware. | MPLAB® ICD4 + ICD4/PICKIT 3 Target Adapter Board using JTAG interface. | . The following development board will be used for EtherCAT application development and run the application. | LAN9253 - EtherCAT Client Controller evaluation kit with SAMD51 Microcontroller . The instructions in this guide are also applicable to other development boards with LAN9253 EtherCAT Client device. Hardware settings are board dependent and may vary between boards. | Connect a micro USB cable to J8 port for power source. | For programming, Connect a ICD4 JTAG cable to the J10 port of the EVB_LAN9253_SAMD51 board. | Connect RJ45 connector J1 to the TwinCAT Manager. | Block diagram of the EVB_LAN9253_SAMD51 board - . | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51_sqi.html#2-hardware-setup",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51_sqi.html#2-hardware-setup"
  },"36": {
    "doc": "EVB LAN9253 SAMD51 Application SQI Communication",
    "title": "3. Beckhoff Slave Stack Code (SSC) Generation",
    "content": "Follow instruction at this link to generate Beckhoff Slave Stack Code: Steps to generate Beckhoff Slave Stack Code . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51_sqi.html#3-beckhoff-slave-stack-code-ssc-generation",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51_sqi.html#3-beckhoff-slave-stack-code-ssc-generation"
  },"37": {
    "doc": "EVB LAN9253 SAMD51 Application SQI Communication",
    "title": "MPLAB® Harmony configuration",
    "content": "There are two options available for downloading/installing the MPLAB® Harmony Software Repositories from github &amp; gitee. The required repositories can be cloned from the github (or gitee) by using a local git client (such git bash). The MPLAB® Harmony Repositories are available at the following links: . https://github.com/Microchip-MPLAB-Harmony/ . https://gitee.com/Microchip-MPLAB-Harmony/ . The csp, dev_packs, mhc and ethercat repositories should be cloned. The required repositories can also be cloned (downloaded) or previously downloaded repositories can be updated by using the MPLAB® Harmony 3 Content Manager. The following sections provide details on using the MPLAB® Harmony 3 Content Manager to download the repositories. | Refer to the EtherCAT MPLAB® Harmony Software Setup https://github.com/Microchip-MPLAB-Harmony/ethercat/wiki/create-your-first-ethercat-application details to create a EtherCAT project. | The following Project Graph diagram shows the required Harmony components those are included for the present EtherCAT application for the EVB_LAN9253_SAMD51 board with sqi communication. | Click on the EtherCAT Stack in the Project Graph window. In the Configuration window, . | The Beckhoff Slave Stack source directory path should point to the folder that contains the files generated by the SSC tool. | Enable FoE checkbox enables File over EtherCAT feature. | . | Click on LAN9253 component from the project graph. | EtherCAT Interrupt Priority Level :- This defines a interrupt priority range. All application interrupts whose priority is more than or equal to this level will be disabled during an EtherCAT interrupt service routine execution. | Following table maps EtherCAT interrupt name with respective peripheral channel selection. EIC interrupt handler and the Error Pin Configuration for EVB_LAN9253_SAMD51 . | Interrupt Name | EIC Channel | . | SYNC0 Interrupt | EIC_EXTINT0 | . | SYNC1 Interrupt | EIC_EXTINT1 | . | IRQ Interrupt | EIC_EXTINT7 | . | Other Usage | Port Pin | . | Error Select Pin | PORT RB31 | . | . *NOTE - , EIC/GPIO/PIO pins for external event registration and event handler processing can be selected based on the microcontroller and LAN9253 interrupt support . | SQI Cofiguration Mode Supported | Description | . | ETHERCAT_SQI_INDIRECT_MODE_ACCESS | SQI Indirect Mode | . | ETHERCAT_SQI_DIRECT_MODE_ACCESS | SQI Direct Mode | . | EIC, QSPI and TC0 are configured as per the application requirement. | These are the below PINs configured for the application . | QSPI PIN Configuration for EVB_LAN9253_SAMD51 . | QSPI Custom Name | Function | PORT PIN ID | . | QSPI_DATA0 | QSPI_DATA0 | PA08 | . | QSPI_DATA1 | QSPI_DATA1 | PA09 | . | QSPI_DATA2 | QSPI_DATA1 | PA10 | . | QSPI_DATA3 | QSPI_DATA3 | PA11 | . | QSPI_SCK | QSPI_SCK | PB10 | . | QSPI_CS | QSPI_CS | PB11 | . | PIN Configuration for EtherCAT External Interrupt Pins for EVB_LAN9253_SAMD51 . | EIC Custom Name | EIC Channel | Port Pin | . | EIC_EXTINT0 | EIC_EXTINT0 | PORT PB16 | . | EIC_EXTINT1 | EIC_EXTINT1 | PORT PB17 | . | EIC_EXTINT7 | EIC_EXTINT7 | PORT PB23 | . | . | The application will use the default clock options. No changes are required in clock settings. | The FoE application uses a customized linker file. The ROM_LENGTH attribute in the linker file is modified to 0x40000 for ATSAMD51J19A. Bank A memory range is configured from 0x00000 to 0x3FFFF. Bank B memory range is configured from 0x40000 to 0x7FFFF. The Dual Bank feature enables the FoE firmware to be executed from one bank while an updated version of the firmware is programmed into the other bank. The APP_BankSwitch() application function is called to swap the banks and to reset the device when the programming is complete. The APP_RunApplication() function is called to execute the new firmware. To add a customized linker file to the project, uncheck the Add linker file to project option. Navigate to System&gt;Project Configuration&gt;Tool Chain Selections&gt;Add linker file in the MHC Project graph to add the customized linked script file. See the below screen shots: . | Generate the code by clicking the Generate Code button. | The following diagram shows the generated EtherCAT project. This contains the sample_app.c file. It does not contain Beckhoff Slave stack code. The Beckhoff Slave Stack Code should be generated using the SSC tool with the required configuration and the Microchip-SAMD51-EtherCAT-Slave_SSC_Config_SQI_&lt; direct/indirect/beckhoff &gt;_mode.xml file. The folder view on the right side in the below figure shows the EtherCAT project with SSC stack files added to the folder. | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51_sqi.html#mplab-harmony-configuration",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51_sqi.html#mplab-harmony-configuration"
  },"38": {
    "doc": "EVB LAN9253 SAMD51 Application SQI Communication",
    "title": "4. Completing The EtherCAT Application",
    "content": "This section demonstrates the MPLAB® X IDE projects for the ethercat_counter_foe_app application. The following table list the MPLAB® X projects available for the demonstration. These projects are available inside &lt; install-dir &gt;/ethercat/apps/ethercat_counter_foe_app/firmware . | MPLAB® X project table . | Project Name | Target Device | Target Development board | Description | . | samd51_lan9253_evb_sqi.X | ATSAMD51J19A | EVB-LAN9253_SAMD51 | ETherCAT evaluation board with LAN9253 SQI Communication | . | The project contains the MPLAB® Harmony components that are required for an EtherCAT application. The application files need to be added to the project. The application source files for the ethercat_counter_foe_app which are available in Harmony_Repo_Path/h3/ethercat/apps/ethercat_counter_foe_app/firmware/src. | The app_lan9253.c, app.h, main.c files are updated to demonstrate the application. | The apps/ethercat_counter_foe_app/firmware/src/config/samd51_lan9253_evb/ethercat_foe.ld linker file is a modified linker file which is used for FOE application. | The sample application files sample_app.c, sample_app.h and sample_appObjects.h, generated by SSC tool, are updated for the application requirement. The SSC tool generated sample application files, highlighted in the above screen capture above, are updated for Counter, Trigger peripheral interface and for the FoE read/write operations. These are available in the apps/ethercat_counter_foe_app/firmware/src/slave_stack folder. | Verify the XC32 Compiler Toolchain version and set the Connected Hardware Tool to ICD4 or PICkit 3. Press Apply button and then press OK button. | Build the application by clicking on the Build Main Project. | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51_sqi.html#4-completing-the-ethercat-application",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51_sqi.html#4-completing-the-ethercat-application"
  },"39": {
    "doc": "EVB LAN9253 SAMD51 Application SQI Communication",
    "title": "5. Running The Application",
    "content": "1. TwinCAT Manager and Microchip EtherCAT Client communication . 1. EtherCAT interface detect and EEPROM Programming . | Upon successful installation of the TwinCAT Manager, the network adapter will be moved to Installed and ready to use devices section as shown in the following figure. Please find the below table for the available Client configuration files which are generated by SSC tool. | Client Configuration File | Mode | . | SAMD51_SQI_Indirect_Mode.xml | SQI Indirect mode | . | SAMD51_SQI_Direct_mode.xml | SQI Direct mode | . Copy the required available configuration file from &lt; harmony-repo &gt;/ethercat/slave_stack/lan9253/ directory to the TwinCAT\\3.1\\Config\\Io\\EtherCAT directory. | In TwinCAT XAE, create a New Project (File=&gt;New=&gt;Project). In the New Project Window, select the TwinCAT Projects option and then click OK . | Connect port zero (J1 connector) of the EVB_LAN9253_SAMD51 board to the TwinCAT Manager using a RJ45 Ethernet cable, and then power up the board. The Link/Act LED should be ON at Port zero when the cable is connected. If the Link/Act LED is not ON, then this indicates that there is an issue with the connection or the cable. | Expand the IO option in the TwinCAT XAE project window and right click on Devices. Select Scan. | Click OK if the TwinCAT network interface is selected and continue scanning as shown in the below image. | Click OK to continue scanning and Click Yes in the Scan for boxes prompt. | EEPROM Programming After a successful scan, click on Device 2 (EtherCAT) in the solution explorer window of the TwinCAT tool and Click Online in the TwinCAT project window. Once this is done, highlight the Device. This should read OP. | . 2. Trigger and Counter Demonstration . This section describes the Tigger ( Output value for the TwinCAT manager and Input value to the LAN9253 EtherCAT device ) parameter and the Counter (Input value to the TwinCAT manager and Output value for the LAn9253 EtherCAT device ) parameter configuration. Counter parameter value is determined based on Trigger parameter value. If Trigger parameter value is 0, then Counter parameter is cleared. Trigger parameter value is used to determine value of Counter parameter. | Input Trigger configuration - From I/O level on the Solution Explorer (left window), go to Devices=&gt;Device # (EtherCAT)=&gt;Box 1 (SAMD51 EtherCAT Slave)=&gt;Outputs process data mapping=&gt;Trigger. | To change the Trigger parameter value, click on the Trigger which is available under Output process data mapping in the Search Solution Explorer window. In the top center window, select Online tab. Click Write and then enter 1 in Decimal: field and click OK.( Trigger parameter value can be anything. The default Trigger parameter value is used here .) . | Output Counter Observation - From I/O level on the Solution Explorer (left window), go to Devices=&gt;Device # (EtherCAT)=&gt;Box 1 (SAMD51 EtherCAT Slave)=&gt;Inputs process data mapping=&gt;Counter. | The Counter parameter value will be incremented based on the Trigger value. ( The Counter value will not be a stable value and will keep incrementing based on the Trigger value. ) In the top center window, select Online tab. | . 3. File over EtherCAT communication . | FoE (File over EtherCAT) Test Update - Click on Box1, Select “Online” tab. Before FoE test Curent state and Requested state should be in OP mode. | For file download, change the mode from INIT to BOOT mode. | After clicking Download button , select BIN file that need to be downloaded and configure the Password as per the EtherCAT component password configuration. The sample BIN files are present in &lt; harmony-repo &gt;/ethercat/apps/ethercat_counter_foe_app/firmware/src/FoE_Bin_imagefiles/lan9253_foe_binfiles folder can be used to verify the operation of the FoE application. NOTE - A BIN file can be generated from the generated EtheCAT HEX image after configuring the Project Properties-&gt;Conf:-&gt;Building-&gt;Execute This Line After Build \${MP_CC_DIR}/xc32-objcopy” -I ihex -O binary “\${DISTDIR}/\${PROJECTNAME}.\${IMAGE_TYPE}.hex” “\${DISTDIR}/\${PROJECTNAME}.\${IMAGE_TYPE}.bin . NOTE - The Execute This Line After Build option should be unchecked in debug mode. | The download process can be tracked as shown in the figure below- . | After successful download, change the state from BOOT to INIT mode. This helps the application switch the bank from BANK B to BANK A and the newly downloaded firmware will be executed from BANK A. The new application starts running from flash location 0x0. | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51_sqi.html#5-running-the-application",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51_sqi.html#5-running-the-application"
  },"40": {
    "doc": "EVB LAN9253 SAMD51 Application SQI Communication",
    "title": "EVB LAN9253 SAMD51 Application SQI Communication",
    "content": ". ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51_sqi.html",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9253_samd51_sqi.html"
  },"41": {
    "doc": "EVB LAN9255 SAME53 Application  SPI Communication",
    "title": "EtherCAT Counter-Trigger and FoE Application for EVM_LAN9255_SAME53 SPI Communication",
    "content": "This EtherCAT example application demonstrates Counter-Trigger and Firmware Update over EtherCAT capability with SPI communication. The Counter and Trigger parameters are configured in the &lt; ethercat repo &gt;/apps/ethercat_counter_foe_app/firmware/src/slave_stack/lan9255 and demonstrates the communication between the EtherCAT Manager (TwinCAT Manager) and the EtherCAT Client (EtherCAT LAN9255).The firmware update is performed by FoE (File over EtherCAT) protocol. It is triggered by the EtherCAT Manager (TwinCAT Manager) which then download the firmware onto the EtherCAT Client (EtherCAT LAN9255) communication with EVB_LAN9255_SAME53 board. Note : The EtherCAT Library can also be configured to execute on other EthertCAT development boards available from Microchip. Additional instruction at available in the Create your first EtherCAT Application section. This demonstration help document contains the following sections: . | MPLAB® Harmony Software Setup | Hardware Setup | Beckhoff Slave Stack Code (SSC) Generation | MPLAB® Harmony Code &amp; Project Configuration | Completing The EtherCAT Application | Running The Application . | TwinCAT Manager and Microchip EtherCAT Client communication . | EtherCAT interface detect and EEPROM Programming | Trigger and Counter Demonstration | File over EtherCAT communication | . | . | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_spi.html#ethercat-counter-trigger-and-foe-application-for-evm_lan9255_same53-spi-communication",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_spi.html#ethercat-counter-trigger-and-foe-application-for-evm_lan9255_same53-spi-communication"
  },"42": {
    "doc": "EVB LAN9255 SAME53 Application  SPI Communication",
    "title": "1. MPLAB® Harmony Software Setup",
    "content": "The following MPLAB® software components are a prerequisite for the rest of the steps in this demonstration. Please follow the download and installation instructions available at below links. | MPLAB® X Integrated Development Environment | MPLAB® XC32/32++ C Compiler | MPLAB® Harmony Configurator | On the management PC, download and Install on the TwinCAT 3 Engineering Full Setup at https://www.beckhoff.com/english.asp?download/tc3-download-xae.htm. Select the latest TwinCAT 3.1 Version and click on the link. Note the dialog box shows the TwinCAT tool that will be installed TC31-Full-Setup.3.1.XXXX.XX and click on “Start Download”. Follow instructions to download. | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_spi.html#1-mplab-harmony-software-setup",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_spi.html#1-mplab-harmony-software-setup"
  },"43": {
    "doc": "EVB LAN9255 SAME53 Application  SPI Communication",
    "title": "2. Hardware Setup",
    "content": "The following tools will be used to program and debug the application on the target hardware. | MPLAB® ICD4 + ICD4/PICKIT 3 Target Adapter Board using JTAG interface. | . The following development board will be used for EtherCAT application development and run the application. | LAN9255 - EtherCAT Client Controller evaluation kit with SAME53 Microcontroller . The instructions in this guide are also applicable to other development boards with LAN9255 EtherCAT Client device. Hardware settings are board dependent and may vary between boards. | Connect a micro USB cable to J8 port for power source. | For programming, Connect a ICD4 JTAG cable to the J10 port of the EVB_LAN9255_SAME53 board. | Connect RJ45 connector J1 to the TwinCAT Manager. | Jumper setup for LAN9255 device w.r.t to different mode . | Block diagram of the EVB_LAN9255_SAME53 board - . | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_spi.html#2-hardware-setup",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_spi.html#2-hardware-setup"
  },"44": {
    "doc": "EVB LAN9255 SAME53 Application  SPI Communication",
    "title": "3. Beckhoff Slave Stack Code (SSC) Generation",
    "content": "Follow instruction at this link to generate Beckhoff Slave Stack Code: Steps to generate Beckhoff Slave Stack Code . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_spi.html#3-beckhoff-slave-stack-code-ssc-generation",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_spi.html#3-beckhoff-slave-stack-code-ssc-generation"
  },"45": {
    "doc": "EVB LAN9255 SAME53 Application  SPI Communication",
    "title": "4. MPLAB® Harmony Code &amp; Project Configuration",
    "content": "The present MPLAB Harmony v3 project is based on both MPLAB Code Configurator (MCC) and MPLAB Harmony3 Project Configurator(MHC). To work with MCC , user require to follow the steps that is mentioned in the below link . https://microchipdeveloper.com/harmony3:update-and-configure-existing-mhc-proj-to-mcc-proj . NOTE - To create a new MPLAB Harmony v3 project using MPLAB Code Configurator (MCC) , user require to follow these steps mentioned in this following link . https://microchipdeveloper.com/harmony3:getting-started-training-module-using-mcc . There are two options available for downloading/installing the MPLAB® Harmony Software Repositories from github &amp; gitee. The required repositories can be cloned from the github (or gitee) by using a local git client (such git bash). The MPLAB® Harmony Repositories are available at the following links: . https://github.com/Microchip-MPLAB-Harmony/ . https://gitee.com/Microchip-MPLAB-Harmony/ . The csp, dev_packs, mcc (or mhc) and ethercat repositories should be cloned. The required repositories can also be cloned (downloaded) or previously downloaded repositories can be updated by using the MPLAB® Harmony 3 Content Manager. The following sections provide details on using the MPLAB® Harmony 3 Content Manager to download the repositories. | Refer to the EtherCAT MPLAB® Harmony Software Setup https://github.com/Microchip-MPLAB-Harmony/ethercat/wiki/create-your-first-ethercat-application details to create a EtherCAT project. | The following Project Graph diagram shows the required Harmony components those are included for the present EtherCAT application for the EVB_LAN9255_SAME53 board. | Click on the EtherCAT Stack in the Project Graph window. In the Configuration window, . | The Beckhoff Slave Stack source directory path should point to the folder that contains the files generated by the SSC tool. | Enable FoE checkbox enables File over EtherCAT feature. | . | Click on LAN9255 component from the project graph. | EtherCAT Interrupt Priority Level :- This defines a interrupt priority range. All application interrupts whose priority is more than or equal to this level will be disabled during an EtherCAT interrupt service routine execution. | Ethercat EEPROM Emulation Enabled :- EEPROM Emulation is enabled for this device to work with TwinCAT manager. | Following table maps EtherCAT interrupt name with respective peripheral channel selection. EIC interrupt handler and the SPI chip select Configuration for EVB_LAN9255_SAME53 . | Interrupt Name | EIC Channel | . | SYNC0 Interrupt | EIC_EXTINT0 | . | SYNC1 Interrupt | EIC_EXTINT1 | . | IRQ Interrupt | EIC_EXTINT7 | . | Other Usage | Port Pin | . | SPI Chip Select | PORT RB11 | . | Error Select Pin | PORT RB31 | . | . NOTE - , EIC/GPIO/PIO pins for external event registration and event handler processing can be selected based on the microcontroller and LAN9255 interrupt support . | SPI Cofiguration Mode Supported | Description | . | ETHERCAT_SPI_INDIRECT_MODE_ACCESS | SPI Indirect Mode | . | ETHERCAT_SPI_DIRECT_MODE_ACCESS | SPI Direct Mode | . | ETHERCAT_SPI_BECKHOFF_MODE_ACCESS | SPI Beckhoff Mode | . | EIC, QSPI and TC0 are configured as per the application requirement. | These are the below PINs configured for the application . | QSPI PIN Configuration for EVB_LAN9255_SAME53 . | QSPI Custom Name | Function | PORT PIN ID | . | QSPI_MOSI | QSPI_DATA0 | PA08 | . | QSPI_MISO | QSPI_DATA1 | PA09 | . | QSPI_SCK | QSPI_SCK | PB10 | . | SPI_CS | GPIO | PB11 | . | PIN Configuration for EtherCAT External Interrupt Pins for EVB_LAN9255_SAME53 . | EIC Custom Name | EIC Channel | Port Pin | . | EIC_EXTINT0 | EIC_EXTINT0 | PORT PB16 | . | EIC_EXTINT1 | EIC_EXTINT1 | PORT PB17 | . | EIC_EXTINT7 | EIC_EXTINT7 | PORT PB23 | . | . | Open NVIC configuration window from MHC→Tools. Set the QSPI Interrupt Priority Level to 2. This interrupt priority level is selected to be less than value of the EtherCAT Interrupt Priority Level configuration option in the LAN9255 component . | The application will use the default clock options. No changes are required in clock settings. | The FoE application uses a customized linker file. The ROM_LENGTH attribute in the linker file is modified to 0x80000 for LAN9255ZMX020 (ATSAME53J20A). Bank A memory range is configured from 0x00000 to 0x7FFFF. Bank B memory range is configured from 0x80000 to 0xFFFFF. | The Dual Bank feature enables the FoE firmware to be executed from one bank while an updated version of the firmware is programmed into the other bank. The APP_BankSwitch() application function is called to swap the banks and to reset the device when the programming is complete. The APP_RunApplication() function is called to execute the new firmware. | EEPROM Emulation starts from 0x3E000 (EEPROM_ORIGIN) and total length is 2000. | To add a customized linker file to the project, uncheck the Add linker file to project option. Navigate to System&gt;Project Configuration&gt;Tool Chain Selections&gt;Add linker file in the MHC Project graph to add the customized linked script file. | . See the below screen shots: . | Generate the code by clicking the Generate Code button (marked in red). | The following diagram shows the generated EtherCAT project. This contains the sample_app.c file. It does not contain Beckhoff Slave stack code. The Beckhoff Slave Stack Code should be generated using the SSC tool with the required configuration and the Microchip-SAME53-EtherCAT-Slave_SSC_Config_SPI_&lt; direct/indirect/beckhoff &gt;_mode.xml file. The folder view on the right side in the below figure shows the EtherCAT project with SSC stack files added to the folder. | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_spi.html#4-mplab-harmony-code--project-configuration",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_spi.html#4-mplab-harmony-code--project-configuration"
  },"46": {
    "doc": "EVB LAN9255 SAME53 Application  SPI Communication",
    "title": "5. Completing The EtherCAT Application",
    "content": "This section demonstrates the MPLAB® X IDE projects for the ethercat_counter_foe_app application. The following table list the MPLAB® X projects available for the demonstration. These projects are available inside &lt; install-dir &gt;/ethercat/apps/ethercat_counter_foe_app/firmware . | MPLAB® X project table . | Project Name | Target Device | Target Development board | Description | . | SAME53_lan9255_evb.X | LAN9255ZMX020 | EVB-LAN9255_SAME53 | ETherCAT evaluation board with LAN9255 with SPI Communication | . | The project contains the MPLAB® Harmony components that are required for an EtherCAT application. The application files need to be added to the project. The application source files for the ethercat_counter_foe_app which are available in Harmony_Repo_Path/h3/ethercat/apps/ethercat_counter_foe_app/firmware/src. | The app_lan9255.c, app_lan9255.h, main.c files are updated to demonstrate the application. | The apps/ethercat_counter_foe_app/firmware/src/config/SAME53_lan9255_evb/ethercat_foe.ld linker file is a modified linker file which is used for FOE application. | The sample application files eeprom.h, sample_app.c, sample_app.h and sample_appObjects.h, generated by SSC tool, are updated for the application requirement. The SSC tool generated sample application files, highlighted in the above screen capture above, are updated for Counter, Trigger peripheral interface and for the FoE read/write operations. These are available in the apps/ethercat_counter_foe_app/firmware/src/slave_stack folder. | Verify the XC32 Compiler Toolchain version and set the Connected Hardware Tool to ICD4 or PICkit 3. Press Apply button and then press OK button. | Build the application by clicking on the Build Main Project. | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_spi.html#5-completing-the-ethercat-application",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_spi.html#5-completing-the-ethercat-application"
  },"47": {
    "doc": "EVB LAN9255 SAME53 Application  SPI Communication",
    "title": "6. Running The Application",
    "content": "1. TwinCAT Manager and Microchip EtherCAT Client communication . 1. EtherCAT interface detect and EEPROM Programming . | Upon successful installation of the TwinCAT Manager, the network adapter will be moved to Installed and ready to use devices section as shown in the following figure. Please find the below table for the available Client configuration files which are generated by SSC tool. | Client Configuration File | Mode | . | SAME53_SPI_Indirect_Mode.xml | SPI Indirect mode | . | SAME53_SPI_Direct_mode.xml | SPI Direct mode | . | MSAME53_SPI_Beckhoff_mode.xml | SPI Beckhoff mode | . | Default EEPROM Emulation is enabled for the EVB-LAN9255_SAME53 evaluation board. The strap along with EE_EMULI1 and EE_EMUL2 configures the default PDI selection during EEPROM Emulation mode. | Jumper Settings as per the different modes - . | Indirect mode - . | Jumper (J17) | Description | Configuration | . | JP3 | EE_MUL0 | 0 and 1 connected | . | JP4 | EE_MUL0 | 0 and 1 connected | . | JP5 | EE_MUL1 | 0 and 1 connected | . | JP5 | EE_MUL2 | 0 and 1 connected | . | Direct mode - . | Jumper (J17) | Description | Configuration | . | JP3 | EE_MUL0 | 0 and 1 connected | . | JP4 | EE_MUL0 | 0 and 1 connected | . | JP5 | EE_MUL1 | 0 and 1 connected | . | JP5 | EE_MUL2 | 1 and 2 connected | . | . | . | In TwinCAT XAE, create a New Project (File=&gt;New=&gt;Project). In the New Project Window, select the TwinCAT Projects option and then click OK . | Connect port zero (J1 connector) of the EVB_LAN9255_SAME53 board to the TwinCAT Manager using a RJ45 Ethernet cable, and then power up the board. The Link/Act LED should be ON at Port zero when the cable is connected. If the Link/Act LED is not ON, then this indicates that there is an issue with the connection or the cable. | Expand the IO option in the TwinCAT XAE project window and right click on Devices. Select Scan. | Click OK if the TwinCAT network interface is selected and continue scanning as shown in the below image. | Click OK to continue scanning and Click Yes in the Scan for boxes prompt. | EEPROM Programming After a successful scan, click on Device 2 (EtherCAT) in the solution explorer window of the TwinCAT tool and Click Online in the TwinCAT project window. Once this is done, highlight the Device. This should read OP. | . 2. Trigger and Counter Demonstration . This section describes the Tigger ( Output value for the TwinCAT manager and Input value to the LAN9255 EtherCAT device ) parameter and the Counter (Input value to the TwinCAT manager and Output value for the LAn9255 EtherCAT device ) parameter configuration. Counter parameter value is determined based on Trigger parameter value. If Trigger parameter value is 0, then Counter parameter is cleared. Trigger parameter value is used to determine value of Counter parameter. | Input Trigger configuration - From I/O level on the Solution Explorer (left window), go to Devices=&gt;Device # (EtherCAT)=&gt;Box 1 (SAME53 EtherCAT Slave)=&gt;Outputs process data mapping=&gt;Trigger. | To change the Trigger parameter value, click on the Trigger which is available under Output process data mapping in the Search Solution Explorer window. In the top center window, select Online tab. Click Write and then enter 1 in Decimal: field and click OK.( Trigger parameter value can be anything. The default Trigger parameter value is used here .) . | Output Counter Observation - From I/O level on the Solution Explorer (left window), go to Devices=&gt;Device # (EtherCAT)=&gt;Box 1 (SAME53 EtherCAT Slave)=&gt;Inputs process data mapping=&gt;Counter. | The Counter parameter value will be incremented based on the Trigger value. ( The Counter value will not be a stable value and will keep incrementing based on the Trigger value. ) In the top center window, select Online tab. | . 3. File over EtherCAT communication . | FoE (File over EtherCAT) Test Update - Click on Box1, Select “Online” tab. Before FoE test Curent state and Requested state should be in OP mode. | For file download, change the mode from INIT to BOOT mode. | After clicking Download button , select BIN file that need to be downloaded and configure the Password as per the EtherCAT component password configuration. The sample BIN files are present in &lt; harmony-repo &gt;/ethercat/apps/ethercat_counter_foe_app/firmware/src/FoE_Bin_imagefiles/lan9252_foe_binfiles folder can be used to verify the operation of the FoE application. NOTE - A BIN file can be generated from the generated EtheCAT HEX image after configuring the Project Properties-&gt;Conf:-&gt;Building-&gt;Execute This Line After Build \${MP_CC_DIR}/xc32-objcopy” -I ihex -O binary “\${DISTDIR}/\${PROJECTNAME}.\${IMAGE_TYPE}.hex” “\${DISTDIR}/\${PROJECTNAME}.\${IMAGE_TYPE}.bin . NOTE - The Execute This Line After Build option should be unchecked in debug mode. | The download process can be tracked as shown in the figure below- . | After successful download, change the state from BOOT to INIT mode. This helps the application switch the bank from BANK B to BANK A and the newly downloaded firmware will be executed from BANK A. The new application starts running from flash location 0x0. | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_spi.html#6-running-the-application",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_spi.html#6-running-the-application"
  },"48": {
    "doc": "EVB LAN9255 SAME53 Application  SPI Communication",
    "title": "EVB LAN9255 SAME53 Application  SPI Communication",
    "content": ". ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_spi.html",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_spi.html"
  },"49": {
    "doc": "EVB LAN9255 SAME53 Application SQI Communication",
    "title": "EtherCAT Counter-Trigger and FoE Application for EVM_LAN9255_SAME53 SQI Communication",
    "content": "This EtherCAT example application demonstrates Counter-Trigger and Firmware Update over EtherCAT capability with SQI communication. The Counter and Trigger parameters are configured in the &lt; ethercat repo &gt;/apps/ethercat_counter_foe_app/firmware/src/slave_stack/LAN9255 and demonstrates the communication between the EtherCAT Manager (TwinCAT Manager) and the EtherCAT Client (EtherCAT LAN9255).The firmware update is performed by FoE (File over EtherCAT) protocol. It is triggered by the EtherCAT Manager (TwinCAT Manager) which then download the firmware onto the EtherCAT Client (EtherCAT LAN9255) communication with EVB_LAN9255_SAME53 board. Note : The EtherCAT Library can also be configured to execute on other EthertCAT development boards available from Microchip. Additional instruction at available in the Create your first EtherCAT Application section. This demonstration help document contains the following sections: . | MPLAB® Harmony Software Setup | Hardware Setup | Beckhoff Slave Stack Code (SSC) Generation | MPLAB® Harmony Code and Project Configuration | Completing The EtherCAT Application | Running The Application . | TwinCAT Manager and Microchip EtherCAT Client communication . | EtherCAT interface detect and EEPROM Programming | Trigger and Counter Demonstration | File over EtherCAT communication | . | . | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_sqi.html#ethercat-counter-trigger-and-foe-application-for-evm_lan9255_same53-sqi-communication",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_sqi.html#ethercat-counter-trigger-and-foe-application-for-evm_lan9255_same53-sqi-communication"
  },"50": {
    "doc": "EVB LAN9255 SAME53 Application SQI Communication",
    "title": "1. MPLAB® Harmony Software Setup",
    "content": "The following MPLAB® software components are a prerequisite for the rest of the steps in this demonstration. Please follow the download and installation instructions available at below links. | MPLAB® X Integrated Development Environment | MPLAB® XC32/32++ C Compiler | MPLAB® Harmony Configurator | On the management PC, download and Install on the TwinCAT 3 Engineering Full Setup at https://www.beckhoff.com/english.asp?download/tc3-download-xae.htm. Select the latest TwinCAT 3.1 Version and click on the link. Note the dialog box shows the TwinCAT tool that will be installed TC31-Full-Setup.3.1.XXXX.XX and click on “Start Download”. Follow instructions to download. | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_sqi.html#1-mplab-harmony-software-setup",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_sqi.html#1-mplab-harmony-software-setup"
  },"51": {
    "doc": "EVB LAN9255 SAME53 Application SQI Communication",
    "title": "2. Hardware Setup",
    "content": "The following tools will be used to program and debug the application on the target hardware. | MPLAB® ICD4 + ICD4/PICKIT 3 Target Adapter Board using JTAG interface. | . The following development board will be used for EtherCAT application development and run the application. | LAN9255 - EtherCAT Client Controller evaluation kit with SAME53 Microcontroller . The instructions in this guide are also applicable to other development boards with LAN9255 EtherCAT Client device. Hardware settings are board dependent and may vary between boards. | Connect a micro USB cable to J8 port for power source. | For programming, Connect a ICD4 JTAG cable to the J10 port of the EVB_LAN9255_SAME53 board. | Connect RJ45 connector J1 to the TwinCAT Manager. | Jumper setup for LAN9255 device w.r.t to different mode . | Block diagram of the EVB_LAN9255_SAME53 board - . | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_sqi.html#2-hardware-setup",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_sqi.html#2-hardware-setup"
  },"52": {
    "doc": "EVB LAN9255 SAME53 Application SQI Communication",
    "title": "3. Beckhoff Slave Stack Code (SSC) Generation",
    "content": "Follow instruction at this link to generate Beckhoff Slave Stack Code: Steps to generate Beckhoff Slave Stack Code . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_sqi.html#3-beckhoff-slave-stack-code-ssc-generation",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_sqi.html#3-beckhoff-slave-stack-code-ssc-generation"
  },"53": {
    "doc": "EVB LAN9255 SAME53 Application SQI Communication",
    "title": "4. MPLAB® Harmony Code and Project Configuration",
    "content": "The present MPLAB Harmony v3 project is based on both MPLAB Code Configurator (MCC) and MPLAB Harmony3 Project Configurator(MHC). To work with MCC , user require to follow the steps that is mentioned in the below link . https://microchipdeveloper.com/harmony3:update-and-configure-existing-mhc-proj-to-mcc-proj . NOTE - To create a new MPLAB Harmony v3 project using MPLAB Code Configurator (MCC) , user require to follow these steps mentioned in this following link . https://microchipdeveloper.com/harmony3:getting-started-training-module-using-mcc . There are two options available for downloading/installing the MPLAB® Harmony Software Repositories from github &amp; gitee. The required repositories can be cloned from the github (or gitee) by using a local git client (such git bash). The MPLAB® Harmony Repositories are available at the following links: . https://github.com/Microchip-MPLAB-Harmony/ . https://gitee.com/Microchip-MPLAB-Harmony/ . The csp, dev_packs, mcc (or mhc) and ethercat repositories should be cloned. The required repositories can also be cloned (downloaded) or previously downloaded repositories can be updated by using the MPLAB® Harmony 3 Content Manager. The following sections provide details on using the MPLAB® Harmony 3 Content Manager to download the repositories. | Refer to the EtherCAT MPLAB® Harmony Software Setup https://github.com/Microchip-MPLAB-Harmony/ethercat/wiki/create-your-first-ethercat-application details to create a EtherCAT project. | The following Project Graph diagram shows the required Harmony components those are included for the present EtherCAT application for the EVB_LAN9255_SAME53 board with sqi communication. | Click on the EtherCAT Stack in the Project Graph window. In the Configuration window, . | The Beckhoff Slave Stack source directory path should point to the folder that contains the files generated by the SSC tool. | Enable FoE checkbox enables File over EtherCAT feature. | . | Click on LAN9255 component from the project graph. | EtherCAT Interrupt Priority Level :- This defines a interrupt priority range. All application interrupts whose priority is more than or equal to this level will be disabled during an EtherCAT interrupt service routine execution. | Ethercat EEPROM Emulation Enabled :- EEPROM Emulation is enabled for this device to work with TwinCAT manager. | Following table maps EtherCAT interrupt name with respective peripheral channel selection. EIC interrupt handler and the Error Pin Configuration for EVB_LAN9255_SAME53 . | Interrupt Name | EIC Channel | . | SYNC0 Interrupt | EIC_EXTINT0 | . | SYNC1 Interrupt | EIC_EXTINT1 | . | IRQ Interrupt | EIC_EXTINT7 | . | Other Usage | Port Pin | . | Error Select Pin | PORT RB31 | . | . *NOTE - , EIC/GPIO/PIO pins for external event registration and event handler processing can be selected based on the microcontroller and LAN9255 interrupt support . | SQI Cofiguration Mode Supported | Description | . | ETHERCAT_SQI_INDIRECT_MODE_ACCESS | SQI Indirect Mode | . | ETHERCAT_SQI_DIRECT_MODE_ACCESS | SQI Direct Mode | . | EIC, QSPI and TC0 are configured as per the application requirement. | These are the below PINs configured for the application . | QSPI PIN Configuration for EVB_LAN9255_SAME53 . | QSPI Custom Name | Function | PORT PIN ID | . | QSPI_DATA0 | QSPI_DATA0 | PA08 | . | QSPI_DATA1 | QSPI_DATA1 | PA09 | . | QSPI_DATA2 | QSPI_DATA1 | PA10 | . | QSPI_DATA3 | QSPI_DATA3 | PA11 | . | QSPI_SCK | QSPI_SCK | PB10 | . | QSPI_CS | QSPI_CS | PB11 | . | PIN Configuration for EtherCAT External Interrupt Pins for EVB_LAN9255_SAME53 . | EIC Custom Name | EIC Channel | Port Pin | . | EIC_EXTINT0 | EIC_EXTINT0 | PORT PB16 | . | EIC_EXTINT1 | EIC_EXTINT1 | PORT PB17 | . | EIC_EXTINT7 | EIC_EXTINT7 | PORT PB23 | . | . | The application will use the default clock options. No changes are required in clock settings. | The FoE application uses a customized linker file. The ROM_LENGTH attribute in the linker file is modified to 0x80000 for LAN9255ZMX020 (ATSAME53J20A). Bank A memory range is configured from 0x00000 to 0x7FFFF. Bank B memory range is configured from 0x80000 to 0xFFFFF. | The Dual Bank feature enables the FoE firmware to be executed from one bank while an updated version of the firmware is programmed into the other bank. The APP_BankSwitch() application function is called to swap the banks and to reset the device when the programming is complete. The APP_RunApplication() function is called to execute the new firmware. | EEPROM Emulation starts from 0x3E000 (EEPROM_ORIGIN) and total length is 2000. | To add a customized linker file to the project, uncheck the Add linker file to project option. Navigate to System&gt;Project Configuration&gt;Tool Chain Selections&gt;Add linker file in the MHC Project graph to add the customized linked script file. | . See the below screen shots: . | Generate the code by clicking the Generate Code button. | The following diagram shows the generated EtherCAT project. This contains the sample_app.c file. It does not contain Beckhoff Slave stack code. The Beckhoff Slave Stack Code should be generated using the SSC tool with the required configuration and the Microchip-SAME53-EtherCAT-Slave_SSC_Config_SQI_&lt; direct/indirect/beckhoff &gt;_mode.xml file. The folder view on the right side in the below figure shows the EtherCAT project with SSC stack files added to the folder. | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_sqi.html#4-mplab-harmony-code-and-project-configuration",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_sqi.html#4-mplab-harmony-code-and-project-configuration"
  },"54": {
    "doc": "EVB LAN9255 SAME53 Application SQI Communication",
    "title": "5. Completing The EtherCAT Application",
    "content": "This section demonstrates the MPLAB® X IDE projects for the ethercat_counter_foe_app application. The following table list the MPLAB® X projects available for the demonstration. These projects are available inside &lt; install-dir &gt;/ethercat/apps/ethercat_counter_foe_app/firmware . | MPLAB® X project table . | Project Name | Target Device | Target Development board | Description | . | same53_lan9255_evb_sqi.X | LAN9255ZMX020 | EVB-LAN9255_SAME53 | ETherCAT evaluation board with LAN9255 SQI Communication | . | The project contains the MPLAB® Harmony components that are required for an EtherCAT application. The application files need to be added to the project. The application source files for the ethercat_counter_foe_app which are available in Harmony_Repo_Path/h3/ethercat/apps/ethercat_counter_foe_app/firmware/src. | The app_lan9255.c, app_lan9255.h, main.c files are updated to demonstrate the application. | The apps/ethercat_counter_foe_app/firmware/src/config/same53_lan9255_evb/ethercat_foe.ld linker file is a modified linker file which is used for FOE application. | The sample application files eeprom.h, sample_app.c, sample_app.h and sample_appObjects.h, generated by SSC tool, are updated for the application requirement. The SSC tool generated sample application files, highlighted in the above screen capture above, are updated for Counter, Trigger peripheral interface and for the FoE read/write operations. These are available in the apps/ethercat_counter_foe_app/firmware/src/slave_stack folder. | Verify the XC32 Compiler Toolchain version and set the Connected Hardware Tool to ICD4 or PICkit 3. Press Apply button and then press OK button. | Build the application by clicking on the Build Main Project. | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_sqi.html#5-completing-the-ethercat-application",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_sqi.html#5-completing-the-ethercat-application"
  },"55": {
    "doc": "EVB LAN9255 SAME53 Application SQI Communication",
    "title": "6. Running The Application",
    "content": "1. TwinCAT Manager and Microchip EtherCAT Client communication . 1. EtherCAT interface detect and EEPROM Programming . | Upon successful installation of the TwinCAT Manager, the network adapter will be moved to Installed and ready to use devices section as shown in the following figure. | Default EEPROM Emulation is enabled for the EVB-LAN9255_SAME53 evaluation board. The strap along with EE_EMULI1 and EE_EMUL2 configures the default PDI selection during EEPROM Emulation mode. | Jumper Settings as per the different modes - . | Indirect mode - . | Jumper (J17) | Description | Configuration | . | JP3 | EE_MUL0 | 0 and 1 connected | . | JP4 | EE_MUL0 | 0 and 1 connected | . | JP5 | EE_MUL1 | 0 and 1 connected | . | JP5 | EE_MUL2 | 0 and 1 connected | . | Direct mode - . | Jumper (J17) | Description | Configuration | . | JP3 | EE_MUL0 | 0 and 1 connected | . | JP4 | EE_MUL0 | 0 and 1 connected | . | JP5 | EE_MUL1 | 0 and 1 connected | . | JP5 | EE_MUL2 | 1 and 2 connected | . | . | . | In TwinCAT XAE, create a New Project (File=&gt;New=&gt;Project). In the New Project Window, select the TwinCAT Projects option and then click OK . | Connect port zero (J1 connector) of the EVB_LAN9255_SAME53 board to the TwinCAT Manager using a RJ45 Ethernet cable, and then power up the board. The Link/Act LED should be ON at Port zero when the cable is connected. If the Link/Act LED is not ON, then this indicates that there is an issue with the connection or the cable. | Expand the IO option in the TwinCAT XAE project window and right click on Devices. Select Scan. | Click OK if the TwinCAT network interface is selected and continue scanning as shown in the below image. | Click OK to continue scanning and Click Yes in the Scan for boxes prompt. | EEPROM Programming After a successful scan, click on Device 2 (EtherCAT) in the solution explorer window of the TwinCAT tool and Click Online in the TwinCAT project window. Once this is done, highlight the Device. This should read OP. | . 2. Trigger and Counter Demonstration . This section describes the Tigger ( Output value for the TwinCAT manager and Input value to the LAN9255 EtherCAT device ) parameter and the Counter (Input value to the TwinCAT manager and Output value for the LAN9255 EtherCAT device ) parameter configuration. Counter parameter value is determined based on Trigger parameter value. If Trigger parameter value is 0, then Counter parameter is cleared. Trigger parameter value is used to determine value of Counter parameter. | Input Trigger configuration - From I/O level on the Solution Explorer (left window), go to Devices=&gt;Device # (EtherCAT)=&gt;Box 1 (SAME53 EtherCAT Slave)=&gt;Outputs process data mapping=&gt;Trigger. | To change the Trigger parameter value, click on the Trigger which is available under Output process data mapping in the Search Solution Explorer window. In the top center window, select Online tab. Click Write and then enter 1 in Decimal: field and click OK.( Trigger parameter value can be anything. The default Trigger parameter value is used here .) . | Output Counter Observation - From I/O level on the Solution Explorer (left window), go to Devices=&gt;Device # (EtherCAT)=&gt;Box 1 (SAME53 EtherCAT Slave)=&gt;Inputs process data mapping=&gt;Counter. | The Counter parameter value will be incremented based on the Trigger value. ( The Counter value will not be a stable value and will keep incrementing based on the Trigger value. ) In the top center window, select Online tab. | . 3. File over EtherCAT communication . | FoE (File over EtherCAT) Test Update - Click on Box1, Select “Online” tab. Before FoE test Curent state and Requested state should be in OP mode. | For file download, change the mode from INIT to BOOT mode. | After clicking Download button , select BIN file that need to be downloaded and configure the Password as per the EtherCAT component password configuration. The sample BIN files are present in &lt; harmony-repo &gt;/ethercat/apps/ethercat_counter_foe_app/firmware/src/FoE_Bin_imagefiles/lan9255_foe_binfiles folder can be used to verify the operation of the FoE application. NOTE - A BIN file can be generated from the generated EtheCAT HEX image after configuring the Project Properties-&gt;Conf:-&gt;Building-&gt;Execute This Line After Build \${MP_CC_DIR}/xc32-objcopy” -I ihex -O binary “\${DISTDIR}/\${PROJECTNAME}.\${IMAGE_TYPE}.hex” “\${DISTDIR}/\${PROJECTNAME}.\${IMAGE_TYPE}.bin . NOTE - The Execute This Line After Build option should be unchecked in debug mode. | The download process can be tracked as shown in the figure below- . | After successful download, change the state from BOOT to INIT mode. This helps the application switch the bank from BANK B to BANK A and the newly downloaded firmware will be executed from BANK A. The new application starts running from flash location 0x0. | . ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_sqi.html#6-running-the-application",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_sqi.html#6-running-the-application"
  },"56": {
    "doc": "EVB LAN9255 SAME53 Application SQI Communication",
    "title": "EVB LAN9255 SAME53 Application SQI Communication",
    "content": ". ",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_sqi.html",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9255_same53_sqi.html"
  },"57": {
    "doc": "Slave Stack Code Generation",
    "title": "Slave Stack Code Generation",
    "content": ". | Download the Slave Stack Code (SSC) Tool from EtherCAT.org website. An EtherCAT membership is necessary to download the code. The following link can be used to download the tool. EtherCAT Slave Stack Code (SSC) v5.12 . | Install the SSC Tool on the control/management PC | Open the SSC Tool and navigate to File =&gt; New to create a New Project | Select Custom and then click Import . | For Lan9252 Slave device Import “Microchip-SAMD51-LAN9252-SSC_Config.xml” configuration file from &lt; Harmony Directory &gt;/ethercat/apps/ethercat_counter_foe_app/firmware/src/slave_stack/lan9252/. For Lan9253 Slave device, there are different SSC configuration files are present at &lt; Harmony Directory &gt;/ethercat/apps/ethercat_counter_foe_app/firmware/src/slave_stack/lan9253/ , please find the below table for the available configuration file LAN9253 device - . | SSC Configuration file | Mode | Supported or Not Supported | . | Microchip-SAMD51-EtherCAT-Slave_SSC_Config_SPI_Indirect_mode.xml | SPI Indirect mode | Supported | . | Microchip-SAMD51-EtherCAT-Slave_SSC_Config_SPI_Direct_mode.xml | SPI Direct mode | Supported | . | Microchip-SAMD51-EtherCAT-Slave_SSC_Config_SPI_Beckhoff_mode.xml | SPI Beckhoff mode | Supported | . After importing this configuration file, the Microchip - EtherCAT Slave Controller &lt; Microchip &gt; will appear in the custom field. Select Microchip - EtherCAT Slave Controller &lt; Microchip &gt; and click OK. Click Yes to the next two prompts . | The driver drv_lan9252.c is required for Lan9252 slave configuration and to select file , Click yes. In the SSC tool navigate to the &lt; Harmony Directory &gt;/ethercat/apps/ethercat_counter_foe_app/firmware/src/config/sam_d51_lan9252_evb/driver/lan9252 directory to point to the drv_lan9252.c file and click Open. This will load the code. NOTE - For Lan9253 Slave configuration file, select drv_lan9253.c file from the driver/lan9253/src/drv_lan9253.c and remaining steps are similar to lan9252 steps. | Click on Tools=&gt;Application=&gt;Import and select sample_app.xlsx application file from the path &lt; Harmony Directory &gt;/ethercat/apps/ethercat_counter_foe_app/firmware/src/slave_stack/lan9252 . Click OK to the next prompt . NOTE - For Lan9253 Slave configuration file, select sample_app.xlsx application file from the path &lt; Harmony Directory &gt;/ethercat/apps/ethercat_counter_foe_app/firmware/src/slave_stack/lan9253 . | Click on Project=&gt;Create New Slave Files. When prompted to save the project, save the project in the directory provided with name SAMD51_EtherCAT_Slave.esp. | Click the Project drop-down menu in the tool bar and select Create New Slave Files. The pop-up windows open the newly slave file project ( SAMD51_EtherCAT_Slave.esp ), . | Click the Start button to create a new project file, Src folder, and ESI file (Slave Information file) in the desired directory path. | A pop-up window will indicate that the files have been successfully created. Click OK to continue. | Along with generated new slave files, ESI file (.xml file) also will be generated. This ESI file will have information about new Vendor ID and object configuration. This ESI file will be used by the TwinCAT Manager and is used to be programmed using “EEPROM Programming”. | . NOTE - EEPROM Programming is described in Running the demo application section . | Browse to the directory where the new files were created, as shown in the example: . | Src (Folder): This folder contains the Beckhoff Slave Stack code. | SAMD51_EtherCAT_Slave (ESP): This is the SSC Tool project file. | SAMD51_EtherCAT_Slave (XML): This is the EtherCAT Client information file that must be used as an input to the EtherCAT Master tool to configure EtherCAT® slave controllers. | . | The generated sample_app.c , does not have the counter, trigger configuration details and FoE read/write operations. So, the present sample_app.c, sample_app.h, sample_appObject.h can be updated from the location &lt; Harmony Directory &gt;/ethercat/apps/ethercat_counter_foe_app/firmware/src/slave_stack. | . ",
    "url": "http://localhost:4000/ethercat/apps/docs/readme_ssctool.html",
    "relUrl": "/apps/docs/readme_ssctool.html"
  },"58": {
    "doc": "EtherCAT Library and Applications",
    "title": "EtherCAT Library and Applications",
    "content": "![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png) ![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png) # MPLAB® Harmony 3 EtherCAT MPLAB® Harmony 3 is an extension of the MPLAB® ecosystem for creating embedded firmware solutions for Microchip 32-bit SAM and PIC® microcontroller and microprocessor devices. Refer to the following links for more information. - [Microchip 32-bit MCUs](https://www.microchip.com/design-centers/32-bit) - [Microchip 32-bit MPUs](https://www.microchip.com/design-centers/32-bit-mpus) - [Microchip MPLAB X IDE](https://www.microchip.com/mplab/mplab-x-ide) - [Microchip MPLAB® Harmony](https://www.microchip.com/mplab/mplab-harmony) - [Microchip MPLAB® Harmony Pages](https://microchip-mplab-harmony.github.io/) This repository contains the MPLAB® Harmony 3 EtherCAT solutions and example applications. Quickly incorporate connectivity to your designs with EtherCAT ICs, modules, software and development kits that make connecting effortless for your customers. Our comprehensive EtherCAT portfolio has the technology to meet your range, data rate, interoperability, frequency and topology needs. Refer to the following links for release notes, training materials, and interface reference information. - [Release Notes](/ethercat/release_notes.html) - [MPLAB® Harmony License](/ethercat/mplab_harmony_license.html) - [MPLAB® Harmony 3 EtherCAT Wiki](https://github.com/Microchip-MPLAB-Harmony/EtherCAT/wiki) - [MPLAB® Harmony 3 EtherCAT API Help](https://microchip-mplab-harmony.github.io/EtherCAT) # Contents Summary | Folder | Description | --- | --- | apps | Example applications for EtherCAT library components | config | EtherCAT module configuration file | docs | EtherCAT library help documentation (html format) | drivers | Driver/Firmware files for different products | # Cloning EtherCAT Microchip-MPLAB-Harmony github repo To clone or download the repo from https://github.com/Microchip-MPLAB-Harmony/ethercat , go to the main page of this repository and then click **code->clone** button to **Clone with HTTPS** to clone this repo using a git client or download the repository as a zip file. The EtherCAT , CSP , MHC and Dev Pack module repositories can also be downloaded using content manager by following the instructions in the **MPLAB® Harmony Repository Download** section in the guide available at this link: https://github.com/Microchip-MPLAB-Harmony/ethercat/wiki/create-your-first-ethercat-application. **NOTE** - Prior to using the EtherCAT demonstration, it is recommended to review the MPLAB® Harmony Release Notes (release_notes.md) for any known issues. ## EtherCAT Library and Demo Application This distribution package contains MPLAB Harmony EtherCAT firmware project that demonstrates the capabilities of Process Data Objects (counter and Trigger objects) and FoE (File Over EtherCAT, Firmware upgrade feature) . The project describes its hardware setup and requirements. The EVB-LAN9252_SAM D51 Evaluation Board is an EtherCAT Client controller with dual integrated Ethernet PHYs. Each Ethernet PHY contains a Full-Duplex 100BASE-TX transceiver and supports 100 Mbps (100BASE-TX) operation. * [EVB LAN9252 SAMD51 (LAN9252 - EtherCAT Client Controller evaluation kit with SAMD51 Microcontroller )](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/EV44C93A#:~:text=EVB%2DLAN9252%2DD51%20allows%20engineers,within%20the%20Microchip%20Harmony%20Framework) All the EtherCAT applications are available in ethercat/apps folder. All the drivers are located in the ethercat/drivers folder. The distribution package also contains details about EtherCAT Beckhoff Slave stack code generation tool and EtherCAT manager (required to verify the operation of the demo). | Name | Description | -------- | ----------- | [ethercat_library_lan925x](/ethercat/driver/readme.html) | This library explains about EtherCAT Client controller Interface layer w.r.t LAN9252/LAN9253/LAN9255 devices | [ethercat_counter_foe_app](/ethercat/apps/ethercat_counter_foe_app/readme.html)| This example application demonstrates the capable of doing firmware updates via FoE (File over EtherCAT).This also demonstrates EtherCAT TwinCAT manager and LAN925x Client communication with SAMD51 , SAME53 and LAN9255 micro-controller. | ____ [![License](https://img.shields.io/badge/license-Harmony%20license-orange.svg)](https://github.com/Microchip-MPLAB-Harmony/ethercat/blob/master/mplab_harmony_license.md) [![Latest release](https://img.shields.io/github/release/Microchip-MPLAB-Harmony/ethercat.svg)](https://github.com/Microchip-MPLAB-Harmony/ethercat/releases/latest) [![Latest release date](https://img.shields.io/github/release-date/Microchip-MPLAB-Harmony/ethercat.svg)](https://github.com/Microchip-MPLAB-Harmony/ethercat/releases/latest) [![Commit activity](https://img.shields.io/github/commit-activity/y/Microchip-MPLAB-Harmony/ethercat.svg)](https://github.com/Microchip-MPLAB-Harmony/ethercat/graphs/commit-activity) [![Contributors](https://img.shields.io/github/contributors-anon/Microchip-MPLAB-Harmony/ethercat.svg)]() ____ [![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/user/MicrochipTechnology) [![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/microchip-technology) [![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/microchiptechnology/) [![Follow us on Twitter](https://img.shields.io/twitter/follow/MicrochipTech.svg?style=social)](https://twitter.com/MicrochipTech) [![](https://img.shields.io/github/stars/Microchip-MPLAB-Harmony/ethercat.svg?style=social)]() [![](https://img.shields.io/github/watchers/Microchip-MPLAB-Harmony/ethercat.svg?style=social)]() ",
    "url": "http://localhost:4000/ethercat/",
    "relUrl": "/"
  }
}
`;
var data_for_search

var repo_name = "ethercat";
var doc_folder_name = "docs";
var localhost_path = "http://localhost:4000/";
var home_index_string = "EtherCAT Library and Applications";

(function (jtd, undefined) {

// Event handling

jtd.addEvent = function(el, type, handler) {
  if (el.attachEvent) el.attachEvent('on'+type, handler); else el.addEventListener(type, handler);
}
jtd.removeEvent = function(el, type, handler) {
  if (el.detachEvent) el.detachEvent('on'+type, handler); else el.removeEventListener(type, handler);
}
jtd.onReady = function(ready) {
  // in case the document is already rendered
  if (document.readyState!='loading') ready();
  // modern browsers
  else if (document.addEventListener) document.addEventListener('DOMContentLoaded', ready);
  // IE <= 8
  else document.attachEvent('onreadystatechange', function(){
      if (document.readyState=='complete') ready();
  });
}

// Show/hide mobile menu

function initNav() {
  jtd.addEvent(document, 'click', function(e){
    var target = e.target;
    while (target && !(target.classList && target.classList.contains('nav-list-expander'))) {
      target = target.parentNode;
    }
    if (target) {
      e.preventDefault();
      target.parentNode.classList.toggle('active');
    }
  });

  const siteNav = document.getElementById('site-nav');
  const mainHeader = document.getElementById('main-header');
  const menuButton = document.getElementById('menu-button');

  jtd.addEvent(menuButton, 'click', function(e){
    e.preventDefault();

    if (menuButton.classList.toggle('nav-open')) {
      siteNav.classList.add('nav-open');
      mainHeader.classList.add('nav-open');
    } else {
      siteNav.classList.remove('nav-open');
      mainHeader.classList.remove('nav-open');
    }
  });
}
// Site search

function initSearch() {

    data_for_search = JSON.parse(myVariable);
    lunr.tokenizer.separator = /[\s/]+/

    var index = lunr(function () {
        this.ref('id');
        this.field('title', { boost: 200 });
        this.field('content', { boost: 2 });
        this.field('url');
        this.metadataWhitelist = ['position']

        var location = document.location.pathname;
        var path = location.substring(0, location.lastIndexOf("/"));
        var directoryName = path.substring(path.lastIndexOf("/")+1);

        var cur_path_from_repo = path.substring(path.lastIndexOf(repo_name));

        // Decrement depth by 2 as HTML files are placed in repo_name/doc_folder_name
        var cur_depth_from_doc_folder = (cur_path_from_repo.split("/").length - 2);

        var rel_path_to_doc_folder = "";

        if (cur_depth_from_doc_folder == 0) {
            rel_path_to_doc_folder = "./"
        }
        else {
            for (var i = 0; i < cur_depth_from_doc_folder; i++)
            {
                rel_path_to_doc_folder = rel_path_to_doc_folder + "../"
            }
        }

        for (var i in data_for_search) {

            data_for_search[i].url = data_for_search[i].url.replace(localhost_path + repo_name, rel_path_to_doc_folder);

            if (data_for_search[i].title == home_index_string)
            {
                data_for_search[i].url = data_for_search[i].url + "index.html"
            }

            this.add({
                id: i,
                title: data_for_search[i].title,
                content: data_for_search[i].content,
                url: data_for_search[i].url
            });
        }
    });

    searchLoaded(index, data_for_search);
}function searchLoaded(index, docs) {
  var index = index;
  var docs = docs;
  var searchInput = document.getElementById('search-input');
  var searchResults = document.getElementById('search-results');
  var mainHeader = document.getElementById('main-header');
  var currentInput;
  var currentSearchIndex = 0;

  function showSearch() {
    document.documentElement.classList.add('search-active');
  }

  function hideSearch() {
    document.documentElement.classList.remove('search-active');
  }

  function update() {
    currentSearchIndex++;

    var input = searchInput.value;
    if (input === '') {
      hideSearch();
    } else {
      showSearch();
      // scroll search input into view, workaround for iOS Safari
      window.scroll(0, -1);
      setTimeout(function(){ window.scroll(0, 0); }, 0);
    }
    if (input === currentInput) {
      return;
    }
    currentInput = input;
    searchResults.innerHTML = '';
    if (input === '') {
      return;
    }

    var results = index.query(function (query) {
      var tokens = lunr.tokenizer(input)
      query.term(tokens, {
        boost: 10
      });
      query.term(tokens, {
        wildcard: lunr.Query.wildcard.TRAILING
      });
    });

    if ((results.length == 0) && (input.length > 2)) {
      var tokens = lunr.tokenizer(input).filter(function(token, i) {
        return token.str.length < 20;
      })
      if (tokens.length > 0) {
        results = index.query(function (query) {
          query.term(tokens, {
            editDistance: Math.round(Math.sqrt(input.length / 2 - 1))
          });
        });
      }
    }

    if (results.length == 0) {
      var noResultsDiv = document.createElement('div');
      noResultsDiv.classList.add('search-no-result');
      noResultsDiv.innerText = 'No results found';
      searchResults.appendChild(noResultsDiv);

    } else {
      var resultsList = document.createElement('ul');
      resultsList.classList.add('search-results-list');
      searchResults.appendChild(resultsList);

      addResults(resultsList, results, 0, 10, 100, currentSearchIndex);
    }

    function addResults(resultsList, results, start, batchSize, batchMillis, searchIndex) {
      if (searchIndex != currentSearchIndex) {
        return;
      }
      for (var i = start; i < (start + batchSize); i++) {
        if (i == results.length) {
          return;
        }
        addResult(resultsList, results[i]);
      }
      setTimeout(function() {
        addResults(resultsList, results, start + batchSize, batchSize, batchMillis, searchIndex);
      }, batchMillis);
    }

    function addResult(resultsList, result) {
      var doc = docs[result.ref];

      var resultsListItem = document.createElement('li');
      resultsListItem.classList.add('search-results-list-item');
      resultsList.appendChild(resultsListItem);

      var resultLink = document.createElement('a');
      resultLink.classList.add('search-result');
      resultLink.setAttribute('href', doc.url);
      resultsListItem.appendChild(resultLink);

      var resultTitle = document.createElement('div');
      resultTitle.classList.add('search-result-title');
      resultLink.appendChild(resultTitle);

      var resultDoc = document.createElement('div');
      resultDoc.classList.add('search-result-doc');
      resultDoc.innerHTML = '<svg viewBox="0 0 24 24" class="search-result-icon"><use xlink:href="#svg-doc"></use></svg>';
      resultTitle.appendChild(resultDoc);

      var resultDocTitle = document.createElement('div');
      resultDocTitle.classList.add('search-result-doc-title');
      resultDocTitle.innerHTML = doc.doc;
      resultDoc.appendChild(resultDocTitle);
      var resultDocOrSection = resultDocTitle;

      if (doc.doc != doc.title) {
        resultDoc.classList.add('search-result-doc-parent');
        var resultSection = document.createElement('div');
        resultSection.classList.add('search-result-section');
        resultSection.innerHTML = doc.title;
        resultTitle.appendChild(resultSection);
        resultDocOrSection = resultSection;
      }

      var metadata = result.matchData.metadata;
      var titlePositions = [];
      var contentPositions = [];
      for (var j in metadata) {
        var meta = metadata[j];
        if (meta.title) {
          var positions = meta.title.position;
          for (var k in positions) {
            titlePositions.push(positions[k]);
          }
        }
        if (meta.content) {
          var positions = meta.content.position;
          for (var k in positions) {
            var position = positions[k];
            var previewStart = position[0];
            var previewEnd = position[0] + position[1];
            var ellipsesBefore = true;
            var ellipsesAfter = true;
            for (var k = 0; k < 5; k++) {
              var nextSpace = doc.content.lastIndexOf(' ', previewStart - 2);
              var nextDot = doc.content.lastIndexOf('. ', previewStart - 2);
              if ((nextDot >= 0) && (nextDot > nextSpace)) {
                previewStart = nextDot + 1;
                ellipsesBefore = false;
                break;
              }
              if (nextSpace < 0) {
                previewStart = 0;
                ellipsesBefore = false;
                break;
              }
              previewStart = nextSpace + 1;
            }
            for (var k = 0; k < 10; k++) {
              var nextSpace = doc.content.indexOf(' ', previewEnd + 1);
              var nextDot = doc.content.indexOf('. ', previewEnd + 1);
              if ((nextDot >= 0) && (nextDot < nextSpace)) {
                previewEnd = nextDot;
                ellipsesAfter = false;
                break;
              }
              if (nextSpace < 0) {
                previewEnd = doc.content.length;
                ellipsesAfter = false;
                break;
              }
              previewEnd = nextSpace;
            }
            contentPositions.push({
              highlight: position,
              previewStart: previewStart, previewEnd: previewEnd,
              ellipsesBefore: ellipsesBefore, ellipsesAfter: ellipsesAfter
            });
          }
        }
      }

      if (titlePositions.length > 0) {
        titlePositions.sort(function(p1, p2){ return p1[0] - p2[0] });
        resultDocOrSection.innerHTML = '';
        addHighlightedText(resultDocOrSection, doc.title, 0, doc.title.length, titlePositions);
      }

      if (contentPositions.length > 0) {
        contentPositions.sort(function(p1, p2){ return p1.highlight[0] - p2.highlight[0] });
        var contentPosition = contentPositions[0];
        var previewPosition = {
          highlight: [contentPosition.highlight],
          previewStart: contentPosition.previewStart, previewEnd: contentPosition.previewEnd,
          ellipsesBefore: contentPosition.ellipsesBefore, ellipsesAfter: contentPosition.ellipsesAfter
        };
        var previewPositions = [previewPosition];
        for (var j = 1; j < contentPositions.length; j++) {
          contentPosition = contentPositions[j];
          if (previewPosition.previewEnd < contentPosition.previewStart) {
            previewPosition = {
              highlight: [contentPosition.highlight],
              previewStart: contentPosition.previewStart, previewEnd: contentPosition.previewEnd,
              ellipsesBefore: contentPosition.ellipsesBefore, ellipsesAfter: contentPosition.ellipsesAfter
            }
            previewPositions.push(previewPosition);
          } else {
            previewPosition.highlight.push(contentPosition.highlight);
            previewPosition.previewEnd = contentPosition.previewEnd;
            previewPosition.ellipsesAfter = contentPosition.ellipsesAfter;
          }
        }

        var resultPreviews = document.createElement('div');
        resultPreviews.classList.add('search-result-previews');
        resultLink.appendChild(resultPreviews);

        var content = doc.content;
        for (var j = 0; j < Math.min(previewPositions.length, 3); j++) {
          var position = previewPositions[j];

          var resultPreview = document.createElement('div');
          resultPreview.classList.add('search-result-preview');
          resultPreviews.appendChild(resultPreview);

          if (position.ellipsesBefore) {
            resultPreview.appendChild(document.createTextNode('... '));
          }
          addHighlightedText(resultPreview, content, position.previewStart, position.previewEnd, position.highlight);
          if (position.ellipsesAfter) {
            resultPreview.appendChild(document.createTextNode(' ...'));
          }
        }
      }
      var resultRelUrl = document.createElement('span');
      resultRelUrl.classList.add('search-result-rel-url');
      resultRelUrl.innerText = doc.relUrl;
      resultTitle.appendChild(resultRelUrl);
    }

    function addHighlightedText(parent, text, start, end, positions) {
      var index = start;
      for (var i in positions) {
        var position = positions[i];
        var span = document.createElement('span');
        span.innerHTML = text.substring(index, position[0]);
        parent.appendChild(span);
        index = position[0] + position[1];
        var highlight = document.createElement('span');
        highlight.classList.add('search-result-highlight');
        highlight.innerHTML = text.substring(position[0], index);
        parent.appendChild(highlight);
      }
      var span = document.createElement('span');
      span.innerHTML = text.substring(index, end);
      parent.appendChild(span);
    }
  }

  jtd.addEvent(searchInput, 'focus', function(){
    setTimeout(update, 0);
  });

  jtd.addEvent(searchInput, 'keyup', function(e){
    switch (e.keyCode) {
      case 27: // When esc key is pressed, hide the results and clear the field
        searchInput.value = '';
        break;
      case 38: // arrow up
      case 40: // arrow down
      case 13: // enter
        e.preventDefault();
        return;
    }
    update();
  });

  jtd.addEvent(searchInput, 'keydown', function(e){
    switch (e.keyCode) {
      case 38: // arrow up
        e.preventDefault();
        var active = document.querySelector('.search-result.active');
        if (active) {
          active.classList.remove('active');
          if (active.parentElement.previousSibling) {
            var previous = active.parentElement.previousSibling.querySelector('.search-result');
            previous.classList.add('active');
          }
        }
        return;
      case 40: // arrow down
        e.preventDefault();
        var active = document.querySelector('.search-result.active');
        if (active) {
          if (active.parentElement.nextSibling) {
            var next = active.parentElement.nextSibling.querySelector('.search-result');
            active.classList.remove('active');
            next.classList.add('active');
          }
        } else {
          var next = document.querySelector('.search-result');
          if (next) {
            next.classList.add('active');
          }
        }
        return;
      case 13: // enter
        e.preventDefault();
        var active = document.querySelector('.search-result.active');
        if (active) {
          active.click();
        } else {
          var first = document.querySelector('.search-result');
          if (first) {
            first.click();
          }
        }
        return;
    }
  });

  jtd.addEvent(document, 'click', function(e){
    if (e.target != searchInput) {
      hideSearch();
    }
  });
}

// Switch theme

jtd.getTheme = function() {
  var cssFileHref = document.querySelector('[rel="stylesheet"]').getAttribute('href');
  return cssFileHref.substring(cssFileHref.lastIndexOf('-') + 1, cssFileHref.length - 4);
}

jtd.setTheme = function(theme) {
  var cssFile = document.querySelector('[rel="stylesheet"]');
  cssFile.setAttribute('href', 'http://localhost:4000/ethercat/assets/css/just-the-docs-' + theme + '.css');
}

// Document ready

jtd.onReady(function(){
  initNav();
  initSearch();
});

})(window.jtd = window.jtd || {});


