; hw1.s ECE353


    export hw1_search_memory
	import WS2812B_write

;**********************************************
; SRAM
;**********************************************
    AREA    SRAM, READWRITE
    align
		
;**********************************************
; Constant Variables (FLASH) Segment
;**********************************************
    AREA    FLASH, CODE, READONLY

hw1_search_memory PROC
	BX LR
	ENDP
		
    align        
    
    END



