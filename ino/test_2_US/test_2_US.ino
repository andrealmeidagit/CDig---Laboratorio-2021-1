/*
 *    Desenvolvido por ANDRE ABREU RODRIGUES DE ALMEIDA
 *    Matricula 12/0007100
 *    
 *    Trabalho de Controle Digital
 *    
 *    Descrição: Testa sensores ultrassonicos
 */

//Definicao de pinos

#define echoPin_US_A 2 //Define o pino D2 como ECHO do sensor ultrassonico(HC-SR04) 1
#define trigPin_US_A 3 //Define o pino D3 (PWM) como TRIG do sensor ultrassonico(HC-SR04) 1

#define echoPin_US_B 12 //Define o pino D12 como ECHO do sensor ultrassonico(HC-SR04) 2
#define trigPin_US_B 11 //Define o pino D11 (PWM) como TRIG do sensor ultrassonico(HC-SR04) 2

//Variaveis globais
int distanceFromSensorA, distanceFromSensorB; // Variavel da afericao de distancia
int sumA, sumB; // Variaveis contendo a soma das distancias
int averageA, averageB; // Variaveis que armazenam a media da distancia
int headA, headB; // Variaveis do nível de liquido
int i;  //iterador

void setup() {
  //Define entradas e saidas
  pinMode(trigPin_US_A, OUTPUT); // Define o pino de TRIG do sensor US1 como saida
  pinMode(echoPin_US_A, INPUT); // Define o pino de ECHO do sensor US1 como entrada
  pinMode(trigPin_US_B, OUTPUT); // Define o pino de TRIG do sensor US2 como saida
  pinMode(echoPin_US_B, INPUT); // Define o pino de ECHO do sensor US2 como entrada
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
}

void loop() {
  // Lê distancia do sensor ultrassonico A
  distanceFromSensorA = leDistancia(trigPin_US_A, echoPin_US_A);
  distanceFromSensorB = leDistancia(trigPin_US_B, echoPin_US_B);
  
  // Calcula tamanho do pote pelo sensor ultrassonico
  /*media(distanceFromSensorA, distanceFromSensorB);
  imprime1();  */

  // Calcula o nível de líquido
  headA = 134 - distanceFromSensorA;
  headB = 132 - distanceFromSensorB;
  media(headA, headB);
  //imprime1();
  //imprime2();
  imprime3();
  
  delay(500);

  
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

/*
 
void imprime1 (){
  Serial.print("Tempo: ");
  Serial.print(millis()/1000);
  Serial.print(" seg - Distancia A: ");
  Serial.print(distanceFromSensorA);
  Serial.print(" - Media A: ");
  Serial.print(averageA);
  Serial.print(" - Distancia B: ");
  Serial.print(distanceFromSensorB);
  Serial.print(" - Media B: ");
  Serial.println(averageB);
}

void imprime2 (){
  Serial.print("Tempo: ");
  Serial.print(millis()/1000);
  Serial.print(" seg - Nivel A: ");
  Serial.print(headA);
  Serial.print(" - Media A: ");
  Serial.print(averageA);
  Serial.print(" - Nivel B: ");
  Serial.print(headB);
  Serial.print(" - Media B: ");
  Serial.println(averageB);
  Serial.print("sumA: ");
  Serial.print(sumA);
  Serial.print(" - sumB: ");
  Serial.print(sumB);
  Serial.print(" - i: ");
  Serial.println(i);
}

*/

void imprime3(){
  Serial.print("Tempo: ");
  Serial.print(millis()/1000);
  Serial.print(" seg - Nivel A: ");
  Serial.print(headA);
  Serial.print(" - Media A: ");
  Serial.print(averageA);
  Serial.print(" - Nivel B: ");
  Serial.print(headB);
  Serial.print(" - Media B: ");
  Serial.println(averageB);
}
