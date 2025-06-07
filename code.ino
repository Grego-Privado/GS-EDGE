// Bibliotecas necessárias
#include <Wire.h>                // Comunicação I2C
#include <LiquidCrystal_I2C.h>   // LCD com interface I2C
#include <RTClib.h>              // Relógio de tempo real (RTC)
#include <EEPROM.h>              // Memória EEPROM para salvar eventos

// Instanciação dos objetos
LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço 0x27, 16 colunas x 2 linhas
RTC_DS1307 rtc;                     // Objeto do módulo RTC

// Definição dos pinos
const int trigPin = 9;
const int echoPin = 10;
const int ledVerde = 2;
const int ledAmarelo = 3;
const int ledVermelho = 4;
const int buzzer = 5;

// Variável de controle para a EEPROM
int eepromIndex = 0;

// Bytes personalizados para desenhar a logo animada no LCD
byte z0[8] = {B00000,B11111,B01111,B00011,B00001,B00000,B00000};
byte z1[8] = {B00000,B11111,B11111,B11111,B11111,B10000,B00011,B00111};
byte z2[8] = {B00000,B11111,B11111,B11111,B11111,B00111,B11111,B11111};
byte z3[8] = {B00000,B11111,B11110,B11100,B11000,B10000,B10000,B00000};
byte z4[8] = {B00000,B00000,B00001,B00011,B00111,B01111,B11111};
byte z5[8] = {B01111,B11111,B11110,B11111,B11111,B11111,B11111};
byte z6[8] = {B11111,B11110,B00000,B11111,B11111,B11111,B11111};
byte z7[8] = {B00000,B00000,B10000,B11000,B11100,B11110,B11111};

// Função que salva a data e hora na EEPROM (somente em risco ALTO)
void salvarNaEEPROM(DateTime dt) {
  EEPROM.update(eepromIndex++, dt.day());
  EEPROM.update(eepromIndex++, dt.month());
  EEPROM.update(eepromIndex++, dt.year() - 2000); // Ano abreviado
  EEPROM.update(eepromIndex++, dt.hour());
  EEPROM.update(eepromIndex++, dt.minute());
  EEPROM.update(eepromIndex++, dt.second());

  // Se alcançar o fim da EEPROM, reinicia o índice
  if (eepromIndex >= EEPROM.length() - 6) {
    eepromIndex = 0;
  }
}

void setup() {
  // Inicializa LCD e liga a luz de fundo
  lcd.begin(16, 2);
  lcd.backlight();

  // Criação dos caracteres personalizados da logo animada
  lcd.createChar(0, z0); lcd.createChar(1, z1); lcd.createChar(2, z2); lcd.createChar(3, z3);
  lcd.createChar(4, z4); lcd.createChar(5, z5); lcd.createChar(6, z6); lcd.createChar(7, z7);

  lcd.display();

  // Exibição da parte de cima do "Z"
  for (int i = 0; i < 4; i++) {
    lcd.setCursor(i, 0);
    lcd.write(byte(i));
    delay(80);
  }

  // Parte de baixo do "Z"
  lcd.setCursor(0, 1); lcd.write(byte(4)); delay(80);
  lcd.setCursor(1, 1); lcd.write(byte(5)); delay(80);
  lcd.setCursor(2, 1); lcd.write(byte(6)); delay(80);
  lcd.setCursor(3, 1); lcd.write(byte(7)); delay(80);

  // Efeito de ida e volta do "Z"
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < 12; i++) {
      lcd.scrollDisplayRight();
      delay(80);
    }
    delay(100);
    for (int i = 0; i < 12; i++) {
      lcd.scrollDisplayLeft();
      delay(80);
    }
  }

  // Escreve "etta Works" letra por letra
  lcd.print("e"); delay(100); lcd.print("t"); delay(100);
  lcd.print("t"); delay(100); lcd.print("a"); delay(100);
  lcd.print(" "); delay(100); lcd.print("W"); delay(100);
  lcd.print("o"); delay(100); lcd.print("r"); delay(100);
  lcd.print("k"); delay(100); lcd.print("s"); delay(3000);

  // Apaga os caracteres gradualmente
  for (int i = 13; i >= 0; i--) {
    lcd.setCursor(i, 1); lcd.print(" ");
    delay(100);
  }
  for (int i = 3; i >= 0; i--) {
    lcd.setCursor(i, 0); lcd.print(" ");
    delay(100);
  }


  Serial.begin(9600);

  // Define os pinos de entrada e saída
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Inicializa o RTC
  if (!rtc.begin()) {
    Serial.println("RTC não encontrado!");
    while (1); // trava o programa
  }

  // Se o RTC estiver parado, ajusta a hora para a hora da compilação
  if (!rtc.isrunning()) {
    Serial.println("RTC inativo, ajustando...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  // Gera pulso para o sensor ultrassônico
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Lê o tempo de retorno do eco
  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;

  // Pega a hora atual
  DateTime now = rtc.now();

  String risco;

  // Avaliação de risco com base na distância
  if (distance > 15) {
    risco = "BAIXO";
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, LOW);
    digitalWrite(buzzer, LOW);
  } else if (distance > 7) {
    risco = "MEDIO";
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(ledVermelho, LOW);
    digitalWrite(buzzer, LOW);
  } else {
    risco = "ALTO";
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(buzzer, HIGH);

    salvarNaEEPROM(now); // Salva evento na EEPROM
  }

  // Mostra informações no LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print(distance);
  lcd.print(" cm");

  lcd.setCursor(0, 1);
  lcd.print("Risco: ");
  lcd.print(risco);

  // Envia dados para o Serial Monitor
  Serial.print("Data: ");
  Serial.print(now.day()); Serial.print('/');
  Serial.print(now.month()); Serial.print('/');
  Serial.print(now.year()); Serial.print(" ");
  Serial.print(now.hour()); Serial.print(':');
  Serial.print(now.minute()); Serial.print(':');
  Serial.println(now.second());

  delay(1500); // Aguarda antes de nova leitura
}