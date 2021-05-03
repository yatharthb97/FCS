//Or use any other library
//TimerOne & TimerThree Libraries

#include ".\TeensyTimerTool\src\TeensyTimerTool.h"
using namespace TeensyTimerTool;


////// Global Variables

//Macro Definations
#define FCS_EVEN_SYNC_PULSE 0         //Measure Every Alternate Pulse
#define FCS_PCOUNT_MAX_SIZE 264       //Array size of p_count
#define FCS_BAUD_RATE 196             //Baud Rate
#define FCS_CHANNEL_SIZE 264          //Size of Channel
#define FCS_SERIAL_COUNTER_MAX 50     //Max Allowed Value of Serial Counter
#define FCS_CPU_CLOCK_SPEED  1e6      //Set Clock Speed for Operation
#define FCS_FAUX_PIN  60              //Invalid PIN Number for placeholding


//Flags/////////////////////////////---------------------------------------
volatile bool TimerFlag = false;
volatile bool MeasureGate = false;
////////////////////////////////////---------------------------------------




//Variables//////////////////////////-----------------------------------------
float clock_speed = F_CPU_ACTUAL;
volatile unsigned int aindex = 0;//---------------------->//Deault measurement index
volatile unsigned int f_index = 0;//-------------------->//Index Used to Access p_count array
volatile unsigned int faux_index = 0;//----------------->//Faux Index Used to store counts outside Tsample
volatile unsigned int SerialCounter = 0;//-------------->//Counter for Serial Out
volatile unsigned int print_index = 0;//---------------->//
volatile unsigned int SyncIndex = 0;//------------------>//Counter for Sync Pulse
volatile unsigned int Tsample = 1; //------------------->//Time for measurement of the sample
                                                         //In microseconds
////////////////////////////////////------------------------------------------



/// Arrays //////////////////////////----------------------------------------
float Channel_Array[FCS_CHANNEL_SIZE]; //Contains G(Tau) values
float Copy_Array[FCS_CHANNEL_SIZE];    //Copy Holder for Channel_Array
unsigned int p_count[FCS_PCOUNT_MAX_SIZE];  //Holds the Photon Count for Sample[index]
////////////////////////////////////-----------------------------------------



/// Pin Definations /////////////////----------------------------------------
const unsigned int SyncPin = 0;
const unsigned int PulsePin = 0;
const unsigned int ErrorLEDPin1 = 0;
const unsigned int ErrorLEDPin2 = 0;
const unsigned int ErrorLEDPin3 = 0;
////////////////////////////////////-----------------------------------------


//OneShotTimer  timer1(GPT1); // next free channel of TMR1 (up to 4)


//1
void setup()
{
	//1.1 Set up Serial
	//Serial.begin(FCS_BAUD_RATE); //Redundant for Teensy
	//Serial.println("Welcome\nSerial Port is set.\n");


	//1.2 Set up PIN Modes
  pinMode(SyncPin, INPUT);
  pinMode(PulsePin, INPUT);
  //LEDPins
  

	//1.3 Setup Timers
	//Empty

	//1.4 Attach Interrupt to Pins
	attachInterrupt(digitalPinToInterrupt(SyncPin), ISR_Open, RISING);
  attachInterrupt(digitalPinToInterrupt(PulsePin), ISR_Pulse, RISING);

  
	//1.5 Set up Interrupt Priorities
  //NVIC_SET_PRIORITY();

} //End of setup()
//--------



//2
void loop()
{

  //Timing Subroutine
	if(TimerFlag)
	{
		//Set timerinterrupt → or Tsample with interrupt ISR_Close()
    
		TimerFlag = false;       //Unset the flag
		//timer1.trigger(Tsample); // trigger the timer with Tsample delay
    //timer1.begin(ISR_Close); //Timer 1 evokes ISR_Close()

    //Attach Pin Interrupt
    attachInterrupt(digitalPinToInterrupt(PulsePin), ISR_Pulse, RISING);
	}

  //Serial Subroutine
	// Print Channel at regular intervals
	if(SerialCounter >= FCS_SERIAL_COUNTER_MAX)
	{
		SerialCounter = 0;  //Set Serial Counter to Zero
		SerialOutChannel(); //Call SerialOutChannel() function
	}

} //End of loop()
//--------


//3
void ISR_Open() //Triggered when the sync pulse is received
{
	SyncIndex++;

	#if FCS_EVEN_SYNC_PULSE == 1
	if(SyncIndex%2 == 0) //Open Gate for only even sysc pulses
	#endif
	{
		//0 index is missed here -> by design
		aindex++;            //Increment the SampleClock index
		f_index = aindex;    //Set f_index
		TimerFlag = true;   //Set timerflag
	}	


} //End of ISR_Open()
//--------


//4
void ISR_Close() //Trigged by the timer after a fixed interval del(SampleT)
{
	
	detachInterrupt(digitalPinToInterrupt(PulsePin)); //Detach Pulse Interrupt
  
	Channeler();           //Channel the read values for G(Tau) calculation
	f_index = faux_index;  // If the detachInterrupt Works -> redundant
} //End of ISR_Close()
//--------


//5
void ISR_Pulse() //Triggered when a pulse is detected on the PulsePin
{
	p_count[f_index]++;
} //End of ISR_pulse()
//--------


//6
//Serial Outs the Channel Array
void SerialOutChannel()
{

	//Make a Copy of the channel array
  
	noInterrupts(); //Disable Interrupts
	ArrayCopy(Copy_Array, Channel_Array, FCS_CHANNEL_SIZE); //ARGS -> Copy_To, Copy_From, ArraySize
	interrupts();   //Enable Interrupts

	//Print Channel from Copy to Serial Out
	ArrayPrint(Channel_Array, FCS_CHANNEL_SIZE);
	print_index++;
} //End of SerialOutChannel()



//7
//Array Print Function -> Sends the array piecewise to Serial Out
void ArrayPrint(float *array, int size)
{
	for(int i = 0; i < size; i++)
	{
		Serial.print(array[i], ',');
	}
	Serial.print('\n');
} //End of ArrayPrint()

//8
//Simple Array Copy Function
void ArrayCopy(float* original, float* duplicate, size_t sizex)
{
  memcpy(duplicate, original, sizex);
}


//9
//Blank function -> Placeholder
void Channeler()
{
  delay(10);  
}
