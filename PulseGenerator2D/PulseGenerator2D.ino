//Main ile

// Macro definations ------------------> 
#if defined(TEENSYDUINO) || defined(AVR_ATmega328) //For Ardunio Uno
  #define FCSSIM_EMBEDDEDSYSTEMS
#endif //def (TEENSYDUINO || AVR_ATmega328)

#define FCSSIM_DEFAULT_EDGE 100
#define FCSSIM_MULTI_PART_STATE 0
#define FCSSIM_ERRORS 0
#define FCSSIM_DSEP ','
#define FCSSIM_FRAME_EXPORT 0
 

#define FCSSIM_MeasureIndexMax 1e10 //Max number of measurements
#define FCSSIM_AverageIndexMax 100  //Max Number of Averages recorded during one run
#define FCSSIM_PULSE_SERIALOUT 1    //Flag to Set/Unset TTL Serial Output
#define FCSSIM_EMPTYENTROPYPIN 0

//Sync Operations
#define FCSSIM_RTTCODE 0b10100011    //Ready To Transmit Code
#define FCSSIM_RTRCODE 0b10111011    //Ready to Receive Code
#define FCSSIM_SYNCOPERATIONS 0 //Enable/Disable Sync Code Operation

// Libraries  ------->
#include "rand.h" //For random number generation
#include "helper.h"

//Pins
#if defined FCSSIM_EMBEDDEDSYSTEMS
const int TTLPin = 9;
const int RedLEDPin = 3;
const int GreenLEDPin = 10;
#endif //FCSSIM_EMBEDDEDSYSTEMS
//////////////////////////////////////////////////


// Configuration Variables -------------------------
//const int TotalArea = 100;   //Total Length of the Line Segment 
const int Edge = 16; //Edge X Edge Square

//Observation Boundaries  
MinMax OBx(3,8); //Occupancy → 3 to 7 [3,8)
MinMax OBy(3,8); //Occupancy → 3 to 7 [3,8)
//Observation Boundaries

const int Walkers_No = 2;     //Number of Walkers/Molecules
const int StepSize = 4;         //Step Size for movement

const unsigned int pulseDuration = 5;       //← In Microseconds
const unsigned int MeasureInterval = 1000;  //← In Microseconds = 1ms
///////////////////////////////////////////////////



// GlobalVariables --------------------------------
const int MaxWalkerIndex = Walkers_No - 1;
int POSx[Walkers_No]= {0};                  //Stores the walker's x positions
int POSy[Walkers_No]= {0};                  //Stores the walker's x positions
bool InArea[Walkers_No] = {false};          //Stores if the variable is in observation segment
uint32_t measureIndex = 0;

unsigned int pulseIndex = 0;          //Index for 1 measurement Interval
unsigned int InAreaParticles = 0;     //Keeps track of number of particles in Observation Segment
/////////////////////////////////////////////////////


//Loop Timing Variables//////////
uint32_t startMeasurementTime = 0;
//uint32_t endMeasurementTime = 0;
//uint32_t elapsedMeasurementTime = 0;
////////////////////////////////


#ifndef FCSSIM_EMBEDDEDSYSTEMS
//PC Specific Global --------------------------------------------------------------
bool DirectorySet = false;
string_t session_name = "Test";
std::string temp_file = "./tempfile.dat";
#endif // FCS_SIM_EMBEDDEDSYSTEMS


void setup()
{

	#if defined FCSSIM_EMBEDDEDSYSTEMS
	//1.1
	//Set PinModes
	pinMode(TTLPin, OUTPUT);
	pinMode(RedLEDPin, OUTPUT);
	pinMode(GreenLEDPin, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);

	//1.2
	//Test Code/////////////////
	digitalWrite(RedLEDPin, HIGH);
	digitalWrite(GreenLEDPin, HIGH);
	delay(1000);
	digitalWrite(RedLEDPin, LOW);
	digitalWrite(GreenLEDPin, LOW);
	//Serial.println("Serial Port Test");
	#endif // FCS_SIM_EMBEDDEDSYSTEMS
	////////////////////////

	//1.3
	//Initialize and seed random lib ---------------
  //void Init(unsigned int walker_size, unsigned int edge, unsigned int StepSize)
	Rand::Init(Walkers_No, Edge, StepSize);

	////////////////////////////////////////////////

	//Serial Initilization for Arduino
	#if !defined(TEENSYDUINO) && defined(FCSSIM_EMBEDDEDSYSTEMS) 
	  Serial.begin(9600);
	#endif // TEENSYDUINO


	//1.4
	//Walker Position Initialization → Random
	for(int i = 0; i < Walkers_No; i++)
	{
	  //TODO → Implement this section for on PC Simulation
	  POSx[i] = Rand::InEdge();
	  POSy[i] = Rand::InEdge();

	  InArea[i] = OBx.within_bounds(POSx[i]) && OBy.within_bounds(POSy[i]);
	}

	//1.5
	//InSegmentParticles Initalization → Simple Accumulate
	for(int i = 0; i < Walkers_No; i++)
	{
	  InAreaParticles += InArea[i]; 
	}

	//1.6 → Sync with other Teensy
	#if FCSSIM_SYNCOPERATIONS == 1
	bool sync_flag = false;
	byte in_code; //One byte
	while(!sync_flag)
	{
					
		//Write Code
		 if(Serial1.availableForWrite())
			Serial1.write(FCSSIM_RTTCODE);

		//Read Code
		if(Serial1.available())
		    in_code = Serial1.read(); //Read One byte at a time
		if(in_code == FCSSIM_RTRCODE)
			  sync_flag = true;
	}

	#endif // FCSSIM_SYNCOPERATIONS
	//----END

}

