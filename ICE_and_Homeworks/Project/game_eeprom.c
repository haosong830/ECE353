#include "main.h"

void eeprom_init_write_read()
{
    uint16_t addr;
    uint8_t read_val;
    bool sw2_logic_level;

    // When reset pressed, print this.
    char reset[80] = "Please press SW2 to get student info\n"
                     "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

    char name1[80] = "Student 1: Kevin Wilson\n";
    char name2[80] = "Student 2: Haosong Ma\n";
    char teamNum[80] = "Team number: 13\n";

    // Overwrite name1,name2, and teamNum in eeprom
    for(addr = ADDR_START; addr < ADDR_START + 80; addr++)
    {
        eeprom_byte_write(I2C1_BASE, addr, reset[addr - ADDR_START]);
    }
    for(addr = ADDR_START; addr < ADDR_START + 80; addr++)
    {
        eeprom_byte_read(I2C1_BASE, addr, &read_val);
        printf("%c", (char) read_val);
    }

    // Make sure SW2 is reset
    lp_io_clear_pin(SW2_BIT);
    while(1)
    {
        sw2_logic_level = lp_io_read_pin(SW2_BIT);
        if (!sw2_logic_level) break;
    }

    // If sw2 pressed, write this data to eeprom
    // Write all the data to the eeprom
    for(addr = ADDR_START; addr < ADDR_START + 80; addr++)
    {
        eeprom_byte_write(I2C1_BASE, addr, name1[addr - ADDR_START]);
    }
    for(addr = ADDR_START + 80; addr < ADDR_START + 160; addr++)
    {
        eeprom_byte_write(I2C1_BASE, addr, name2[addr - (ADDR_START + 80)]);
    }
    for(addr = ADDR_START + 160; addr < ADDR_START + 240; addr++)
    {
        eeprom_byte_write(I2C1_BASE, addr, teamNum[addr - (ADDR_START + 160)]);
    }

    // Read from the eeprom
    for(addr = ADDR_START; addr < ADDR_START + 240; addr++)
    {
        eeprom_byte_read(I2C1_BASE, addr, &read_val);
        printf("%c", (char)read_val);
    }
    return;
}


