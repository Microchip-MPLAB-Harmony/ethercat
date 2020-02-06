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
	
	# driver/lan9252/drv_lan9252.h to config/<configName>/driver/lan9252/drv_lan9252.h
	lan9252HeaderFile = ethercatLan9252Component.createFileSymbol(None, None)
	lan9252HeaderFile.setSourcePath("driver/lan9252/drv_lan9252.h")
	lan9252HeaderFile.setOutputName("drv_lan9252.h")
	lan9252HeaderFile.setDestPath("driver/lan9252/")
	lan9252HeaderFile.setProjectPath("config/" + configName + "/driver/lan9252/")
	lan9252HeaderFile.setType("HEADER")
	lan9252HeaderFile.setOverwrite(True)
	lan9252HeaderFile.setEnabled(True)	
	
	# file driver/lan9252/drv_lan9252.c to config/<configName>/driver/lan9252/drv_lan9252.c    
	lan9252SourceFile = ethercatLan9252Component.createFileSymbol(None, None)
	lan9252SourceFile.setSourcePath("driver/lan9252/drv_lan9252.c")
	lan9252SourceFile.setOutputName("drv_lan9252.c")
	lan9252SourceFile.setOverwrite(True)
	lan9252SourceFile.setDestPath("driver/lan9252/")
	lan9252SourceFile.setProjectPath("config/" + configName + "/driver/lan9252/")
	lan9252SourceFile.setType("SOURCE")
	lan9252SourceFile.setEnabled(True)
	
	# set TCP/IP include paths
	lan9252DriverDefSym = ethercatLan9252Component.createSettingSymbol("ETHERCAT_LAN9252_INCLUDE_DIRS", None)
	lan9252DriverDefSym.setCategory("C32")
	lan9252DriverDefSym.setKey("extra-include-directories")
	lan9252DriverDefSym.setValue( "../src/config/" + configName
						+ "/driver/lan9252"
						)
	lan9252DriverDefSym.setAppend(True, ";")

#def onAttachmentConnected(source, target):
#	if (source["id"] == "LAN9252_SPI_Dependency"): 
#		Database.setSymbolValue("ethercat_lan9252", None, target["component"].getDisplayName(),2)
		
#def onAttachmentDisconnected(source, target):
#	if (source["id"] == "LAN9252_SPI_Dependency"): 
#		Database.clearSymbolValue("ethercat_lan9252", None)		