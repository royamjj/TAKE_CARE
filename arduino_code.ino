int red = 13;
int yellow = 12;
int green = 8;
int buzzer = 2;
int fan_pin = 6;
String status;
void setup() {
  Serial.begin(9600);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT); 
  pinMode(fan, OUTPUT);
}

void loop() {
  while(Serial.available()==0){}
  status=Serial.readString();
  Serial.println(status);
  if (status == "0"){
  signal(0, green, buzzer, 1, 125, fan);
  }
  else if (status == "1"){
    signal(1200, yellow, buzzer, 10, 200, fan);
  }
  else if (status == "2"){
    signal(1200, yellow, buzzer, 10, 50, fan);
  }
  else if (status == "3"){
    signal(5000, red, buzzer, 1, 255, fan);
  }
  else if (status == "4"){
    signal(5000, red, buzzer, 1, 0, fan);
  }
  else if (status == "5"){
    signal(200, red, buzzer, 10, 255, fan);
  }
  else if (status == "6"){
    signal(200, red, buzzer, 10, 0, fan);
}
void signal(int time_delay, int led_pin,int pin_buzzer,int times, int fan_speed, int fan_pin){
  analogWrite(fan_pin, fan_speed);
  for (int i=0; i< times; i++){
  digitalWrite(led_pin, HIGH);
  digitalWrite(pin_buzzer, HIGH);
  delay(time_delay);
  digitalWrite(pin_buzzer,LOW);
  digitalWrite(led_pin, LOW);
  delay(time_delay);
  }
  delay(1000);
}
