#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// === DATOS DE CONEXIÓN ===
const char* ssid = "NOMBRE DE TU RED DE INTERNET";
const char* password = "CONTRASEÑA DE TU RED DE INTERNET";
const char* botToken = "TOKEN DE TU BOT DE TELEGRAM";  // Tu token de @BotFather

WiFiClientSecure secured_client;
UniversalTelegramBot bot(botToken, secured_client);

// === PIN DEL RELÉ ===
const int pinRele = 5;
bool estadoRele = false;

// === TIEMPO ENTRE CONSULTAS (en ms) ===
unsigned long lastTimeBotRan;
const unsigned long intervaloBot = 5000;

void setup() {
  Serial.begin(9600);
  pinMode(pinRele, OUTPUT);
  digitalWrite(pinRele, HIGH);  // Relé apagado al inicio (activo en LOW)

  // Conectar a WiFi
  Serial.print("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");

  // Configurar cliente seguro sin certificado
  secured_client.setInsecure();
}

void loop() {
  if (millis() - lastTimeBotRan > intervaloBot) {
    int numMensajes = bot.getUpdates(bot.last_message_received + 1);

    while (numMensajes) {
      Serial.println("Nuevo mensaje recibido");
      for (int i = 0; i < numMensajes; i++) {
        String texto = bot.messages[i].text;
        String chat_id = bot.messages[i].chat_id;

        if (texto == "/encender") {
          digitalWrite(pinRele, LOW);  // Encender (activo en LOW)
          estadoRele = true;
          bot.sendMessage(chat_id, "🔌 Foco encendido", "");
        }
        else if (texto == "/apagar") {
          digitalWrite(pinRele, HIGH);  // Apagar
          estadoRele = false;
          bot.sendMessage(chat_id, "💤 Foco apagado", "");
        }
        else {
         bot.sendMessage(chat_id, "❌ Comando incorrecto. Usa /encender o /apagar.", "");

        }

      }
      numMensajes = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTimeBotRan = millis();
  }
}
