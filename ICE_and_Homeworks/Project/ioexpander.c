#include "driver_defines.h"
#include "gpio_port.h"
#include "TM4C123.h"
#include "i2c.h"
#include "ioexpander.h"
#define BTN_NONE	0x0F
#define BTN_U		0x0E
#define BTN_D		0x0D
#define BTN_L		0x0B
#define BTN_R		0x07
#define	BTN_UD		0x0C
#define BTN_UL		0x0A
#define BTN_UR		0x06
#define BTN_LR		0x03
#define BTN_LD		0x09
#define BTN_DR		0x05
i2c_status_t io_expander_byte_write( uint32_t i2c_base, uint16_t addr, uint8_t data) {
	i2c_status_t i2c_status;
  
  while (I2CMasterBusy(i2c_base)) {};
	i2c_status = i2cSetSlaveAddr(i2c_base, 0x27, I2C_WRITE);
	if ( i2c_status != I2C_OK ){return i2c_status;}

	i2c_status = i2cSendByte(i2c_base, (uint8_t)addr, I2C_MCS_START | I2C_MCS_RUN);
	if ( i2c_status != I2C_OK ){return i2c_status;}
	
	i2c_status = i2cSendByte(i2c_base, data, I2C_MCS_RUN | I2C_MCS_STOP);
	if ( i2c_status != I2C_OK ){return i2c_status;}
	
  return i2c_status;
}
																		
i2c_status_t io_expander_byte_read (uint32_t i2c_base, uint16_t addr, uint8_t *data) {
	
  i2c_status_t i2c_status;
  
  while ( I2CMasterBusy(i2c_base)) {};

	if ( i2cSetSlaveAddr(i2c_base, 0x27, I2C_WRITE) != I2C_OK )
		return i2c_status;

	i2c_status = i2cSendByte(i2c_base, (uint8_t)(addr), I2C_MCS_START | I2C_MCS_RUN);
	if ( i2c_status != I2C_OK )
		return i2c_status;

	i2c_status = i2cSetSlaveAddr(i2c_base, 0x27, I2C_READ);
	if ( i2c_status != I2C_OK )
		return i2c_status;

	i2c_status = i2cGetByte(i2c_base, data, I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP);
	
  return i2c_status;
}

bool io_expander_init()
{
	i2c_status_t i2c_status;
	
  if(!gpio_enable_port(GPIOA_BASE))
		return false;
	
  // Configure SCL 
  if(!gpio_config_digital_enable(GPIOA_BASE, PA6))
		return false;
  if(!gpio_config_alternate_function(GPIOA_BASE, PA6))
		return false;
  if(!gpio_config_port_control(GPIOA_BASE, GPIO_PCTL_PA6_M, GPIO_PCTL_PA6_I2C1SCL)) 
		return false;
    
  // Configure SDA 
  if(!gpio_config_digital_enable(GPIOA_BASE, PA7))
		return false;
  if(!gpio_config_open_drain(GPIOA_BASE, PA7))
		return false;  
  if(!gpio_config_alternate_function(GPIOA_BASE, PA7))
		return false;
  if(!gpio_config_port_control(GPIOA_BASE, GPIO_PCTL_PA7_M, GPIO_PCTL_PA7_I2C1SDA))
		return false;
	
  //initialize i2c master to set the master address to be the I2C1 on tiva board
  if(initializeI2CMaster(I2C1_BASE)!= I2C_OK)
		return false;
	
	i2c_status = io_expander_byte_write(I2C1_BASE, 0x00, 0x00);//set  the LEDs to be output
	if(i2c_status != I2C_OK) 
		return false;
	
	i2cSetSlaveAddr(I2C1_BASE, 0x27, I2C_WRITE);//set slave addr to be address of MCP23017_DEV_ID(0x27)
	i2cSendByte(I2C1_BASE, 0x0D, I2C_MCS_START | I2C_MCS_RUN);//0D is the address of register GPPUB(The pull-up resistor used to control push button values)
	i2cSendByte(I2C1_BASE, 0x0F, I2C_MCS_RUN | I2C_MCS_STOP);//0F is the interrupt enable port of GPPUB. 

	return true;
}


										
void disableLeds(void) {
	io_expander_byte_write(I2C1_BASE,0x12,0x00);//0x12 is the GPIOA register address, which is connected to LEDs
}

