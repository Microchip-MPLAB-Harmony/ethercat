# coding: utf-8
"""*****************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
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

global sort_alphanumeric
global portPlib
global chipSelectPortPlib
global lan9255Sync0Pin
global lan9255Sync1Pin
global lan9255IrqPin

ETHERCAT_DRV_LAN9255_SPI_MODE_ACCESS_TYPE = ["ETHERCAT_SPI_INDIRECT_MODE_ACCESS", "ETHERCAT_SPI_DIRECT_MODE_ACCESS", "ETHERCAT_SPI_BECKHOFF_MODE_ACCESS"]

ETHERCAT_DRV_LAN9255_SQI_MODE_ACCESS_TYPE = ["ETHERCAT_SQI_INDIRECT_MODE_ACCESS", "ETHERCAT_SQI_DIRECT_MODE_ACCESS"]


lan9255ChipSelect      = ["Chip Select 0", "Chip Select 1"]
lan9255ProtocolUsed    = ["SQI", "SPI"]


def handleMessage(messageID, args):

	result_dict = {}

	if (messageID == "REQUEST_CONFIG_PARAMS"):
		if args.get("localComponentID") != None:
			result_dict = Database.sendMessage(args["localComponentID"], "SPI_MASTER_MODE", {"isReadOnly":True, "isEnabled":True})
			result_dict = Database.sendMessage(args["localComponentID"], "SPI_MASTER_INTERRUPT_MODE", {"isReadOnly":True, "isEnabled":True})
			result_dict = Database.sendMessage(args["localComponentID"], "SPI_MASTER_HARDWARE_CS", {"isReadOnly":True, "isEnabled":False})

	return result_dict

def sort_alphanumeric(l):
	import re
	convert = lambda text: int(text) if text.isdigit() else text.lower()
	alphanum_key = lambda key: [ convert(c) for c in re.split('([0-9]+)', key) ]
	return sorted(l, key = alphanum_key)

def instantiateComponent(ethercatlan9255Component):
		
	print("Ethercat LAN 9255 Driver Component")
	configName = Variables.get("__CONFIGURATION_NAME")
	
	lan9255PLIBUse = ethercatlan9255Component.createBooleanSymbol("DRV_LAN9255_USE", None)
	lan9255PLIBUse.setLabel("Driver LAN9255 is Used")
	lan9255PLIBUse.setReadOnly(True)
	lan9255PLIBUse.setVisible(False)
	lan9255PLIBUse.setDefaultValue(True)
		
	lan9255SPIPLIB = ethercatlan9255Component.createStringSymbol("DRV_LAN9255_PLIB", None)
	lan9255SPIPLIB.setLabel("PLIB Used")
	lan9255SPIPLIB.setReadOnly(True)
	lan9255SPIPLIB.setVisible(False)
	
	lan9255Protocol = ethercatlan9255Component.createComboSymbol("DRV_LAN9255_PROTOCOL", None, lan9255ProtocolUsed)
	lan9255Protocol.setLabel("Communication Protocol Used")
	lan9255Protocol.setVisible(False)
	lan9255Protocol.setDefaultValue("SQI")

	lan9255TimerPLIB = ethercatlan9255Component.createStringSymbol("DRV_LAN9255_TIMER_PLIB", None)
	lan9255TimerPLIB.setLabel("Timer PLIB Used")
	lan9255TimerPLIB.setReadOnly(True)
	lan9255TimerPLIB.setVisible(False)
	
	lan9255EEPROMEmulationPLIB = ethercatlan9255Component.createBooleanSymbol("DRV_LAN9255_EEPROM_EMULATION", None)
	lan9255EEPROMEmulationPLIB.setLabel("Default EEPROM EMULATION Enabled")
	lan9255EEPROMEmulationPLIB.setReadOnly(True)
	lan9255EEPROMEmulationPLIB.setVisible(True)
	lan9255EEPROMEmulationPLIB.setDescription("Default EEPROM EMULATION Enabled")
	lan9255EEPROMEmulationPLIB.setDefaultValue(True)
	
	# lan9255 Set Interrupt priority level and disable interrupts upto this value for SAM devices and after that value for PIC32M devices.
	lan9255InterruptPriority = ethercatlan9255Component.createIntegerSymbol("DRV_LAN9255_INT_PRIO", None)
	lan9255InterruptPriority.setLabel("EtherCAT Interrupt Priority Level")
	lan9255InterruptPriority.setDescription("EtherCAT Interrupt Priority Level to disable interrupt during critical section")
	lan9255InterruptPriority.setDefaultValue(4)
	lan9255InterruptPriority.setMax(7)
	lan9255InterruptPriority.setMin(0)
	lan9255InterruptPriority.setVisible(True)
	
	periphNode = ATDF.getNode("/avr-tools-device-file/devices/device/peripherals")
	modules = periphNode.getChildren()


	for module in range (0, len(modules)):
		periphName = str(modules[module].getAttribute("name"))
		if periphName == "EIC":
			portPlib = "EIC"
		if periphName == "PIO":
			portPlib = "PIO"
		elif periphName == "GPIO":
			portPlib = "GPIO"

	for module in range (0, len(modules)):
		periphName = str(modules[module].getAttribute("name"))
		if periphName == "PORT":
			chipSelectPortPlib = "PORT"
		if periphName == "PIO":
			chipSelectPortPlib = "PIO"
		elif periphName == "GPIO":
			chipSelectPortPlib = "GPIO"

	lan9255portPlib = ethercatlan9255Component.createStringSymbol("PORT_PLIB", None)
	lan9255portPlib.setDefaultValue(portPlib)
	lan9255portPlib.setVisible(False)

	lan9255ChipSelectPortPlib = ethercatlan9255Component.createStringSymbol("DRV_LAN9255_SPI_CHIP_SELECT_PORT_PLIB", None)
	lan9255ChipSelectPortPlib.setDefaultValue(chipSelectPortPlib)
	lan9255ChipSelectPortPlib.setVisible(False)
	
	lan9255ErrorPinSelectPortPlib = ethercatlan9255Component.createStringSymbol("DRV_LAN9255_ERROR_SELECT_PORT_PLIB", None)
	lan9255ErrorPinSelectPortPlib.setDefaultValue(chipSelectPortPlib)
	lan9255ErrorPinSelectPortPlib.setVisible(False)
	
	# lan9255 External interrupt SYNC0 Select pin
	lan9255Sync0Pin = ethercatlan9255Component.createKeyValueSetSymbol("DRV_LAN9255_SYNC0_INT", None)
	lan9255Sync0Pin.setLabel("SYNC0 Interrupt Pin")
	lan9255Sync0Pin.setOutputMode("Key")
	lan9255Sync0Pin.setDisplayMode("Description")

	# lan9255 External interrupt SYNC1 Select pin
	lan9255Sync1Pin = ethercatlan9255Component.createKeyValueSetSymbol("DRV_LAN9255_SYNC1_INT", None)
	lan9255Sync1Pin.setLabel("SYNC1 Interrupt Pin")
	lan9255Sync1Pin.setOutputMode("Key")
	lan9255Sync1Pin.setDisplayMode("Description")

	# lan9255 External interrupt IRQ Select pin
	lan9255IrqPin = ethercatlan9255Component.createKeyValueSetSymbol("DRV_LAN9255_IRQ_INT", None)
	lan9255IrqPin.setLabel("IRQ Interrupt Pin")
	lan9255IrqPin.setOutputMode("Key")
	lan9255IrqPin.setDisplayMode("Description")
	
	# lan9255 SPI Chip Select pin
	lan9255ChipSelectPin = ethercatlan9255Component.createKeyValueSetSymbol("DRV_LAN9255_CHIP_SELECT_PIN", None)
	lan9255ChipSelectPin.setLabel("SPI Chip Select Pin")
	lan9255ChipSelectPin.setOutputMode("Key")
	lan9255ChipSelectPin.setDisplayMode("Description")
	lan9255ChipSelectPin.setVisible(False)
	
	# lan9255 Driver Error Select pin
	lan9255ErrorSelectPin = ethercatlan9255Component.createKeyValueSetSymbol("DRV_LAN9255_ERROR_SELECT_PIN", None)
	lan9255ErrorSelectPin.setLabel("Error Select Pin")
	lan9255ErrorSelectPin.setOutputMode("Key")
	lan9255ErrorSelectPin.setDisplayMode("Description")
	
	lan9255SPIModeAccessType = ethercatlan9255Component.createComboSymbol("DRV_LAN9255_SPI_MODE_ACCESS_TYPE", None, ETHERCAT_DRV_LAN9255_SPI_MODE_ACCESS_TYPE)
	lan9255SPIModeAccessType.setLabel("Driver SPI Mode Access Type")
	lan9255SPIModeAccessType.setVisible(False)
	lan9255SPIModeAccessType.setDescription("Select Ethercat Driver Mode Access Type")
	lan9255SPIModeAccessType.setDefaultValue("ETHERCAT_SPI_INDIRECT_MODE_ACCESS") 
	
	lan9255SQIModeAccessType = ethercatlan9255Component.createComboSymbol("DRV_LAN9255_SQI_MODE_ACCESS_TYPE", None, ETHERCAT_DRV_LAN9255_SQI_MODE_ACCESS_TYPE)
	lan9255SQIModeAccessType.setLabel("Driver SQI Mode Access Type")
	lan9255SQIModeAccessType.setVisible(False)
	lan9255SQIModeAccessType.setDescription("Select Ethercat Driver SQI Mode Access Type")
	lan9255SQIModeAccessType.setDefaultValue("ETHERCAT_SQI_INDIRECT_MODE_ACCESS") 
	

	# Send message to core to get available pins
	if portPlib == "EIC":
		eicNode = ATDF.getNode("/avr-tools-device-file/devices/device/peripherals/module@[name=\"EIC\"]/instance@[name=\"EIC\"]/parameters/param@[name=\"EXTINT_NUM\"]")
		extIntCount = int(eicNode.getAttribute('value'))
		
		for value in range(extIntCount):
			key = "EIC_PIN_" + str(value)
			description = "EIC_EXTINT" + str(value)
			
			lan9255Sync0Pin.addKey(key, str(value), description)
			lan9255Sync1Pin.addKey(key, str(value), description)
			lan9255IrqPin.addKey(key, str(value), description)
			
 	elif portPlib == "GPIO" or portPlib == "PIO":
		# Send message to core to get available pins
		availablePinDictionary = {}
		availablePinDictionary = Database.sendMessage("core", "PIN_LIST", availablePinDictionary)
		
		for pad in sort_alphanumeric(availablePinDictionary.values()):
			if portPlib == "GPIO":
				key = "GPIO_PIN_" + pad
			elif portPlib == "PIO":
				key = "PIO_PIN_" + pad
			elif portPlib == "PORT":
				key = "PORT_PIN_" + pad
			value = list(availablePinDictionary.keys())[list(availablePinDictionary.values()).index(pad)]
			description = pad
			lan9255Sync0Pin.addKey(key, value, description)
			lan9255Sync1Pin.addKey(key, value, description)
			lan9255IrqPin.addKey(key, value, description)

	if chipSelectPortPlib == "GPIO" or chipSelectPortPlib == "PIO" or chipSelectPortPlib == "PORT":
		# Send message to core to get available pins
		chipSelectavailablePinDictionary = {}
		chipSelectavailablePinDictionary = Database.sendMessage("core", "PIN_LIST", chipSelectavailablePinDictionary)
		
		for pad in sort_alphanumeric(chipSelectavailablePinDictionary.values()):
			if chipSelectPortPlib == "GPIO":
				key = "GPIO_PIN_" + pad
			elif chipSelectPortPlib == "PIO":
				key = "PIO_PIN_" + pad
			elif chipSelectPortPlib == "PORT":
				key = "PORT_PIN_" + pad
			value = list(chipSelectavailablePinDictionary.keys())[list(chipSelectavailablePinDictionary.values()).index(pad)]
			description = pad
			lan9255ChipSelectPin.addKey(key, value, description)
			lan9255ErrorSelectPin.addKey(key, value, description)

	lan9255ChipConfigComment = ethercatlan9255Component.createCommentSymbol("DRV_lan9255_PINS_CONFIG_COMMENT", None)
	lan9255ChipConfigComment.setLabel("***Above selected pins must be configured in Pin Manager***")
	
	# driver/lan9255/drv_lan9255.h to config/<configName>/driver/lan9255/drv_lan9255.h
	lan9255HeaderFile = ethercatlan9255Component.createFileSymbol(None, None)
	lan9255HeaderFile.setSourcePath("driver/lan9255/src/drv_lan9255.h")
	lan9255HeaderFile.setOutputName("drv_lan9255.h")
	lan9255HeaderFile.setDestPath("driver/lan9255/")
	lan9255HeaderFile.setProjectPath("config/" + configName + "/driver/lan9255/")
	lan9255HeaderFile.setType("HEADER")
	lan9255HeaderFile.setOverwrite(True)
	lan9255HeaderFile.setEnabled(True)
	
	# file driver/lan9255/drv_lan9255.c to config/<configName>/driver/lan9255/drv_lan9255.c    
	lan9255SourceFile = ethercatlan9255Component.createFileSymbol(None, None)
	lan9255SourceFile.setSourcePath("driver/lan9255/src/drv_lan9255.c")
	lan9255SourceFile.setOutputName("drv_lan9255.c")
	lan9255SourceFile.setOverwrite(True)
	lan9255SourceFile.setDestPath("driver/lan9255/")
	lan9255SourceFile.setProjectPath("config/" + configName + "/driver/lan9255/")
	lan9255SourceFile.setType("SOURCE")
	lan9255SourceFile.setEnabled(True)
	
	# file driver/lan9255/drv_lan9255_definitions.h to config/<configName>/driver/lan9255/drv_lan9255_definitions.h 
	lan9255DefinitionHeaderFile = ethercatlan9255Component.createFileSymbol("DRV_LAN9255_DEF", None)
	lan9255DefinitionHeaderFile.setSourcePath("driver/lan9255/templates/drv_lan9255_definitions.h.ftl")
	lan9255DefinitionHeaderFile.setOutputName("drv_lan9255_definitions.h")
	lan9255DefinitionHeaderFile.setDestPath("driver/lan9255")
	lan9255DefinitionHeaderFile.setProjectPath("config/" + configName + "/driver/lan9255/")
	lan9255DefinitionHeaderFile.setType("HEADER")
	lan9255DefinitionHeaderFile.setOverwrite(True)
	lan9255DefinitionHeaderFile.setEnabled(True)
	lan9255DefinitionHeaderFile.setMarkup(True)
	
	lan9255SystemDefFile = ethercatlan9255Component.createFileSymbol("LAN9255_DEF", None)
	lan9255SystemDefFile.setType("STRING")
	lan9255SystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	lan9255SystemDefFile.setSourcePath("driver/lan9255/templates/system/definitions.h.ftl")
	lan9255SystemDefFile.setMarkup(True)
	
	lan9255SymSystemInitDataFile = ethercatlan9255Component.createFileSymbol("DRV_LAN9255_INIT_DATA", None)
	lan9255SymSystemInitDataFile.setType("STRING")
	lan9255SymSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
	lan9255SymSystemInitDataFile.setSourcePath("driver/lan9255/templates/system/initialize_data.c.ftl")
	lan9255SymSystemInitDataFile.setMarkup(True)
	
	lan9255SystemInitFile = ethercatlan9255Component.createFileSymbol("LAN9255_INIT", None)
	lan9255SystemInitFile.setType("STRING")
	lan9255SystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
	lan9255SystemInitFile.setSourcePath("driver/lan9255/templates/system/initialize.c.ftl")
	lan9255SystemInitFile.setMarkup(True)
	
	# set driver/lan9255 include paths
	lan9255DriverDefSym = ethercatlan9255Component.createSettingSymbol("ETHERCAT_LAN9255_INCLUDE_DIRS", None)
	lan9255DriverDefSym.setCategory("C32")
	lan9255DriverDefSym.setKey("extra-include-directories")
	lan9255DriverDefSym.setValue( "../src/config/" + configName
						+ "/driver/lan9255"
						)
	lan9255DriverDefSym.setAppend(True, ";")
	
	# driver/lan9255/drv_lan9255_ecat_util.h to lan9255 driver path
	lan9255ecatUtilHeaderFile = ethercatlan9255Component.createFileSymbol(None, None)
	lan9255ecatUtilHeaderFile.setSourcePath("driver/lan9255/templates/system/drv_lan9255_ecat_util.h.ftl")
	lan9255ecatUtilHeaderFile.setOutputName("drv_lan9255_ecat_util.h")
	lan9255ecatUtilHeaderFile.setDestPath("driver/lan9255/")
	lan9255ecatUtilHeaderFile.setProjectPath("config/" + configName + "/driver/lan9255/")
	lan9255ecatUtilHeaderFile.setType("HEADER")
	lan9255ecatUtilHeaderFile.setOverwrite(True)
	lan9255ecatUtilHeaderFile.setEnabled(True)
	lan9255ecatUtilHeaderFile.setMarkup(True)
	
	# file driver/lan9255/drv_lan9255_ecat_util.c to the lan9255 driver path
	lan9255ecatUtilSourceFile = ethercatlan9255Component.createFileSymbol(None, None)
	lan9255ecatUtilSourceFile.setSourcePath("driver/lan9255/templates/system/drv_lan9255_ecat_util.c.ftl")
	lan9255ecatUtilSourceFile.setOutputName("drv_lan9255_ecat_util.c")
	lan9255ecatUtilSourceFile.setOverwrite(True)
	lan9255ecatUtilSourceFile.setDestPath("driver/lan9255/")
	lan9255ecatUtilSourceFile.setProjectPath("config/" + configName + "/driver/lan9255/")
	lan9255ecatUtilSourceFile.setType("SOURCE")
	lan9255ecatUtilSourceFile.setEnabled(True)
	lan9255ecatUtilSourceFile.setMarkup(True)
	
		
def onAttachmentConnected(source, target):

	localComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	connectID = source["id"]
	
	lan9255PlibId = remoteID.upper()
			
	if connectID == "LAN9255_SPI_Dependency":
		localComponent.getSymbolByID("DRV_LAN9255_PROTOCOL").setValue("SPI")
		plibUsed = localComponent.getSymbolByID("DRV_LAN9255_PLIB")
		plibUsed.clearValue()
		
		plibUsed.setValue(lan9255PlibId.upper())
		plibUsed.setVisible(True)
		print("Remote SPI PLIB connected: " + lan9255PlibId)
		
		localComponent.getSymbolByID("DRV_LAN9255_SPI_MODE_ACCESS_TYPE").setVisible(True)
		localComponent.getSymbolByID("DRV_LAN9255_CHIP_SELECT_PIN").setVisible(True)
		
		localComponent.getSymbolByID("DRV_LAN9255_SQI_MODE_ACCESS_TYPE").setVisible(False)
		
		localComponent.setDependencyEnabled("LAN9255_SQI_Dependency", False)
		
		localComponent.setDependencyEnabled("LAN9255_SPI_Dependency", True)

		
	if connectID == "LAN9255_SQI_Dependency":
		localComponent.getSymbolByID("DRV_LAN9255_PROTOCOL").setValue("SQI")
		plibUsed = localComponent.getSymbolByID("DRV_LAN9255_PLIB")
		plibUsed.clearValue()
		print("Remote SQI PLIB connected: " + lan9255PlibId)
		plibUsed.setValue(lan9255PlibId.upper())
		plibUsed.setVisible(True)
		
		localComponent.getSymbolByID("DRV_LAN9255_SPI_MODE_ACCESS_TYPE").setVisible(False)
		localComponent.getSymbolByID("DRV_LAN9255_CHIP_SELECT_PIN").setVisible(False)
		
		localComponent.getSymbolByID("DRV_LAN9255_SQI_MODE_ACCESS_TYPE").setVisible(True)
		
		localComponent.setDependencyEnabled("LAN9255_SQI_Dependency", True)
		
		localComponent.setDependencyEnabled("LAN9255_SPI_Dependency", False)
	
		
	
	if connectID == "LAN9255_TMR_Dependency":
		timerPlibUsed = localComponent.getSymbolByID("DRV_LAN9255_TIMER_PLIB")
		timerPlibUsed.clearValue()
		lan9255PlibId = remoteID.upper()
		timerPlibUsed.setValue(lan9255PlibId)

		
def onAttachmentDisconnected(source, target):

	localComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	connectID = source["id"]
	targetID = target["id"]

	if connectID == "LAN9255_SPI_Dependency":
		plibUsed = localComponent.getSymbolByID("DRV_LAN9255_PLIB")
		plibUsed.clearValue()
		
		dummyDict = {}
		dummyDict = Database.sendMessage(remoteID, "SPI_MASTER_MODE", {"isReadOnly":False})
		dummyDict = Database.sendMessage(remoteID, "SPI_MASTER_INTERRUPT_MODE", {"isReadOnly":False})
		dummyDict = Database.sendMessage(remoteID, "SPI_MASTER_HARDWARE_CS", {"isReadOnly":False})
		
		localComponent.setDependencyEnabled("LAN9255_SQI_Dependency", True)
		
		localComponent.setDependencyEnabled("LAN9255_SPI_Dependency", True)
		
		localComponent.getSymbolByID("DRV_LAN9255_SPI_MODE_ACCESS_TYPE").setVisible(False)
		localComponent.getSymbolByID("DRV_LAN9255_CHIP_SELECT_PIN").setVisible(False)
		
		localComponent.getSymbolByID("DRV_LAN9255_SQI_MODE_ACCESS_TYPE").setVisible(False)
		
	if connectID == "LAN9255_SQI_Dependency":
		plibUsed = localComponent.getSymbolByID("DRV_LAN9255_PLIB")
		plibUsed.clearValue()
		
		localComponent.setDependencyEnabled("LAN9255_SQI_Dependency", True)
		
		localComponent.setDependencyEnabled("LAN9255_SPI_Dependency", True)
		
		localComponent.getSymbolByID("DRV_LAN9255_SPI_MODE_ACCESS_TYPE").setVisible(False)
		localComponent.getSymbolByID("DRV_LAN9255_CHIP_SELECT_PIN").setVisible(False)
		
		localComponent.getSymbolByID("DRV_LAN9255_SQI_MODE_ACCESS_TYPE").setVisible(False)	
		
	if connectID == "LAN9255_TMR_Dependency":
		timerPlibUsed = localComponent.getSymbolByID("DRV_LAN9255_TIMER_PLIB")
		timerPlibUsed.clearValue()
		
