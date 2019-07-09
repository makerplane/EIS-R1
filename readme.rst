****************************************
MakerPlane Engine Information System
****************************************

Revision 1

The MakerPlane Engine Information System (EIS) is an advanced and very flexible
aircraft engine monitoring system.  It is part of the MakerPlane *System of
Systems* architecture where disparate devices can be used together in a *Plug
and Play* fasion.

The MakerPlane EIS is a combination of an Arduino Mega as well as a custom
Arduino Sheild especially desgined to read aircraft engine data.

The Shield contains the necessary components for reading the inputs as well
as the communications controllers and tranceivers to communicate via both
CAN and RS-232.

This repository contains the official MakerPlane sketch that gives
the MakerPlane EIS its functionality.


Features
--------

  - 5 Resistance Inputs
  - 6 0-5V DC Auxillary Inputs
  - 2 Tachmoeter Inputs
  - 2 Fuel Flow Inputs
  - 6 EGT Inputs
  - 6 CHT Inputs
  - Engines from 1 to 6 cylinders
  - Flight Time and Engine Run Time
  - Audible Warning Output
  - Transistor Warning Output
  - Device Temperature
  - RS-232 Port for Communication to EFIS Displays
  - Very Flexible Configuration System
  - Full CAN-FIX Integration


Measured Parameters
-------------------
  - Fuel Pressure
  - Manifold Pressure
  - Oil Pressure
  - Oil Temperature
  - Coolant Temperature
  - Fuel Quantity Tank
  - Carburetor Temperature
  - Cabin Temperature
  - Outside Air Temperature
  - Electrical Bus Voltage
  - Electrical Bus Current
  - Alternator Voltage
  - Alternator Current
  - Generic Analog Value
  - Engine RPM
  - Propeller RPM
  - Fuel Flow
  - Return Fuel Flow
  - Fuel Total
  - Exhaust Gas Temperature
  - Cylinder Head Temperature
  - EGT and CHT Deviation and Rate of Change


Status
------

Currently the code is not ready for flight but work is continuing.

The MakerPlane CAN-ArduinoLib and CANFIX-ArduinoLib libraries are required.

https://github.com/makerplane/CAN-ArduinoLib
https://github.com/makerplane/CAN-FIX-ArduinoLib