void enableLeds(void) {
	io_expander_byte_write(I2C1_BASE,0x12,0xFF);
}


typedef enum 
{
  DEBOUNCE_ONE,
  DEBOUNCE_1ST_ZERO,
  DEBOUNCE_2ND_ZERO,
  DEBOUNCE_PRESSED,
	DEBOUNCE_DONE
} DEBOUNCE_STATES;

//*****************************************************************************
// 
//*****************************************************************************
uint8_t pushbutton_debounce_fsm(void) {
	uint8_t button_val;
	static DEBOUNCE_STATES debounce_state = DEBOUNCE_ONE;	
	static uint8_t curr_btn_val = BTN_NONE;
	static uint8_t prev_btn_val = BTN_NONE;
	while(I2CMasterBusy(I2C1_BASE)){}
	io_expander_byte_read(I2C1_BASE, 0x13, &button_val);	//13 is the address GPIOB. We read the output
	curr_btn_val = button_val;
	
	switch(debounce_state) {
		case DEBOUNCE_ONE:
			if(curr_btn_val != BTN_NONE) 
			{
				debounce_state = DEBOUNCE_1ST_ZERO;
				prev_btn_val = curr_btn_val;
			}
			return BTN_NONE;
			
		case DEBOUNCE_1ST_ZERO:
			if(curr_btn_val == prev_btn_val) 
				debounce_state = DEBOUNCE_2ND_ZERO;
			else 
			{
				prev_btn_val = curr_btn_val;
				debounce_state = DEBOUNCE_ONE;
			}
			break;
			
		case DEBOUNCE_2ND_ZERO:
			if(curr_btn_val == prev_btn_val) 
				debounce_state = DEBOUNCE_PRESSED;
			else 
			{
				prev_btn_val = curr_btn_val;
				debounce_state = DEBOUNCE_ONE;
			}
			return BTN_NONE;
			
		case DEBOUNCE_PRESSED:
			if(curr_btn_val == prev_btn_val) 
				debounce_state = DEBOUNCE_DONE;
			else 
			{
				prev_btn_val = curr_btn_val;
				debounce_state = DEBOUNCE_ONE;
			}
			return curr_btn_val;
			
		case DEBOUNCE_DONE:
			if(curr_btn_val == prev_btn_val) 
				debounce_state = DEBOUNCE_DONE;
			else {
				prev_btn_val = curr_btn_val;
				debounce_state = DEBOUNCE_ONE;
			}
			return BTN_NONE;
	}
}

uint8_t detect_button_press() {//put this function into the timer and use the timer interrupt????
	uint8_t btn_data = pushbutton_debounce_fsm();
	
	//if(checkButton) {
		switch( btn_data ){
			case BTN_D:
				printf("DOWN BTN PRESSED\n");
				break;
			case BTN_U:
				printf("UP BTN PRESSED\n");
				break;
			case BTN_L:
				printf("LEFT BTN PRESSED\n");
				break;
			case BTN_R:
				printf("RIGHT BTN PRESSED\n");
				break;
			case BTN_UD:
				printf("BTN_UD PRESSED\n");
				break;
			case BTN_UL:
				printf("BTN_UL PRESSED\n");
				break;
			case BTN_UR:
				printf("BTN_UR PRESSED\n");
				break;
			case BTN_LR:
				printf("BTN_LR PRESSED\n");
				break;
			case BTN_LD:
				printf("BTN_LD PRESSED\n");
				break;
			case BTN_DR:
				printf("BTN_DR PRESSED\n");
				break;
			default:
				break;
		}
	return btn_data;

}



