; Filename:     addMultiply.s 
; Author:       ece353 staff 
; Description:  

    export addMultiply

;**********************************************
; SRAM
;**********************************************
    AREA    SRAM, READWRITE
    align
        
;**********************************************
; Constant Variables (FLASH) Segment
;**********************************************
    AREA    FLASH, CODE, READONLY
    align


;**********************************************
; Four arrays of 8-bit signed numbers are 
; passed via the first four parameters.
; The 5th paramter indicates the length of the
; arrays. For each entry in the array, the
; following operation takes place.
;
; Array3[i] = (Array0[i] + Array1[i]) * Array2[i]
;
; Parameter 0       Array Address 0
; Parameter 1       Array Address 1
; Parameter 2       Array Address 2
; Parameter 3       Array Address 3
; Parameter 4       Array Size 
;
; Returns
;   if ALL parameters are valid, return 0
;   else return -1.
;
;  An address is valid if it is non zero
;  The size is valid if it is greater than zero
;**********************************************
addMultiply PROC
    
	; R0 = Array Address 0
	; R1 = Array Address 1
	; R2 = Array Address 2
	; R3 = Array Address 3
	
    ; Validate Address Parameters (nonzero)
	
	
	
	; Get 5th parameter
	LDR R4, [SP, 0]
	; Validate Size Parameter (greater than zero)
	
	
	
	
	
	
	
	
	
	; Must save anything besides R0-R3. Caller must save R0-R3
	; R4 will hold the array size
	; R5 will hold Array0[i]
	; R6 will hold Array1[i]
	; R7 will hold Array2[i]
	; R8 will hold i
	; R9 will hold Array0[i] + Array1[i] and (Array0[i] + Array1[i]) * Array2[i]
	PUSH {R4-R9}
	
	
    
    ; For each index in the arrays, compute  
    ; Array3[i] = (Array0[i] + Array1[i]) * Array2[i]
	
	; initialize i
	MOV R8, #0
	
array_loop_begin
	; compare i to array size
	CMP R8, R4
	BEQ array_loop_end
	
	; since each entry is 8 bits, only have to offset 
	; by 1 byte each time. No shifting of i necessary
	; load array values. Signed values
	LDRSB R5, [R0, R8]
	LDRSB R6, [R1, R8]
	LDRSB R7, [R2, R8]
	
	;(Array0[i] + Array1[i])
	ADD R9, R5, R6
	;(Array0[i] + Array1[i]) * Array2[i]
	MUL R9, R9, R7
	
	; store result in Array3[i]
	STRSB R9, [R3, R8]
	
	; increment index
	ADD R8, R8, #1
	
	; keep looping
	B array_loop_begin
array_loop_end
	
    ; Restore registers saved to the stack
	POP {R4-R9}
	
    ; Return from the loop
	BX LR
	
	
    ENDP
    
    align
        

    END            
