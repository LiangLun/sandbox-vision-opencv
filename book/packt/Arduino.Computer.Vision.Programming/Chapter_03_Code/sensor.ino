#define TEMPERATURE_SENSOR_ADC (0)
float temperatureInCelcius=0.0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  temperatureInCelcius = (analogRead(TEMPERATURE_SENSOR_ADC) * 5.0) / (1024.0 * 10.0);
  Serial.print("TEMPRATURE = ");
  Serial.print(temperatureInCelcius);
  Serial.print("*C");
  Serial.println();
  delay(1000);
}

