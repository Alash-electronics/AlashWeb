// [ru] AlashWeb: панель приборов. Температура с графиком за последнюю минуту, строка статуса, ползунок яркости и кнопка.
//      Датчик температуры здесь имитирован: подставьте настоящий (AlashAHT, AlashDS18B20, AlashBME280).
// [kk] AlashWeb: аспаптар тақтасы. Соңғы минут графигі бар температура, күй жолы, жарықтық жүгірткісі және батырма.
//      Мұндағы температура датчигі имитацияланған: нағыз датчикті қойыңыз (AlashAHT, AlashDS18B20, AlashBME280).
// [en] AlashWeb: a dashboard. The temperature with a chart for the last minute, a status line, a brightness slider and a button.
//      The temperature sensor here is simulated: put in a real one (AlashAHT, AlashDS18B20, AlashBME280).
#include <AlashWeb.h>

const char *WIFI_NAME = "ИМЯ_СЕТИ";
const char *WIFI_PASS = "ПАРОЛЬ";
const uint8_t LED_PIN = 4;                             // светодиод с резистором на этом пине (яркость через ШИМ)

AlashWeb web;
uint32_t last = 0;
int presses = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_NAME, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) delay(300);

#if defined(ESP32)
  ledcAttach(LED_PIN, 5000, 8);                        // ESP32 core 3.x: пин, частота, разрядность
#else
  pinMode(LED_PIN, OUTPUT);
#endif
  web.title("Лаборатория");
  web.addValue("temp", "Температура", "C", true);      // true: рисовать график
  web.addText("status", "Статус");
  web.addSlider("bright", "Яркость", 0, 255, [](int v) {
#if defined(ESP32)
    ledcWrite(LED_PIN, v);
#else
    analogWrite(LED_PIN, v);
#endif
  }, 0);
  web.addButton("count", "Нажми меня", []() { presses++; });
  web.begin();
  Serial.println(web.url());
}

void loop() {
  web.update();
  if (millis() - last > 1000) {
    last = millis();
    web.setValue("temp", 22.0 + 3.0 * sin(millis() / 5000.0));          // подставьте настоящее значение
    char text[40];
    snprintf(text, sizeof text, "Нажатий: %d, работаем %lu с", presses, (unsigned long)(millis() / 1000));
    web.setText("status", text);
  }
}
