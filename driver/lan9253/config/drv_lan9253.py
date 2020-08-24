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

global sort_alphanumeric
global portPlib
global chipSelectPortPlib
global lan9253Sync0Pin
global lan9253Sync1Pin
global lan9253IrqPin

ETHERCAT_DRV_LAN9253_MODE_ACCESS_TYPE = ["ETHERCAT_SPI_INDIRECT_MODE_ACCESS", "ETHERCAT_SPI_DIRECT_MODE_ACCESS", "ETHERCAT_SPI_BECKHOFF_MODE_ACCESS"]

def sort_alphanumeric(l):
	import re
	convert = lambda text: int(text) if text.isdigit() else text.lower()
	alphanum_key = lambda key: [ convert(c) for c in re.split('([0-9]+)', key) ]
	return sorted(l, key = alphanum_key)

def instantiateComponent(ethercatlan9253Component):
		
	print("Ethercat LAN 9252 Driver Component")
	configName = Variables.get("__CONFIGURATION_NAME")
	
	lan9253PLIBUse = ethercatlan9253Component.createBooleanSymbol("DRV_LAN9253_USE", None)
	lan9253PLIBUse.setLabel("Driver LAN9253 is Used")
	lan9253PLIBUse.setReadOnly(True)
	lan9253PLIBUse.setVisible(False)
	lan9253PLIBUse.setDefaultValue(True)
		
	lan9253SPIPLIB = ethercatlan9253Component.createStringSymbol("DRV_LAN9253_PLIB", None)
	lan9253SPIPLIB.setLabel("PLIB Used")
	lan9253SPIPLIB.setReadOnly(True)
	lan9253SPIPLIB.setVisible(False)

	lan9253TimerPLIB = ethercatlan9253Component.createStringSymbol("DRV_LAN9253_TIMER_PLIB", None)
	lan9253TimerPLIB.setLabel("PLIB Used")
	lan9253TimerPLIB.setReadOnly(True)
	lan9253TimerPLIB.setVisible(False)
	
	# lan9253 Set Interrupt priority level and disable interrupts upto this value for SAM devices and after that value for PIC32M devices.
	lan9253InterruptPriority = ethercatlan9253Component.createIntegerSymbol("DRV_LAN9253_INT_PRIO", None)
	lan9253InterruptPriority.setLabel("EtherCAT Interrupt Priority Level")
	lan9253InterruptPriority.setDescription("EtherCAT Interrupt Priority Level to disable interrupt during critical section")
	lan9253InterruptPriority.setDefaultValue(4)
	lan9253InterruptPriority.setMax(7)
	lan9253InterruptPriority.setMin(0)
	lan9253InterruptPriority.setVisible(True)
	
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

	lan9253portPlib = ethercatlan9253Component.createStringSymbol("PORT_PLIB", None)
	lan9253portPlib.setDefaultValue(portPlib)
	lan9253portPlib.setVisible(False)

	lan9253ChipSelectPortPlib = ethercatlan9253Component.createStringSymbol("DRV_LAN9253_SPI_CHIP_SELECT_PORT_PLIB", None)
	lan9253ChipSelectPortPlib.setDefaultValue(chipSelectPortPlib)
	lan9253ChipSelectPortPlib.setVisible(False)
	
	lan9253ErrorPinSelectPortPlib = ethercatlan9253Component.createStringSymbol("DRV_LAN9253_ERROR_SELECT_PORT_PLIB", None)
	lan9253ErrorPinSelectPortPlib.setDefaultValue(chipSelectPortPlib)
	lan9253ErrorPinSelectPortPlib.setVisible(False)
	
	# lan9253 External interrupt SYNC0 Select pin
	lan9253Sync0Pin = ethercatlan9253Component.createKeyValueSetSymbol("DRV_LAN9253_SYNC0_INT", None)
	lan9253Sync0Pin.setLabel("SYNC0 Interrupt Pin")
	lan9253Sync0Pin.setOutputMode("Key")
	lan9253Sync0Pin.setDisplayMode("Description")

	# lan9253 External interrupt SYNC1 Select pin
	lan9253Sync1Pin = ethercatlan9253Component.createKeyValueSetSymbol("DRV_LAN9253_SYNC1_INT", None)
	lan9253Sync1Pin.setLabel("SYNC1 Interrupt Pin")
	lan9253Sync1Pin.setOutputMode("Key")
	lan9253Sync1Pin.setDisplayMode("Description")

	# lan9253 External interrupt IRQ Select pin
	lan9253IrqPin = ethercatlan9253Component.createKeyValueSetSymbol("DRV_LAN9253_IRQ_INT", None)
	lan9253IrqPin.setLabel("IRQ Interrupt Pin")
	lan9253IrqPin.setOutputMode("Key")
	lan9253IrqPin.setDisplayMode("Description")
	
	# lan9253 SPI Chip Select pin
	lan9253ChipSelectPin = ethercatlan9253Component.createKeyValueSetSymbol("DRV_LAN9253_CHIP_SELECT_PIN", None)
	lan9253ChipSelectPin.setLabel("SPI Chip Select Pin")
	lan9253ChipSelectPin.setOutputMode("Key")
	lan9253ChipSelectPin.setDisplayMode("Description")
	
	# lan9253 Driver Error Select pin
	lan9253ErrorSelectPin = ethercatlan9253Component.createKeyValueSetSymbol("DRV_LAN9253_ERROR_SELECT_PIN", None)
	lan9253ErrorSelectPin.setLabel("Error Select Pin")
	lan9253ErrorSelectPin.setOutputMode("Key")
	lan9253ErrorSelectPin.setDisplayMode("Description")
	
	lan9253SPIModeAccessType = ethercatlan9253Component.createComboSymbol("DRV_LAN9253_SPI_MODE_ACCESS_TYPE", None, ETHERCAT_DRV_LAN9253_MODE_ACCESS_TYPE)
	lan9253SPIModeAccessType.setLabel("Driver SPI Mode Access Type")
	lan9253SPIModeAccessType.setVisible(True)
	lan9253SPIModeAccessType.setDescription("Select Ethercat Driver Mode Access Type")
	lan9253SPIModeAccessType.setDefaultValue("ETHERCAT_SPI_INDIRECT_MODE_ACCESS") 
	
	

	# Send message to core to get available pins
	if portPlib == "EIC":
		eicNode = ATDF.getNode("/avr-tools-device-file/devices/device/peripherals/module@[name=\"EIC\"]/instance@[name=\"EIC\"]/parameters/param@[name=\"EXTINT_NUM\"]")
		extIntCount = int(eicNode.getAttribute('value'))
		
		for value in range(extIntCount):
			key = "EIC_PIN_" + str(value)
			description = "EIC_EXTINT" + str(value)
			
			lan9253Sync0Pin.addKey(key, str(value), description)
			lan9253Sync1Pin.addKey(key, str(value), description)
			lan9253IrqPin.addKey(key, str(value), description)
			
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
			lan9253Sync0Pin.addKey(key, value, description)
			lan9253Sync1Pin.addKey(key, value, description)
			lan9253IrqPin.addKey(key, value, description)

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
			lan9253ChipSelectPin.addKey(key, value, description)
			lan9253ErrorSelectPin.addKey(key, value, description)

	lan9253ChipConfigComment = ethercatlan9253Component.createCommentSymbol("DRV_LAN9253_PINS_CONFIG_COMMENT", None)
	lan9253ChipConfigComment.setLabel("***Above selected pins must be configured in Pin Manager***")
	
	# driver/lan9253/drv_lan9253.h to config/<configName>/driver/lan9253/drv_lan9253.h
	lan9253HeaderFile = ethercatlan9253Component.createFileSymbol(None, None)
	lan9253HeaderFile.setSourcePath("driver/lan9253/src/drv_lan9253.h")
	lan9253HeaderFile.setOutputName("drv_lan9253.h")
	lan9253HeaderFile.setDestPath("driver/lan9253/")
	lan9253HeaderFile.setProjectPath("config/" + configName + "/driver/lan9253/")
	lan9253HeaderFile.setType("HEADER")
	lan9253HeaderFile.setOverwrite(True)
	lan9253HeaderFile.setEnabled(True)
	
	# file driver/lan9253/drv_lan9253.c to config/<configName>/driver/lan9253/drv_lan9253.c    
	lan9253SourceFile = ethercatlan9253Component.createFileSymbol(None, None)
	lan9253SourceFile.setSourcePath("driver/lan9253/src/drv_lan9253.c")
	lan9253SourceFile.setOutputName("drv_lan9253.c")
	lan9253SourceFile.setOverwrite(True)
	lan9253SourceFile.setDestPath("driver/lan9253/")
	lan9253SourceFile.setProjectPath("config/" + configName + "/driver/lan9253/")
	lan9253SourceFile.setType("SOURCE")
	lan9253SourceFile.setEnabled(True)
	
	# file driver/lan9253/drv_lan9253_definitions.h to config/<configName>/driver/lan9253/drv_lan9253_definitions.h 
	lan9253DefinitionHeaderFile = ethercatlan9253Component.createFileSymbol("DRV_LAN9253_DEF", None)
	lan9253DefinitionHeaderFile.setSourcePath("driver/lan9253/drv_lan9253_definitions.h")
	lan9253DefinitionHeaderFile.setOutputName("drv_lan9253_definitions.h")
	lan9253DefinitionHeaderFile.setDestPath("driver/lan9253")
	lan9253DefinitionHeaderFile.setProjectPath("config/" + configName + "/driver/lan9253/")
	lan9253DefinitionHeaderFile.setType("HEADER")
	lan9253DefinitionHeaderFile.setOverwrite(True)
	
	
	lan9253SystemDefFile = ethercatlan9253Component.createFileSymbol("LAN9253_DEF", None)
	lan9253SystemDefFile.setType("STRING")
	lan9253SystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	lan9253SystemDefFile.setSourcePath("driver/lan9253/templates/definitions.h.ftl")
	lan9253SystemDefFile.setMarkup(True)
	
	lan9253SymSystemInitDataFile = ethercatlan9253Component.createFileSymbol("DRV_LAN9253_INIT_DATA", None)
	lan9253SymSystemInitDataFile.setType("STRING")
	lan9253SymSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
	lan9253SymSystemInitDataFile.setSourcePath("driver/lan9253/templates/initialize_data.c.ftl")
	lan9253SymSystemInitDataFile.setMarkup(True)
	
	lan9253SystemInitFile = ethercatlan9253Component.createFileSymbol("LAN9253_INIT", None)
	lan9253SystemInitFile.setType("STRING")
	lan9253SystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
	lan9253SystemInitFile.setSourcePath("driver/lan9253/templates/initialize.c.ftl")
	lan9253SystemInitFile.setMarkup(True)
	
	# set driver/lan9253 include paths
	lan9253DriverDefSym = ethercatlan9253Component.createSettingSymbol("ETHERCAT_LAN9253_INCLUDE_DIRS", None)
	lan9253DriverDefSym.setCategory("C32")
	lan9253DriverDefSym.setKey("extra-include-directories")
	lan9253DriverDefSym.setValue( "../src/config/" + configName
						+ "/driver/lan9253"
						)
	lan9253DriverDefSym.setAppend(True, ";")
	
	# driver/lan9253/drv_lan9253_ecat_util.h to lan9253 driver path
	lan9253ecatUtilHeaderFile = ethercatlan9253Component.createFileSymbol(None, None)
	lan9253ecatUtilHeaderFile.setSourcePath("driver/lan9253/templates/drv_lan9253_ecat_util.h.ftl")
	lan9253ecatUtilHeaderFile.setOutputName("drv_lan9253_ecat_util.h")
	lan9253ecatUtilHeaderFile.setDestPath("driver/lan9253/")
	lan9253ecatUtilHeaderFile.setProjectPath("config/" + configName + "/driver/lan9253/")
	lan9253ecatUtilHeaderFile.setType("HEADER")
	lan9253ecatUtilHeaderFile.setOverwrite(True)
	lan9253ecatUtilHeaderFile.setEnabled(True)
	lan9253ecatUtilHeaderFile.setMarkup(True)
	
	# file driver/lan9253/drv_lan9253_ecat_util.c to the lan9253 driver path
	lan9253ecatUtilSourceFile = ethercatlan9253Component.createFileSymbol(None, None)
	lan9253ecatUtilSourceFile.setSourcePath("driver/lan9253/templates/drv_lan9253_ecat_util.c.ftl")
	lan9253ecatUtilSourceFile.setOutputName("drv_lan9253_ecat_util.c")
	lan9253ecatUtilSourceFile.setOverwrite(True)
	lan9253ecatUtilSourceFile.setDestPath("driver/lan9253/")
	lan9253ecatUtilSourceFile.setProjectPath("config/" + configName + "/driver/lan9253/")
	lan9253ecatUtilSourceFile.setType("SOURCE")
	lan9253ecatUtilSourceFile.setEnabled(True)
	lan9253ecatUtilSourceFile.setMarkup(True)
	
