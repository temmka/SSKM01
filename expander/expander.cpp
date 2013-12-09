#include "user_conf.h"


static const u8 EXP_IODIRA	 = 0x00;
static const u8 EXP_IODIRB	 = 0x01;
//static const u8 EXP_IPOLA	 = 0x02;
//static const u8 EXP_IPOLB	 = 0x03;
//static const u8 EXP_GPINTENA = 0x04;
//static const u8 EXP_GPINTENB = 0x05;
//static const u8 EXP_DEFVALA	 = 0x06;
//static const u8 EXP_DEFVALB	 = 0x07;
//static const u8 EXP_INTCONA	 = 0x08;
//static const u8 EXP_INTCONB	 = 0x09;
static const u8 EXP_IOCONA	 = 0x0A;
static const u8 EXP_IOCONB	 = 0x0B;
//static const u8 EXP_GPPUA	 = 0x0C;
//static const u8 EXP_GPPUB	 = 0x0D;
//static const u8 EXP_INTFA	 = 0x0E;
//static const u8 EXP_INTFB	 = 0x0F;
//static const u8 EXP_INTCAPA	 = 0x10;
//static const u8 EXP_INTCAPB	 = 0x11;
static const u8 EXP_GPIOA	 = 0x12;
static const u8 EXP_GPIOB	 = 0x13;
//static const u8 EXP_OLATA	 = 0x14;
//static const u8 EXP_OLATB	 = 0x15;



mcp23s17::mcp23s17(){
	spiNum = 0;
	deviceAddress = 0;
}
mcp23s17::mcp23s17(softSpi* spiNumber){
	spiNum = spiNumber;
	deviceAddress = 0;

}
mcp23s17::mcp23s17(softSpi* spiNumber, u8 Address){
	spiNum = spiNumber;
	deviceAddress = Address;
}




u8 mcp23s17::Read(u8 reg)
{

	u8 rcvByte = 0;

	spiNum->CsActive();
	spiNum->pause();

	spiNum->Write((deviceAddress << 1) | 0x41); //opcode
	spiNum->Write(reg);
	rcvByte = spiNum->Read(); //Read value

	spiNum->CsDeactive();
	spiNum->pause();

	return rcvByte;

}
void mcp23s17::Write(u8 reg, u8 data)
{
	spiNum->CsActive();
	spiNum->pause();

	spiNum->Write((deviceAddress << 1) | 0x40); //opcode
	spiNum->Write(reg);
	spiNum->Write(data);

	spiNum->CsDeactive();
	spiNum->pause();

}


u8 mcp23s17::A(){
	return Read(EXP_GPIOA);
};

u8 mcp23s17::B(){
	return Read(EXP_GPIOB);
};

void mcp23s17::A(u8 data){
	 Write(EXP_GPIOA, data);
};

void mcp23s17::B(u8 data){
	 Write(EXP_GPIOB, data);
};

u16 mcp23s17::AB(){
	u16 retVal=0;
	retVal =  B() << 8 ;
	retVal |= A() ;

return retVal;
}

void mcp23s17::AB(u16 data){
	A( data & 0xFF);
	B( data >> 8);

}


/* 0xFF = input; 0x00 = output  */
void mcp23s17::Init(uint8_t portA, uint8_t portB)
{
	Write(EXP_IOCONA, 0x08);
	Write(EXP_IOCONB, 0x08);

	Write(EXP_IODIRA, portA);
	Write(EXP_IODIRB, portB);


}

