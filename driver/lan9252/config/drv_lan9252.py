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

def instantiateComponent(ethercatLan9252Component):
		
	print("Ethercat LAN 9252 Driver Component")
	configName = Variables.get("__CONFIGURATION_NAME")
	
		
	lan9252PLIB = ethercatLan9252Component.createStringSymbol("DRV_LAN9252_PLIB", None)
	lan9252PLIB.setLabel("PLIB Used")
	lan9252PLIB.setReadOnly(True)

	lan9252TimerPLIB = ethercatLan9252Component.createStringSymbol("DRV_LAN9252_TIMER_PLIB", None)
	lan9252TimerPLIB.setLabel("PLIB Used")
	lan9252TimerPLIB.setReadOnly(True)
	
	# driver/lan9252/drv_lan9252.h to config/<configName>/driver/lan9252/drv_lan9252.h
	lan9252HeaderFile = ethercatLan9252Component.createFileSymbol(None, None)
	lan9252HeaderFile.setSourcePath("driver/lan9252/src/drv_lan9252.h")
	lan9252HeaderFile.setOutputName("drv_lan9252.h")
	lan9252HeaderFile.setDestPath("driver/lan9252/")
	lan9252HeaderFile.setProjectPath("config/" + configName + "/driver/lan9252/")
	lan9252HeaderFile.setType("HEADER")
	lan9252HeaderFile.setOverwrite(True)
	lan9252HeaderFile.setEnabled(True)
	
	# file driver/lan9252/drv_lan9252.c to config/<configName>/driver/lan9252/drv_lan9252.c    
	lan9252SourceFile = ethercatLan9252Component.createFileSymbol(None, None)
	lan9252SourceFile.setSourcePath("driver/lan9252/src/drv_lan9252.c")
	lan9252SourceFile.setOutputName("drv_lan9252.c")
	lan9252SourceFile.setOverwrite(True)
	lan9252SourceFile.setDestPath("driver/lan9252/")
	lan9252SourceFile.setProjectPath("config/" + configName + "/driver/lan9252/")
	lan9252SourceFile.setType("SOURCE")
	lan9252SourceFile.setEnabled(True)
	
	# file driver/lan9252/drv_lan9252_definitions.h to config/<configName>/driver/lan9252/drv_lan9252_definitions.h 
	lan9252DefinitionHeaderFile = ethercatLan9252Component.createFileSymbol("DRV_LAN9252_DEF", None)
	lan9252DefinitionHeaderFile.setSourcePath("driver/lan9252/drv_lan9252_definitions.h")
	lan9252DefinitionHeaderFile.setOutputName("drv_lan9252_definitions.h")
	lan9252DefinitionHeaderFile.setDestPath("driver/lan9252")
	lan9252DefinitionHeaderFile.setProjectPath("config/" + configName + "/driver/lan9252/")
	lan9252DefinitionHeaderFile.setType("HEADER")
	lan9252DefinitionHeaderFile.setOverwrite(True)
	
	
	lan9252SystemDefFile = ethercatLan9252Component.createFileSymbol("LAN9252_DEF", None)
	lan9252SystemDefFile.setType("STRING")
	lan9252SystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	lan9252SystemDefFile.setSourcePath("driver/lan9252/templates/definitions.h.ftl")
	lan9252SystemDefFile.setMarkup(True)

	lan9252SymSystemDefObjFile = ethercatLan9252Component.createFileSymbol("DRV_LAN9252_SYSTEM_DEF_OBJECT", None)
	lan9252SymSystemDefObjFile.setType("STRING")
	lan9252SymSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
	lan9252SymSystemDefObjFile.setSourcePath("driver/lan9252/templates/definitions_objects.h.ftl")
	lan9252SymSystemDefObjFile.setMarkup(True)
	
	lan9252SymSystemInitDataFile = ethercatLan9252Component.createFileSymbol("DRV_LAN9252_INIT_DATA", None)
	lan9252SymSystemInitDataFile.setType("STRING")
	lan9252SymSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
	lan9252SymSystemInitDataFile.setSourcePath("driver/lan9252/templates/initialize_data.c.ftl")
	lan9252SymSystemInitDataFile.setMarkup(True)
	
	lan9252SystemInitFile = ethercatLan9252Component.createFileSymbol("LAN9252_INIT", None)
	lan9252SystemInitFile.setType("STRING")
	lan9252SystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
	lan9252SystemInitFile.setSourcePath("driver/lan9252/templates/initialize.c.ftl")
	lan9252SystemInitFile.setMarkup(True)
	
	# set driver/lan9252 include paths
	lan9252DriverDefSym = ethercatLan9252Component.createSettingSymbol("ETHERCAT_LAN9252_INCLUDE_DIRS", None)
	lan9252DriverDefSym.setCategory("C32")
	lan9252DriverDefSym.setKey("extra-include-directories")
	lan9252DriverDefSym.setValue( "../src/config/" + configName
						+ "/driver/lan9252"
						)
	lan9252DriverDefSym.setAppend(True, ";")
	
	# driver/lan9252/drv_lan9252_ecat_util.h to lan9252 driver path
	lan9252ecatUtilHeaderFile = ethercatLan9252Component.createFileSymbol(None, None)
	lan9252ecatUtilHeaderFile.setSourcePath("driver/lan9252/drv_lan9252_ecat_util.h")
	lan9252ecatUtilHeaderFile.setOutputName("drv_lan9252_ecat_util.h")
	lan9252ecatUtilHeaderFile.setDestPath("driver/lan9252/")
	lan9252ecatUtilHeaderFile.setProjectPath("config/" + configName + "/driver/lan9252/")
	lan9252ecatUtilHeaderFile.setType("HEADER")
	lan9252ecatUtilHeaderFile.setOverwrite(True)
	lan9252ecatUtilHeaderFile.setEnabled(True)
	
	# file driver/lan9252/drv_lan9252_ecat_util.c to the lan9252 driver path
	lan9252ecatUtilSourceFile = ethercatLan9252Component.createFileSymbol(None, None)
	lan9252ecatUtilSourceFile.setSourcePath("driver/lan9252/drv_lan9252_ecat_util.c")
	lan9252ecatUtilSourceFile.setOutputName("drv_lan9252_ecat_util.c")
	lan9252ecatUtilSourceFile.setOverwrite(True)
	lan9252ecatUtilSourceFile.setDestPath("driver/lan9252/")
	lan9252ecatUtilSourceFile.setProjectPath("config/" + configName + "/driver/lan9252/")
	lan9252ecatUtilSourceFile.setType("SOURCE")
	lan9252ecatUtilSourceFile.setEnabled(True)

