# Carroto
Autonomous robot based on the @mimeindustries chassis as a base. 

Carroto means Carro Maroto in Portuguese, or simply Silly Car. Don't ask why. =P

Components used:

-1x Arduino Uno Rev3 (101, Mega2560 and Nano also worked)

-2x 2828byj-48 step motors, including 2x ULN2003 drivers for each

-1x HC-05 (5v) module communicating via Serial comms

-1x Step-down power-supply

-----------------------
This code accepts the following commands via a Bluetooth Serial connection, single or chained as a string.

Fxxx (Move forwards, xxx cm)

Txxx (Move backwards, xxx cm)

Exxx or exxx (Move to the left xxxº, with one wheel locked in place).

Dxxx or dxxx (same as Exxx/exxx, but to the right)

Rxxx (rotate xxxº clockwise)

rxxx (same as above, but counter-clockwise)


