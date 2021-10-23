/*
 *    Desenvolvido por ANDRE ABREU RODRIGUES DE ALMEIDA
 *    Matricula 12/0007100
 *    
 *    Trabalho de Controle Digital
 *    
 *    Descrição: Controle proporcional de 1 tanque A e 1 tanque B
 *    
 *    Referencia:
 *    (1) K. Ogata.Modern Control Engineering, chapter 4, pages 100–158.
 *                Prentice Hall, 5th edition,2009
 */

//Definicao de pinos

  // Sensores ultrassonicos (nivel)
#define echoPin_US_A 7 // Define o pino D7 como ECHO do sensor ultrassonico(HC-SR04) A
#define trigPin_US_A 8 // Define o pino D8 (PWM) como TRIG do sensor ultrassonico(HC-SR04) A
#define echoPin_US_B 12 // Define o pino D12 como ECHO do sensor ultrassonico(HC-SR04) B
#define trigPin_US_B 11 // Define o pino D11 (PWM) como TRIG do sensor ultrassonico(HC-SR04) B
  // Sensores de fluxo
#define flowPin_A 2 // Define o pino D2 como pino de dados do sensor de fluxo (YF-S401) A
#define flowPin_B 3 // Define o pino D3 como pino de dados do sensor de fluxo (YF-S401) B
  // LED
#define PIN_LED 13
  // Motores
        // Motor A
#define PIN_MOTORA_1 5
#define PIN_MOTORA_2 6
        // Motor B
#define PIN_MOTORB_1 9
#define PIN_MOTORB_2 10
  // Botao
#define button 14
  
  // Unidades internas do programa
#define delayMultiplier 1   //PARA ALTERAR O TEMPO DE AMOSTRAGEM (TS), UTILIZAR ESTE DEFINE

//Variaveis globais

  // Sensores ultrassonicos (nivel)
int distanceFromSensorA, distanceFromSensorB; // Variavel da afericao de distancia
int sumA, sumB; // Variaveis contendo a soma das distancias
int averageA, averageB; // Variaveis que armazenam a media da distancia
int headA, headB; // Variaveis do nível de liquido
int i;  //iterador
  // Sensores de fluxo
double flowRate_A;  // Variavel que armazena o a vazao/fluxo do tanque A
double flowRate_B;  // Variavel que armazena o a vazao/fluxo do tanque B
volatile int count1; // Precisa ser declarado como volatil para ser calculado
                          // corretamente durante as interrupções
volatile int count2; // Precisa ser declarado como volatil para ser calculado
                          // corretamente durante as interrupções
int rA, rB;  // Variavel instantanea R para calculo de polos e zeros
int rMedioA, rMedioB;  // Variavel para armazenar valor medio de R
  // Motores
int speedMotorA, speedMotorB;
  // Referencia
int headRef;
int errorA, errorB;


void setup() {
  //Define entradas e saidas
  pinMode(trigPin_US_A, OUTPUT);  // Define o pino de TRIG do sensor US1 como saida
  pinMode(echoPin_US_A, INPUT);   // Define o pino de ECHO do sensor US1 como entrada
  pinMode(trigPin_US_B, OUTPUT);  // Define o pino de TRIG do sensor US2 como saida
  pinMode(echoPin_US_B, INPUT);   // Define o pino de ECHO do sensor US2 como entrada
  pinMode(flowRate_A, INPUT);     // Define o pino de dados do sensor de fluxo A como entrada
  pinMode(flowRate_B, INPUT);     // Define o pino de dados do sensor de fluxo B como entrada
  pinMode(PIN_LED, OUTPUT);       // Define o LED do pino 13 como saida
  pinMode(PIN_MOTORA_1, OUTPUT);  // Define o pino de controle do motor como saida
  pinMode(PIN_MOTORA_2, OUTPUT);  // Define o pino de controle do motor como saida
  pinMode(PIN_MOTORB_1, OUTPUT);  // Define o pino de controle do motor como saida
  pinMode(PIN_MOTORB_2, OUTPUT);  // Define o pino de controle do motor como saida
  pinMode(button, INPUT);         // Le botao

// Inicia os motores em estado parado
  analogWrite(PIN_MOTORA_1,0);
  analogWrite(PIN_MOTORA_2,0);
  analogWrite(PIN_MOTORB_1,0);
  analogWrite(PIN_MOTORB_2,0);

  // Define as interrupcoes
  attachInterrupt(digitalPinToInterrupt(flowPin_A), flow1, RISING);  // Configura interrupção
                                   // do pino 2 do Arduino para executar a funcao "flow"
  attachInterrupt(digitalPinToInterrupt(flowPin_B), flow2, RISING);  // Configura interrupção
                                   // do pino 3 do Arduino para executar a funcao "flow"
  
  //Define frequencia de comunicacao serial
  Serial.begin(9600); // Define BAUDRATE em 9600
  
  //Inicia variaveis
  distanceFromSensorA = 0;
  distanceFromSensorB = 0;
  sumA = 0;
  sumB = 0;
  averageA = 0;
  averageB = 0;
  i = 0;
  rA = 0;
  rB = 0;
  rMedioA = 0;
  rMedioB = 0;
  speedMotorA = 0;
  speedMotorB = 0;
  headRef = 70;
  errorA = 0;
  errorB = 0;
}

