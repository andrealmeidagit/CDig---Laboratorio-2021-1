#define PIN_LED 13
#define PIN_MOTORA_1 5
#define PIN_MOTORA_2 6

#define PIN_MOTORB_1 9
#define PIN_MOTORB_2 10

// HC-SR04
#define PIN_US_ECHO 2
#define PIN_US_TRIG 3

#define TIME_ON 20000
#define TIME_OFF 1000

int speedMotorA = 255;
int speedMotorB = 255;

// Timers (ms)
unsigned long currentTime;
unsigned long lastTime;
const unsigned long periodTime = 500;

int headRef;

void setup() {
  
  Serial.begin(9600);

  pinMode(PIN_LED, OUTPUT);

  pinMode(PIN_US_TRIG, OUTPUT);
  pinMode(PIN_US_ECHO, INPUT);
  
  pinMode(PIN_MOTORA_1, OUTPUT);
  pinMode(PIN_MOTORA_2, OUTPUT);
  pinMode(PIN_MOTORB_1, OUTPUT);
  pinMode(PIN_MOTORB_2, OUTPUT);

  analogWrite(PIN_MOTORA_1,0);
  analogWrite(PIN_MOTORA_2,0);
  analogWrite(PIN_MOTORB_1,0);
  analogWrite(PIN_MOTORB_2,0);

  speedMotorA = 255;
  speedMotorB = 0;

  headRef = 400;
}

void loop() {
    // Controla a frequência do loop
    if(millis() > lastTime + periodTime)
    {
      lastTime = millis(); // Reinicia Timer

      // Ler Altura
      int head = readUltrasound();
      int error = headRef - head;

      // Controle
      if( error > 255 )
      {
        speedMotorA = 255;
        speedMotorB = 0;
      }
      else if( error < -255 )
      {
        speedMotorA = 0;
        speedMotorB = 255;
      }
      else if ( error < 0 )
      {
        speedMotorA = 0;
        speedMotorB = -error;
      }
      else if ( error > 0 )
      {
        speedMotorA = error;
        speedMotorB = 0;
      }
      else
      {
        speedMotorA = 0;
        speedMotorB = 0;
      }


      // Aciona Motores
      analogWrite(PIN_MOTORA_1,0);
      analogWrite(PIN_MOTORA_2,speedMotorA);
      analogWrite(PIN_MOTORB_1,0);
      analogWrite(PIN_MOTORB_2,speedMotorB);
      digitalWrite(PIN_LED,HIGH);

      // Imprime Dados na 
      Serial.print("Tempo: ");
      Serial.print( lastTime );
      Serial.print(", Distância: ");
      Serial.print( head );
      Serial.print(", Erro: ");
      Serial.print( error );
      Serial.print(", MotorA: ");
      Serial.print( speedMotorA );
      Serial.print(", MotorB: ");
      Serial.println( speedMotorB );
    }
}

int readUltrasound()
{
  long duration; // variable for the duration of sound wave travel
  int distance; // variable for the distance measurement
  
  digitalWrite(PIN_US_TRIG, LOW); 
  delayMicroseconds(2);

  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(PIN_US_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_US_TRIG, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(PIN_US_ECHO, HIGH);

  // Calculating the distance
  distance = duration * 3.4 / 2;

  return distance;
}
