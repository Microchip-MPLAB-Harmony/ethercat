var myVariable = `
{
  
  "0": {
    "title": "EtherCAT Counter FoE Application",
    "content": ". EtherCAT Counter FoE Application . This EtherCAT example application demonstrates the capable of doing firmware updates via FoE (File over EtherCAT) and EtherCAT Master ( TwinCAT Master ) slave ( EtherCAT LAN925x) communication with EVB_LAN925x_SAMD51 micro-controller. Microchip’s EtherCAT provides the information about how to configure and run the application on different EtherCAT platform. . Target Development board Description . EVB_LAN9252_SAMD51 | ETherCAT evaluation board with LAN9252 | .",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/readme.html",
    "relUrl": "/apps/ethercat_counter_foe_app/readme.html"
  }
  ,"1": {
    "title": "EtherCAT LAN925x Library",
    "content": ". EtherCAT LAN925x Library . Ethernet for Control Automation Technology (EtherCAT) was developed by Beckhoff. EtherCAT is a fast and deterministic network, and processes data using dedicated hardware and software. It uses a full duplex, master-slave configuration. . The LAN925x is a 2/3-port EtherCAT slave controller with dual integrated Ethernet PHYs which each contain a fullduplex 100BASE-TX transceiver and support 100Mbps (100BASE-TX) operation. . EtherCAT Slave Controller Interface layer is designed for Microchip MCUs to communicate with LAN9252 EtherCAT Slave controller. This EtherCAT framework is configured with the communication interfaces (QSPI ( SPI mode), GPIO). This communication Interface layer which will acts like bridge between EtherCAT Slave Stack Code (SSC) and Host peripherals to communicate with LAN9252. SSC Stack (generated from standard SSC tool) have feature to transfer the File over EtherCAT (FoE) for MCU firmware upgrade from TwinCAT tool. . . Use of the Microchip EVB-LAN9252 (and similar EtherCAT interface devices) requires use of the Beckhoff EtherCAT Slave Stack Code (SSC) and its associated configuration and code generation tool. . The interrupts have to be configured during hardware initialization. PDI Interrupt - The programmable system interrupts are generated internally by the various device sub-modules and can be configured to generate a single external host interrupt via the IRQ interrupt output pin. That is TwinCAT manager with enabled Synchron Synchronization mode. . | DC - SYNC0 AND SYNC1 - If the application running on the SOC requires Distributed clock, then SYNC0 and SYNC1 should be connected to the microcontroller’s interrupts lines. Refer to LAN9252 datasheet for configuration of SYNC0 and SYNC1. . | TIMER Interupt - SSC has a variable which will count every millisecond, which can be implemented either timer interrupt or polling method. The interrupt/polling mode can be selected in the SSC Tool before creating the slave stack code. SSC will access EtherCAT core registers from both interrupt context and polling mode. So, the ECAT_CSR_CMD and ECAT_CSR_DATA registers has to be protected against simultaneous access which can corrupt the state machine inside the slave stack code. . | . | Configure of Library . Configure the peripheral library using the MHC. EtherCAT Slave Device Indicators This section describes the LAN9252 driver and EtherCAT trigger and counter variables are used to support visual inspection and troubleshooting of the driver and networks. . PDO ( Process Data Object Trigger and Counter ) - The PDO protocol is used for communication with External interrupt IRQ. . Index Object Type Direction . 1 | 0x6000 | 32bit Counter | TX | . | Receive PDO Mapping ( Outputs ) - The PDO protocol is used for communication with External interrupt IRQ. . Index Object Type Direction . 1 | 0x7010 | 32bit Counter | RX | . | Synchronization - The Beckhoff Slave Stack Code supports different modes of synchronization which are based on three physical signals: (PDI_)IRQ, Sync0 and Sync1 Microchip EtherCAT device also supports different modes. . Master and Slaves for synchronization: Free run The Master cycle time and Slave cycle time are independent. &lt; This mode is Supported &gt; | IRQ interrupt event is triggered from Master for SM-Synchron Synchronization. &lt; This mode is supported &gt; | Both IRQ and SYNC0 event occurs for the DC-Synchron operation mode and SYNC0 and SYNC1 unit cycle time is configured 1000µs. &lt; This mode is supported &gt; | . | | Using Library . File over EtherCAT (FoE) - Architecture – Host/Slave interaction states for firmware update . . FOE demostartion with respect to ATSAMD51J19A device - . . Master changes from INIT to BOOT, then download of a file initiated. Wen state changes from INIT to BOOT, slave or the FoE application is ready to write the New FW or bin file at the BankB location. Flag gFirmwareDownload_Started set to 1. . | Master initiates Download command with password value as 0x11223344 and bin file (this file is generated from the existing D51 foe studio project). In the slave FOE_FILEDOWNLOAD_PASSWORD is set to 0x11223344. FoE_Write() checks file name size ( limited to 16) and checks password value and then proceed to FoE_WriteData(). . | Once Master completes bin file transfer, then Master need to change the state from BOOT to INIT. In the FW application or slave code, APP_FW_GetDownloadStateFinished() changes state to gFirmwareDownload_Started = 0 and APP_FW_StateBOOTtoINIT() changes gFirmwareDownload_Finished = 1. . | Once State changes from BOOT to INIT, Slave application initiates bank switch application(APP_BankSwitch()) and software reset(run_application()). Then Bank A moves to BANK B location and the new application stored in BANK B , moved to BANK A with flash location 0x0. The new application starts running from flash location 0x0 and will be ready with new application. . | . SSC tool FoE configuration - . . | .",
    "url": "http://localhost:4000/ethercat/driver/readme.html",
    "relUrl": "/driver/readme.html"
  }
  ,"2": {
    "title": "EVB LAN9252 SAMD51 Application",
    "content": ". EtherCAT Counter FoE Application for EVM_LAN9252_SAMD51 . This EtherCAT example application demonstrates the capable of doing firmware updates via FoE (File over EtherCAT) and EtherCAT Master ( TwinCAT Master ) slave ( EtherCAT LAN9252) communication with EVB_LAN9252_SAMD51 micro-controller. Microchip’s EtherCAT provides the information about how to configure and run the application on different EtherCAT platform. . MPLAB® Harmony Software Setup | Hardware Setup | MPLAB Harmony configuration | Build The EtherCAT Application | Running The Application Slave Stack Code (SSC) Generation | TwinCAT Manager and Microchip EtherCAT Slave communication TwinCAT EtherCAT interface detect and EEPROM Programming | Trigger and Counter Demonstration | File over EtherCAT communication | | | MPLAB® Harmony Software Setup . The following MPLAB® software components are a prerequisite for the rest of the steps in this demonstration. Please follow the download and installation instructions available at below links. . MPLAB® X Integrated Development Environment | MPLAB® XC32/32++ C Compiler | MPLAB® Harmony Configurator | On the management PC, download and Install on the TwinCAT 3 Engineering Full Setup at https://www.beckhoff.com/english.asp?download/tc3-download-xae.htm. Select the latest TwinCAT 3.1 Version and click on the link. Note the dialog box shows the TwinCAT tool that will be installed TC31-Full-Setup.3.1.XXXX.XX and click on “Start Download”. Follow instructions to download. | . Hardware Setup . The following tools can be used to program and debug the application on the target hardware. . MPLAB® ICD4 + ICD4/PICKIT 4 Target Adapter Board using JTAG interface. | . The following development board will be used to develop and run EtherCAT application for the SAMD51 device family. The instructions in this guide are applicable to any of these development boards. Hardware settings are board dependent and may vary between boards. . SAMD51 LAN9252 EVB ( correct web link need to be added ) . | Connect a micro USB cable to J8 port for power source. | For programming, Connect a ICD4 JTAG cable to the J10 port of the EVB_LAN9252_SAMD51. | RJ45 connector is connected to the TwinCAT Manager using J1 port. | . . Block diagram of the EVB_LAN9252_SAMD51 board - | . . Complete HW Setup | . . MPLAB Harmony configuration . There are two options available for downloading/installing the MPLAB® Harmony Software Repositories from github &amp; gitee. . The required repositories can be cloned from the github (or gitee) by using a local git client (such git bash). The MPLAB® Harmony Repositories are available at the following links: . https://github.com/Microchip-MPLAB-Harmony/ . https://gitee.com/Microchip-MPLAB-Harmony/ . The csp, dev_packs, mhc and ethercat repositories should be cloned. The repositories can also be cloned (downloaded) or previously downloaded repositories can be updated by using the MPLAB® Harmony 3 Content Manager. The following sections provide details on using the MPLAB® Harmony 3 Content Manager to download the repositories. . Refer to the EtherCAT MPLAB® Harmony Software Setup https://github.com/Microchip-MPLAB-Harmony/ethercat/wiki/create-your-first-ethercat-application details to create a EtherCAT project. . | The following Project Graph diagram shows the required Harmony components which are included for the present EtherCAT application for the EVB_LAN9252_SAMD51. . . | Click on the EtherCAT Stack in the Project Graph window. In the Configuration window, The Slave Stack source directory path points to the folder that contains the files generated by the SSC tool. | Enable FoE checkbox enables File over Ethercat feature. | . . | Click on LAN9252 component from the project graph. EtherCAT Interrupt Priority Level :- This is the range, equal to or more than this value, all the interrupts will be disabled during EtherCAT interrupt handler in process. | Following table maps EtherCAT interrupt name with respective peripheral channel selection. EIC interrupt handler and the SPI chip select Configuration for EVB_LAN9252_SAMD51 . Interrupt Name EIC Channel . SYNC0 Interrupt | EIC_EXTINT0 | . SYNC1 Interrupt | EIC_EXTINT1 | . IRQ Interrupt | EIC_EXTINT7 | . Other Usage Port Pin . SPI Chip Select | PORT RB11 | . | . NOTE - As per the Microcontroller and LAN9252 interrupt support, EIC/GPIO/PIO pins can be selected for External event registration and event handler processing. . . | EIC, QSPI and TC0 are configured as per the application requirement. . | These are the below PINs configured for the application * QSPI PIN Configuration for EVB_LAN9252_SAMD51 . | QSPI Custom Name | Function | PORT PIN ID | |:--:|:-:|::| | QSPI_MOSI | QSPI_DATA0 | PA08 | | QSPI_MISO | QSPI_DATA1 | PA09 | | QSPI_SCK | QSPI_SCK | PB10 | | SPI_CS | GPIO | PB11 | . PIN Configuration for EtherCAT External Interrupt Pins for EVB_LAN9252_SAMD51 . EIC Custom Name EIC Channel Port Pin . EIC_EXTINT0 | EIC_EXTINT0 | PORT PB16 | . EIC_EXTINT1 | EIC_EXTINT1 | PORT PB17 | . EIC_EXTINT7 | EIC_EXTINT7 | PORT PB23 | . | . . | Open NVIC configuration window from MHC→Tools. The QSPI Interrupt Priority Level to 2 and it is below to the configured parameter of the EtherCAT Interrupt Priority Level from LAN9252 component . . . | The application will use the default clock options. No changes are required in clock settings. . | Generate the code by clicking the Generate Code button (marked in red). . . | This following diagram presents the One Click generated EtherCAT project, which is without Slave stack code except sample_app.c file. Slave stack code will be generated using SSC tool with the required configuration and Microchip-SAMD51-EtherCAT-Slave_SSC_Config.xml file. Right side is the EtherCAT project with supported SSC stack files. . . | Building The EtherCAT Application . This section identifies the MPLAB X IDE projects for the ethercat_counter_foe_app demonstration application. The following table list the MPLAB X projects available for this demonstration. These projects are available inside &lt; install-dir &gt;/ethercat/apps/ethercat_counter_foe_app/firmware . . MPLABx project table . Project Name Target Device Target Development board Description . sam_d51_lan9252_evb.X | ATSAMD51J19A | EVB-LAN9252_SAMD51 | ETherCAT evaluation board with LAN9252 | . | At this point, the project will contain the MPLAB® Harmony components required for an EtherCAT application. The EtherCAT application files can now be added to the project. . The MPLAB® Harmony EtherCAT repository contains an EtherCAT demo application. The source files for which are available in Harmony_Repo_Path/h3/ethercat/apps/ethercat_counter_foe_app/firmware/src. . | The app.c, app.h, main.c , and ethercat_foe.ld files are updated to demonstrate the application. The ethercat_foe.ld - is an updated linker file which is used for FoE demonstration. ROM_LENGTH of the linker file is modified to 0x40000 for ATSAMD51J19A. The memory mapped for Bank A is from 0x00000 to 0x3FFFF and the memory mapped for Bank B is from 0x40000 to 0x7FFFF. . . | The sample application files sample_app.c, sample_app.h and sample_appObjects.h which are generated by SSC tool and are updated for the application requirement. . . These are the above files which are already shared with EtherCAT ethercat_counter_foe_app demo release. . | Verify the XC32 Compiler Toolchain version and set the Connected Hardware Tool to ICD4 or PICkit 3. Press Apply button and then press OK button. . | Build the application by clicking on the Build Main Project. . | Running The Application . Slave Stack Code (SSC) Generation . Steps to generate Slave Stack Code . TwinCAT Manager and Microchip EtherCAT Slave communication . TwinCAT EtherCAT interface detect and EEPROM Programming . Upon successful installation of the TwinCAT Manager, the network adapter will be moved to Installed and ready to use devices section as shown in the following figure. . . Copy the available SAMD51_EtherCAT_Slave.xml file from &lt; harmony-repo &gt;/ethercat/slave_stack directory to the TwinCAT 3.1 Config Io EtherCAT directory. . | In TwinCAT XAE, create a New Project (File=&gt;New=&gt;Project). In the New Project Window, select the TwinCAT Projects option and then click OK . . | Connect Port zero of the device to the EtherCAT master using a RJ45 Ethernet cable, and then power up the board. The Link/Act LED should be ON at Port zero when the cable is connected. If the Link/Act LED is not ON, this indicates that there is an issue with the connection or the cable. . | Expand the IO option in the TwinCAT XAE project window and right click on Devices. Select Scan. . . | Click OK if the TwinCAT network interface is selected and continue scanning as shown in the below image. . . | Click OK to continue scanning and Click Yes in the Scan for boxes prompt. . . | EEPROM Programming After a successful scan, click on Device 2 (EtherCAT) in the solution explorer window of the TwinCAT tool and Click Online in the TwinCAT project window. Once this is done, highlight the Device, and it should read OP. . . | Trigger and Counter Demonstration . Input Trigger configuration - From I/O level on the Solution Explorer (left window), go to Devices=&gt;Device # (EtherCAT)=&gt;Box 1 (SAMD51 EtherCAT Slave)=&gt;Outputs process data mapping=&gt;Trigger. . | In the top center window, select Online tab. Click Write and then enter 1 in Decimal: field and click OK. . . | Output Counter Observation - From I/O level on the Solution Explorer (left window), go to Devices=&gt;Device # (EtherCAT)=&gt;Box 1 (SAMD51 EtherCAT Slave)=&gt;Inputs process data mapping=&gt;Counter. . | In the top center window, select Online tab. The counter should be incrementing. . . | File over EtherCAT communication . FoE (File over EtherCAT) Test Update - Click on Box1, Select “Online” tab. Before FoE test Curent state and Requested state should be in OP mode. . . | For file download, change the mode from INIT to BOOT mode. . . | After clicking Download button , select BIN file that need to be downloaded and configure the Password as per the EtherCAT component password configuration. . . One can find the sample BIN files in **/ethercat/apps/ethercat_counter_foe_app/firmware/src/FoE_Hex_imagefiles** which can be used for the FoE verification. . NOTE - One can generate BIN file from the generated EtheCAT HEX image after configuring the Project Properties-&gt;Conf:-&gt;Building-&gt;Execute This Line After Build \${MP_CC_DIR}/xc32-objcopy” -I ihex -O binary “\${DISTDIR}/\${PROJECTNAME}.\${IMAGE_TYPE}.hex” “\${DISTDIR}/\${PROJECTNAME}.\${IMAGE_TYPE}.bin . . NOTE - Please uncheck Execute This Line After Build option for a debug session of this project. . | The download process can be tracked as shown in the figure below- . . | After successful download, change the state from BOOT to INIT mode. This helps the application switch the bank from BANK B to BANK A and the newly downloaded firmware will be executed from BANK A. The new application starts running from flash location 0x0. . . |",
    "url": "http://localhost:4000/ethercat/apps/ethercat_counter_foe_app/docs/readme_lan9252_samd51.html",
    "relUrl": "/apps/ethercat_counter_foe_app/docs/readme_lan9252_samd51.html"
  }
  ,"3": {
    "title": "Slave Stack Code Generation",
    "content": "Slave Stack Code Generation . Download the Slave Stack Code (SSC) Tool from EtherCAT.org website. An EtherCAT membership is necessary to download the code. The following link can be used to download the tool. . EtherCAT Slave Stack Code (SSC) v5.12 . | Install the SSC Tool on the control/management PC | Open the SSC Tool and navigate to File =&gt; New to create a New Project | Select Custom and then click Import . . | Import “Microchip-SAMD51-LAN925x-SSC_Config.xml” configuration file from &lt; Harmony Directory &gt;/ethercat/apps/ethercat_counter_foe_app/firmware/src/slave_stack. After importing this configuration file, the Microchip - EtherCAT Slave Controller &lt; Microchip &gt; will appear in the custom field. . . Select this and click OK. Click Yes to the next two prompts . . | The driver drv_lan9252.c is required for this configuration and to select file , Click yes. In the SSC tool navigate to the &lt; Harmony Directory &gt;/ethercat/apps/ethercat_counter_foe_app/firmware/src/config/sam_d51_lan9252_evb/driver/lan9252 directory to point to the drv_lan9252.c file and click Open. This will load the code. . . | Click on Tools=&gt;Application=&gt;Import and select sample_app.xlsx application file from the path &lt; Harmony Directory &gt;/ethercat/apps/ethercat_counter_foe_app/firmware/src/slave_stack . Click OK to the next prompt . . | Click on Tools=&gt;Create New Slave Files. When prompted to save the project, save the project in the directory provided with name SAMD51_EtherCAT_Slave.esp. Click the Project drop-down menu in the tool bar and select Create New Slave Files. The pop-up windows open the newly slave file project ( SAMD51_EtherCAT_Slave.esp ), . . | Click the Start button to create a new project file, Src folder, and ESI file (Slave Information file) in the desired directory path. . | A pop-up window will indicate that the files have been successfully created. Click OK to continue. . | Along with generated new slave files, ESI file (.xml file) also will be generated. This ESI file will have information about new Vendor ID and object configuration. This ESI file will be used by the TwinCAT Manager and is used to be programmed using “EEPROM Programming”. . | . NOTE - EEPROM Programming is described in Running the demo application section . | Browse to the directory where the new files were created, as shown in the example: Src (Folder): This folder contains the Beckhoff Slave Stack code. | SAMD51_EtherCAT_Slave (ESP): This is the SSC Tool project file. | SAMD51_EtherCAT_Slave (XML): This is the EtherCAT® slave information file that must be used as an input to the EtherCAT master tool to configure EtherCAT® slave controllers. | . | The generated sample_app.c , does not have the counter, trigger configuration details and FoE read/write operations. So, the present sample_app.c, sample_app.h, sample_appObject.h can be updated from the location &lt; Harmony Directory &gt;/ethercat/apps/ethercat_counter_foe_app/firmware/src/slave_stack. |",
    "url": "http://localhost:4000/ethercat/apps/docs/readme_ssctool.html",
    "relUrl": "/apps/docs/readme_ssctool.html"
  }
  ,"4": {
    "title": "",
    "content": "IMPORTANT: READ CAREFULLY . MICROCHIP IS WILLING TO LICENSE THIS INTEGRATED SOFTWARE FRAMEWORK SOFTWARE AND ACCOMPANYING DOCUMENTATION OFFERED TO YOU ONLY ON THE CONDITION THAT YOU ACCEPT ALL OF THE FOLLOWING TERMS. TO ACCEPT THE TERMS OF THIS LICENSE, CLICK “I ACCEPT” AND PROCEED WITH THE DOWNLOAD OR INSTALL. IF YOU DO NOT ACCEPT THESE LICENSE TERMS, CLICK “I DO NOT ACCEPT,” AND DO NOT DOWNLOAD OR INSTALL THIS SOFTWARE. . NON-EXCLUSIVE SOFTWARE LICENSE AGREEMENT FOR MICROCHIP MPLAB HARMONY INTEGRATED SOFTWARE FRAMEWORK . This Nonexclusive Software License Agreement (“Agreement”) is between you, your heirs, agents, successors and assigns (“Licensee”) and Microchip Technology Incorporated, a Delaware corporation, with a principal place of business at 2355 W. Chandler Blvd., Chandler, AZ 85224-6199, and its subsidiary, Microchip Technology (Barbados) II Incorporated (collectively, “Microchip”) for Microchip’s MPLAB Harmony Integrated Software Framework (“Software”) and accompanying documentation (“Documentation”). The Software and Documentation are licensed under this Agreement and not sold. U.S. copyright laws and international copyright treaties, and other intellectual property laws and treaties protect the Software and Documentation. Microchip reserves all rights not expressly granted to Licensee in this Agreement. . License and Sublicense Grant. . (a) Definitions. As used this Agreement, the following terms shall have the meanings defined below: . (i) &quot;Licensee Products&quot; means Licensee products that use or incorporate Microchip Products. (ii) &quot;Microchip Product&quot; means Microchip 16-bit and 32-bit microcontrollers, digital signal controllers or other Microchip semiconductor products with PIC16 and PIC18 prefix and specifically excepting the CX870 and CY920, which are not covered under this Agreement, that use or implement the Software. (iii) &quot;Object Code&quot; means the Software computer programming code provided by Microchip that is in binary form (including related documentation, if any) and error corrections, improvements and updates to such code provided by Microchip in its sole discretion, if any. (iv) &quot;Source Code&quot; means the Software computer programming code provided by Microchip that may be printed out or displayed in human readable form (including related programmer comments and documentation, if any), and error corrections, improvements, updates, modifications and derivatives of such code developed by Microchip, Licensee or Third Party. (v) &quot;Third Party&quot; means Licensee&#39;s agents, representatives, consultants, clients, customers, or contract manufacturers. (vi) &quot;Third Party Products&quot; means Third Party products that use or incorporate Microchip Products. . (b) Software License Grant. Subject to the terms of this Agreement, Microchip grants strictly to Licensee a personal, worldwide, non-exclusive, non-transferable limited license to use, modify (except as limited by Section 1(f) below), copy and distribute the Software only when the Software is embedded on a Microchip Product that is integrated into Licensee Product or Third Party Product pursuant to Section 2(d) below. . Any portion of the Software (including derivatives or modifications thereof) may not be: . (i) embedded on a non-Microchip microcontroller or digital signal controller; (ii) distributed (in Source Code or Object Code), except as described in Section 2(d) below. . (c) Documentation License Grant. Subject to all of the terms and conditions of this Agreement, Microchip grants strictly to Licensee a perpetual, worldwide, non-exclusive license to use the Documentation in support of Licensee’s use of the Software. . (d) Sublicense Grants. Subject to terms of this Agreement, Licensee may grant a limited sublicense to a Third Party to use the Software as described below only if such Third Party expressly agrees to be bound by terms of confidentiality and limited use that are no broader in scope and duration than the confidentiality and limited use terms of this Agreement: . (i) Third Party may modify Source Code for Licensee, except as limited by Section 1(f) below. (ii) Third Party may program Software into Microchip Products for Licensee. (iii) Third Party may use Software to develop and/or manufacture Licensee Product. (iv) Third Party may use Software to develop and/or manufacture Third Party Products where either: (x) the sublicensed Software contains Source Code modified or otherwise optimized by Licensee for Third Party use; or (y) the sublicensed Software is programmed into Microchip Products by Licensee on behalf of such Third Party. (v) Third Party may use the Documentation in support of Third Party&#39;s authorized use of the Software in conformance with this Section 2(d). . (e) Audit. Authorized representatives of Microchip shall have the right to reasonably inspect Licensee’s premises and to audit Licensee’s records and inventory of Licensee Products, whether located on Licensee’s premises or elsewhere at any time, announced or unannounced, and in its sole and absolute discretion, in order to ensure Licensee’s adherence to the terms of this Agreement. . (f) License and Sublicense Limitation. This Section 1 does not grant Licensee or any Third Party the right to modify any dotstack™ Bluetooth® stack, profile, or iAP protocol included in the Software. . | Third Party Requirements. Licensee acknowledges that it is Licensee’s responsibility to comply with any third party license terms or requirements applicable to the use of such third party software, specifications, systems, or tools, including but not limited to SEGGER Microcontroller GmbH &amp; Co. KG’s rights in the emWin software and certain libraries included herein. Microchip is not responsible and will not be held responsible in any manner for Licensee’s failure to comply with such applicable terms or requirements. . | Open Source Components. Notwithstanding the license grants contained herein, Licensee acknowledges that certain components of the Software may be covered by so-called “open source” software licenses (“Open Source Components”). Open Source Components means any software licenses approved as open source licenses by the Open Source Initiative or any substantially similar licenses, including any license that, as a condition of distribution, requires Microchip to provide Licensee with certain notices and/or information related to such Open Source Components, or requires that the distributor make the software available in source code format. Microchip will use commercially reasonable efforts to identify such Open Source Components in a text file or “About Box” or in a file or files referenced thereby (and will include any associated license agreement, notices, and other related information therein), or the Open Source Components will contain or be accompanied by its own license agreement. To the extent required by the licenses covering Open Source Components, the terms of such licenses will apply in lieu of the terms of this Agreement, and Microchip hereby represents and warrants that the licenses granted to such Open Source Components will be no less broad than the license granted in Section 1(b). To the extent the terms of the licenses applicable to Open Source Components prohibit any of the restrictions in this Agreement with respect to such Open Source Components, such restrictions will not apply to such Open Source Components. . | Licensee’s Obligations. . (a) Licensee will ensure Third Party compliance with the terms of this Agreement. . (b) Licensee will not: (i) engage in unauthorized use, modification, disclosure or distribution of Software or Documentation, or its derivatives; (ii) use all or any portion of the Software, Documentation, or its derivatives except in conjunction with Microchip Products; or (iii) reverse engineer (by disassembly, decompilation or otherwise) Software or any portion thereof; or (iv) copy or reproduce all or any portion of Software, except as specifically allowed by this Agreement or expressly permitted by applicable law notwithstanding the foregoing limitations. . (c) Licensee must include Microchip’s copyright, trademark and other proprietary notices in all copies of the Software, Documentation, and its derivatives. Licensee may not remove or alter any Microchip copyright or other proprietary rights notice posted in any portion of the Software or Documentation. . (d) Licensee will defend, indemnify and hold Microchip and its subsidiaries harmless from and against any and all claims, costs, damages, expenses (including reasonable attorney’s fees), liabilities, and losses, including without limitation product liability claims, directly or indirectly arising from or related to: (i) the use, modification, disclosure or distribution of the Software, Documentation or any intellectual property rights related thereto; (ii) the use, sale, and distribution of Licensee Products or Third Party Products, and (iii) breach of this Agreement. THE FOREGOING STATES THE SOLE AND EXCLUSIVE LIABILITY OF THE PARTIES FOR INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT. . | Confidentiality. . (a) Licensee agrees that the Software (including but not limited to the Source Code, Object Code and library files) and its derivatives, Documentation and underlying inventions, algorithms, know-how and ideas relating to the Software and the Documentation are proprietary information belonging to Microchip and its licensors (“Proprietary Information”). Except as expressly and unambiguously allowed herein, Licensee will hold in confidence and not use or disclose any Proprietary Information and shall similarly bind its employees and Third Party(ies) in writing. Proprietary Information shall not include information that: (i) is in or enters the public domain without breach of this Agreement and through no fault of the receiving party; (ii) the receiving party was legally in possession of prior to receiving it; (iii) the receiving party can demonstrate was developed by it independently and without use of or reference to the disclosing party’s Proprietary Information; or (iv) the receiving party receives from a third party without restriction on disclosure. If Licensee is required to disclose Proprietary Information by law, court order, or government agency, such disclosure shall not be deemed a breach of this Agreement provided that Licensee gives Microchip prompt notice of such requirement in order to allow Microchip to object or limit such disclosure, Licensee cooperates with Microchip to protect Proprietary Information, and Licensee complies with any protective order in place and discloses only the information required by process of law. . (b) Licensee agrees that the provisions of this Agreement regarding unauthorized use and nondisclosure of the Software, Documentation and related Proprietary Rights are necessary to protect the legitimate business interests of Microchip and its licensors and that monetary damages alone cannot adequately compensate Microchip or its licensors if such provisions are violated. Licensee, therefore, agrees that if Microchip alleges that Licensee or Third Party has breached or violated such provision then Microchip will have the right to petition for injunctive relief, without the requirement for the posting of a bond, in addition to all other remedies at law or in equity. . | Ownership of Proprietary Rights. . (a) Microchip and its licensors retain all right, title and interest in and to the Software and Documentation (“Proprietary Rights”) including, but not limited to: (i) patent, copyright, trade secret and other intellectual property rights in the Software, Documentation, and underlying technology; (ii) the Software as implemented in any device or system, all hardware and software implementations of the Software technology (expressly excluding Licensee and Third Party code developed and used in conformance with this Agreement solely to interface with the Software and Licensee Products and/or Third Party Products); and (iii) all modifications and derivative works thereof (by whomever produced). Further, modifications and derivative works shall be considered works made for hire with ownership vesting in Microchip on creation. To the extent such modifications and derivatives do not qualify as a “work for hire,” Licensee hereby irrevocably transfers, assigns and conveys the exclusive copyright thereof to Microchip, free and clear of any and all liens, claims or other encumbrances, to the fullest extent permitted by law. Licensee and Third Party use of such modifications and derivatives is limited to the license rights described in Section 1 above. . (b) Licensee shall have no right to sell, assign or otherwise transfer all or any portion of the Software, Documentation or any related intellectual property rights except as expressly set forth in this Agreement. . | Termination of Agreement. Without prejudice to any other rights, this Agreement terminates immediately, without notice by Microchip, upon a failure by License or Third Party to comply with any provision of this Agreement. Further, Microchip may also terminate this Agreement upon reasonable belief that Licensee or Third Party have failed to comply with this Agreement. Upon termination, Licensee and Third Party will immediately stop using the Software, Documentation, and derivatives thereof, and immediately destroy all such copies, remove Software from any of Licensee’s tangible media and from systems on which the Software exists, and stop using, disclosing, copying, or reproducing Software (even as may be permitted by this Agreement). Termination of this Agreement will not affect the right of any end user or consumer to use Licensee Products or Third Party Products provided that such products were purchased prior to the termination of this Agreement. . | Dangerous Applications. The Software is not fault-tolerant and is not designed, manufactured, or intended for use in hazardous environments requiring failsafe performance (“Dangerous Applications”). Dangerous Applications include the operation of nuclear facilities, aircraft navigation, aircraft communication systems, air traffic control, direct life support machines, weapons systems, or any environment or system in which the failure of the Software could lead directly or indirectly to death, personal injury, or severe physical or environmental damage. Microchip specifically disclaims (a) any express or implied warranty of fitness for use of the Software in Dangerous Applications; and (b) any and all liability for loss, damages and claims resulting from the use of the Software in Dangerous Applications. . | Warranties and Disclaimers. THE SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. MICROCHIP AND ITS LICENSORS ASSUME NO RESPONSIBILITY FOR THE ACCURACY, RELIABILITY OR APPLICATION OF THE SOFTWARE OR DOCUMENTATION. MICROCHIP AND ITS LICENSORS DO NOT WARRANT THAT THE SOFTWARE WILL MEET REQUIREMENTS OF LICENSEE OR THIRD PARTY, BE UNINTERRUPTED OR ERROR-FREE. MICROCHIP AND ITS LICENSORS HAVE NO OBLIGATION TO CORRECT ANY DEFECTS IN THE SOFTWARE. LICENSEE AND THIRD PARTY ASSUME THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE AND DOCUMENTATION PROVIDED UNDER THIS AGREEMENT. . | Limited Liability. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL OR EQUITABLE THEORY FOR ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS. The aggregate and cumulative liability of Microchip and its licensors for damages hereunder will in no event exceed $1000 or the amount Licensee paid Microchip for the Software and Documentation, whichever is greater. Licensee acknowledges that the foregoing limitations are reasonable and an essential part of this Agreement. . | General. . (a) Governing Law, Venue and Waiver of Trial by Jury. THIS AGREEMENT SHALL BE GOVERNED BY AND CONSTRUED UNDER THE LAWS OF THE STATE OF ARIZONA AND THE UNITED STATES WITHOUT REGARD TO CONFLICTS OF LAWS PROVISIONS. Licensee agrees that any disputes arising out of or related to this Agreement, Software or Documentation shall be brought in the courts of State of Arizona. The parties agree to waive their rights to a jury trial in actions relating to this Agreement. . (b) Attorneys’ Fees. If either Microchip or Licensee employs attorneys to enforce any rights arising out of or relating to this Agreement, the prevailing party shall be entitled to recover its reasonable attorneys’ fees, costs and other expenses. . (c) Entire Agreement. This Agreement shall constitute the entire agreement between the parties with respect to the subject matter hereof. It shall not be modified except by a written agreement signed by an authorized representative of Microchip. . (d) Severability. If any provision of this Agreement shall be held by a court of competent jurisdiction to be illegal, invalid or unenforceable, that provision shall be limited or eliminated to the minimum extent necessary so that this Agreement shall otherwise remain in full force and effect and enforceable. . (e) Waiver. No waiver of any breach of any provision of this Agreement shall constitute a waiver of any prior, concurrent or subsequent breach of the same or any other provisions hereof, and no waiver shall be effective unless made in writing and signed by an authorized representative of the waiving party. . (f) Export Regulation. Licensee agrees to comply with all export laws and restrictions and regulations of the Department of Commerce or other United States or foreign agency or authority. . (g) Survival. The indemnities, obligations of confidentiality, and limitations on liability described herein, and any right of action for breach of this Agreement prior to termination shall survive any termination of this Agreement. . (h) Assignment. Neither this Agreement nor any rights, licenses or obligations hereunder, may be assigned by Licensee without the prior written approval of Microchip except pursuant to a merger, sale of all assets of Licensee or other corporate reorganization, provided that assignee agrees in writing to be bound by the Agreement. . (i) Restricted Rights. Use, duplication or disclosure by the United States Government is subject to restrictions set forth in subparagraphs (a) through (d) of the Commercial Computer-Restricted Rights clause of FAR 52.227-19 when applicable, or in subparagraph (c)(1)(ii) of the Rights in Technical Data and Computer Software clause at DFARS 252.227-7013, and in similar clauses in the NASA FAR Supplement. Contractor/manufacturer is Microchip Technology Inc., 2355 W. Chandler Blvd., Chandler, AZ 85225-6199. . | If Licensee has any questions about this Agreement, please write to Microchip Technology Inc., 2355 W. Chandler Blvd., Chandler, AZ 85224-6199 USA, ATTN: Marketing. . Microchip MPLAB Harmony Integrated Software Framework. Copyright © 2015 Microchip Technology Inc. All rights reserved. . License Rev. 11/2015 . Copyright © 2015 Qualcomm Atheros, Inc. All Rights Reserved. . Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies. . THE SOFTWARE IS PROVIDED “AS IS” AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. .",
    "url": "http://localhost:4000/ethercat/mplab_harmony_license.html",
    "relUrl": "/mplab_harmony_license.html"
  }
  ,"5": {
    "title": "",
    "content": ". Microchip MPLAB® Harmony 3 EtherCAT Release Notes (August 2020) . EtherCAT Release v3.0.1 . ADDITIONS AND UPDATES FOR 3.0.1: . Updated Library and application Documentation from CHM file to Markdown file. | . Known Issues . None . Microchip MPLAB® Harmony 3 EtherCAT Release Notes (April, 2020) . EtherCAT Release v3.0.0 . New Features . EtherCAT component is added in Harmony 3 to enable customer to develop EtherCAT Applications with Harmony 3 framework using LAN9252 device. . | Development kit and demo application support - This release include demo application that demonstrates firmware update over EtherCAT on SAM D51 with LAN925x Evaluation board. . Development kits Applications Description . EVB-LAN925x SAMD51 | Process Trigger and Counter Index With Firmware Update Over EtherCAT | . | . Known Issues . None . Development Tools . MPLAB® X IDE v5.35 | MPLAB® XC32 C/C++ Compiler v2.40 | MPLAB® X IDE plug-ins: MPLAB® Harmony Configurator (MHC) v3.3.5 and above. | . | TwinCAT Manager: TC31-Full-Setup.3.1.4024.7x and above. (https://www.beckhoff.com/english.asp?download/tc3-downloads.htm) | . | . Dependent Components . CSP v3.6.1 | MHC v3.3.5 | EtherCAT Slave Stack Code (SSC) v5.12 | .",
    "url": "http://localhost:4000/ethercat/release_notes.html",
    "relUrl": "/release_notes.html"
  }
  ,"6": {
    "title": "EtherCAT Library and Applications",
    "content": ". MPLAB® Harmony 3 EtherCAT . MPLAB® Harmony 3 is an extension of the MPLAB® ecosystem for creating embedded firmware solutions for Microchip 32-bit SAM and PIC® microcontroller and microprocessor devices. Refer to the following links for more information. . Microchip 32-bit MCUs | Microchip 32-bit MPUs | Microchip MPLAB X IDE | Microchip MPLAB® Harmony | Microchip MPLAB® Harmony Pages | . This repository contains the MPLAB® Harmony 3 EtherCAT solutions and example applications. Quickly incorporate connectivity to your designs with EtherCAT ICs, modules, software and development kits that make connecting effortless for your customers. Our comprehensive EtherCAT portfolio has the technology to meet your range, data rate, interoperability, frequency and topology needs. Refer to the following links for release notes, training materials, and interface reference information. . Release Notes | MPLAB® Harmony License | MPLAB® Harmony 3 EtherCAT Wiki | MPLAB® Harmony 3 EtherCAT API Help | . Contents Summary . Folder Description . apps | Example applications for EtherCAT library components | . config | EtherCAT module configuration file | . docs | EtherCAT library help documentation (html format) | . drivers | Driver/Firmware files for different products | . Cloning EtherCAT Microchip-MPLAB-Harmony github repo . To clone or download the repo from https://github.com/Microchip-MPLAB-Harmony/ethercat , go to the main page of this repository and then click Clone button to clone this repo or download as zip file. This content can also be download using content manager by following https://github.com/Microchip-MPLAB-Harmony/EtherCAT/wiki. . NOTE - Prior to using the EtherCAT demonstration, it is recommended to review the MPLAB® Harmony Release Notes (release_notes.md) for any known issues. . EtherCAT Library and Demo Application . This distribution package contains MPLAB Harmony EtherCAT firmware project that demonstrate the capabilities of Process Data Objects( counter and Trigger objects) and FoE( File Over EtherCAT ,Firmware upgrade feature) . The project describes its hardware setup and requirements. . The EVB-LAN9252_SAM D51 Evaluation Board is an EtherCAT slave controller with dual integrated Ethernet PHYs. Each Ethernet PHY contains a Full-Duplex 100BASE-TX transceiver and supports 100 Mbps (100BASE-TX) operation. . SAMD51 LAN9252 EVB ( Evaluation page of EVB_LAN9252_SAMD51 ) | . All the EtherCAT applications are available in ethercat/apps folder. All the drivers are located in the ethercat/drivers folder. The distribution package also contains details about EtherCAT Slave stack code generation tool and also it contains details of EtherCAT manager ,where the demo is verified with TwinCAT manager. . Name Description . ethercat_library_lan925x | This library explains about EtherCAT slave controller Interface layer w.r.t LAN9252 device | . ethercat_counter_foe_app | This example application demonstrates the capable of doing firmware updates via FoE (File over EtherCAT) and EtherCAT Master slave communication with SAMD51 micro-controller. | . . . . . .",
    "url": "http://localhost:4000/ethercat/",
    "relUrl": "/"
  }
  
}`;
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
  const mainNav = document.querySelector('.js-main-nav');
  const pageHeader = document.querySelector('.js-page-header');
  const navTrigger = document.querySelector('.js-main-nav-trigger');

  jtd.addEvent(navTrigger, 'click', function(e){
    e.preventDefault();
    var text = navTrigger.innerText;
    var textToggle = navTrigger.getAttribute('data-text-toggle');

    mainNav.classList.toggle('nav-open');
    pageHeader.classList.toggle('nav-open');
    navTrigger.classList.toggle('nav-open');
    navTrigger.innerText = textToggle;
    navTrigger.setAttribute('data-text-toggle', text);
    textToggle = text;
  })
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

    searchResults(index, data_for_search);

