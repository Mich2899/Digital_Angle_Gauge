/*
 * **********************************************************************************************
 * menu_and_command.h
 * brief	:	provides explanation for menu, command processor and check functions.
 * 				menu provides options to the user
 * 				command processor processes those options to provide appropriate output
 * 				check function checks for changes in angle and pitch
 * @author	:   Michelle Christian
 * **********************************************************************************************
 */


#ifndef MENU_AND_COMMAND_H_
#define MENU_AND_COMMAND_H_

/*
 * function		: command_processor
 * params		: none
 * brief		: processes the command provided as input by the user
 * return_type	: void
 */
void command_processor();

/*
 * function		: blink
 * params		: int r2		value for red led
 * 				  int g2		value for green led
 * 				  int b2 		value for blue led
 * brief		: blinks led at a delay of 100ms
 * return_type	: void
*/
void blink(int r2, int g2, int b2);

/*
 * function		: menu and input
 * params		: none
 * brief		: provides menu options and takes input from user
 * return_type	: void
 */
void menu_and_input();


/*
 * function		: check
 * params		: none
 * brief		: checks for change in angle or pitch
 * return_type	: void
 */
void check();

#endif /* MENU_AND_COMMAND_H_ */
