//For type optimizations
using uint_t = unsigned int;     //Default unsigned int type
using int_t = int;               //Deault signed int type
using float_t = float;           //Default float type
using double_t = double;         //Default double type




//Autocorrelation


//g(T) = sum(from t = 1 to m-T) [ (e_t)*(e_(T+t)] - (sum(from t = 1 to m) [e_t/m] )^2

int     tau = 7      
 int     m   = 80;
 double  *e; /* An array filled with m values */
 double  lhs,rhs,answer;
 int     tau0;

 /* Left Summation */
 for(sum=0,tau0=1; tau0 < m-tau; ++tau0)
   sum += e[tau0] * e[tau+tau0]; 
 lhs = sum / m;

 /* Right Summation */
 for(sum=0,tau0=1; tau0 < m; ++tau0)
   sum += e[tau0] / m;
 rhs = sum * sum;

 answer = lhs - rhs;





 //----- Include files -------------------------------------------------------
 #include <stdio.h>                 // Needed for printf() and feof()
 #include <math.h>                  // Needed for pow()�
 #include <stdlib.h>                // Needed for exit() and atof()
 #include <string.h>                // Needed for strcmp()

 //----- Defines -------------------------------------------------------------
 #define MAX_SIZE 10000000          // Maximum size of time series data array
 #define NUM_LAG       100          // Number of lags to compute for

 //----- Globals -------------------------------------------------------------
 double    *X;                      // Time series read from "in.dat"
 long int   N;                      // Number of values in "in.dat"
 double     Mean;                   // Mean of series X
 double     Variance;               // Variance of series X

 //----- Function prototypes -------------------------------------------------
 void   load_X_array(void);         // Load X array
 double compute_mean(void);         // Compute Mean for X
 double compute_variance(void);     // Compute Variance for X
 double compute_autoc(int lag);     // Compute autocorrelation for X

 //===========================================================================
 //=  Main program                                                           =
 //===========================================================================
 void main(void)
 {
   double   ac_value;                 // Computed autocorrelation value
   int      i;                        // Loop counter

   // Malloc space for X
   X = (double *) malloc(sizeof(double) * MAX_SIZE);
   if (X == NULL)
   {
     printf("*** ERROR - Could not malloc() enough space \n");
     exit(1);
   }

   // Load the series X
   printf("------------------------------------------------- autoc.c -----\n");
   load_X_array();

   // Compute global variables Mean and Variance for X
   Mean = compute_mean();
   Variance = compute_variance();

   // Compute and output AC value (rho) for series X of length N
   for (i=1; i<=NUM_LAG; i++)
   {
     ac_value = compute_autoc(i);
     printf("  Autocorrelation for lag %4ld = %f \n", i, ac_value);
   }
   printf("---------------------------------------------------------------\n");

   // Free the malloced memory
   free(X);
 }

 //===========================================================================
 //=  Function to load X array from stdin and determine N                    =
 //===========================================================================
 void load_X_array(void)
 {
   char      temp_string[1024];     // Temporary string variable

   // Read all values into X
   N = 0;
   while(1)
   {
     scanf("%s", temp_string);
     if (feof(stdin)) goto end;

     // This handles a comment bounded by "&" symbols
     while (strcmp(temp_string, "&") == 0)
     {
       do
       {
         scanf("%s", temp_string);
         if (feof(stdin)) goto end;
       } while (strcmp(temp_string, "&") != 0);
       scanf("%s", temp_string);
       if (feof(stdin)) goto end;
     }

     // Enter value in array and increment array index
     X[N] = atof(temp_string);
     N++;

     // Check if MAX_SIZE data values exceeded
     if (N >= MAX_SIZE)
     {
       printf("*** ERROR - greater than %ld data values \n", MAX_SIZE);
       exit(1);
     }
   }

   // End-of-file escape
   end:

   return;
 }

 //===========================================================================
 //=  Function to compute mean for a series X                                =
 //===========================================================================
 double compute_mean(void)
 {
   double   mean;        // Computed mean value to be returned
   int      i;           // Loop counter

   // Loop to compute mean
   mean = 0.0;
   for (i=0; i<N; i++)
     mean = mean + (X[i] / N);

   return(mean);
 }

 //===========================================================================
 //=  Function to compute variance for a series X                            =
 //===========================================================================
 double compute_variance(void)
 {
   double   var;         // Computed variance value to be returned
   int      i;           // Loop counter

   // Loop to compute variance
   var = 0.0;
   for (i=0; i<N; i++)
     var = var + (pow((X[i] - Mean), 2.0) / N);

   return(var);
 }

 //===========================================================================
 //=  Function to compute autocorrelation for a series X                     =
 //=   - Corrected divide by N to divide (N - lag) from Tobias Mueller       =
 //===========================================================================
 double compute_autoc(int lag)
 {
   double   autocv;      // Autocovariance value
   double   ac_value;    // Computed autocorrelation value to be returned
   int      i;           // Loop counter

   // Loop to compute autovariance
   autocv = 0.0;
   for (i=0; i<(N - lag); i++)
     autocv = autocv + ((X[i] - Mean) * (X[i+lag] - Mean));
   autocv = (1.0 / (N - lag)) * autocv;

   // Autocorrelation is autocovariance divided by variance
   ac_value = autocv / Variance;

   return(ac_value);
 }