void loop() {
  // Medicao de nivel dos tanques
    // Lê distancia do sensor ultrassonico A
  distanceFromSensorA = leDistancia(trigPin_US_A, echoPin_US_A);
  distanceFromSensorB = leDistancia(trigPin_US_B, echoPin_US_B);
    // Calcula o nível de líquido
  headA = 134 - distanceFromSensorA;
  headB = 132 - distanceFromSensorB;
  //media(headA, headB);

  // Medicao de fluxo de saida dos tanques
    // Executa interrupcoes
  count1 = 0;     // Reseta o contador para iniciar do 0 a cada ciclo
  count2 = 0;     // Reseta o contador para iniciar do 0 a cada ciclo
  interrupts();   // Habilita as interrupcoes do Arduino
  delay(1000);    // Aguarda 1 segundo
  noInterrupts(); // Desablita interrupcoes do Arduino
    // Calcula fluxo
  flowRate_A = (count1 * 2.25 / delayMultiplier);   // Multiplica os pulsos contados por 2.25mL   (mL/s)
  flowRate_B = (count2 * 2.25 / delayMultiplier);   // Multiplica os pulsos contados por 2.25mL   (mL/s)
    // Ajusta unidades
  flowRate_A = flowRate_A * 60;   // Converte segundos para minutos (mL / Minuto)
  flowRate_B = flowRate_B * 60;   // Converte segundos para minutos (mL / Minuto)
  flowRate_A = flowRate_A / 1000; // Converte mL para litro (L / min)
  flowRate_B = flowRate_B / 1000; // Converte mL para litro (L / min)

  // Calculo de R para alocacao de polos e zeros empiricos
  rA = headA / flowRate_A;  // A partir de Ogata(1): q = h / R ---> R = h / q
  rB = headB / flowRate_B;  // A partir de Ogata(1): q = h / R ---> R = h / q
  media(rA, rB);
  rMedioA = averageA;
  rMedioB = averageB;

  pumpProportionalControl();

  imprime();
  //imprimeCSV();

  delay(1000 * delayMultiplier);
}


int leDistancia (int trigPin, int echoPin){
  long duratFCT;
  int distFCT;
  digitalWrite(trigPin, LOW);   // Limpa o trigPin
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);    // Ativa o trigPin por 10ms
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);   // Desativa o trigPin
  // Leitura do echoPin para o tempo de retorno do som emitido em microsegundos
  duratFCT = pulseIn(echoPin, HIGH);
  // Calculando a distancia de viagem da onda sonora
  distFCT = duratFCT * 343 / 1000;
          // Tempo de propagacao * Velocidade do som
          // Velocidade do som = 0.343 mm/us
  distFCT = distFCT / 2;  // Divide por 2 para considerar trecho de ida e retorno do som
  return distFCT; //em mm
}


void media (int numberA, int numberB){
  i++;
  sumA = sumA + numberA;
  averageA = sumA / i;
  sumB = sumB + numberB;
  averageB = sumB / i;
}


void flow1 (){
  count1++;
}


void flow2 (){
  count2++;
}


void pumpProportionalControl(){
  // Calcula erros
  errorA = headRef - headA;
  errorB = headRef - headB;

  /*
  // Logica motor A
  if(errorA > 255){
    speedMotorA = 255;
  }
  else if (errorA <=0){
    speedMotorA = 0;
  }
  else{
    if (errorA < (255/5)){
      speedMotorA = errorA * 5;
    }
    else{
      speedMotorA = 255;
    }
  }
  */

  speedMotorA = 255;

  /*if (digitalRead(button) == HIGH){
    speedMotorB = 255;
    digitalWrite(PIN_LED, HIGH);
  }
  else {
    speedMotorB = 0;
    digitalWrite(PIN_LED, LOW);
  }*/

  // Aciona Motores
  analogWrite(PIN_MOTORA_1,0);
  analogWrite(PIN_MOTORA_2,speedMotorA);
  analogWrite(PIN_MOTORB_1,0);
  analogWrite(PIN_MOTORB_2,speedMotorB);
}

void imprime(){
  Serial.print("Tempo: ");
  Serial.print(millis()/1000);
  Serial.print("s - Nivel A: ");
  Serial.print(headA);
  Serial.print("mm - Nivel B: ");
  Serial.print(headB);
  Serial.print("mm - Fluxo A: ");
  Serial.print(flowRate_A);
  Serial.print("L/min - Fluxo B: ");
  Serial.print(flowRate_B);
  Serial.println("L/min");

  /*Serial.print("R_A: ");
  Serial.print(rA);
  Serial.print(" - R_A_med: ");
  Serial.print(rMedioA);
  Serial.print(" - R_B: ");
  Serial.print(rB);
  Serial.print(" - R_B_med: ");
  Serial.println(rMedioB);
  */

  Serial.print("ErroA: ");
  Serial.print(errorA);
  Serial.print(" - speedMotorA: ");
  Serial.print(speedMotorA);
  Serial.print(" - ErroB: ");
  Serial.print(errorB);
  Serial.print(" - speedMotorB: ");
  Serial.println(speedMotorB);
  Serial.println("");
  
}


void imprimeCSV (){
  Serial.print(millis()/1000);
  Serial.print(",");
  Serial.print(headA);
  Serial.print(",");
  Serial.print(headB);
  Serial.print(",");
  Serial.print(flowRate_A);
  Serial.print(",");
  Serial.print(flowRate_B);
  Serial.print(",");

  Serial.print(rA);
  Serial.print(",");
  Serial.print(rMedioA);
  Serial.print(",");
  Serial.print(rB);
  Serial.print(",");
  Serial.println(rMedioB);
}
