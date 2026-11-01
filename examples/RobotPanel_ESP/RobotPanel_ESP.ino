// AlashWeb: пульт для робота в браузере. Кнопки направлений и ползунок скорости. Здесь команды только печатаются,
// а в своём проекте вызывайте управление моторами (например, библиотека AlashCar: car.forward(speed)).
// Пароль защищает панель в домашней сети (передаётся без шифрования, это не защита от взлома).
#include <AlashWeb.h>

const char *WIFI_NAME = "ИМЯ_СЕТИ";
const char *WIFI_PASS = "ПАРОЛЬ";

AlashWeb web;
int speedNow = 60;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_NAME, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) delay(300);

  web.title("Робот Фобо");
  web.setPassword("1234", "robot");                    // логин robot, пароль 1234; без этой строки доступ открыт всем в сети
  web.addButton("fwd", "Вперёд", []() { Serial.println("вперёд"); });
  web.addButton("back", "Назад", []() { Serial.println("назад"); });
  web.addButton("left", "Влево", []() { Serial.println("влево"); });
  web.addButton("right", "Вправо", []() { Serial.println("вправо"); });
  web.addButton("stop", "Стоп", []() { Serial.println("стоп"); });
  web.addSlider("speed", "Скорость", 0, 100, [](int v) { speedNow = v; Serial.print("скорость "); Serial.println(v); }, speedNow);
  web.addValue("rssi", "Сигнал Wi-Fi", "dBm");
  web.begin();
  Serial.println(web.url());
}

void loop() {
  web.update();
  static uint32_t last = 0;
  if (millis() - last > 2000) {
    last = millis();
    web.setValue("rssi", WiFi.RSSI());
  }
}
