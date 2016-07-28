#define TEMPERATURE_SENSOR_ADC (0)
float temperatureInCelcOld=0.0;
float temperatureInCelcNew=0.0;
float filterCoeff = 0.1;

void setup()
{
  Serial.begin(9600);
  analogReference(INTERNAL);
  temperatureInCelcNew = (analogRead(TEMPERATURE_SENSOR_ADC) * 5.0) / (1024.0 * 10.0);
  temperatureInCelcOld = temperatureInCelcNew;
}

void loop()
{
  temperatureInCelcNew = analogRead(TEMPERATURE_SENSOR_ADC) / 9.31;
  temperatureInCelcNew = ((1.0 -filterCoeff) * temperatureInCelcNew) + (filterCoeff * temperatureInCelcOld);
  temperatureInCelcOld = temperatureInCelcNew;
  Serial.print("TEMPRATURE = ");
  Serial.print(temperatureInCelcNew);
  Serial.print("*C");
  Serial.println();
  delay(1000);
}

