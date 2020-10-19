---
parent: EtherCAT Library and Applications
title: Slave Stack Code Generation
has_children: false
has_toc: false
nav_order: 3
---

# **Slave Stack Code Generation**

1. Download the Slave Stack Code (SSC) Tool from EtherCAT.org website. An EtherCAT membership is necessary to download the code. The following link can be used to download the tool.

    [EtherCAT Slave Stack Code (SSC) v5.12](https://www.ethercat.org/en/downloads/downloads_01DCC32A10294F2EA866F7E46FB0285F.htm)

2.	Install the SSC Tool on the control/management PC
3.	Open the SSC Tool and navigate to File => New to create a New Project
4.	Select Custom and then click Import

    ![SSC Tool](images/ssc_import_configuration_file.png)

5. For **Lan9252 Slave** device	Import “Microchip-SAMD51-LAN9252-SSC_Config.xml” configuration file from **< Harmony Directory >/ethercat/apps/ethercat_counter_foe_app/firmware/src/slave_stack/lan9252/**.

    For **Lan9253  Slave** device, there are different SSC configuration files are present at **< Harmony Directory >/ethercat/apps/ethercat_counter_foe_app/firmware/src/slave_stack/lan9253/** , please find the below table for the available configuration file LAN9253 device -

    | SSC Configuration file  |   Mode         |    Supported or Not Supported    |
    |-------------------------|:--------------:|:--------------:|
    | Microchip-SAMD51-EtherCAT-Slave_SSC_Config_SPI_Indirect_mode.xml   |  SPI Indirect mode   |   Supported   |
    | Microchip-SAMD51-EtherCAT-Slave_SSC_Config_SPI_Direct_mode.xml     |  SPI Direct mode   |   Supported   |
    | Microchip-SAMD51-EtherCAT-Slave_SSC_Config_SPI_Beckhoff_mode.xml     |  SPI Beckhoff mode |   Supported   |

    After importing this configuration file, the **Microchip - EtherCAT Slave Controller < Microchip >**  will appear in the custom field.

    ![SSC Tool](images/custom_sscfile_selected.png)

    Select **Microchip - EtherCAT Slave Controller < Microchip >** and click OK. Click Yes to the next two prompts

    ![SSC Tool](images/ssc_import_configuration_file_2.png)

6.	The driver **drv_lan9252.c** is required for Lan9252 slave configuration and to select file , Click **yes**. In the SSC tool navigate to the **< Harmony Directory >/ethercat/apps/ethercat_counter_foe_app/firmware/src/config/sam_d51_lan9252_evb/driver/lan9252** directory to point to the **drv_lan9252.c** file and click Open. This will load the code.

    ![SSC Tool](images/drv_lan9252_selection.png)

    **NOTE** - For **Lan9253 Slave** configuration file, select drv_lan9253.c file from the **driver/lan9253/src/drv_lan9253.c** and remaining steps are similar to lan9252 steps.

7.	Click on **Tools=>Application=>Import**  and select **sample_app.xlsx** application file from the path **< Harmony Directory >/ethercat/apps/ethercat_counter_foe_app/firmware/src/slave_stack/lan9252** . Click OK to the next prompt

    ![SSC Tool](images/ssc_import_configuration_file_3.png)

    **NOTE** - For **Lan9253 Slave** configuration file, select **sample_app.xlsx** application file from the path **< Harmony Directory >/ethercat/apps/ethercat_counter_foe_app/firmware/src/slave_stack/lan9253**

8.	Click on **Project=>Create New Slave Files**. When prompted to save the project, save the project in the directory provided with name **SAMD51_EtherCAT_Slave.esp**.
    *  Click the Project drop-down menu in the tool bar and select Create New Slave Files. The pop-up windows open the newly slave file project ( SAMD51_EtherCAT_Slave.esp ),

        ![SSC Tool](images/ssc_import_configuration_file_4.png)

    * Click the Start button to create a new project file, **Src** folder, and **ESI** file (Slave Information file) in the desired directory path.

    * A pop-up window will indicate that the files have been successfully created. Click OK to continue.

    * Along with generated new slave files, ESI file (.xml file) also will be generated. This ESI file will have information about new Vendor ID and object configuration. This ESI file will be used by the **TwinCAT Manager** and is used to be programmed using "EEPROM Programming".

    **NOTE** - **EEPROM Programming** is described in **Running the demo application** section

9. Browse to the directory where the new files were created, as shown in the example:
    * Src (Folder): This folder contains the Beckhoff Slave Stack code.
    * SAMD51_EtherCAT_Slave (ESP): This is the SSC Tool project file.
    * SAMD51_EtherCAT_Slave (XML): This is the EtherCAT® slave information file that must be used as an input to the EtherCAT master tool to configure EtherCAT® slave controllers.

10. The generated sample_app.c , does not have the counter, trigger configuration details and FoE read/write operations. So, the present **sample_app.c, sample_app.h, sample_appObject.h** can be updated from the location **< Harmony Directory >/ethercat/apps/ethercat_counter_foe_app/firmware/src/slave_stack**.
