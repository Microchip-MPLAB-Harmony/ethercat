![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Microchip MPLAB® Harmony 3 Release Notes (February, 2020)

## EtherCAT Release v3.0.0
### New Features

- **New part support** - This release introduces initial support for <EVB-LAN9252 D51 device>.

- **Development kit and demo application support** - The following table provides the list of library application available for different development kits

    | Development Kits                                         EVB-LAN925x SAMD51                                                                                      | Number of applications |
    | ethercat_counter_foe_app  demo application for the sam_d51_lan9252_evb configuration.                                                                       
**Device Support**
Microchip EtherCAT device supports only one modes; 
    Master and Slaves for synchronization:
    Free run The Master cycle time and Slave cycle time are
    independent.
Synchronization with distributed clocks The Master cycle time and Slave cycle time are
synchronized.                                               

**Linker Support**-
    - ATSAMD51 device supports bank switching and EtherCAT use this feature for the Firmware upgrade. The range of BANK-B is 0x40000 to 0x7ffff , this BANK-B is used to store the new firmware. 
    -ethercat_foe.ld file is added to the demo to restrict the **ROM_LENGTH** to 0x40000. 

**NOTES**
    As the Fuse settings are disabled for the released demo to demonstrate FoE , we can observer this following **warning**
    **Configuration memory will not be programmed because no configuration bits settings have been defined in your code. To program configuration memory, either define the settings in your code or use the Program Configuration Bits button on the configuration memory window.**
    -   Please ignore this warning

### Known Issues

The current known issues are as follows:

* Distributed Clocks (DC Mode) - Synchronous with SYNC0 is not supported. That is Synchronization with distributed clocks The Master cycle time and Slave cycle time is not supported.



### Development Tools

* [MPLAB® X IDE v5.30](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v2.30](https://www.microchip.com/mplab/compilers)
* MPLAB® X IDE plug-ins:
    * MPLAB® Harmony Configurator (MHC) v3.3.0.0 and above.