function searchResults(index, data) {
    var index = index;
    var docs = data;
    var searchInput = document.querySelector('.js-search-input');
    var searchResults = document.querySelector('.js-search-results');

    function hideResults() {
      searchResults.innerHTML = '';
      searchResults.classList.remove('active');
    }

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

    jtd.addEvent(searchInput, 'keyup', function(e){
      switch (e.keyCode) {
        case 27: // When esc key is pressed, hide the results and clear the field
          hideResults();
          searchInput.value = '';
          return;
        case 38: // arrow up
        case 40: // arrow down
        case 13: // enter
          e.preventDefault();
          return;
      }

      hideResults();

      var input = this.value;
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

      if (results.length > 0) {
        searchResults.classList.add('active');
        var resultsList = document.createElement('ul');
        resultsList.classList.add('search-results-list');
        searchResults.appendChild(resultsList);

        for (var i in results) {
          var result = results[i];
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
          resultTitle.innerText = doc.title;
          resultLink.appendChild(resultTitle);

          var resultRelUrl = document.createElement('span');
          resultRelUrl.classList.add('search-result-rel-url');
          resultRelUrl.innerText = doc.relUrl;
          resultTitle.appendChild(resultRelUrl);

          var metadata = result.matchData.metadata;
          var contentFound = false;
          for (var j in metadata) {
            if (metadata[j].title) {
              var position = metadata[j].title.position[0];
              var start = position[0];
              var end = position[0] + position[1];
              resultTitle.innerHTML = doc.title.substring(0, start) + '<span class="search-result-highlight">' + doc.title.substring(start, end) + '</span>' + doc.title.substring(end, doc.title.length)+'<span class="search-result-rel-url">'+doc.relUrl+'</span>';

            } else if (metadata[j].content && !contentFound) {
              contentFound = true;

              var position = metadata[j].content.position[0];
              var start = position[0];
              var end = position[0] + position[1];
              var previewStart = start;
              var previewEnd = end;
              var ellipsesBefore = true;
              var ellipsesAfter = true;
              for (var k = 0; k < 3; k++) {
                var nextSpace = doc.content.lastIndexOf(' ', previewStart - 2);
                var nextDot = doc.content.lastIndexOf('.', previewStart - 2);
                if ((nextDot > 0) && (nextDot > nextSpace)) {
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
                var nextDot = doc.content.indexOf('.', previewEnd + 1);
                if ((nextDot > 0) && (nextDot < nextSpace)) {
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
              var preview = doc.content.substring(previewStart, start);
              if (ellipsesBefore) {
                preview = '... ' + preview;
              }
              preview += '<span class="search-result-highlight">' + doc.content.substring(start, end) + '</span>';
              preview += doc.content.substring(end, previewEnd);
              if (ellipsesAfter) {
                preview += ' ...';
              }

              var resultPreview = document.createElement('div');
              resultPreview.classList.add('search-result-preview');
              resultPreview.innerHTML = preview;
              resultLink.appendChild(resultPreview);
            }
          }
        }
      }
    });

    jtd.addEvent(searchInput, 'blur', function(){
      setTimeout(function(){ hideResults() }, 300);
    });
  }
}

function pageFocus() {
  var mainContent = document.querySelector('.js-main-content');
  mainContent.focus();
}

// Document ready

jtd.onReady(function(){
  initNav();
  pageFocus();
  if (typeof lunr !== 'undefined') {
    initSearch();
  }
});

})(window.jtd = window.jtd || {});


