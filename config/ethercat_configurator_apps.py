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
	sscStackComment.setLabel("Note: Generated SlaveStackCode should be included manually in the project.")
	
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
	
	#Add to ethercat_configuration.h
	ethercatConfigurationHeaderFtl = etherCatComponent.createFileSymbol(None, None)
	ethercatConfigurationHeaderFtl.setSourcePath("config/ethercat_template/ethercat_config.h.ftl")
	ethercatConfigurationHeaderFtl.setOutputName("ethercat_configuration.h")
	ethercatConfigurationHeaderFtl.setDestPath("")
	ethercatConfigurationHeaderFtl.setProjectPath("config/" + configName + "/")
	ethercatConfigurationHeaderFtl.setType("HEADER")
	ethercatConfigurationHeaderFtl.setOverwrite(True)
	ethercatConfigurationHeaderFtl.setMarkup(True)
	
	
	# Message to provide the source web page path which will be used for the webpage.py
	etheercatSlaveStackcodeDirPath = etherCatComponent.createStringSymbol("ETHERCAT_SLAVESTACK_DIRECTORY_PATH", None)
	etheercatSlaveStackcodeDirPath.setLabel("Slave Stack source directory path")
	etheercatSlaveStackcodeDirPath.setVisible(True)
	etheercatSlaveStackcodeDirPath.setDescription("Configure Slave Stack directory path")
	etheercatSlaveStackcodeDirPath.setDefaultValue(Module.getPath() + "slave_stack")
	
	
	execfile(Module.getPath() + "/config/ethercatSlaveStack.py")
	
	# set TCP/IP include paths
	slaveStackDefSym = etherCatComponent.createSettingSymbol("ETHERCAT_SLAVE_STACK_INCLUDE_DIRS", None)
	slaveStackDefSym.setCategory("C32")
	slaveStackDefSym.setKey("extra-include-directories")
	slaveStackDefSym.setValue( "../src/slave_stack")
	slaveStackDefSym.setAppend(True, ";")
	
	
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
		
