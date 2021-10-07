// <editor-fold defaultstate="collapsed" desc="EtherCAT Main Initialization Data">

<#if DRV_LAN9253_PROTOCOL == "SPI">
/* SQI PLIB Interface Initialization for ethercat LAN driver */
const DRV_LAN9253_UTIL_SPI_PLIB_INTERFACE drvLAN9253SpiPlibAPI = {

	/* SPI PLIB WriteRead function */
	.spiWriteRead = (DRV_LAN9253_SPI_PLIB_WRITE_READ)${.vars["${DRV_LAN9253_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_WriteRead,

	/* SPI PLIB Write function */
	.spiWrite = (DRV_LAN9253_SPI_PLIB_WRITE)${.vars["${DRV_LAN9253_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_Write,

	/* SPI PLIB Read function */
	.spiRead = (DRV_LAN9253_SPI_PLIB_READ)${.vars["${DRV_LAN9253_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_Read,

	/* SPI PLIB Transfer Status function */
	.spiIsBusy = (DRV_LAN9253_SPI_PLIB_IS_BUSY)${.vars["${DRV_LAN9253_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_IsBusy,

	/* SPI PLIB Callback Register */
	.spiCallbackRegister = (DRV_LAN9253_SPI_PLIB_CALLBACK_REGISTER)${.vars["${DRV_LAN9253_PLIB?lower_case}"].SPI_PLIB_API_PREFIX}_CallbackRegister,
};
<#elseif DRV_LAN9253_PROTOCOL == "SQI">
/* SQI PLIB Interface Initialization for ethercat LAN driver */
<#if DRV_LAN9253_PLIB?contains("QSPI") >
const DRV_LAN9253_UTIL_SQI_PLIB_INTERFACE drvLAN9253SQIPlibAPI = {
	/* SQI PLIB Command Write function */
	.sqiCommandWrite = (DRV_LAN9253_SQI_PLIB_CMD_WRITE)${DRV_LAN9253_PLIB}_CommandWrite,

	/* SQI PLIB Register Read function */
	.sqiRegisterRead = (DRV_LAN9253_SQI_PLIB_REG_READ)${DRV_LAN9253_PLIB}_RegisterRead,
	
	/* SQI PLIB Register Write function */
	.sqiRegisterWrite = (DRV_LAN9253_SQI_PLIB_REG_WRITE)${DRV_LAN9253_PLIB}_RegisterWrite,

	/* SQI PLIB Memory Read Transfer Status function */
	.sqiMemoryRead = (DRV_LAN9253_SQI_PLIB_MEM_READ)${DRV_LAN9253_PLIB}_MemoryRead,

	/* SQI PLIB Callback Register */
	.sqiMemoryWrite = (DRV_LAN9253_SQI_PLIB_MEM_WRITE)${DRV_LAN9253_PLIB}_MemoryWrite,
};
<#elseif DRV_LAN9253_PLIB?contains("SQI") >
const DRV_LAN9253_UTIL_SQI_PLIB_INTERFACE drvSST26PlibAPI = {
	.sqiDMATransfer       = ${DRV_LAN9253_PLIB}_DMATransfer,
	.sqiRegisterCallback  = ${DRV_LAN9253_PLIB}_RegisterCallback,
	};	
</#if>
</#if>
const DRV_LAN9253_UTIL_TMR_PLIB_INTERFACE drvLAN9253TimerPlibAPI = {

	.timerCallbackSet = (DRV_LAN9253_TMR_PLIB_CALLBACK_REGISTER)${.vars["${DRV_LAN9253_TIMER_PLIB?lower_case}"].CALLBACK_API_NAME},
	
	.timerStart = (DRV_LAN9253_TMR_PLIB_START)${.vars["${DRV_LAN9253_TIMER_PLIB?lower_case}"].TIMER_START_API_NAME},
	
	.timerStop = (DRV_LAN9253_TMR_PLIB_STOP)${.vars["${DRV_LAN9253_TIMER_PLIB?lower_case}"].TIMER_STOP_API_NAME},
};

/* LAN9253 Driver Initialization Data */
const DRV_LAN9253_UTIL_INIT drvLAN9253InitData = {
<#if DRV_LAN9253_PROTOCOL == "SPI">
	/* SPI PLIB API  interface*/
	.spiPlib = &drvLAN9253SpiPlibAPI,
<#elseif DRV_LAN9253_PROTOCOL == "SQI">
	/* SPI PLIB API  interface*/
	.sqiPlib = &drvLAN9253SQIPlibAPI,
</#if>
	
	/* Timer PLIB API Interface */
	.timerPlib = &drvLAN9253TimerPlibAPI,

};

// </editor-fold>