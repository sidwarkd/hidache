Thanks for downloading the HIDAche source files! The following are basic instructions on how 
to get the project code working.

=============================================================================================
Build the Hardware
=============================================================================================

The schematic for the the HIDAche device can be found in the Codeproject article located at 
http://www.codeproject.com/Articles/32749/HIDAche-An-Exercise-in-Hardware-Software-Integrati
Note that the button is necessary to allow you to enter programming mode vs prank mode.

=============================================================================================
Install the Bootloader
=============================================================================================

The first piece of the prank device is the bootloader which allows you to enter programming
mode. It also allows you to put the actual hidache firmware onto your microcontroller.

The project is located in the bootldr folder of the downloaded source. It's an MPLAB project
and can either be built from source or you can use the prebuilt hex file in the output folder
to program your microcontroller if you are using a PIC18F2455. If you are using another 
device you may need to slightly modify the configuration of the project specifically the 
linker files. I use the Pickit3 to program my PICs.

Once the bootloader is programmed onto your device you are ready to install the microcontroller 
in your project.

Bootloader Mode
----------------
Hold the button down while plugging in the USB cable. This will cause the bootloader code to 
run.

Prank Mode
----------
If the button is not pressed on power up the bootloader code will be skipped and the device will
try to run whatever application code has been loaded. 

** NOTE **
If you have not installed hidache.hex as described below your device may do strange things 
when powered up into prank mode.

=============================================================================================
Install hidache Firmware
=============================================================================================
With the bootloader installed it's time to put the actual HIDAche device code onto your 
microcontroller. Again, if you are using the PIC18F2455 you can just use the hidache.hex file
found in firmware/output. If you are using another device you may need to make some minor
modifications. To load the hidache firmware:

  1) Hold the button down while plugging in your device. This will cause it to enter bootload
     mode
  2) Launch HIDBootLoader.exe found at the root of the hidache folder. It should indicate that
     a bootloader is attached.
  3) Click Open Hex File
  4) Browse to and select hidache.hex
  5) Click Program/Verify

  The device should program successfully but likely will fail verification. That is an issue with
  the program from Microchip and not an indication of a failed load.

=============================================================================================
Program the Prank Device
=============================================================================================
To program your device simply enter bootload mode and then launch HIDAche.exe. It should 
detect your device and allow you to change settings. Once programmed disconnect your device,
close HIDAche.exe and then plug your device back in WITHOUT pressing the button to enter
prank mode. Because of the USB enumeration process whatever prank you have programmed could
take up to a minute to start executing.

=============================================================================================
Questions
=============================================================================================
If you have any problems or questions feel free to send me an email or reach out on Twitter.

sidwarkd@hardlysoftware.com
@KevinSidwar