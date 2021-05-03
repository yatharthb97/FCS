
//Catch Pulse


//Macro Definations
#define FCSSIM_AvIndexMax 20   //Max number of measurement within an Averaging period
#define FCSSIM_RTTCODE 0b10100011    //Ready To Transmit Code
#define FCSSIM_RTRCODE 0b10111011    //Ready to Receive Code
#define FCSSIM_SYNCOPERATIONS 0 //Enable/Disable Sync Code Operation
//PINs
const int TTLInPin = 6;
const int RedLEDPin = LED_BUILTIN;
const int GreenLEDPin = 10;





unsigned int AveragingInterval = 100000; //In Microseconds ~100ms
long unsigned int pulse_count[FCSSIM_AvIndexMax];  //Stores the Photon Count Per Average Interval
unsigned int averageIndex = 0;                //Index for 1 averaging Interval
volatile uint32_t pc_holder = 0;

uint32_t startAveragingTime = 0;
uint32_t endAveragingTime = 0;
uint32_t elapsedAveragingTime = 0;


//1
void setup() {

  //1.1
  //Set PinModes
  pinMode(TTLInPin, INPUT);
  pinMode(RedLEDPin, OUTPUT);
  pinMode(GreenLEDPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  //1.2
  //Test Code/////////////////
  LEDTestSequence();
  //Serial.println("Serial Port Test"); 
  ////////////////////////

  //Serial Initilization for Arduino
  #ifndef TEENSYDUINO
    Serial.begin(9600);
  #endif

  //Attach Interrupt
  attachInterrupt(digitalPinToInterrupt(TTLInPin), ISR_Pulse, RISING);

  //Serial INOUT Sync
  
  #if FCSSIM_SYNCOPERATIONS == 1
    byte in_code; //One byte
    digitalWrite(RedLEDPin, HIGH); //Wait State
    bool wait_flag = true;
	  while(wait_flag) //while wait_flag is true ↓
	  {
	  	if(Serial1.available())
        in_code = Serial1.read(); //Read One byte at a time
	  	if(in_code == FCSSIM_RTTCODE)
	  		wait_flag = false;
	  }

	  //Send Ready to Read Code
   if(Serial1.availableForWrite())
   {
      //Send Thrice
      Serial1.write(FCSSIM_RTRCODE);
      Serial1.write(FCSSIM_RTRCODE);
      Serial1.write(FCSSIM_RTRCODE);
   }
  #endif // FCSSIM_SYNCOPERATIONS
  //Now the devices are ready

  ArrayPrint(pulse_count, FCSSIM_AvIndexMax);

  //----END
  
} //End of setup() 




void loop()
{
	while(averageIndex < FCSSIM_AvIndexMax)
	{
		digitalWrite(LED_BUILTIN, HIGH); //LED ON

		elapsedAveragingTime = 0;
		while(elapsedAveragingTime < AveragingInterval)
		{
			startAveragingTime = micros();
			//Do Nothing
			//delayMicroseconds(2);

			endAveragingTime = micros();
			elapsedAveragingTime += endAveragingTime - startAveragingTime;
		}

		noInterrupts(); //Disable Interrepts
			pulse_count[averageIndex] = pc_holder; //Copy Value to the array
			pc_holder = 0; //Reset Pulse Count Holder
			averageIndex++; //Increment Averaging Index
		interrupts(); //Enable Interrupts

		digitalWrite(LED_BUILTIN, LOW); //OFF
	} //End of while loop

	//Turn Off the interrupt
	detachInterrupt(digitalPinToInterrupt(TTLInPin));

	ArrayPrint(pulse_count, FCSSIM_AvIndexMax);
  exit(0);
} //End of loop()

void ISR_Pulse()
{
	pc_holder++;
}




void LEDTestSequence()
{
	digitalWrite(RedLEDPin, HIGH);
	digitalWrite(GreenLEDPin, HIGH);
	delay(500);
	digitalWrite(RedLEDPin, LOW);
	digitalWrite(GreenLEDPin, LOW);
	delay(100);
	digitalWrite(RedLEDPin, HIGH);
	digitalWrite(GreenLEDPin, HIGH);
	delay(500);
	digitalWrite(RedLEDPin, LOW);
	digitalWrite(GreenLEDPin, LOW);
}



void ArrayPrint(uint32_t *array, int size)
{
  for(int i = 0; i < size-1; i++)
  {
    Serial.print(array[i]);
    Serial.print(',');
  }
  Serial.print(array[size-1]);
  Serial.print('\n');
} //End of ArrayPrint()
