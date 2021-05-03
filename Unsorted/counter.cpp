#pragma once

//16bit quad timer controller

//Enable TMR1 for pulse counting
//do → Check if overflow flag is reset
void TMR1_Pulse_Init() 
{
	/* TMR1_CTRL: CM=0,PCS=3,SCS=0,ONCE=0,LENGTH=0,DIR=0,COINIT=0,OUTMODE=0 */ 
	setReg(TMR1_CTRL,0x0600); //Binary → 0110 0000 0000
	/* Set up mode */
	/* TMR1_SCTRL: TCF=0,TCFIE=0,TOF=0,TOFIE=0,IEF=0,IEFIE=0,IPS=0,INPUT=0, Capture_Mode=0,MSTR=0,EEOF=0,VAL=0,FORCE=0,OPS=0,OEN=0 */

	setReg(TMR1_SCTRL,0x00); setReg(TMR1_CNTR,0x00); 
	setReg(TMR1_LOAD,0x00);
	setRegBitGroup(TMR1_CTRL,CM,0x01); /* Run counter */
}


void StopTMR1()
{
	//If CTRL[CM] is set to '000', the counter is inert. No counting will occur. Stop mode will also disable the interrupts caused by input transitions on a selected input pin.
	setReg(TMR1_CTRL, 0x0000); //→ Sets the whole 16bit register to zero
}

//
void TicCount()
{
	//Read 'NewCount' from the capture register. Asuming OldCount = 0;
	

	bool Overflow = (TMR1_SCTRL & (1<<13) != 0);
	if(Overflow) //Overlow flag is set(TOF) is bit 13
	{
		CountVal = FCS_QUAD_TIMER_FULLCYCLE + NewCount;
	}

	else
		CountVal = NewCount;
}

#define ReadBIT(reg, bitid) {#reg & (1<<bitid) == 0}