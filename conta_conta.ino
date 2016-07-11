//**************************************************************
//  Nome       : Conta Conta
//  Autor      : Sergio Porto de Oliveira Hess
//  Data       : 09/07/2016
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

const int latchPinDig = 11;
const int dataPinDig = 12;
const int clockPinDig = 13;
const int digito = 2;

const int latchPinLed = 8;
const int dataPinLed = 9;
const int clockPinLed = 10;

const int botaoCheck = 3;
const int botaoContador = 4;
const int ledContador = 5;

const int buzz = 14;

const int numeros[] = {126, 12, 182, 158, 204, 218, 250, 14, 254, 222};
const byte anima[] = {0b10000001, 0b01000010, 0b00100100, 0b00011000, 0b00100100, 0b01000010, 0b10000001};

const byte ledVermelho = 0b00110011;
const byte ledVerde = 0b11001100;

int pushJogador;
int numeroAtual;
boolean apertou;

int toqueAcerto[] = {NOTE_E6, NOTE_G6, NOTE_A6, NOTE_A6, NOTE_A6, NOTE_B6};
int toqueAcertoTempo[] = {12, 12, 6, 6, 12, 3};

int toqueErro[] = {NOTE_B3, NOTE_A3, NOTE_A3, NOTE_A3, NOTE_G3, NOTE_E3};
int toqueErroTempo[] = {6, 6, 12, 12, 6, 3};

void setup() {
  Serial.begin(9600);

  randomSeed(analogRead(A5));

  pinMode(latchPinDig, OUTPUT);
  pinMode(clockPinDig, OUTPUT);
  pinMode(dataPinDig, OUTPUT);
  pinMode(digito, OUTPUT);

  pinMode(latchPinLed, OUTPUT);
  pinMode(clockPinLed, OUTPUT);
  pinMode(dataPinLed, OUTPUT);

  pinMode(botaoCheck, INPUT_PULLUP);
  pinMode(botaoContador, INPUT_PULLUP);
  pinMode(ledContador, OUTPUT);

  pinMode(buzz, OUTPUT);

  sendDig595(0);

  rodaAnima();
}

void loop() {
  pushJogador = 0;
  numeroAtual = random(0, 10);

  while (!leCheck()) {
    sendDig595(numeros[numeroAtual]);
    apertou = leContador();
    while ( apertou ) {
      if (!leContador()) {
        apertou = false;
        pushJogador++;
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

void sendDig595(int valor)
{
  digitalWrite(digito, LOW);
  digitalWrite(latchPinDig, LOW);
  shiftOut(dataPinDig, clockPinDig, MSBFIRST, valor);
  digitalWrite(latchPinDig, HIGH);
}

void sendLed595(int valor)
{
  digitalWrite(digito, LOW);
  digitalWrite(latchPinLed, LOW);
  shiftOut(dataPinLed, clockPinLed, LSBFIRST, valor);
  digitalWrite(latchPinLed, HIGH);
}

void rodaAnima()
{
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 8; i++) {
      sendLed595(anima[i]);
      delay(100);
    }
  }
  sendLed595(0);
}

boolean leContador()
{
  int leitura = digitalRead(botaoContador);
  Serial.println(leitura);
  if ( leitura == 1 ) {
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

void chamaToque(int toque[], int toqueTempo[], int tamanho, byte led)
{
  for (int nota = 0; nota < tamanho; nota++) {

    int tempo = 1000 / toqueTempo[nota];

    toca(buzz, toque[nota], tempo, led);

    int pausaNota = tempo * 1.30;
    delay(pausaNota);

    toca(buzz, 0, tempo, led);
  }
}

void toca(int pino, long frequency, long length, byte led)
{

  long delayValue = 1000000 / frequency / 2;
  long numCycles = frequency * length / 1000;

  for (long i = 0; i < numCycles; i++) {
    digitalWrite(pino, HIGH);
    sendLed595(led);
    delayMicroseconds(delayValue);
    digitalWrite(pino, LOW);
    sendLed595(0);
    delayMicroseconds(delayValue);
  }
  digitalWrite(13, LOW);

}

void etapaAcerto()
{
  chamaToque(toqueAcerto, toqueAcertoTempo, 6, 0b11001100);
}

void etapaErro()
{
  chamaToque(toqueErro, toqueErroTempo, 6, 0b00110011);
}
