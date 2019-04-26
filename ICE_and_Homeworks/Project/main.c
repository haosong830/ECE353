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

// global variables for saucer x and y positions
uint16_t ufo_xPos, ufo_yPos;
extern void hw1_search_memory(uint32_t addr); 
// from git



//*****************************************************************************
// This is an ARRAY of strings.  If you wanted to access the 3rd string
// ("P10000"), you could do so using COMMANDS[2].
//*****************************************************************************

void print_string_toLCD(char string[], 
	uint16_t x_start, 
	uint16_t y_start, 
	uint16_t fColor, 
	uint16_t bColor)
{
	char c1, c2;
	int i,j, xPos;
	xPos = x_start;
	c1 =' ';
	
	for (i = 0; i < strlen(string); i++)
	{
		c2 = string[i]; 
		
		lcd_draw_image(xPos, alphabet_Descriptors[c2-c1].widthBits, y_start, 14,
									&alphabet_Bitmap[alphabet_Descriptors[c2-c1].offset], fColor, bColor);
		/*
		// draw in between the spaces
		for (j = xPos; j < alphabet_Descriptors[c2-c1].widthBits/2 + 4; j++)
		{
			lcd_draw_image(xPos, j, y_start, 14,
									&alphabet_Bitmap[0], fColor, bColor);
		}
		*/
		
		xPos += alphabet_Descriptors[c2-c1].widthBits/2 + 4;
	}
}


//*****************************************************************************
//*****************************************************************************
void init_hardware(void)
{
    DisableInterrupts();
    //************************************************************************
    // Configures the serial debug interface at 115200.
    // UART IRQs can be anbled using the two paramters to the function.
    //************************************************************************
    init_serial_debug(true, true);
	
    eeprom_init();
    // initialize launchpad
    lp_io_init();
    //enable LCD stuff
    lcd_config_gpio();
    lcd_config_screen();
    lcd_clear_screen(LCD_COLOR_GREEN);
		
		// Initialize the GPIO Port D
		gpio_enable_port(GPIOD_BASE);
		gpio_config_digital_enable(GPIOD_BASE,0xFF);
		gpio_config_enable_output(GPIOD_BASE,0xFF);
	
	 // gp_timer_config_32(TIMER0_BASE, ONE_SHOT, false, false, SEC_ONE);
		//gp_timer_config_32(TIMER1_BASE, PERIODIC, false, true, FIVE_SEC);
		//gp_timer_config_16(TIMER4_BASE, PERIODIC, false, true, EIGHT_MS, PRESCLR);

		// I2C touchscreen
		// ft6x06_init();
		 // timer, TODO: don't know if right for project
		 //gp_timer_config_32(TIMER0_BASE, TIMER_TAMR_TAMR_1_SHOT, false, false);
		 
		 // joystick
		 //ps2_initialize(); 
	
    //enable accelerometer
    accel_initialize();
		
    EnableInterrupts();
}

void DisableInterrupts(void)
{
    __asm
    {
        CPSID  I
    }
}

//*****************************************************************************
//
//*****************************************************************************
void EnableInterrupts(void)
{
    __asm
    {
        CPSIE  I
    }
}




//*****************************************************************************
//*****************************************************************************
int main(void)
{
	
	 int i;
   int32_t x, y, z;
   char msg[80];
	 //char startPrompt[80] = "Please press SW 2 to begin.\n";

    init_hardware();
	
	/*
	//draw initial box
			lcd_draw_box(
			xPos, //x start
			30, // x len
			yPos, //y s start
			30, // y len
			LCD_COLOR_BLUE, //border
			LCD_COLOR_RED, //fill
			1
		);
		*/
		lcd_draw_image
    (
        ufo.xPos,                         // X Pos
        ufo.width,        // Image Horizontal Width
        ufo.yPos,                       // Y Pos
        ufo.height,       // Image Vertical Height
        ufo.bitmap,            // Image
        ufo.fColor,              // Foreground Color
        ufo.bColor              // Background Color
    );
	
    put_string("\n\r");
    put_string("******************************\n\r");
    put_string("ECE353 HW2 Spring 2019\n\r");
    put_string("Kevin Wilson\n\r");
    put_string("******************************\n\r");
	
		//printf("testing\n");
    //eeprom_init_write_read();
	
			// Reach infinite loop
			//while(1) {};
	
    //Read accelerometer
		
    while(1)
    {
    // Read the Accelerometer data
    for(i=0; i < 10000; i++)
    {
      x = accel_read_x();
    };

      // Check x values

      sprintf(msg,"X: %d\n\r",x);
      if (x > MOVE_LEFT)
      {
        //put_string("Move left\n\r");
				//xPos-=2;
				move_Left(ufo.xPos, ufo.yPos, 4, (ufo.width/2), ufo.type, &ufo);
        //continue;
				//printf("%s", msg);
       // put_string(msg);
      }
      else if (x < MOVE_RIGHT)
      {
       // xPos+=2;
				move_Right(ufo.xPos, ufo.yPos, 4, (240 - (ufo.width/2)), ufo.type, &ufo);
        //continue;
        //put_string("Move right\n\r");
			//	printf("%s", msg);
       // put_string(msg);
      }
      else
      {
				continue;
        //put_string("stay still X\n\r");
				//drawSaucer(xPos,yPos);
					//printf("stay still X\n\r");
      }
		/*
    // Read the Accelerometer data
    for(i=0; i < 10000; i++)
    {
      z = accel_read_z();
    };

      // Check z values
      sprintf(msg,"Z: %d\n\r",z);
      if (z > MOVE_UP)
      {
        put_string(msg);
        printf("Move up\n\r");
        yPos--;
        continue;
      }
      else if (z < MOVE_DOWN)
      {
        put_string(msg);
        put_string("Move down\n\r");
        yPos++;
        continue;
      }
      else
      {
        put_string(msg);
        put_string("stay still Y\n\r");
      }
			*/
	/*
		// draw with updated coordinates
		lcd_clear_screen(LCD_COLOR_WHITE);
		lcd_draw_box(
			xPos, //x start
			30, // x len
			yPos, //y s start
			30, // y len
			LCD_COLOR_BLUE, //border
			LCD_COLOR_RED, //fill
			1
		);
		*/
 }
}

