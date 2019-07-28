# Presencedetection
Presencedetection with audiojacks on keys
![alt text](https://github.com/Sleepwalker350/presencedetection/blob/master/Presencedetection_front.png)

This git shows my approach to habe a reliable presence detection. The presence detection is then used to trigger certain events. For example turning the lights off when the last person leaves the house.

## Operating principle
The presence detection is done by recognizing different keys, each assosciated to a certain person or vehicle. To recognize the different keys each of them has a unique resistorvalue soldered in an male audio jack. The male audiojack when plugged in its counterpart forms a [voltage divider](https://en.wikipedia.org/wiki/Voltage_divider). The voltage is then measured. To measure more then one audiojack mosfets switch through all of them one after another.

## Wiring diagram
In my example I am using an Wemos D1 mini to do the measurment. The A0 Pin of the Wemos is connected over a Resistor to 3.3V at the same time it is connected to all female audiojacks. The female audiojacks themself are each connected to a mosfet. See the pictures below. The mosfets are each switched by a different pin. When a mosfet is closed the corresonding female audiojack is measured. If no male audiojack is pluged (infinit resistance) the analog voltage measueret is egual (tolerances beside) to 3.3V. When a male audiojack is plugged in the voltage on A0 is different depending on the resistor value in the male audiojack. For my example you can see the calculated values in the [Exceltable](https://github.com/Sleepwalker350/presencedetection/blob/master/resistorcalculation_presencedetection.xlsx) I put on this git.
![alt text](https://github.com/Sleepwalker350/presencedetection/blob/master/presencedetection_shematics.png)
![alt text](https://github.com/Sleepwalker350/presencedetection/blob/master/Presencedetection%20Back.jpeg)
The second pictures also shows, that the Wemos is connected to a battery as I have no suitable power outlet where our keys are kept.

## Code
The code used by my is in the arduino file. The code controls the different mosfets and executes the measuring. After the measuring processes it send an enconded MQTT-Massage that includes which audiojack and therefore which key is connected. The MQTT-message is then, in my case, used by OpenHAB to trigger certain events.
