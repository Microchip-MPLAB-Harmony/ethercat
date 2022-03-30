![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Microchip MPLAB® Harmony 3 EtherCAT Release Notes  (March 2022)
## EtherCAT Release v3.2.0
### ADDITIONS AND UPDATES FOR  3.2.0:
- Added LAN9253 EtherCAT device with SQI support with Harmony 3 framework.
- Added EtherCAT Applications for LAN9255 device with Harmony 3 framework and all the applications support both SPI and SQI.
- Added EEPROM Emulation support for LAN9255 device.
- Added support for Dummy bytes (Individually programmable per read and write command type) for both LAN9253 and LAN9255.
- EtherCAT application demos are verified with Beckhoff SSC stack v5.12.
- Added MCC support for all demo applications
- Added production **hex** files in the application for both LAN9253 and LAN9255 devices

### Known Issues
None

### Development Tools

* [MPLAB® X IDE v6.0.0](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v4.0.0](https://www.microchip.com/mplab/compilers)
* TwinCAT Manager:
    * [TC31-Full-Setup.3.1.4024.7x and above.](https://www.beckhoff.com/english.asp?download/tc3-downloads.htm)
* [EtherCAT Beckhoff Slave Stack Code (SSC) v5.12](https://www.ethercat.org/en/downloads/downloads_01DCC32A10294F2EA866F7E46FB0285F.htm)
* MPLAB® X IDE plug-ins:
    - MPLAB® Code Configurator 5.1.2

# Microchip MPLAB® Harmony 3 EtherCAT Release Notes  (October 2020)
## EtherCAT Release v3.1.0
### ADDITIONS AND UPDATES FOR  3.1.0:
- Updated Library and application Documentation  from CHM file to Markdown file.
- Added EtherCAT Applications for LAN9253 with Harmony 3 framework.
- Added Error LED support to indicate these following errors
	* EEPROM loading error
    * The failure EtherCAT Client connectivity link
    * Application Layer Error Event (AL Event)

### Known Issues
None

### Development Tools

* [MPLAB® X IDE v5.45](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v2.50](https://www.microchip.com/mplab/compilers)
* TwinCAT Manager:
    * TC31-Full-Setup.3.1.4024.7x and above. (https://www.beckhoff.com/english.asp?download/tc3-downloads.htm)

### Dependent Components
* [CSP v3.8.1](https://github.com/Microchip-MPLAB-Harmony/csp/tree/v3.8.1)
* [MHC v3.6.0](https://github.com/Microchip-MPLAB-Harmony/mhc/tree/v3.6.0)
* [EtherCAT Beckhoff Slave Stack Code (SSC) v5.12](https://www.ethercat.org/en/downloads/downloads_01DCC32A10294F2EA866F7E46FB0285F.htm)

# Microchip MPLAB® Harmony 3 EtherCAT Release Notes (April, 2020)
## EtherCAT Release v3.0.0

### New Features
- EtherCAT component is added in Harmony 3 to enable customer to develop EtherCAT Applications with Harmony 3 framework using LAN9252 device.

- **Development kit and demo application support** - This release include demo application that demonstrates firmware update over EtherCAT on SAM D51 with LAN925x Evaluation board.

	| Development kits   	| Applications Description												|
	| ---					| ---																	|
	| EVB-LAN925x SAMD51	| Process Trigger and Counter Index With Firmware Update Over EtherCAT	|


### Known Issues

None

### Development Tools

* [MPLAB® X IDE v5.35](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v2.40](https://www.microchip.com/mplab/compilers)
* MPLAB® X IDE plug-ins:
    * MPLAB® Harmony Configurator (MHC) v3.3.5 and above.
* TwinCAT Manager:
    * TC31-Full-Setup.3.1.4024.7x and above. (https://www.beckhoff.com/english.asp?download/tc3-downloads.htm)

### Dependent Components
* [CSP v3.6.1](https://github.com/Microchip-MPLAB-Harmony/csp/tree/v3.6.1)
* [MHC v3.3.5](https://github.com/Microchip-MPLAB-Harmony/mhc/tree/v3.3.5)
* [EtherCAT Beckhoff Slave Stack Code (SSC) v5.12](https://www.ethercat.org/en/downloads/downloads_01DCC32A10294F2EA866F7E46FB0285F.htm)
