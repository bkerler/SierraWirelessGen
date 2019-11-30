# Challenge/Response Generator for Sierra Wireless Cards V1.0
(c) B. Kerler 2019
MIT License

## Why

- For enabling development mode and tests
- For setting band options if locked
  
## Installation

- Get python >=3.6 64-Bit

    ```bash
    ~> git clone https://github.com/bkerler/SierraWirelessGen
    ~> chmod +x SierraWirelessGen/sierrakeygen.py
    ~> pip3 install -r requirements.txt
    ```

## Usage

- Open up a terminal and enable enhanced commands (generic pwd is "A710")
    
    ```
    AT!ENTERCND=A710
    ```
    
    Other known pwds are (thx to 4PDA):
    
    ```
    AC815s: "fallow"
    MR1100: “lindeman”
    AC790-Telstra: "sunflower"
    LB1111: "granville"
    AC810-100EUS: "whistler"
    AC810S-1P1PLS: "seymour"
    AC810S-1TLAUS: "grouse"
    AC810S-1RDQAS: "cypress"
    AC790-100EUS: "lavender"
    AC790S-1SPSUS : "bluebell"
    ```

- Get a specific challenge for your task from the modem
    
    ```
    AT!OPENLOCK?
    ```
    
    or
    
    ```
    AT!OPENMEP?
    ```
    
    or
    
    ```
    AT!OPENCND?
    ```

- Run generator (here challenge is BE96CBBEE0829BCA and device generation is MDM9200):

    For AT!OPENLOCK use -l, for AT!OPENMEP use -m and for AT!OPENCND use -c accordingly
    
    ```bash
    ~> ./sierrakeygen.py -l BE96CBBEE0829BCA -d MDM9200
    ```

- Send generated response back to the modem
    
    ```
    AT!OPENLOCK=[response from generator]
    ```
    
    or
    
    ```
    AT!OPENMEP=[response from generator]
    ```
    
    or
    
    ```
    AT!OPENCND=[response from generator]
    ```

## Help

    ```bash
    ~> ./sierrakeygen.py -h
    ```

## Remarks

- MDM9200/MDM9x15/MDM9x30/MDM9x40/MDM9x50 confirmed to work

- For AC785/AC790/AC810, you can access the serial port via tcp:

    ```bash
    HostName: 192.168.1.1
    Port: 5510
    ConnectionType: Telnet
    ```

- Get firmware details :

    ```
    ATI
    AT!PACKAGE?
    ```
    
- Get flash memory info :

    ```
    AT!FMBADBLOCKS?
    AT!BSINFO
    ```

- Set password for opencnd:

    ```
    AT!SETCND="[pwd]"
    ```

    Example: 

    ```
    AT!SETCND="A710"
    ```
    
- For band selection, see possible bands via :

    ```
    AT!BAND=?
    ```
 
- Set Modem to use all bands :

    ```
    AT!BAND=00
    ```

- Set Modem to only use LTE :

    ```
    AT!SELRAT=06
    ```

- Reboot modem and save settings :

    ```
    AT!RESET
    ```

- To add a new band :

    ```
    AT!BAND=[index],"[name]",0,8000000
    ```

    Examples: 

    ```
    AT!BAND=03,"LTE B28 700",0,8000000
    AT!BAND=04,"LTE B1 2100",0,1
    AT!BAND=05,"LTE B3 1800",0,4
    AT!BAND=06,"LTE B7 2600",0,40
    AT!BAND=07,"LTE B8 900",0,80
    ```
    
- To remove a band :

    ```
    AT!BAND=[index],"",0,0
    ```

    Example: 

    ```
    AT!BAND=03,"",0,0
    ```
    
 - Get signal info :

    ```
    AT!GSTATUS?
    ```
    
 - Get partition info :

    ```
    AT!PARTINFO?
    ```

 - Switch to qc download mode :

    ```
    AT!BOOTHOLD
    AT!QPSTDLOAD
    ```
 
 - Show Secure Boot info :

    ```
    AT!SECBOOTCFG?       Show Secure Boot config
    AT!SECBOOTPKHASH?    Show Secure Boot PKHASH
    ```
 
 - Show Product Info :

    ```
    AT!USBPRODUCT?
    Sierra Wireless EM7565 Qualcomm® Snapdragon™ X16 LTE-A
   
    AT!USBMANUFACTURER?
    Sierra Wireless, Incorporated
    ```
 
 - Set vid and pid :

    ```
    AT!USBVID=1199          Set usb vid of 0x1199
    AT!USBPID=9091,9090     Set usb pid (app=0x9091, boot=0x9090) 
    ```
 
 - Set product identifier :

    ```
    AT!PRIID?               Show product identifier
        PRI Part Number: 9907344
        Revision: 002.001
        Customer: Generic-M2M
        Carrier PRI: 9999999_9907259_SWI9X50C_01.08.04.00_00_GENERIC_002.012_000
        

    AT!USBPID="9907344","002.001","Generic-M2M"     Set PartNr, Revision and Customer 
    ```
    
 - Set preferred modem image :

    ```
    AT!IMPREF="GENERIC"
    
    AT!IMAGE=?
        AT!IMAGE=<op>[,<type>[,<slot>[,"<build_id>","<unique_id>"]]]
        op   - 0:delete 1:list 2:get max num images
        type - 0:FW 1:CONFIG
        slot - FW slot index - none implies all slots
        AT!IMAGE?[<op>[,<type>]]
    
   AT!IMAGE=0,0,1       Op=0 (Delete), Type=0 (FW), Slot Index=1
    ```

 - Reset to factor settings :

    ```
    AT!RMARESET=1
    ```

 - Lenovo laptop whitelist bypass :

    ```
    AT!ENTERCND="A710"
    AT!CUSTOM="FASTENUMEN",2    Disable fast enumeration and only show up after init
    AT!PCOFFEN=2                Ignore W_DISABLE pin
    AT!USBSPEED=0               Force usb2 mode
    AT!RESET
    ```
 
 - Set usb composition (diag, nmea, modem, mbim, same as USBCOMP=8):

    ```
    AT!USBCOMP=1,3,0000100D
    ```
 
 - List custom settings :

    ```
    AT!CUSTOM?
    ```

 - Enable telnet (after sending valid openlock request)

    ```
    at!custom="TELNETENABLE",1
    ```

 - Enable adb (after sending valid openlock request, here: MC7304/AC810)

    ```
    AT!CUSTOM="ADBENABLE", 1
    ```

    Regulary, tcp port 5555 is used for adb

    ```
    adb tcpip 5555
    adb connect 192.168.1.1
    ```
  
 - Flash firmware :

    ```bash
    ~ > sudo apt install libqmi-glib5 libqmi-proxy libqmi-utils -y
    ~ > qmi-firmware-update --update -d 1199:9091 firmware.cwe firmware.nvu      1199:9091 is usb vid/pid
    ```
   
## Other useful links

- https://github.com/danielewood/sierra-wireless-modems/blob/master/README.md

## ToDo

- Nothing :)
 
## License

Published under MIT license
Additional license limitations: No use in commercial products without prior permit by me.

Enjoy !