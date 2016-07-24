//**************************************************************
//  Nome       : Conta Conta
//  Autor      : Sergio Porto de Oliveira Hess
//  Data       : 23/07/2016
//  Modificado : --
//  Version    : 1.0
//  Notes      : Jogo para ajudar no aprendizado dos números
//**************************************************************

#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951

const int latchPinDig = 11;
const int dataPinDig = 12;
const int clockPinDig = 13;

const int botaoContador = 2;
const int ledContador = 3;

const int botaoCheck = 4;

const int ledErro = 5;
const int ledSucesso = 6;

const int buzz = 7;

const byte numeros[11] = {
  0b01111111, // 0
  0b00001101, // 1
  0b10110111, // 2
  0b10011111, // 3
  0b11001101, // 4
  0b11011011, // 5
  0b11111011, // 6
  0b00001111, // 7
  0b11111111, // 8
  0b11011111, // 9
  0b00000000  // desligado
};


const byte animaDigito[7] = {
  0b00000000, // desligado
  0b00000011,
  0b00000100,
  0b00001001,
  0b00010000,
  0b00100001,
  0b01000000
};

int toqueAcerto[] = {NOTE_E7, NOTE_G7, NOTE_A7, NOTE_A7, NOTE_A7, NOTE_B7};
int toqueAcertoTempo[] = {12, 12, 6, 6, 12, 3};

int toqueErro[] = {NOTE_B3};
int toqueErroTempo[] = {2};

int pushJogador;
int numeroAtual;
boolean apertou;

void setup() {

  Serial.begin(9600);

  randomSeed(analogRead(A5));

  pinMode(latchPinDig, OUTPUT);
  pinMode(clockPinDig, OUTPUT);
  pinMode(dataPinDig, OUTPUT);

  sendDig595(numeros[10]);

  pinMode(botaoContador, INPUT_PULLUP);
  pinMode(ledContador, OUTPUT);

  pinMode(botaoCheck, INPUT_PULLUP);

  pinMode(ledErro, OUTPUT);
  pinMode(ledSucesso, OUTPUT);

  pinMode(buzz, OUTPUT);

  animaInicio();

}

void loop() {

  pushJogador = 0;
  numeroAtual = random(0, 10);

  rodaAnimaDigito();

  while (!leCheck()) {
    sendDig595(numeros[numeroAtual]);
    apertou = leContador();
    while ( apertou ) {
      delay(100);
      if (!leContador()) {
        apertou = false;
        pushJogador++;
        Serial.print("numeroAtual = ");
        Serial.print(numeroAtual);
        Serial.print(" | pushJogador = ");
        Serial.println(pushJogador);
      }
    }
  }

  if ( pushJogador == numeroAtual ) {
    etapaAcerto();
  } else {
    etapaErro();
  }

}

void sendDig595(byte valor)
{
  digitalWrite(latchPinDig, LOW);
  shiftOut(dataPinDig, clockPinDig, MSBFIRST, valor);
  digitalWrite(latchPinDig, HIGH);
}

boolean leContador()
{
  int leitura = digitalRead(botaoContador);
  if ( leitura == 0 ) {
    digitalWrite(ledContador, LOW);
    return false;
  } else {
    digitalWrite(ledContador, HIGH);
    return true;
  }
}

boolean leCheck()
{
  int leitura = digitalRead(botaoCheck);
  if ( leitura == 0 ) {
    return false;
  } else {
    return true;
  }
}

void animaInicio()
{
  digitalWrite(ledContador, HIGH);
  digitalWrite(ledErro, HIGH);
  digitalWrite(ledSucesso, HIGH);
  delay(300);
  digitalWrite(ledContador, LOW);
  digitalWrite(ledErro, LOW);
  digitalWrite(ledSucesso, LOW);
  delay(200);
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledContador, HIGH);
    digitalWrite(ledErro, LOW);
    digitalWrite(ledSucesso, LOW);
    delay(300);
    digitalWrite(ledContador, LOW);
    digitalWrite(ledErro, HIGH);
    digitalWrite(ledSucesso, LOW);
    delay(300);
    digitalWrite(ledContador, LOW);
    digitalWrite(ledErro, LOW);
    digitalWrite(ledSucesso, HIGH);
    delay(300);
  }
  digitalWrite(ledContador, HIGH);
  digitalWrite(ledErro, HIGH);
  digitalWrite(ledSucesso, HIGH);
  delay(300);
  digitalWrite(ledContador, LOW);
  digitalWrite(ledErro, LOW);
  digitalWrite(ledSucesso, LOW);
}

void rodaAnimaDigito()
{
  for(int i = 0; i < 7; i++){
    sendDig595(animaDigito[i]);
    delay(250);
  }
}

void chamaToque(int toque[], int toqueTempo[], int tamanho, int led)
{
  for (int nota = 0; nota < tamanho; nota++) {

    int tempo = 1000 / toqueTempo[nota];

    toca(buzz, toque[nota], tempo, led);

    int pausaNota = tempo * 1.30;
    delay(pausaNota);

    toca(buzz, 0, tempo, led);
  }
}

void toca(int pino, long frequency, long length, int led)
{

  long delayValue = 1000000 / frequency / 2;
  long numCycles = frequency * length / 1000;

  for (long i = 0; i < numCycles; i++) {
    digitalWrite(pino, HIGH);
    digitalWrite(led, HIGH);
    delayMicroseconds(delayValue);
    digitalWrite(pino, LOW);
    digitalWrite(led, LOW);
    delayMicroseconds(delayValue);
  }
  digitalWrite(13, LOW);

}

void etapaAcerto()
{
  chamaToque(toqueAcerto, toqueAcertoTempo, 6, ledSucesso);
}

void etapaErro()
{
  chamaToque(toqueErro, toqueErroTempo, 1, ledErro);
}