//Template function that records the number of photons and updates the p_count array
 //Do not inline
void PhotonCounter(int index)
{
	//int readval = Perform Reading Operation;

	p_count[index] = readval;

} //End of PhotonCounter()










 const int photoPIN = 12; // This is where the input is fed.
 const int baud_rate = 9600;
 int pulse = 0; // Variable for saving pulses count.
 int var = 0;

 void setup(){
   pinMode(input, INPUT);
  
   Serial.begin(baud_rate);

 }

 void loop(){ 
   if(digitalRead(photoPIN) == 1)
   {

     pulsecount++;
    
     Serial.print(pulse);
     Serial.print(F(" pulse"));

     // Put an "s" if the amount of pulses is greater than 1.
     if(pulse > 1) {Serial.print(F("s"));}
    
     Serial.println(F(" detected."));
   }
  
   if(digitalRead(input) == 0) {var = 0;}
  
   delay(1); // Delay for stability.
 }



#define y_intervals 50
 binDuration = 1; //In Microseconds

 unsigned int count[y_intervals]; //Initalized to zero


void OutputData()
{
 	for(int i = 0; i < y_intervals; i++)
 	{
 		Serial.print(count[i]);
 		Serialrial.print(",");
 	}

 	Serial.print('\n');
}



// Create an IntervalTimer object 
IntervalTimer myTimer;

const int ledPin = LED_BUILTIN;  // the pin with a LED

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(57600);
  myTimer.begin(blinkLED, 150000);  // blinkLED to run every 0.15 seconds
}

// The interrupt will blink the LED, and keep
// track of how many times it has blinked.
volatile unsigned long freqCount = 0; // use volatile for shared variables

// functions called by IntervalTimer should be short, run as quickly as
// possible, and should avoid calling other functions if possible.
void blinkLED() {
  if (ledState == LOW) {
    ledState = HIGH;
    blinkCount = blinkCount + 1;  // increase when LED turns on
  } else {
    ledState = LOW;
  }
  digitalWrite(ledPin, ledState);
}

// The main program will print the blink count
// to the Arduino Serial Monitor
void loop() {
  unsigned long blinkCopy;  // holds a copy of the blinkCount

  // to read a variable which the interrupt code writes, we
  // must temporarily disable interrupts, to be sure it will
  // not change while we are reading.  To minimize the time
  // with interrupts off, just quickly make a copy, and then
  // use the copy while allowing the interrupt to keep working.
  noInterrupts();
  blinkCopy = blinkCount;
  interrupts();

  Serial.print("blinkCount = ");
  Serial.println(blinkCopy);
  delay(100);
}


//Toogles Counter Gate
OpenGate = !OpenGate;

void ISR_PulseReceived()
{
	//Serial Out the current frequency count
	Serial.println(FreqCounter);

	//Or Store in the array
	//p_count[index] = FreqCounter;
	//index++; //Increment the index

	//Reset the frequency Counter to zero
	FreqCounter = 0;
}


void loop()
{
	if(OpenGate)
	{

	}
}






#define SIG 3

float clock_speed = F_CPU_ACTUAL;
const int buffer_size = 32;
uint32_t buffer[buffer_size];
uint32_t last = 0;
int write_index = 0;
int read_index = 0;

void setup() {
	ARM_DEMCR |= ARM_DEMCR_TRCENA;
	ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
	attachInterrupt(digitalPinToInterrupt(SIG), pulseReceived, RISING);
}

void pulseReceived() {
	uint32_t current = ARM_DWT_CYCCNT;
	if (last != 0) {
		buffer[write_index % buffer_size] = current - last;
		write_index += 1;
	}
	last = current;
}

void loop() {
	while (read_index < write_index) {
		Serial.print(read_index);
		Serial.print(",");
		Serial.println(buffer[read_index % buffer_size] * 1/(clock_speed) * 1e9);
		read_index += 1;
	}
}


volatile uint_8 p_count;
void ISR_pulse()
{
	p_count++;
}


