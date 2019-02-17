# Zeitschaltuhr. 
time switch, supporting 8 AC outputs (10A max) and a 5V DC PWM output (2A max). 

This is a time switch supporting 8 AC outputs and a 5V DC PWM output. It also offers visualization with a LC-Display, configuration by a 2 button interface, a Real-Time-Clock(RTC) powered by a SuperCap and an on-board AC/DC power module.  

The combined switched power of all the AC outputs should not exceed 16A while a single output is limited to 10A. The DC PWM output is limited to 2A by the 15W IRM-15-5 AC/DC power module, more power output might be possible by selecting a better power module and power mosfet.

The current output status is presented by 4x40 Character LC-Display. The first line shows the current time while the lines 2 to 4 show the outputs. Only 3 outputs are shown at once, it is possible to scroll trough the other outputs by pressing the scroll button.

In case of an blackout, the RTC is powered an supercap for at least a week.  

It is possible to reconfigure the names and times by scrolling the target output to line 2 and pressing the select button.

The USB-B port is used to power the device while programming and debuging.

MCU: Microchip PIC18F26k22<br />
RTC: Microchip MCP 7940N (7940M also, but without backup power)<br />
LCD: ERM4004SBS-1 (2x KS0066 (HD44780 compatible))<br />

/demo includes a photo of the front and back side of the finished time switch.
