// [ru] AlashWeb: самый простой пример. Веб-страница с переключателем светодиода. Откройте адрес из монитора порта на телефоне или компьютере
//      (устройство должно быть в той же сети Wi-Fi, что и плата). Wi-Fi только 2.4 ГГц.
// [kk] AlashWeb: ең қарапайым мысал. Жарықдиод ауыстырғышы бар веб-бет. Порт мониторындағы мекенжайды телефонда немесе компьютерде ашыңыз
//      (құрылғы тақтамен бір Wi-Fi желісінде болуы керек). Тек 2.4 ГГц Wi-Fi.
// [en] AlashWeb: the simplest example. A web page with an LED switch. Open the address from the serial monitor on a phone or computer
//      (the device must be on the same Wi-Fi network as the board). 2.4 GHz Wi-Fi only.
#include <AlashWeb.h>

const char *WIFI_NAME = "ИМЯ_СЕТИ";
const char *WIFI_PASS = "ПАРОЛЬ";
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

AlashWeb web;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.begin(WIFI_NAME, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) delay(300);

  web.title("Моя панель");
  web.addSwitch("led", "Светодиод", [](bool on) { digitalWrite(LED_BUILTIN, on); });   // функция вызывается при нажатии
  web.begin();                                         // запустить сервер после добавления элементов
  Serial.println(web.url());                           // например http://192.168.1.50/
}

void loop() {
  web.update();                                        // обслуживать страницу
}
