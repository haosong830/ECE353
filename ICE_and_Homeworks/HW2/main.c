// Copyright (c) 2015-16, Joe Krachey
// All rights reserved.
//
// Redistribution and use in source or binary form, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in source form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "main.h"

// declare function
void loopThroughArray	(void);
void drawSaucer_Up		(uint32_t num_pixels);
void drawSaucer_Down	(uint32_t num_pixels);
void drawSaucer_Left	(uint32_t num_pixels);
void drawSaucer_Right	(uint32_t num_pixels);
void pauseSaucer		(uint32_t pauseLength);

//*****************************************************************************
// This is an ARRAY of strings.  If you wanted to access the 3rd string
// ("P10000"), you could do so using COMMANDS[2].
//*****************************************************************************
char *COMMANDS[] =
{
    "P200000",
    "R500",
    "P10000",
    "U1000",
    "P5000",
    "L2000",
    "P2500",
    "D2000",
    "P1250",
    "R94",
    "P1",
    "U142"
};

//*****************************************************************************
//*****************************************************************************
void init_hardware(void)
{
    initialize_serial_debug();
    lcd_config_gpio();
    lcd_config_screen();
    lcd_clear_screen(LCD_COLOR_WHITE);
}



void loopThroughArray(void)
{
	// loop variable
	uint8_t i;
	
	// X position to draw saucer
	
	
	// Y position to draw saucer
	
	
	char *commandLetter;
	
	// this double pointer will be used to store 
	// the command string from the COMMANDS array.
	char **commandString = COMMANDS;
	
	// The remaining number following the 
	// char specifying which command to do.
	char *numFromCommand_String;
	
	// numFromCommand_String converted to an int using atoi()
	uint32_t numFromCommand_Int;

	// Loop through COMMAND array, check first character of 
	// array element, and skip if not U,D,L,R, or P. Perform command if valid.
	// Stop when the pointer doesn't point to anything in the array
	while (*commandString != 0)
	
	{
		/* Add 1 to pointer that points to a command string to remove
		 * the first character and set equal to numFromCommand_String
		 * Example: *commandString 		= R500
		 *							 	  ^pointer starts here
					*commandString + 1 = 500
										 ^pointer starts here							
		*/
		numFromCommand_String = *commandString + 1;
		
		// COMMANDS[i] will be the first character of the array element
		commandLetter = COMMANDS[i];

		/* Check if U,D,L,R, or P, or invalid. If valid, use atoi() to convert
		 * the string after the first character into an integer, and call appropriate
		 * function to draw saucer.
		*/
		
		// Move saucer up (sub from y location) by specified number of pixels
		if(*commandLetter == 'U')
		{
			numFromCommand_Int = atoi(numFromCommand_String);
			drawSaucer_Up(numFromCommand_Int);
		}
		
		// Move saucer up (add to y location) by specified number of pixels
		else if(*commandLetter == 'D')
		{
			numFromCommand_Int = atoi(numFromCommand_String);
			drawSaucer_Down(numFromCommand_Int);
		}
		
		// Move saucer left (sub from x location) by specified number of pixels
		else if(*commandLetter == 'L')
		{
			numFromCommand_Int = atoi(numFromCommand_String);
			drawSaucer_Left(numFromCommand_Int);
		}
		
		// Move saucer right (add to x location) by specified number of pixels
		else if(*commandLetter== 'R')
		{
			numFromCommand_Int = atoi(numFromCommand_String);
			drawSaucer_Right(numFromCommand_Int);
		}
		
		// Pause by specified duration using empty for loop
		else if(*commandLetter == 'P')
		{
			numFromCommand_Int = atoi(numFromCommand_String);
			pauseSaucer(numFromCommand_Int);
		}
		
		// unrecognized command, skip
		else continue;
		
		// increment the double pointer to the next element
		commandString++;
	}
}

void drawSaucer_Up(uint32_t num_pixels)
{

}

void drawSaucer_Down(uint32_t num_pixels)
{

}

void drawSaucer_Left(uint32_t num_pixels)
{

}

void drawSaucer_Right(uint32_t num_pixels)
{

}

void pauseSaucer(uint32_t pauseLength)
{
	// use a while loop to wait until we've 
	// paused enough time
	while (pauseLength != 0)
	{
		pauseLength--;
	}
}

//*****************************************************************************
//*****************************************************************************
int main(void)
{
    init_hardware();


    put_string("\n\r");
    put_string("******************************\n\r");
    put_string("ECE353 HW2 Spring 2019\n\r");
    put_string("Kevin Wilson\n\r");
    put_string("******************************\n\r");
	
	
	// draw saucer in middle of screen first
	lcd_draw_image
		(
        120,                       	// X Pos
        space_shipWidthPixels,    	// Image Horizontal Width
        160,                    	// Y Pos
        space_shipHeightPixels,   	// Image Vertical Height
        space_shipBitmaps,        	// Image
        LCD_COLOR_BLUE2,          	// Foreground Color
        LCD_COLOR_WHITE           	// Background Color
		);
	
	// loop through the COMMANDS array which 
	// will call appropriate functions to draw the saucer
    loopThroughArray();

 
    // Reach infinite loop
    while(1) {};
}
