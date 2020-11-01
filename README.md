# Open Geiger Counter

Open hardware for a simple all-in-one geiger counter design using a GMT like the popular SGM-20.

Hardware design has been done in EasyEDA and all the needed files for you to import the project as well as
a schematic can be found in the hardware folder. There is also a gerber file available for direct pcb manufacturing.

Software aims to be as simple as possible to understand and maintain; to achieve this the two ATTiny84s can be
programmed using the ArduinoIDE and a popular AVR ICSP programmer like the USBasp.
`opengc_psu` is the sketch for the geiger power supply, that's the MCU near the inductor.
And `opengc_counter` is the main detector which is just the other ATTiny chip on board.

In order for the code to work you will need following two libraries:

* [NicoHood/PinChangeInterrupt](https://github.com/NicoHood/PinChangeInterrupt)
* [datacute/Tiny4kOLED](https://github.com/datacute/Tiny4kOLED)

To compile and program sketches for any ATTiny microcontrollers you will also need [SpenceKonde/ATTinyCore](https://github.com/SpenceKonde/ATTinyCore).

Before you push this to your MCU be sure to select the right chip (ATTiny84a without bootloader), optionally choose a BOD level
and use following clock speeds:

* PSU: 1 MHz (I found this to be the best speed for psu stability)
* Counter: 8 MHz, just the fastest without an external crystal

Also don't forget to check the pin orientation of your programmer! Select your programmer and if you've done everything right
and plugged in a geiger tube the counter is ready to go.

The display will show the battery voltage and counts per minute (evaluated over a span of 3 seconds default).
You can reset the counter with one button and mute/unmute the buzzer with the other (saved on reboot).

Additionally, you can alter `REFRESH_INT` in the counter sketch to change the cpm refresh interval,
and the `TARGET_V` in the psu sketch if you need a different voltage for your specific geiger tube.

Please note that this is not a professional geiger counter by any means. It will never be as sensitive to radiation as if you put the same
tube in a commercial device. The hardware, most importantly, the transistor pull down is pretty crude in conjunction with the ATTiny84 -
some pulses are just way to fast to create a _nice_ logic low for the MCU to register. Suggestions are highly appreciated of course.

Oh and obviously don't touch the board if it's powered on, the PSU can deliver 500+ Volts DC (@some uA)!
