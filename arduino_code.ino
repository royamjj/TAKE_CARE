int red = 13;
int yellow = 12;
int green = 8;
int buzzer = 2;
int fan = 6;
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
if (status == "GREEN"){
signal(0, green, 2, 1);
analogWrite(fan, 125);
}
else if (status == "YELLOW"){
  signal(1200, yellow, 2, 10);
  analogWrite(fan, 200);
}
else if (status == "RED"){
  signal(200, red, 2, 20);
  analogWrite(fan, 255);
}
else if (status == "ANOMALY"){
  digitalWrite(red, HIGH);
  digitalWrite(buzzer, HIGH);
  delay(5000);
  digitalWrite(red, LOW);
  digitalWrite(buzzer, LOW);
  analogWrite(fan, 255);
}
else if (status == "COLD"){
  signal(200, red, 2, 20);
  analogWrite(fan, 0);
}
void signal(int time_delay, int led_pin,int pin_buzzer,int times){
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
