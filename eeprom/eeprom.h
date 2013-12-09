#ifndef EEPROM_H
#define EEPROM_H




class eeprom {
public:

	softSpi* spiNum;

	eeprom();


	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t data);
	void init();

private:


	static const u8 EEPROM_CMD_WREN = 0x06; //WREN Set Write Enable Latch
	static const u8 EEPROM_CMD_WRDI = 0x04; //WRDI Write Disable
	static const u8 EEPROM_CMD_RDSR = 0x05; //RDSR Read Status Register
	static const u8 EEPROM_CMD_WRSR = 0x01; //WRSR Write Status Register
	static const u8 EEPROM_CMD_READ = 0x03; //READ Read Memory Data
	static const u8 EEPROM_CMD_WRITE = 0x02; //WRITE Write Memory Data


};









#endif /* EEPROM_H */




