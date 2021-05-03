#include <Entropy.h>
const int sizex = 264;
int array[sizex];

void setup() 
{
  Entropy.Initialize();
  pinMode(LED_BUILTIN, OUTPUT);
   //Fill the array
}

void loop() 
{
    digitalWrite(LED_BUILTIN, HIGH);
    //Fill
    for (int i=0; i < sizex; i++) 
    {
      array[i] = Entropy.random(0, 15);
    }

    //Send
    for (int i=0; i < sizex; i++) 
    {
      Serial.print(array[i]);
      Serial.print(",");
    }
    Serial.println();
    Serial.flush();

    digitalWrite(LED_BUILTIN, LOW);
    delay(75); 
    
}