def onAttachmentConnected(source, target):

	localComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	connectID = source["id"]
	targetID = target["id"]
	sysTimeDict = {"ID":"sys_time"}

	if connectID == "LAN9252_SPI_Dependency":
		plibUsed = localComponent.getSymbolByID("DRV_LAN9252_PLIB")
		plibUsed.clearValue()
		lan9252PlibId = remoteID.upper()
		plibUsed.setValue(lan9252PlibId.upper())
		Database.setSymbolValue(lan9252PlibId, "SPI_DRIVER_CONTROLLED", True)
	if (connectID == "LAN9252_TMR_Dependency"):
		plibUsed = localComponent.getSymbolByID("DRV_LAN9252_TIMER_PLIB")
		plibUsed.clearValue()
		lan9252PlibId = remoteID.upper()
		plibUsed.setValue(lan9252PlibId)
		#Request PLIB to publish it's capabilities
		sysTimeDict = Database.sendMessage(remoteID, "SYS_TIME_PUBLISH_CAPABILITIES", sysTimeDict)
		print "RemoteComponentID = " + remoteID
		
def onAttachmentDisconnected(source, target):

	localComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	connectID = source["id"]
	targetID = target["id"]

	if connectID == "LAN9252_SPI_Dependency":
		plibUsed = localComponent.getSymbolByID("DRV_LAN9252_PLIB")
		plibUsed.clearValue()
		lan9252PlibId = remoteID.upper()
		Database.setSymbolValue(lan9252PlibId, "SPI_DRIVER_CONTROLLED", False)
	if (connectID == "LAN9252_TMR_Dependency"):
		plibUsed = localComponent.getSymbolByID("DRV_LAN9252_TIMER_PLIB")
		plibUsed.clearValue()
		plibUsed.setValue("")
		
