// <editor-fold defaultstate="collapsed" desc="EtherCAT Main Initialization Data">

<#if DRV_LAN9255_PROTOCOL == "SPI">
/* SQI PLIB Interface Initialization for ethercat LAN driver */
const DRV_LAN9255_UTIL_SPI_PLIB_INTERFACE drvLAN9255SpiPlibAPI = {

	/* SPI PLIB WriteRead function */
	.spiWriteRead = (DRV_LAN9255_SPI_PLIB_WRITE_READ)${.vars["${DRV_LAN9255_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_WriteRead,

	/* SPI PLIB Write function */
	.spiWrite = (DRV_LAN9255_SPI_PLIB_WRITE)${.vars["${DRV_LAN9255_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_Write,

	/* SPI PLIB Read function */
	.spiRead = (DRV_LAN9255_SPI_PLIB_READ)${.vars["${DRV_LAN9255_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_Read,

	/* SPI PLIB Transfer Status function */
	.spiIsBusy = (DRV_LAN9255_SPI_PLIB_IS_BUSY)${.vars["${DRV_LAN9255_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_IsBusy,

	/* SPI PLIB Callback Register */
	.spiCallbackRegister = (DRV_LAN9255_SPI_PLIB_CALLBACK_REGISTER)${.vars["${DRV_LAN9255_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_CallbackRegister,
};
<#elseif DRV_LAN9255_PROTOCOL == "SQI">
/* SQI PLIB Interface Initialization for ethercat LAN driver */
<#if DRV_LAN9255_PLIB?contains("QSPI") >
const DRV_LAN9255_UTIL_SQI_PLIB_INTERFACE drvLAN9255SQIPlibAPI = {
	/* SQI PLIB Command Write function */
	.sqiCommandWrite = (DRV_LAN9255_SQI_PLIB_CMD_WRITE)${DRV_LAN9255_PLIB}_CommandWrite,

	/* SQI PLIB Register Read function */
	.sqiRegisterRead = (DRV_LAN9255_SQI_PLIB_REG_READ)${DRV_LAN9255_PLIB}_RegisterRead,
	
	/* SQI PLIB Register Write function */
	.sqiRegisterWrite = (DRV_LAN9255_SQI_PLIB_REG_WRITE)${DRV_LAN9255_PLIB}_RegisterWrite,

	/* SQI PLIB Memory Read Transfer Status function */
	.sqiMemoryRead = (DRV_LAN9255_SQI_PLIB_MEM_READ)${DRV_LAN9255_PLIB}_MemoryRead,

	/* SQI PLIB Callback Register */
	.sqiMemoryWrite = (DRV_LAN9255_SQI_PLIB_MEM_WRITE)${DRV_LAN9255_PLIB}_MemoryWrite,
};
<#elseif DRV_LAN9255_PLIB?contains("SQI") >
const DRV_LAN9255_UTIL_SQI_PLIB_INTERFACE drvSST26PlibAPI = {
	.sqiDMATransfer       = ${DRV_LAN9255_PLIB}_DMATransfer,
	.sqiRegisterCallback  = ${DRV_LAN9255_PLIB}_RegisterCallback,
	};	
</#if>
</#if>
const DRV_LAN9255_UTIL_TMR_PLIB_INTERFACE drvLAN9255TimerPlibAPI = {

	.timerCallbackSet = (DRV_LAN9255_TMR_PLIB_CALLBACK_REGISTER)${.vars["${DRV_LAN9255_TIMER_PLIB?lower_case}"].CALLBACK_API_NAME},
	
	.timerStart = (DRV_LAN9255_TMR_PLIB_START)${.vars["${DRV_LAN9255_TIMER_PLIB?lower_case}"].TIMER_START_API_NAME},
	
	.timerStop = (DRV_LAN9255_TMR_PLIB_STOP)${.vars["${DRV_LAN9255_TIMER_PLIB?lower_case}"].TIMER_STOP_API_NAME},
};

/* LAN9255 Driver Initialization Data */
const DRV_LAN9255_UTIL_INIT drvLAN9255InitData = {
<#if DRV_LAN9255_PROTOCOL == "SPI">
	/* SPI PLIB API  interface*/
	.spiPlib = &drvLAN9255SpiPlibAPI,
<#elseif DRV_LAN9255_PROTOCOL == "SQI">
	/* SPI PLIB API  interface*/
	.sqiPlib = &drvLAN9255SQIPlibAPI,
</#if>
	
	/* Timer PLIB API Interface */
	.timerPlib = &drvLAN9255TimerPlibAPI,

};

// </editor-fold>