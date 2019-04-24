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
uint16_t xPos, yPos;

// from git
uint8_t myID[]      = { '1', '1', '1', '1', '1'};
uint8_t remoteID[]  = { '2', '2', '2', '2', '2'};

//*****************************************************************************
// This is an ARRAY of strings.  If you wanted to access the 3rd string
// ("P10000"), you could do so using COMMANDS[2].
//*****************************************************************************

// from git
void rfInit(void)
{  
  wireless_set_pin_config(
    RF_SPI_BASE,
    RF_PAYLOAD_SIZE,
    RF_CHANNEL,
    RF_CS_BASE,
    RF_CS_PIN,
    RF_CE_GPIO_BASE,
    RF_CE_PIN
  );
  
  gpio_enable_port(RF_GPIO_BASE);
  
  // Configure SPI CLK
  gpio_config_digital_enable(  RF_GPIO_BASE, RF_CLK_PIN);
  gpio_config_alternate_function(RF_GPIO_BASE, RF_CLK_PIN);
  gpio_config_port_control(     RF_GPIO_BASE, RF_SPI_CLK_PCTL_M ,RF_CLK_PIN_PCTL);
  
  // Configure SPI MISO
  gpio_config_digital_enable(RF_GPIO_BASE, RF_MISO_PIN);
  gpio_config_alternate_function(RF_GPIO_BASE, RF_MISO_PIN);
  gpio_config_port_control(RF_GPIO_BASE, RF_SPI_MISO_PCTL_M, RF_MISO_PIN_PCTL);
  
  // Configure SPI MOSI
  gpio_config_digital_enable(RF_GPIO_BASE, RF_MOSI_PIN);
  gpio_config_alternate_function(RF_GPIO_BASE, RF_MOSI_PIN);
  gpio_config_port_control(RF_GPIO_BASE, RF_SPI_MOSI_PCTL_M, RF_MOSI_PIN_PCTL);
  
  // Configure CS to be a normal GPIO pin that is controlled 
  // explicitly by software
    gpio_enable_port(RF_CS_BASE);
  gpio_config_digital_enable(RF_CS_BASE,RF_CS_PIN);
  gpio_config_enable_output(RF_CS_BASE,RF_CS_PIN);
  
  // Configure CE Pin as an output
  
  gpio_enable_port(RF_CE_GPIO_BASE);
  gpio_config_digital_enable(RF_CE_GPIO_BASE,RF_CE_PIN);
  gpio_config_enable_output(RF_CE_GPIO_BASE,RF_CE_PIN);

  
  initialize_spi(RF_SPI_BASE, 0, 10);
  RF_CE_PERIH->DATA |= (1 << 1);
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
    lcd_clear_screen(LCD_COLOR_WHITE);

		// I2C touchscreen
		// ft6x06_init();
		 
		 // timer, TODO: don't know if right for project
		 //gp_timer_config_32(TIMER0_BASE, TIMER_TAMR_TAMR_1_SHOT, false, false);
		 
		 // joystick
		 //ps2_initialize(); 
		 
		 	// from git for wireless stuff
		// rfInit();
		 
    //enable accelerometer
    //accel_initialize();

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

/*
	 char msg[80];
  wireless_com_status_t status;
  bool   tx_mode = true;
  uint32_t data;
  int i = 0;
  
  init_hardware();

 	put_string("\n\r");
   put_string("******************************\n\r");
    put_string("ECE353 HW2 Spring 2019\n\r");
	    put_string("Kevin Wilson\n\r");
    put_string("******************************\n\r");
	printf("hello kevin");
  
    
  
  memset(msg,0,sizeof(msg));
  sprintf(msg,"MyID:%c%c%c%c%c",myID[0],myID[1],myID[2],myID[3],myID[4]);
  printf("%s\n",msg);
  
  memset(msg,0,sizeof(msg));
  sprintf(msg,"RmID:%c%c%c%c%c",remoteID[0],remoteID[1],remoteID[2],remoteID[3],remoteID[4]);
  printf("%s\n",msg);
  
  wireless_configure_device(myID, remoteID ) ;
	*/
    int i;
    int32_t x, y, z;
    char msg[80];

    xPos = 120;
    yPos = 160;
		
		init_hardware();

		put_string("\n\r");
    put_string("******************************\n\r");
    put_string("ECE353 HW2 Spring 2019\n\r");
    put_string("Kevin Wilson\n\r");
    put_string("******************************\n\r");
		
    eeprom_init_write_read();
		
		/*
		 lcd_draw_image
    (
        xPos,                         // X Pos
        space_shipWidthPixels,        // Image Horizontal Width
        yPos,                       // Y Pos
        space_shipHeightPixels,       // Image Vertical Height
        space_shipBitmaps,            // Image
        LCD_COLOR_RED,              // Foreground Color
        LCD_COLOR_YELLOW              // Background Color
    );
		*/
		
		
lcd_draw_box(
  50, //x start
  30, // x len
  50, //y s start
  30, // y len
  LCD_COLOR_BLUE, //border
  LCD_COLOR_RED, //fill
  1
);

lcd_draw_box(
  100, 
  30, 
  100, 
  30, 
  LCD_COLOR_RED, //border
  LCD_COLOR_BLUE, //fill
  2
);

 // Reach infinite loop
 while(1) {};
	 
    /*
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
        xPos--;
        continue;
        //put_string(msg);
      }
      else if (x < MOVE_RIGHT)
      {
        xPos++;
        continue;
        //put_string("Move right\n\r");
        //put_string(msg);
      }
      else
      {
        put_string("stay still X\n\r");
      }

    // Read the Accelerometer data
    for(i=0; i < 10000; i++)
    {
      z = accel_read_z();
    };

      // Check z values
      sprintf(msg,"Z: %d\n\r",z);
      if (z > MOVE_UP)
      {
        //put_string(msg);
        //put_string("Move up\n\r");
        //printf("Move up\n\r");
        yPos--;
        continue;
      }
      else if (z < MOVE_DOWN)
      {
        //put_string(msg);
        //put_string("Move down\n\r");
        yPos++;
        continue;
      }
      else
      {
        put_string(msg);
        put_string("stay still Y\n\r");
      }
    }
    */
   
}