void loop()
{
	while(measureIndex < FCSSIM_MeasureIndexMax)
	{
		//One Pulse every millisecond
   
		  //Run Indicator LED
		  #ifdef FCSSIM_EMBEDDEDSYSTEMS
		  	digitalWrite(LED_BUILTIN, HIGH);
		  #endif
		             
		  startMeasurementTime = micros();
		  //Move ------------------------------------------------------------
		  //Select Random PID
		  int partid = Rand::NewPart();
		
		  //Move
		  Move(partid);
		
		  //Check If the Particle is within Observation Volume
		  bool now_InArea = OBx.within_bounds(POSx[partid]) && OBy.within_bounds(POSx[partid]);

		   //A
		   //Particle has entered the observation Volume
		   if(now_InArea && !InArea[partid])
		   {
		     InArea[partid] = true;
		     InAreaParticles++;

		   }
		    
		   //B
		   //Particle has left the observation Volume
		   if(!now_InArea && InArea[partid])
		   {
		      InAreaParticles--;
		      InArea[partid] = false;
		   }
		   //Move is complete ----------------------------------------------
		   
		
		   //Generate TTL Pulse ----------------------------------------------

		   //Bring Down the OUT State to 0
		   #if FCSSIM_PULSE_SERIALOUT == 1
			   Serial.println(0);
			   Serial.flush();
		   #endif
		   

		   #if FCSSIM_SINGLEPULSEONLY == 1
		   if(InAreaParticles > 0)
		   {
		     digitalWrite(TTLPin, HIGH);
		     delayMicroseconds(pulseDuration);
		     digitalWrite(TTLPin, LOW);
		
		    #if FCSSIM_PULSE_SERIALOUT == 1
		     //Produce Serial Output of the Pulse
		       Serial.println(1);
		       Serial.flush();
		    #endif //FCSSIM_PULSE_SERIALOUT
		   }
		   #endif //FCSSIM_SINGLEPULSEONLY

		   #if FCSSIM_SINGLEPULSEONLY == 0
		   if(InAreaParticles > 0)
		   {
		     
		   	 //Process is repeated for each particle
		   	 for(unsigned int i = 0; i < InAreaParticles; i++)
		   	 {
		     	digitalWrite(TTLPin, HIGH);
		     	delayMicroseconds(pulseDuration);
		     	digitalWrite(TTLPin, LOW);
		   
		     	#if FCSSIM_PULSE_SERIALOUT == 1
		     	//Produce Serial Output of the Pulse
		       	Serial.println(1); //ON
		       	Serial.println(0); //OFF
		       	Serial.flush();

		     	#endif
		     }

		   }
		   #endif //FCSSIM_SINGLEPULSEONLY


		   else
		   {
		      #if FCSSIM_PULSE_SERIALOUT == 1
		        Serial.println(0);
		        Serial.flush();
		      #endif
		      delayMicroseconds(pulseDuration);
		      
		   }

		   #ifdef FCSSIM_EMBEDDEDSYSTEMS

			   //Set-Unset Error State if there are more than 1 particle in the observation Segment
			   digitalWrite(RedLEDPin, (InAreaParticles > 1));   //More than one particle in observation segment
			   digitalWrite(GreenLEDPin, (InAreaParticles > 0)); //At least one particle present in Observation Segment

		   #endif  //FCS_SIM_EMBEDDEDSYSTEMS
		    
		    //Update Time Variable
		    measureIndex++; //Increment Measure Index

		    //Set appropriate delay
		    delayMicroseconds(MeasureInterval - (micros() - startMeasurementTime));


		  //Turn Off the Run Indicator LED
		  #ifdef FCSSIM_EMBEDDEDSYSTEMS
		  	digitalWrite(LED_BUILTIN, LOW);
		  #endif //FCS_SIM_EMBEDDEDSYSTEMS

	} //End of the while loop


	exit(0); //Stall processor till reset or exit program successfully on PC

} //End of loop()



void Move(int rndid)
{

	//Move the selected Particle
	POSx[rndid] += Rand::DStep()*Rand::Sign();
	POSy[rndid] += Rand::DStep()*Rand::Sign(); 

	//Apply PBC
	POSx[rndid] = (POSx[rndid] > Edge)*(POSx[rndid] - Edge) + 
				  (POSx[rndid] < 0)*(Edge + POSx[rndid]);

	POSy[rndid] = (POSy[rndid] > Edge)*(POSy[rndid] - Edge) + 
				  (POSy[rndid] < 0)*(Edge + POSy[rndid]);

	//Position is final ----
}
