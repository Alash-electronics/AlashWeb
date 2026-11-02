// [ru] AlashWeb: пульт для робота в браузере. Кнопки направлений и ползунок скорости. Здесь команды только печатаются,
//      а в своём проекте вызывайте управление моторами (например, библиотека AlashCar: car.forward(speed)).
//      Пароль защищает панель в домашней сети (передаётся без шифрования, это не защита от взлома).
// [kk] AlashWeb: браузердегі робот пульті. Бағыт батырмалары және жылдамдық жүгірткісі. Мұнда командалар тек басылады,
//      ал өз жобаңызда моторларды басқаруды шақырыңыз (мысалы, AlashCar кітапханасы: car.forward(speed)).
//      Құпиясөз үй желісіндегі тақтаны қорғайды (шифрсыз жіберіледі, бұл бұзудан қорғау емес).
// [en] AlashWeb: a robot remote in a browser. Direction buttons and a speed slider. Here the commands are only printed,
//      and in your project call the motor control (for example the AlashCar library: car.forward(speed)).
//      The password protects the panel on a home network (it is sent unencrypted, this is not protection from hacking).
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
