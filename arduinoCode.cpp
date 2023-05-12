// C++ code
//

#define OUTPUT_LED_BATTERY 2
#define INPUT_TILT 3

#define INPUT_HUMIDITY A0
#define INPUT_TEMPERATURE A1
#define INPUT_LIGHT A4
#define INPUT_GAZ A5

#define NO_GAZ 306
#define NIGHT 644
#define ALERT_HUMIDITY_VALUE 300

int temperatureState = -35;
int tiltOldState = false;
bool gazState = false;
bool alertState = false;
bool nightState = false;

void setup()
{
  pinMode(OUTPUT_LED_BATTERY, OUTPUT);
  
  pinMode(INPUT_HUMIDITY, INPUT);
  pinMode(INPUT_TEMPERATURE, INPUT);
  pinMode(INPUT_LIGHT, INPUT);
  pinMode(INPUT_GAZ, INPUT);
  pinMode(INPUT_TILT, INPUT);
  Serial.begin(9600);
}

void temperature(){
  int temperatureValue = map(((analogRead(INPUT_TEMPERATURE) - 20) * 3.04), 0, 1023, -40, 125);
  
  if(temperatureState == temperatureValue) return;
  temperatureState = temperatureValue;
  Serial.print("Temperature: ");
  Serial.print(temperatureValue);
  Serial.print(" C");
  Serial.println();
}

bool hasSameTilt(int tiltState){
  return tiltState == tiltOldState;
}

void tilt(){
  int tiltState = digitalRead(INPUT_TILT);
  
  if (hasSameTilt(tiltState)) return;
  
  if(tiltState)
    Serial.println("Votre nain bouge se deplace de facon anormale");
  else
    Serial.println("Votre nain semble etre droit");
  tiltOldState = tiltState;
}


bool hasGaz(int gazValue){
  return gazValue > NO_GAZ;
}

void gaz(){
  int gazValue = analogRead(INPUT_GAZ);
  
  if (gazState == hasGaz(gazValue)) return;
  
  if (hasGaz(gazValue)){
    Serial.println("gaz");
    gazState = true;
  }else{
    Serial.println("no gaz");
    gazState = false;
  }
}

bool isHumidity(int humidityValue){
  return humidityValue >= ALERT_HUMIDITY_VALUE;
}

void humidity(){
  int humidityValue = analogRead(INPUT_HUMIDITY);
  
  if(alertState == isHumidity(humidityValue)) return;
  
  if(isHumidity(humidityValue)){
    Serial.println("Votre sol est trop humide il pleut ?");
    alertState = true;
  }else{
    Serial.println("il fait sec, pensez a arroser");
    alertState = false;
  }
}

bool isNight(int lightValue){
  return lightValue > NIGHT;
}

void sun(){
  int lightValue = analogRead(INPUT_LIGHT);

  if(nightState == isNight(lightValue)) return;

  if(isNight(lightValue)){
    Serial.println("il fait nuit");
    nightState = true;
  }else{
    Serial.println("il fait jour");
    nightState = false;
  }
}

void loop()
{
  digitalWrite(OUTPUT_LED_BATTERY, HIGH);
  temperature();
  tilt();
  gaz();
  sun();
  humidity();
}
