void TMR1_Pulse_Init(void) 
{
	/* TMR1_CTRL: CM=0,PCS=3,SCS=0,ONCE=0,LENGTH=0,DIR=0,COINIT=0,OUTMODE=0 */ 
	setReg(TMR1_CTRL,0x0600);
	/* Set up mode */
	/* TMR1_SCTRL: TCF=0,TCFIE=0,TOF=0,TOFIE=0,IEF=0,IEFIE=0,IPS=0,INPUT=0, Capture_Mode=0,MSTR=0,EEOF=0,VAL=0,FORCE=0,OPS=0,OEN=0 */

	setReg(TMR1_SCTRL,0x00); setReg(TMR1_CNTR,0x00); 
	setReg(TMR1_LOAD,0x00);
	setRegBitGroup(TMR1_CTRL,CM,0x01); /* Run counter */
}

void StopTMR1()
{
	//If CTRL[CM] is set to '000', the counter is inert. No counting will occur. Stop mode will also disable the interrupts caused by input transitions on a selected input pin.

	
}