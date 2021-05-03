#ifdef TEENSYDUINO
  #include <Entropy.h>
#endif

//Trivial Average Monte Carlo Randomizer for FCS on Teensy → 1D Simulator

/*
1. Every ms → one move → OSegment is checked for molecule → Returns Boolean
2. One interval of measurement is 100ms
3. Probality of 0.1 to be inside the Observation Segment (Veff)
*/

//Macrodefinations ----------------------------
#define OneD_PULSE_SERIALOUT 1     //Flag to Set/Unset TTL Serial Output
////////////////////////////////////////////////


//Board Controls -------------------------------
unsigned int pulseDuration = 5;       // In Microseconds ~ms
unsigned int MeasureInterval = 1000;  //In Microseconds → 1us

//Pins
const int TTLPin = 9;
const int RedLEDPin = 3;
const int GreenLEDPin = 10;
//////////////////////////////////////////////////

// Configuration Variables -------------------------
const int TotalSegment = 100;   //Total Length of the Line Segment   
const int OBSegment = 10;        //Observation Segment Length
const int Walkers_No = 5;     //Number of Walkers/Molecules
const int StepSize = 1;         //Step Size for movement
///////////////////////////////////////////////////

// GlobalVariables --------------------------------
const int MaxWalkerIndex = Walkers_No - 1;
int Pos[Walkers_No];                          //Stores the walker's positions
bool InSegment[Walkers_No];                   //Stores if the variable is in observation segment
uint32_t measureIndex = 0;                //Index for 1 measurement Interval
unsigned int InSegmentParticles = 0;          //Keeps track of number of particles in Observation Segment
/////////////////////////////////////////////////////

//Loop Timing Variables//////////
uint32_t startMeasurementTime = 0;
uint32_t endMeasurementTime = 0;
uint32_t elapsedMeasurementTime = 0;

uint32_t startAveragingTime = 0;
uint32_t endAveragingTime = 0;
uint32_t elapsedAveragingTime = 0;
////////////////////////////////

//1
void setup() {

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
  ////////////////////////

  //1.3
  //Initialize and seed random lib ---------------
  #ifdef TEENSYDUINO
    Entropy.Initialize();
    uint32_t seed_value = Entropy.random();
  #else
    uint32_t seed_value = analogRead(0);
  #endif
  
  //Seed Arduino PRNG library
  randomSeed(seed_value);
  ////////////////////////////////////////////////

  //Serial Initilization for Arduino
  #ifndef TEENSYDUINO
    Serial.begin(9600);
  #endif

  //1.4
  //Walker Position Initialization → Random
  for(int i = 0; i < Walkers_No; i++)
  {
    Pos[i] = random(0, TotalSegment);
    InSegment[i] = Pos[i] < OBSegment;
    //Serial.println(Pos[i]);
  }

  //1.5
  //InSegmentParticles Initalization → Simple Accumulate
  for(int i = 0; i < Walkers_No; i++)
  {
    InSegmentParticles += InSegment[i]; 
  }
  
  //1.6
  //Measure Init Timeperiod
  //lastMeasurementTime = micros();
  //lastAverageTime = millis();

  //----END
  
  //ArrayPrint(Pos, Walkers_No);
  
} //End of setup()


//2
void loop() 
{
      //One measurement every millisecond
      startAveragingTime  = millis();
      elapsedMeasurementTime = 0;
      
        //Run Indicator LED
        digitalWrite(LED_BUILTIN, HIGH);
                   
        startMeasurementTime = micros();
        //Move ------------------------------------------------------------
        //Select Random PID
        int partid = random(0, MaxWalkerIndex);
      
        //Move
        int sign = random(0, 2); 
        sign += (!sign)*-1;
        Pos[partid] += sign*StepSize;
      
        //Periodic Boundary Condition
        Pos[partid] += (Pos[partid] >= TotalSegment)*(-TotalSegment) + (Pos[partid] < 0)*(TotalSegment);
      
        //Check If the Particle is within Observation Volume

        //A
        //Particle has entered the observation Volume
        if(Pos[partid] < OBSegment && !InSegment[partid])
        {
          InSegment[partid] = true;
          InSegmentParticles++;
        }
          
         //B
         //Particle has left the observation Volume
         if(Pos[partid] >= OBSegment && InSegment[partid])
         {
            InSegmentParticles--;
            InSegment[partid] = false;
         }
      
         //Move is complete ----------------------------------------------
         
      
         //Generate TTL Pulse ----------------------------------------------

         //Bring Down the OUT State to 0
         Serial.println(0);
         Serial.flush();
         
         if(InSegmentParticles > 0)
         {
           digitalWrite(TTLPin, HIGH);
           delayMicroseconds(pulseDuration);
           digitalWrite(TTLPin, LOW);
   
           #if OneD_PULSE_SERIALOUT == 1
           //Produce Serial Output of the Pulse
             Serial.println(1);
             Serial.flush();
      
           #endif
         }

         else
         {
            #if OneD_PULSE_SERIALOUT == 1
              Serial.println(0);
              Serial.flush();
            #endif
            delayMicroseconds(pulseDuration);
            
         }

         //Set-Unset Error State if there are more than 1 particle in the observation Segment
         digitalWrite(RedLEDPin, (InSegmentParticles > 1));   //More than one particle in observation segment
         digitalWrite(GreenLEDPin, (InSegmentParticles > 0)); //At least one particle present in Observation Segment
          
          //Update Time Variable
          measureIndex++; //Increment Measure Index

          //Set appropriate delay
          delayMicroseconds(MeasureInterval - (micros() - startMeasurementTime));

        //Turn Off the Run Indicator LED
         digitalWrite(LED_BUILTIN, LOW);
    
} //End of loop()



void ArrayPrint(long int *array, int size)
{
  for(int i = 0; i < size-1; i++)
  {
    Serial.print(array[i]);
    Serial.print(',');
  }
  Serial.print(array[size-1]);
  Serial.print('\n');
} //End of ArrayPrint()
