; hw1.s ECE353


    export hw1_search_memory
	import WS2812B_write

WORD        EQU     4
HALF_WORD   EQU     2
BYTE        EQU     1

;**********************************************
; SRAM
;**********************************************
    AREA    SRAM, READWRITE
LED_ARRAY	SPACE 	6*WORD
    align
		
;**********************************************
; Constant Variables (FLASH) Segment
;**********************************************
    AREA    FLASH, CODE, READONLY
	align
// All functions are callee saved.

/* Command 			Bytes		Description
LOAD				4			Turn all LEDs off.

HALT				4			Leave LEDs in current state, enter infinite loop

HANG				8			HANGXXXX = empty loop that executes X,XXX * 10,000	
								times and then exits. XXXX is a 4 digit ASCII string

LEDx				10			x represents which LED is being modified
								6 remaining characters indicate color intensity. 
								5 & 6 = Green
								7 & 8 = Red
								9 & 10 = Blue
								Example for 2nd LED : LED2FF8800
*/

/* Description
 *	Updates the WS2812B LEDs with the current values in the LED_ARRAY
 *	Uses the supplied function WS2812B_write to update the colors

 * Parameters
 *	NONE
 
 * Returns
 *	NOTHING
*/
hw1_update_leds PROC
	; Need to save LR for nested function call
	PUSH {R1, R2, LR}
	; Load Array address and # LEDS for function call to WS2812B_write
	LDR R1, =(LED_ARRAY)
	MOV R2, #8
	BL WS2812B_write
	POP {R1, R2, PC}
	BX LR
ENDP
		
		

/* Description
 *	Converts a single ASCII HEX character to its numerical value. 
 *	Valid characters are 0-9, a-f , A-F

 * Parameters
 *	R0 - ASCII Char

 * Returns
 *	R0 - numerical value. If invalid, return 0xFFFFFFFF
*/
hw1_ascii_to_hex PROC
	; Need to save LR for nested function call
	PUSH {R1, LR}
	
	; 'a'-'f' = 0x61 - 0x66
	; 'A'-'F' = 0x41 - 0x46
	; '0'-'9' = 0x30 - 0x39
characters_a_f
	; First check if greater than 'f'
	CMP R0, #0x66
	; If it is, invalid, so set R0 to -1 and return
	MOVGT R0, #0xFFFFFFFF
	BGT return
	; Then check if less than 'a'
	CMP R0, #0x61
	; if it is, then check if 'A' through 'F'
	BLT characters_A_F
	; otherwise, character is 'a' - 'f'. 
	; Subtract 0x60 and return! :) 
	SUB R0, R0, #0x60
	B return
	
characters_A_F
	; Now check if greater than 'F'
	CMP R0, #0x46
	; If it is, invalid, so set R0 to -1 and return
	MOVGT R0, #0xFFFFFFFF
	BGT return
	; Then check if less than 'A'
	CMP R0, #0x41
	; if it is, finally check if '0' - '9'
	BLT characters_0_9
	; otherwise, character is 'A' - 'F'. 
	; subtract 0x40 and return!  :)
	SUB R0, R0, #0x40
	B return

characters_0_9
	; if not 'a'-'f' or 'A'-'F' call function to see if '0'-'9'
	; if not '0'-'9' either, R0 will be 0xFFFFFFFF
	BL hw1_ascii_to_dec
return
	POP {R1, PC}
	BX LR
ENDP


/* Description
 *	Converts a single ASCII DECIMAL character to its numerical value. 
 *	Valid characters are 0-9. 
 *  Will be called from within hw1_ascii_to_hex

 * Parameters
 *	R0 - ASCII Char
 
 * Returns
 *  R0 -  numerical value. If invalid, return 0xFFFFFFFF
*/
hw1_ascii_to_dec PROC
	; '0'-'9' = 0x30 - 0x39
	; Check if less than '0'. And update flags
	SUBS R0, #0x30
	; if it is, invalid
	MOVLT R0, #0xFFFFFFFF
	BX LR
ENDP


/* Description
 *	Turns off all of the WS2812B LEDs by writing 0x00000000 to each LED
 *	Should be called with 'LOAD'

 * Parameters
 *	NONE
 
 * Returns
 *	NOTHING
*/
hw1_init PROC
	PUSH {R0-R2}
	; set R0 to 0
	MOV R0, #0
	; load array address 
	LDR R1, =(LED_ARRAY)
	; R2 will be used to increment through loop
	MOV R2, #0
	
; LED_ARRAY is 6 words. In each iteration of loop,
; set 1 of those words to zero.
turn_off_led_loop_begin
	CMP R2, #6
	; if R2 > 6, end loop
	BHI turn_off_led_loop_end
	STR R0, [R1], #4
	; increment index
	ADD R2, R2, #1
	B turn_off_led_loop_begin
turn_off_led_loop_end
	POP {R0-R2}
	BX LR
ENDP


/* Description
 *   Updates the color of the specified LED. All other LEDs should maintain
 *	 their current color. Makes use of the hw1_update_leds to update
 *   the color of the LEDs.
    
 * Parameters
 *   R0 - LED # to be updated. Assume 0 is the leftmost LED
 *   R1 - An unsigned 32 bit #. Bits 31-24 are unused. Bits 23-0 
 *	 represent the color value to write to the LED

 * Returns
 *   NOTHING
*/
hw1_ledx PROC
	BX LR
ENDP
	
		
/* Description
 *	Delays the examination of the next memory address by a variable 
 *	amount of time (hang)
 * Parameters
 *	R0 - A 32 bit unsigned number representing the # of iterations
 *	of an empty for loop that must be executed.

 * Returns
 *	NOTHING
*/
hw1_wait PROC
	BX LR
ENDP


/* Description
 *	This function will search through memory a byte at a time looking for valid DISPLAY
 *	commands. When a valid DISPLAY command is found, carry out the corresponding
 *	behavior described in the DISPLAY command table above. Memory that holds invalid
 *	commands are ignored. This function will examine memory until and HALT command
 *	is found.

 * Parameters
 *	R0 - The starting address of the memory that contains the commands
 *	     to update LEDs.

 * Returns
 *	NOTHING
*/
hw1_search_memory PROC
	BX LR
ENDP
		
		

		

	
    align        
    
    END



