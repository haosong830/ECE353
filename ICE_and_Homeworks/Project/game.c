#include "game.h"


_GameCharacter ufo = {52, //w
											36, //h
											120,UFO_Y_MAX, //x, y
											space_shipBitmaps,
											LCD_COLOR_RED,
											LCD_COLOR_GREEN,
											"character",
											false};

_GameObj rectangle1 = {50, 50, 1, // width, height, border
											100, 50, 	//x,y
											LCD_COLOR_BLUE,
											LCD_COLOR_RED,
											"object",
											false}; 



											
void move_Left(uint16_t xPos, 
							uint16_t yPos, 
							uint32_t num_pixels, 
							uint16_t minX,
							const char type[],
							void* ptr)
{
	// loop variable
	uint16_t i;
	
	
	// check if going to hit left of screen
	if ((int)(xPos - num_pixels) < minX){
		// draw saucer left until reach left of screen
		for(i = xPos; i >= minX; i--){
			// update x, keep y fixed
			//lcd_clear_screen(LCD_COLOR_GREEN);
			if (!strcmp(type,"character")) drawCharacter((_GameCharacter*)ptr, i, yPos); 
			else if (!strcmp(type, "object"))  drawShield((_GameObj*) ptr, i, yPos);
		}
	}
	else {
		// must save because xPos is updated every iteration in the loop
		uint16_t final_x = xPos - num_pixels;
		// draw saucer left the correct # of pixels
		// start at xPos and go until you hit (xPos - num_pixels)
		for(i = xPos; i >= final_x; i--){
			// update x, keep y fixed
			//lcd_clear_screen(LCD_COLOR_GREEN);
			if (!strcmp(type,"character")) drawCharacter((_GameCharacter*)ptr, i, yPos); 
			else if (!strcmp(type, "object"))  drawShield((_GameObj*) ptr, i, yPos);
		}
	}
}


void move_Right(uint16_t xPos, 
								uint16_t yPos, 
								uint32_t num_pixels, 
								uint16_t maxX,
								const char type[],
								void* ptr)
{
	// loop variable
	uint16_t i;
	
	// check if going to hit right of screen
	if ((xPos + num_pixels) > maxX){
		// draw saucer right until reach right of screen
		for(i = xPos; i <= maxX; i++){
			// update x, keep y fixed
			//lcd_clear_screen(LCD_COLOR_GREEN);
			if (!strcmp(type,"character")) drawCharacter((_GameCharacter*)ptr, i, yPos);
			else if (!strcmp(type, "object"))  drawShield((_GameObj*) ptr, i, yPos);
		}
	}
	else {
		// must save because xPos is updated every iteration in the loop
		uint16_t final_x = xPos + num_pixels;
		// draw saucer right the correct # of pixels
		// start at xPos and go until you hit (xPos + num_pixels)
		for(i = xPos; i <= final_x; i++){
			// update x, keep y fixed
			//lcd_clear_screen(LCD_COLOR_GREEN);
			if (!strcmp(type,"character")) drawCharacter((_GameCharacter*)ptr, i, yPos); 
			else if (!strcmp(type, "object"))  drawShield((_GameObj*) ptr, i, yPos);
		}
	}
}


// draw the character based on struct
void drawCharacter(_GameCharacter* character, uint16_t x, uint16_t y) 
{		
	put_string("hello");
	// update x and y struct variables
	character->yPos = y;
	character->xPos = x;
	lcd_draw_image
    (
        x,         							// X Pos
        character->width,        // Image Horizontal Width
        y,         							// Y Pos
        character->height,       // Image Vertical Height
        character->bitmap,       // Image
        character->fColor,       // Foreground Color
        character->bColor        // Background Color
    );
}


void drawShield(_GameObj* obj, uint16_t x, uint16_t y)
{
	obj->yPos = y;
	obj->xPos = x;
	lcd_draw_box(
			x, //x start
			obj->width, // x len
			y, //y s start
			obj->height, // y len
			obj->fColor, //border
			obj->fColor, //fill
			obj->border_weight
		);
}