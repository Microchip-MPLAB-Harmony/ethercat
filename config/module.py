######################  Harmony EtherCAT  ######################
def loadModule():
	print("Load Module: Harmony EtherCAT")
	etherCatComponent = Module.CreateComponent("ethercat_apps_config", "EtherCAT Stack", "/Libraries/EtherCAT/", "config/ethercat_configurator_apps.py")
	etherCatComponent.addDependency("EtherCAT_LANDRV_Dependency", "LAN_DRV", None, True, True)
#	etherCatComponent.addDependency("EtherCAT_LAN9253_Dependency", "LAN_9253", None, True, True)
	etherCatComponent.setDisplayType("EtherCAT Library")
	
	###########  ETHERCAT LAN 9252 driver componet ###########   
	ethercatLan9252Component = Module.CreateComponent("ethercat_lan9252", "LAN 9252", "/Harmony/Drivers/EtherCAT Driver/Internal/", "config/drv_lan9252.py")
	ethercatLan9252Component.addCapability("drvEthercatLan9252","LAN_DRV", True)
	ethercatLan9252Component.addDependency("LAN9252_SPI_Dependency", "SPI", None, False, False)
	
	###########  ETHERCAT LAN 9253 driver componet ###########   
	ethercatLan9253Component = Module.CreateComponent("ethercat_lan9253", "LAN 9253", "/Harmony/Drivers/EtherCAT Driver/Internal/", "config/drv_lan9253.py")
	ethercatLan9253Component.addCapability("drvEthercatLan9253","LAN_DRV", True)
	ethercatLan9253Component.addDependency("LAN9253_SPI_Dependency", "SPI", None, False, False)
	