def onAttachmentConnected(source, target):

	localComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	connectID = source["id"]
	targetID = target["id"]
	sysTimeDict = {"ID":"sys_time"}

	if connectID == "LAN9253_SPI_Dependency":
		plibUsed = localComponent.getSymbolByID("DRV_LAN9253_PLIB")
		plibUsed.clearValue()
		lan9253PlibId = remoteID.upper()
		plibUsed.setValue(lan9253PlibId.upper())
		Database.setSymbolValue(lan9253PlibId, "SPI_DRIVER_CONTROLLED", True)
		print "SPI RemoteComponetID = " + remoteID
		
	if (connectID == "LAN9253_TMR_Dependency"):
		plibUsed = localComponent.getSymbolByID("DRV_LAN9253_TIMER_PLIB")
		plibUsed.clearValue()
		lan9253PlibId = remoteID.upper()
		plibUsed.setValue(lan9253PlibId)
		#Request PLIB to publish it's capabilities
		sysTimeDict = Database.sendMessage(remoteID, "SYS_TIME_PUBLISH_CAPABILITIES", sysTimeDict)
		print "RemoteComponentID = " + remoteID
		
def onAttachmentDisconnected(source, target):

	localComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	connectID = source["id"]
	targetID = target["id"]

	if connectID == "LAN9253_SPI_Dependency":
		plibUsed = localComponent.getSymbolByID("DRV_LAN9253_PLIB")
		plibUsed.clearValue()
		lan9253PlibId = remoteID.upper()
		Database.setSymbolValue(lan9253PlibId, "SPI_DRIVER_CONTROLLED", False)
	if (connectID == "LAN9253_TMR_Dependency"):
		plibUsed = localComponent.getSymbolByID("DRV_lan9253_TIMER_PLIB")
		plibUsed.clearValue()
		plibUsed.setValue("")
		
