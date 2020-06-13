# CyberDigit Clock

[![](http://img.youtube.com/vi/4m0iQuYpwDA/0.jpg)](http://www.youtube.com/watch?v=4m0iQuYpwDA "CyberClock")

This repo contains firmware and CAD designs for a clock built using the Cyber City Circuits [CyberDigit](https://shop.cybercitycircuits.com/products/cyber-digit-rgb-7-segment-display)

Parts List:

- 4x CyberDigit
- ESP32 Dev Board (any with a 0.9" or 1.0" spacing between header rows)
- 8x M3x40mm bolts
- 8x M3x6mm bolts
- 16x M3 Nylock Nuts
- 3mm opaque black acrylic
- 3mm smoked clear acrylic
- 1mm thick VHB tape

There's not much to the actual hardware build:

- Solder alternating male / female right angle headers onto the lower  connectors of the CyberDigit, however between digits 2 and 3 you will need extra long male headers because there's an extra 5mm gap. We just used extra long male headers and bent them.
- Solder wires to Gnd, VUSB/5V, and Pin 27 on the EPS32 board.
- Solder those same 3 wires to the input on the first digit
- Bolt the digits to the digit backer plate with the 6mm bolts
- Use the VHB tape to fix the ESP32 board to the acylic plate as shown in the design.
- Complete the acrylic stack and bolt everything together with the 40mm bolts

In general, check the Fusion 360 design provided to see how everything fits together and where.

You will need the [ESP32 Core](https://github.com/espressif/arduino-esp32) to upload the firmware.

Set `gmtOffset_sec` in `globals.h` to your UTC timezone offset in seconds.

Upload the firmware to the ESP32 (there is a hole over the BOOT button in the back of the case so you can do this after assembled).

On first boot, the digits will go blue and this means it is in setup mode. Within 60 seconds connect to the `CYBERCLOCK` WiFi access point and browse to 192.168.2.1
From there you will be able to configure access to your local WiFi. This is required for the clock to work - there is no other way to set the time.
