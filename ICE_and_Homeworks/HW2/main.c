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
uint16_t i;
uint32_t j;

// declare function
void loopThroughArray(void);

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
    // number of elements in COMMANDS array
    uint8_t sizeArray = sizeof(COMMANDS) / sizeof(COMMANDS[0]);

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

    loopThroughArray();

    // loop through command array changing position of ufo and checking if reached edge of screen
    // write code to loop through command array


    lcd_draw_image(
        X_MAX,                       // X Pos
        space_shipWidthPixels,    // Image Horizontal Width
        Y_MAX,                       // Y Pos
        space_shipHeightPixels,   // Image Vertical Height
        space_shipBitmaps,        // Image
        LCD_COLOR_BLUE2,          // Foreground Color
        LCD_COLOR_WHITE            // Background Color
    );



    /*
    // testing moving ufo left to right
    for (i = X_MIN; i <= X_MAX; i++)
    {
      lcd_draw_image
        (
        i,                       // X Pos
        space_shipWidthPixels,    // Image Horizontal Width
        160,                       // Y Pos
        space_shipHeightPixels,   // Image Vertical Height
        space_shipBitmaps,        // Image
        LCD_COLOR_BLUE2,          // Foreground Color
        LCD_COLOR_WHITE            // Background Color
        );

        for (j = 0; j <20000; j++)
        {
          continue;
        }
    }
    */

    /*
      // testing moving ufo up to down
    for (i = Y_MIN; i <= Y_MAX; i++)
    {
      lcd_draw_image
        (
        120,                       // X Pos
        space_shipWidthPixels,    // Image Horizontal Width
        i,                       // Y Pos
        space_shipHeightPixels,   // Image Vertical Height
        space_shipBitmaps,        // Image
        LCD_COLOR_BLUE2,          // Foreground Color
        LCD_COLOR_WHITE            // Background Color
        );

        for (j = 0; j <20000; j++)
        {
          continue;
        }
      }
    */



    // Reach infinite loop
    while(1) {};
}
