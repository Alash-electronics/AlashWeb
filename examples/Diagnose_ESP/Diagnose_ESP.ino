// AlashWeb: страница не открывается? Скетч покажет адрес панели, число элементов и подскажет, что проверить.
#include <AlashWeb.h>

const char *WIFI_NAME = "ИМЯ_СЕТИ";
const char *WIFI_PASS = "ПАРОЛЬ";

AlashWeb web;

void setup() {
  Serial.begin(115200);
  delay(500);
  WiFi.begin(WIFI_NAME, WIFI_PASS);
  uint32_t t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 15000) delay(300);
  web.setLanguage(AlashWeb::LANG_RU);                  // LANG_KK: қазақша, LANG_EN: English
  web.addSwitch("test", "Проверка", [](bool) {});
  web.begin();
  web.diagnose(Serial);
}

void loop() {
  web.update();
}
