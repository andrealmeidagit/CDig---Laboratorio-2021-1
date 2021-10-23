/*
 *    Desenvolvido por ANDRE ABREU RODRIGUES DE ALMEIDA
 *    Matricula 12/0007100
 *    
 *    Trabalho de Controle Digital
 *    
 *    Descrição: Mede vazão de saída dos tanques
 */

//Definicao de pinos

  // Sensores de fluxo
#define flowPin_A 2 // Define o pino D2 como pino de dados do sensor de fluxo (YF-S401) A
#define flowPin_B 3 // Define o pino D3 como pino de dados do sensor de fluxo (YF-S401) B

//Variaveis globais
double flowRate_A;  // Variavel que armazena o a vazao/fluxo do tanque A
double flowRate_B;  // Variavel que armazena o a vazao/fluxo do tanque B
volatile int count1; // Precisa ser declarado como volatil para ser calculado
                          // corretamente durante as interrupções
volatile int count2; // Precisa ser declarado como volatil para ser calculado
                          // corretamente durante as interrupções

void setup() {
  // Define entradas e saidas
  pinMode(flowRate_A, INPUT);   //Define o pino de dados do sensor de fluxo A como entrada
  pinMode(flowRate_B, INPUT);   //Define o pino de dados do sensor de fluxo B como entrada

  // Define as interrupcoes
  attachInterrupt(digitalPinToInterrupt(flowPin_A), flow1, RISING);  // Configura interrupção
                                   // do pino 2 do Arduino para executar a funcao "flow"
  attachInterrupt(digitalPinToInterrupt(flowPin_B), flow2, RISING);  // Configura interrupção
                                   // do pino 3 do Arduino para executar a funcao "flow"

  // Define frequencia de comunicacao serial
  Serial.begin(9600); // Define BAUDRATE em 9600
}

void loop() {
  // Executa interrupcoes
  count1 = 0;     // Reseta o contador para iniciar do 0 a cada ciclo
  count2 = 0;     // Reseta o contador para iniciar do 0 a cada ciclo
  interrupts();   // Habilita as interrupcoes do Arduino
  delay(1000);    // Aguarda 1 segundo
  noInterrupts(); // Desablita interrupcoes do Arduino

  // Calcula fluxo
  flowRate_A = (count1 * 2.25);   // Multiplica os pulsos contados por 2.25mL   (mL/s)
  flowRate_B = (count2 * 2.25);   // Multiplica os pulsos contados por 2.25mL   (mL/s)

  flowRate_A = flowRate_A * 60;   // Converte segundos para minutos (mL / Minuto)
  flowRate_B = flowRate_B * 60;   // Converte segundos para minutos (mL / Minuto)
  flowRate_A = flowRate_A / 1000; // Converte mL para litro (L / min)
  flowRate_B = flowRate_B / 1000; // Converte mL para litro (L / min)

  imprime();
}

void imprime(){
  Serial.print("Tempo: ");
  Serial.print(millis() / 1000);
  Serial.print("s - Fluxo A: ");
  Serial.print(flowRate_A);
  Serial.print("L/min - Fluxo B: ");
  Serial.print(flowRate_B);
  Serial.println("L/min");
  Serial.print("count1: ");
  Serial.print(count1);
  Serial.print(" - count2: ");
  Serial.println(count2);
}


void flow1 (){
  count1++;
}

void flow2 (){
  count2++;
}
