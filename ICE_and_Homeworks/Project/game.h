#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lcd.h"
#include "images.h"
#include <string.h>
#include "validate.h"

#define UFO_X_MAX 214
#define UFO_X_MIN 26
#define UFO_Y_MAX 302
#define UFO_Y_MIN 18


// for things that need a bitmap
typedef struct _GameCharacter
{
	 const uint16_t width;
	 const uint16_t height;
	 uint16_t xPos;					// width, in bits (or pixels), of the character
	 uint16_t yPos;					// offset of the character's bitmap, in bytes, into the the FONT_INFO's data array
	 const uint8_t* bitmap;
	 uint16_t fColor;
	 uint16_t bColor;
	 const char* type;
	 uint16_t max_X;
	 uint16_t min_X;
	 bool hit;
}_GameCharacter;

// for rectangle obstacles
typedef struct _GameObj
{
	 uint16_t width;
	 uint16_t height;
	 uint16_t border_weight;
	 uint16_t xPos;					// width, in bits (or pixels), of the character
	 uint16_t yPos;					// offset of the character's bitmap, in bytes, into the the FONT_INFO's data array
	 uint16_t fColor;
	 uint16_t bColor;
	 const char* type;
	 uint16_t max_X;
	 bool hit;
}_GameObj;



extern _GameCharacter ufo;
extern _GameObj rectangle1;
extern _GameObj rectangle2;

void move_Right(uint16_t xPos, 
								uint16_t yPos, 
								uint32_t num_pixels, 
								uint16_t maxX,
								const char type[],
								void* ptr);

void move_Left(uint16_t xPos, 
							uint16_t yPos, 
							uint32_t num_pixels, 
							uint16_t minX,
							const char type[],
							void* ptr);


void drawCharacter(_GameCharacter* character, uint16_t x, uint16_t y); 
void drawShield(_GameObj* obj, uint16_t x, uint16_t y);