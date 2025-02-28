# Water pump pressure control with arduino

The water pump pressure control software is made to control the pressure inside water tank. The program uses pressure sensor, 4 digit 7 segment display, two buttons and actuator for the pump, through the display and the buttons the program features interface for setting the parameters (i call them menus). Also the program detects errors in the system.

## Current menus

#### Menu 0: Setting the turn on pressure
#### Menu 1: Setting the turn off pressure

## Error definition

#### Error 0: Sensor output is below it's minimal output
#### Error 1: Sensor output is above it's maximal output
#### Error 2: The pump has worked more time than the set maximum time

## Usage

At turn on on the display will be shown the current pressure (the display has automatic turn off after approximately 5 min).  
For accessing the program menu (all the menus), hold both buttons for about 5 seconds (hold until 4 F's appear on the display).  
Using the left and right button individually holding them for 1,5 second you switch between menus (hold until left or right arrow appears, after that F will be shown on the left and the corresponding menu digit on the right, indicating the current menu).  
For changing the value click on the buttons left decrement, right increment. The more you click the bigger the change in value will be, if you stop it will reset.  
For saving the value hold both buttons until 4 _ are displayed.  
After approximately 30 seconds of inactivity the program will exit program mode and return to normal operation.

## IMPORTANT

THE PROGRAM IS NOT FINISHED IF YOU PLAN USING IT WAIT UNTIL I FINISH THE PROJECT AND ALSO PUT THE DIAGRAMS.
MORE IS PLANNED TO BE ADDED.  
ALSO THE CODE IS IN MY NATIVE LANGUAGE I WILL INCLUDE ENGLISH CODE IN THE FINAL COMMIT.

## License

[MIT](https://choosealicense.com/licenses/mit/)