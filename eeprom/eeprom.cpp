#include "user_conf.h"






eeprom::eeprom(){

}



uint8_t eeprom::read(u16 address)
{
	u8 rcvByte = 0;
    u8 tbuff[2];


    tbuff[0] = (address>>8) & 0xFF;
    tbuff[1] = address & 0xFF;


	spiNum->CsActive();
	spiNum->pause();

	spiNum->Write(EEPROM_CMD_READ); //Command
	spiNum->Write(tbuff,2); //Address register
	rcvByte = spiNum->Read(); //Read value

	spiNum->CsDeactive();
	spiNum->pause();

	return rcvByte;
}

void eeprom::write(uint16_t address, uint8_t data)
{
	 u8 tbuff[2];

	    tbuff[0] = (address>>8) & 0xFF;
	    tbuff[1] = address & 0xFF;

	spiNum->CsActive();
	spiNum->pause();
	spiNum->Write(EEPROM_CMD_WREN);

	spiNum->CsDeactive();
	spiNum->pause();

	spiNum->CsActive();
	spiNum->Write(EEPROM_CMD_WRITE);
	spiNum->Write(tbuff,2); //Address register
	spiNum->Write(data);

	spiNum->CsDeactive();
	spiNum->pause();
}

void eeprom::init()
{



	spiNum->CsActive();
	spiNum->pause();

	spiNum->Write(EEPROM_CMD_WREN); //WREN

	spiNum->CsDeactive();
	spiNum->pause();

}
