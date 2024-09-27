#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

#define LED_VERMELHO 2
#define LED_AMARELO 3
#define LED_VERDE 4

MFRC522 mfrc522(SS_PIN, RST_PIN);
String storedUID = "";

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  digitalWrite(LED_AMARELO, HIGH);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);
}

void loop() {
  Serial.println("MSG: Aproxime o cartao...");
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
      String currentUID = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        currentUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
        currentUID += String(mfrc522.uid.uidByte[i], HEX);
      }

      currentUID.toUpperCase();

      if (storedUID == "") {
        storedUID = currentUID;
        Serial.println("MSG: Aguarde...");
        sendUID(storedUID);
        String ledCommand = "";
        delay(1000);
        while (Serial.available() == 0) {}
        ledCommand = Serial.readStringUntil('\n');
        Serial.println(ledCommand);

        if (ledCommand == "green") {
          piscarLed(LED_VERDE, 3);
          Serial.println("MSG: Acesso Liberado");
        } else {
          piscarLed(LED_VERMELHO, 3);
          Serial.println("MSG: Acesso Negado");
        }

        mfrc522.PICC_HaltA();

        delay(1000);
        storedUID = "";
      }
}

void piscarLed(int ledPin, int vezes) {
  digitalWrite(LED_AMARELO, LOW);
  for (int i = 0; i < vezes; i++) {
    digitalWrite(ledPin, HIGH);
    delay(250);
    digitalWrite(ledPin, LOW);
    delay(250);
  }
  digitalWrite(LED_AMARELO, HIGH);
}

void sendUID(String uid) {
  Serial.print("MAIN_UID:");
  Serial.println(uid);
}