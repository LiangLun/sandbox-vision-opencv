String inputString = "";         /* a string to hold incoming data */
boolean stringComplete = false;  /* whether the string is complete */
int led = 13;

void setup() {
  /* initialize the digital pin as an output. */
  pinMode(led, OUTPUT);   
  /* initialize serial: */
  Serial.begin(9600);
  /* reserve 256 bytes for the inputString: */
  inputString.reserve(256);
  Serial.println("Arduino: I am ready!");
}

void loop() {
  /* print the string when a newline arrives: */
  if (stringComplete) {
     Serial.println("Sir: " + inputString);
    if(inputString=="Hello Arduino!")
    {
      Serial.println("Arduino: Hello Sir!");
    }
    else if(inputString == "How are you?" )
    {
      Serial.println("Arduino: Fine thanks, and you?");
    }
    else if(inputString == "Fine!" )
    {
      Serial.println("Arduino: This makes me happy sir!");
    }
    else if(inputString == "Light up your LED!" )
    {
      Serial.println("Arduino: Surely Sir!");
      digitalWrite(led, HIGH);  
    }
    else if(inputString == "Light down your LED!" )
    {
      Serial.println("Arduino: Surely Sir!");
      digitalWrite(led, LOW);  
    }
    else if(inputString == "Goodbye Arduino!" )
    {
      Serial.println("Arduino: Goodbye Sir!");
    }
    else
    {
      Serial.println("Arduino: I cannot understand you Sir!");
    }
    /* clear the string: */
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    /* get the new byte: */
    char inChar = (char)Serial.read(); 
    /* add it to the inputString: */
    inputString += inChar;
    /* if the incoming character is a newline, set a flag
     so the main loop can do something about it: */
    if (inChar == '!' || inChar == '?') {
      stringComplete = true;
    } 
  }
}

