"""*****************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

usbHostTplEntryNumber = None
usbHostHubSaveValue = None 

def setVisible(symbol, event):
	if (event["value"] == True):
		symbol.setVisible(True)
	else:
		symbol.setVisible(False)
		
def ethercatLan9252DriverInfoVisible(symbol, event):
	if (event["value"] == "LAN 9252"):
		res = Database.activateComponents(["ethercat_lan9252"],None)     
		print("LAN 9252")
		symbol.setVisible(False)
	elif (event["value"] == "None"):
		symbol.setVisible(False)
	else:
		res = Database.deactivateComponents(["ethercat_lan9252"])
		symbol.setVisible(False)
		
		
		
def ethercatLan9253DriverInfoVisible(symbol, event):
	if (event["value"] == "LAN 9253"):
		res = Database.activateComponents(["ethercat_lan9253"],None)     
		print("LAN 9253")
		symbol.setVisible(False)
	elif (event["value"] == "None"):
		symbol.setVisible(False)
	else:
		res = Database.deactivateComponents(["ethercat_lan9253"])
		symbol.setVisible(False)
		
ETHERCAT_DRIVER_TYPES = ["LAN 9252", "LAN 9253" , "None"]	


def instantiateComponent(etherCatComponent):
	global usbHostTplEntryNumber
	global usbHostHubSaveValue
	
	driverIndex = "DRV_USB_UHP_INDEX_0"
	driverInterface = "DRV_USB_UHP_HOST_INTERFACE"
	configName = Variables.get("__CONFIGURATION_NAME")
	
	#SSC Stack need will be included to the MPLABx project manually. It is not MHC generated
	sscStackComment = etherCatComponent.createCommentSymbol("EtherCATSlaveStackComment", None)
	sscStackComment.setLabel("Note: SlaveStackCode should be included manually to the project after required generation.")
	
	# Enable FOE
	ethercatFOEEnable = etherCatComponent.createBooleanSymbol("ETHERCAT_FOE_ENABLE", None)
	ethercatFOEEnable.setLabel("Enable FOE")
	ethercatFOEEnable.setVisible(True)
	ethercatFOEEnable.setDescription("Enable FOE")
	ethercatFOEEnable.setDefaultValue(True)
	
	# MAX FILE Name LENGTH 
	foeFileSIzeLength =  etherCatComponent.createIntegerSymbol("ETHERCAT_FOE_FILE_NAME_LENGTH",ethercatFOEEnable)
	foeFileSIzeLength.setVisible(True)
	foeFileSIzeLength.setLabel("Download File Name length")
	foeFileSIzeLength.setDescription("Download File Name length")
	foeFileSIzeLength.setDefaultValue(16)
	foeFileSIzeLength.setDependencies(ethercatFoEAttrVisible, ["ETHERCAT_FOE_ENABLE"])
	
	# Message to provide the source web page path which will be used for the webpage.py
	foeFileDownloadPassword =  etherCatComponent.createHexSymbol("ETHERCAT_FOE_FILE_DOWNLOAD_PASSWORD", ethercatFOEEnable)
	foeFileDownloadPassword.setLabel("FOE File Download Password")
	foeFileDownloadPassword.setVisible(True)
	foeFileDownloadPassword.setDescription("Configure FOE File Download Password")
	foeFileDownloadPassword.setDefaultValue(0x11223344)	
	foeFileDownloadPassword.setDependencies(ethercatFoEAttrVisible, ["ETHERCAT_FOE_ENABLE"])
	
	#enableLanDriverUse = etherCatComponent.createComboSymbol("ETHERCAT_DRIVER_ENABLE", None,ETHERCAT_DRIVER_TYPES)
	#enableLanDriverUse.setLabel("Enable Ethernet Driver Use")
	#enableLanDriverUse.setVisible(True)
	#enableLanDriverUse.setDescription("Enable Ethernet Driver Use")
	#enableLanDriverUse.setDefaultValue("None")
	#enableLanDriverUse.setDependencies(ethercatLanDriverMutualExclusiveVisible,["ETHERCAT_LAN9252_ENABLE"])
	
	# LAN9252 driver configuration
	#ethercatLan9252Info = etherCatComponent.createBooleanSymbol("ETHERCAT_LAN9252_INFO", enableLanDriverUse)	
	#ethercatLan9252Info.setVisible(False)
	#ethercatLan9252Info.setDefaultValue(False)
	#ethercatLan9252Info.setDependencies(ethercatLan9252DriverInfoVisible, ["ETHERCAT_DRIVER_ENABLE"])

	# LAN9253 driver Configuration
	#ethercatLan9253Info = etherCatComponent.createBooleanSymbol("ETHERCAT_LAN9253_INFO", enableLanDriverUse)	
	#ethercatLan9253Info.setVisible(False)
	#ethercatLan9253Info.setDefaultValue(False)
	#ethercatLan9253Info.setDependencies(ethercatLan9253DriverInfoVisible, ["ETHERCAT_DRIVER_ENABLE"])
		
	# config/ethercat_template/ecat_main.h to root of the path
	ecatMainHeaderFile = etherCatComponent.createFileSymbol(None, None)
	ecatMainHeaderFile.setSourcePath("config/ethercat_template/ecat_main.h")
	ecatMainHeaderFile.setOutputName("ecat_main.h")
	ecatMainHeaderFile.setDestPath("../../")
	ecatMainHeaderFile.setProjectPath("")
	ecatMainHeaderFile.setType("HEADER")
	ecatMainHeaderFile.setOverwrite(True)
	ecatMainHeaderFile.setEnabled(True)	
	
	# file config/ethercat_template/ecat_main.c to root of the path    
	lan9252SourceFile = etherCatComponent.createFileSymbol(None, None)
	lan9252SourceFile.setSourcePath("config/ethercat_template/ecat_main.c")
	lan9252SourceFile.setOutputName("ecat_main.c")
	lan9252SourceFile.setOverwrite(True)
	lan9252SourceFile.setDestPath("../../")
	lan9252SourceFile.setProjectPath("")
	lan9252SourceFile.setType("SOURCE")
	lan9252SourceFile.setEnabled(True)
	
	#Add to configuration.h
	ethercatConfigurationHeaderFtl = etherCatComponent.createFileSymbol(None, None)
	ethercatConfigurationHeaderFtl.setSourcePath("config/ethercat_config.h.ftl")
	ethercatConfigurationHeaderFtl.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	ethercatConfigurationHeaderFtl.setMarkup(True)
	ethercatConfigurationHeaderFtl.setType("STRING")
	
	
	# Message to provide the source web page path which will be used for the webpage.py
	etheercatSlaveStackcodeDirPath = etherCatComponent.createStringSymbol("ETHERCAT_SLAVESTACK_DIRECTORY_PATH", None)
	etheercatSlaveStackcodeDirPath.setLabel("Slave Stack source directory path")
	etheercatSlaveStackcodeDirPath.setVisible(True)
	etheercatSlaveStackcodeDirPath.setDescription("Configure Slave Stack directory path")
	etheercatSlaveStackcodeDirPath.setDefaultValue(Module.getPath() + "slave_stack")
   
	
	execfile(Module.getPath() + "/config/ethercatSlaveStack.py")

	
def ethercatFoEAttrVisible(symbol, event):
	if (event["value"] == True):
		symbol.setVisible(True)
	else:
		symbol.setVisible(False)
	
def ethercatLanDriverMutualExclusiveVisible(symbol, event):
	if (event["value"] == True):
		symbol.setVisible(True)
	else:
		symbol.setVisible(False)
		
