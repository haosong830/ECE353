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

char en_command[] = "LOAD LED0105020 LED1440070  LED2111111  LED3002233  LED4440066  LED5667700 LED6550000 LED79900bb HALT";
char clear_command[] = "LOAD LED000000 LED1000000  LED2000000  LED3000000  LED4000000  LED5000000 LED6000000 LED7000000 HALT";
int count1A = 0;
extern bool alert_T4A;
extern bool alert_T1A;
volatile bool readyShoot = false;
volatile uint8_t touch_event = 0;

extern int score, numBullets;



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
    lcd_clear_screen(BG_COLOR);
		
		// Initialize the GPIO Port D
		gpio_enable_port(GPIOD_BASE);
		gpio_config_digital_enable(GPIOD_BASE,0xFF);
		gpio_config_enable_output(GPIOD_BASE,0xFF);
	
	// LED timer
		gp_timer_config_32(TIMER1_BASE, PERIODIC, false, true, SEC_ONE);
		
		// Accelerometer timer
		gp_timer_config_16(TIMER4_BASE, PERIODIC, false, true, 1570, TIMER_TAPR_TAPSR_M);
	
	 // enable io expander
	 io_expander_init();
 
		// I2C touchscreen
		 ft6x06_init();
		 
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
	 int count;
	 char bulletString[80], scoreString[80];
	 
	
	 // ACCELEROMETER
   int16_t x_accel, y_accel, z_accel;
	 uint16_t x_touch, y_touch;

   init_hardware();
	 gameSetup();
	 
	
		put_string("\n\r");
    put_string("******************************\n\r");
    put_string("ECE353 HW2 Spring 2019\n\r");
    put_string("Kevin Wilson\n\r");
    put_string("******************************\n\r");
	
	
		// draw all initial things to screen
		
		
		
    while(1)
    {
			// Blink red LEDS
			if(alert_T1A) 
				{
				//put_string("T1A\n");
				if(count1A == 0) 
				{
					//disableLeds();
					lp_io_set_pin(BLUE_BIT);
				}
				else if(count1A == 1) {
					//enableLeds();
					lp_io_clear_pin(BLUE_BIT);
				}
				alert_T1A = false;
				count1A = (count1A + 1) % 2;
			}
	
			if(alert_T4A)
			 {
				 if (count == 0)
				 {
					 readyShoot = true;
				 }
				 
				 // check touchscreen
				 touch_event = ft6x06_read_td_status();
				
				 // read accelerometer
				 	x_accel = accel_read_x();
					z_accel = accel_read_z();
				 
				 // can shoot about ~0.5s
				  count = (count + 1) % 30;
					alert_T4A = false;
			}
			 
			
		
			// call method to move shields back and forth
			moveShields();
			moveFish();
			
			// Check x values of accelerometer
			//printf("touch_event: %d \n", touch_event);
     // sprintf(msg,"X: %d\n\r",x);
			//checkShooting();
      if (x_accel > MOVE_LEFT)
      {
        //put_string("Move left\n\r");
				checkShooting();
				move_Left(octopus.xPos, octopus.yPos, 4, octopus.min_X, octopus.type, &octopus);
				checkShooting();
				//move_Right(shieldArray[0].xPos, shieldArray[0].yPos, 5, shieldArray[0].max_X, shieldArray[0].type, &shieldArray[0]);
				//move_Left(shieldArray[1].xPos, shieldArray[1].yPos, 10,1, shieldArray[1].type, &shieldArray[1]);
      }
      else if (x_accel < MOVE_RIGHT)
      {
				
			  checkShooting();
				move_Right(octopus.xPos, octopus.yPos, 4, octopus.max_X, octopus.type, &octopus);
				checkShooting();
				//move_Right(shieldArray[1].xPos, shieldArray[1].yPos, 5, shieldArray[1].max_X, shieldArray[1].type, &shieldArray[1]);
				//move_Left(shieldArray[0].xPos, shieldArray[0].yPos, 10,1, shieldArray[0].type, &shieldArray[0]);
      
      }
      else
      {
					checkShooting();
      }
			
			
			// num bullets
			lcd_draw_box(0,65, 300, 20, LCD_COLOR_BLACK, LCD_COLOR_RED, 2); // only do if bullets changed
			sprintf(bulletString,"%d bullets:",numBullets);
			print_string_toLCD(bulletString, 7, 310, LCD_COLOR_WHITE, LCD_COLOR_RED);
			
			// score
			lcd_draw_box(0,65, 280, 20, LCD_COLOR_BLACK, LCD_COLOR_GREEN, 2); // only do if score changed
			sprintf(scoreString,"score %d:", score);
			print_string_toLCD(scoreString, 6, 290, LCD_COLOR_BLACK, LCD_COLOR_GREEN);
			
		}
	}