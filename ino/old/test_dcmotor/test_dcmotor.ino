#define PIN_LED 13
#define PIN_MOTORA_1 5
#define PIN_MOTORA_2 6

#define PIN_MOTORB_1 9
#define PIN_MOTORB_2 10

#define TIME_ON 5000
#define TIME_OFF 500

int speedMotor = 255;

void setup() {
  
  Serial.begin(9600);

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_MOTORA_1, OUTPUT);
  pinMode(PIN_MOTORA_2, OUTPUT);
  pinMode(PIN_MOTORB_1, OUTPUT);
  pinMode(PIN_MOTORB_2, OUTPUT);

  analogWrite(PIN_MOTORA_1,0);
  analogWrite(PIN_MOTORA_2,0);
  analogWrite(PIN_MOTORB_1,0);
  analogWrite(PIN_MOTORB_2,0);
}

void loop() {
    // Liga Só motorA
    analogWrite(PIN_MOTORA_1,0);
    analogWrite(PIN_MOTORA_2,speedMotor);
    analogWrite(PIN_MOTORB_1,0);
    analogWrite(PIN_MOTORB_2,0);
    digitalWrite(PIN_LED,HIGH);

    Serial.print("A1: 0 - A2: ");
    Serial.print(speedMotor);
    Serial.println(" - B1: 0 - B2:0");

    delay(TIME_ON);
    
    // Desliga Tudo
    analogWrite(PIN_MOTORA_1,0);
    analogWrite(PIN_MOTORA_2,0);
    analogWrite(PIN_MOTORB_1,0);
    analogWrite(PIN_MOTORB_2,0);
    digitalWrite(PIN_LED,LOW);

    delay(TIME_OFF);

    // Liga Só motorB
    analogWrite(PIN_MOTORA_1,0);
    analogWrite(PIN_MOTORA_2,0);
    analogWrite(PIN_MOTORB_1,0);
    analogWrite(PIN_MOTORB_2,speedMotor);
    digitalWrite(PIN_LED,HIGH);

    Serial.print("A1: 0 - A2: 0 - B1: 0 - B2: ");
    Serial.println(speedMotor);

    delay(TIME_ON);
    
    // Desliga Tudo
    analogWrite(PIN_MOTORA_1,0);
    analogWrite(PIN_MOTORA_2,0);
    analogWrite(PIN_MOTORB_1,0);
    analogWrite(PIN_MOTORB_2,0);
    digitalWrite(PIN_LED,LOW);

    delay(TIME_OFF);
}
