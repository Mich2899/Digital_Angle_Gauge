# PES_Final_Project
This is the final project for ECEN 5813 Principles of Embedded Software by Michelle Christian. 
The project focuses on building a prototype of Digital Angle Gauge using KL25Z and its inbuilt MMA8451 3-axis accelerometer. The gauge prototype provides an interactive terminal interface with user defined reference calibration, angle and pitch setting options. The roll angle ranges from -180/+180 degrees while pitch angle once set to ground level reference/ zero level reference, ranges from -90/ +90 degrees.
UART terminal configurations are as mentioned below
```
Baud rate 	115200
Stop bits 	1
Data bits 	8
Parity bits	0
```
There are testing operations implemented for different modules as well:
```
MODULE	TEST
Cbfifo 	assignment tester made by Prof. Howdy
UART	Debug prints at different stages
Accelerometer		bubble level demo
Capacitive touch  	led display based on left, center, or right position of slider
```

This is the process followed:
1.	On startup, the led color changes according to the orientation of the board (bubble level demo). This demo sequence runs for a duration of 7 seconds for confirming proper working of accelerometer.
2.	Next, the user is expected to calibrate the board to an angle of 0 degrees (respective to the surface) by touching the center position on the slider. (Capacitive touch calibrates the gauge to any reference surface.)
3.	Now, the user can set the board (saw) at any angle with respect to the reference by entering the angle in UART terminal.
4.	Once the user specified position is met, a specific led color is displayed.
5.	If there is a change in angle from the specified position it notifies the user by blinking the LED and the user is prompted to enter the value again.
6.	User is provided a menu on UART terminal where there are two options mainly, first to recalibrate the system and the other to enter some specified position or angle
EXTRA FUNCTIONALITY
Third functionality added  in the UART menu to set pitch angle entered by the user. This functionality works if the board is kept on a flat surface with ground level/ zero level reference.
*More details provided in the report.*
