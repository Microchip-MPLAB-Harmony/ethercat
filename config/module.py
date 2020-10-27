# coding: utf-8
"""*****************************************************************************
* Copyright (C) 2019 - 2020 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*****************************************************************************"""
######################  Harmony EtherCAT  ######################
def loadModule():
	print("Load Module: Harmony EtherCAT")
	etherCatComponent = Module.CreateComponent("ethercat_apps_config", "EtherCAT Stack", "/Libraries/EtherCAT/", "config/ethercat_configurator_apps.py")
	etherCatComponent.addDependency("EtherCAT_LANDRV_Dependency", "LAN_DRV", None, False, False)
#	etherCatComponent.addDependency("EtherCAT_LAN9253_Dependency", "LAN_9253", None, True, True)
	etherCatComponent.setDisplayType("EtherCAT Library")
	
	###########  ETHERCAT LAN 9252 driver componet ###########   
	ethercatLan9252Component = Module.CreateComponent("ethercat_lan9252", "LAN 9252", "/Harmony/Drivers/EtherCAT Driver/Internal/", "driver/lan9252/config/drv_lan9252.py")
	ethercatLan9252Component.addCapability("drvEthercatLan9252","LAN_DRV", False)
	ethercatLan9252Component.addDependency("LAN9252_SPI_Dependency", "SPI", None, False, False)
	ethercatLan9252Component.addDependency("LAN9252_TMR_Dependency", "TMR", None, False, False)
	
	###########  ETHERCAT LAN 9253 driver componet ###########   
	ethercatLan9253Component = Module.CreateComponent("ethercat_lan9253", "LAN 9253", "/Harmony/Drivers/EtherCAT Driver/Internal/", "driver/lan9253/config/drv_lan9253.py")
	ethercatLan9253Component.addCapability("drvEthercatLan9253","LAN_DRV", False)
	ethercatLan9253Component.addDependency("LAN9253_SPI_Dependency", "SPI", None, False, False)
	ethercatLan9253Component.addDependency("LAN9253_TMR_Dependency", "TMR", None, False, False)
	
