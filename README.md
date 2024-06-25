# Crane Control

 :building_construction: The following circuit has the purpose of managing three engines controlled by a web app also used as a safety system, 
since upon activation of one of the available motors, If the crane is performing a previously selected operation, 
the operation stops allowing the new selected movement to start. Within the site there are several selectable boxes, such as:
- "Motor 1"; which is associated with the movement of the crane shaft, allowing the rotation to the right or left.
- "Motor 2";  to which is associated the sliding (forward-backward) of the basket along the axis.
- "Motor 3";  which is associated with the climb-down of the basket.
- "Reset"; used to reset the crane axles.

The esp32 connects to a wifi network of which must be provided SSID & PASSWORD and sets the status of the web app to the state "Idle".
Next, if connected to wifi, send requests to the web app and upload the data received in the variable "payload". 
```
int httpCode = http.GET();
if (httpCode > 0){
      payload = http.getString();
      intpayload = payload.toInt();
}
```
The data that can be received, integer type,  can be:
- "1" if Motor 1 has been selected (clockwise spinning)
- "2" if Motor 1 has been selected (counter clockwise spinning)
- "3" if Motor 2 has been selected (clockwise spinning)
- "4" if Motor 2 has been selected (counter clockwise spinning)
- "5" if Motor 3 has been selected (clockwise spinning)
- "6" if Motor 3 has been selected (counter clockwise spinning)
- "7" if selected Reset
  
Depending on the data received, the program will operate the corresponding motor in the corresponding direction.



## Central circuit :space_invader:
[The following circuit](https://github.com/hinnhi/crane_control/tree/main/circuit/circuito_centrale) is responsible for controlling the 3 crane engines. If the relay is not powered 
(so the bistable button, which takes care of stopping all the motors in case of failure/ emergency, has not been pressed) 
the two h bridges are powered by the VCC supply voltage. Once the data is received by the web app, the esp32 sends a 3.3V 
signal output, the signal is sent to a photocoupler and successively to the inputs of the bridge h. The photocouplers are 
used to separate the control part (powered at 3.3V) and that of power (powered by 5V).


## Acoustic and light warning circuit
[The following circuit](https://github.com/hinnhi/crane_control/tree/main/circuit/segnalazione_acustico_luminosa) deals with the acoustic and light signaling of the crane, the LED and the buzzer are operated thanks 
to the use of the integrated circuit [NE555](https://en.wikipedia.org/wiki/555_timer_IC) as a square wave generator. If the relay is powered (so the bistable button, which takes 
care of stopping all the motors in case of failure/ emergency, has been pressed), the integrated NE555 is powered by the VCC 
supply voltage. The circuit also takes care of receiving the VCC supply voltage from the generator and "distribute" it to the 
other circuits, "host" the connectors that connect the limit switches to the espe32 and the bistable button that activates the 
relay. 


## LED blinking circuit
[The following circuit](https://github.com/hinnhi/crane_control/tree/main/circuit/lampeggio_led), realized on millephous base, deals with the light signaling of the crane. The LED flashes thanks to the 
use of the integrated circuit NE555 as a square wave generator. The capacitor C3 is charged with the supply voltage VCC=5V, through the 
two resistors RA and RB; as soon as the power on the capacitor exceeds the level 2/3VCC, the comparator A passes the output Q of the 
flip-flop from low to high, as a result, the T1 transistor goes into saturation and the C3 capacitor begins to discharge through 
the RB resistor. The discharge proceeds until the charge on the capacitor C3 becomes less than 1/3VCC; at this point the comparator 
B makes again change state on the flip-flop bringing the output Q to the low level and the cycle starts again.

Wave period square output:  T = Tc + Ts = (RA+2RB)C3ln(2)

> [!NOTE]
> The power supply voltage can be raised up to 40 volts, but the interfaces for the acoustic and light warning circuit and for the LED blinking circuit must be redesigned.

> [!WARNING]
> Neutral wires of esp32 and power supply must not be connected together.
