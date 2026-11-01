# AlashWeb

[Русский](#русский) | [Қазақша](#қазақша) | [English](#english)

---

## Русский

Простая библиотека, которая делает из **ESP32 или ESP8266 веб-панель управления**: `web.addSwitch("led", "Свет", функция)`, кнопки, **ползунки**, **живые значения с графиком**. Откройте адрес платы на телефоне или компьютере: ничего устанавливать не нужно. Страница удобна на телефоне, имеет тёмную тему и необязательный пароль. Работает на встроенном `WebServer`: **AsyncWebServer не нужен**. Сделана для уроков и наборов Alash Electronics.

```cpp
#include <AlashWeb.h>
AlashWeb web;

void setup() {
  Serial.begin(115200);
  WiFi.begin("СЕТЬ", "ПАРОЛЬ");
  while (WiFi.status() != WL_CONNECTED) delay(300);
  web.title("Моя панель");
  web.addSwitch("led", "Светодиод", [](bool on) { digitalWrite(2, on); });
  web.addValue("temp", "Температура", "C", true);   // true: график
  web.begin();
  Serial.println(web.url());                         // откройте адрес / open this address
}
void loop() {
  web.update();
  web.setValue("temp", 23.5);
}
```

### Что умеет
- **Элементы в одну строку:** `addSwitch`, `addButton`, `addSlider(id, подпись, мин, макс, функция, начальное)`, `addValue(id, подпись, единица, график)`, `addText`.
- **Страница сама:** адрес `web.url()` открывается на любом телефоне и компьютере в той же сети, значения обновляются раз в секунду, график последней минуты, тёмная тема, крупные кнопки.
- **Ваши функции вызываются при нажатии:** `[](bool on){ digitalWrite(2, on); }`, а значение ползунка зажимается в диапазоне уже на плате.
- **Обновление из программы:** `setValue`, `setText`, `setSwitch`, `setSlider`; `getSwitch`, `getSlider`.
- **Пароль:** `setPassword("1234", "логин")` (HTTP Basic, без шифрования: для домашней сети).
- **Нет связи:** страница показывает красную плашку «Нет связи» на выбранном языке и не зависает.
- **Понятные ошибки** на трёх языках: неверный идентификатор, слишком много элементов, нет Wi-Fi; `diagnose(Serial)` печатает адрес и подсказки.
- Для **ESP32 и ESP8266**, без AsyncWebServer и без файловой системы.

### Чем отличается от других библиотек
Мы посмотрели ESPAsyncWebServer, ESP-DASH, Blynk-подобные примеры и «веб-сервер за 100 строк» из курсов:
- ESPAsyncWebServer требует ещё AsyncTCP, и эти библиотеки часто не обновляются; у нас встроенный `WebServer`.
- В примерах HTML страницы вставлен в скетч строкой на сотню строк, и каждое изменение требует править JavaScript; у нас страница строится из `addSwitch(...)`.
- ESP-DASH требует AsyncWebServer и ArduinoJson; у нас нет зависимостей.
- Нет пароля, графика и подсказок, почему страница не открывается; у нас есть.

### Примеры
`Basic_ESP`, `Dashboard_ESP`, `RobotPanel_ESP`, `Diagnose_ESP`

### Тесты
Идентификаторы и лимит элементов, точные JSON описания интерфейса и состояния (числа без хвоста нулей, null, кириллица и кавычки), команды кнопки, переключателя и ползунка с зажимом значения, неизвестный идентификатор, значение только для чтения, пароль на всех адресах, диагностика проверены без сети на имитации WebServer и WiFi: `extras/test`. JavaScript страницы проверен Node.js на имитации браузера (построение элементов, подстановка значений, правильные запросы `/set`): `extras/test/page`.

Подробное руководство: [docs/WIKI.md](docs/WIKI.md).

---

## Қазақша

**ESP32 не ESP8266-ны веб-басқару панеліне** айналдыратын қарапайым кітапхана: `web.addSwitch("led", "Жарық", функция)`, түймелер, **жүгірткілер**, **графигі бар тірі мәндер**. Тақта мекенжайын телефонда не компьютерде ашыңыз: ештеңе орнатудың қажеті жоқ. Қараңғы тақырып пен міндетті емес құпиясөз бар. Кірістірілген `WebServer`: **AsyncWebServer қажет емес**.

```cpp
#include <AlashWeb.h>
AlashWeb web;

void setup() {
  Serial.begin(115200);
  WiFi.begin("СЕТЬ", "ПАРОЛЬ");
  while (WiFi.status() != WL_CONNECTED) delay(300);
  web.title("Моя панель");
  web.addSwitch("led", "Светодиод", [](bool on) { digitalWrite(2, on); });
  web.addValue("temp", "Температура", "C", true);   // true: график
  web.begin();
  Serial.println(web.url());                         // откройте адрес / open this address
}
void loop() {
  web.update();
  web.setValue("temp", 23.5);
}
```

### Мүмкіндіктері
- **Элементтер бір жолмен:** `addSwitch`, `addButton`, `addSlider`, `addValue`, `addText`.
- **Бет өзі жасалады:** `web.url()` мекенжайы кез келген телефонда ашылады, мәндер секунд сайын жаңарады, соңғы минут графигі, қараңғы тақырып.
- **Функцияларыңыз шақырылады.**
- **Бағдарламадан жаңарту:** `setValue`, `setText`, `setSwitch`, `setSlider`.
- **Құпиясөз:** `setPassword("1234", "логин")`.
- **Байланыс жоқ** жазбасы.
- **Түсінікті қателер** үш тілде.
- **ESP32 және ESP8266**.

### Басқа кітапханалардан айырмашылығы
ESPAsyncWebServer, ESP-DASH және курстағы «100 жолдағы веб-сервер» мысалдарын қарадық:
- ESPAsyncWebServer-ге AsyncTCP керек; бізде кірістірілген `WebServer`.
- Мысалдарда HTML жүз жолдық жолмен; бізде `addSwitch(...)`.
- ESP-DASH-ке AsyncWebServer мен ArduinoJson керек; бізде тәуелділік жоқ.
- Құпиясөз, график және кеңестер бізде бар.

### Мысалдар
`Basic_ESP`, `Dashboard_ESP`, `RobotPanel_ESP`, `Diagnose_ESP`

### Тесттер
Идентификаторлар, JSON, команда түймелері, жүгірткі, құпиясөз, диагностика тексерілген: `extras/test`. Бет JavaScript-і Node.js-пен тексерілген.

Толық нұсқаулық: [docs/WIKI.md](docs/WIKI.md).

---

## English

A simple library that turns an **ESP32 or ESP8266 into a web control panel**: `web.addSwitch("led", "Light", function)`, buttons, **sliders**, **live values with a chart**. Open the board address on a phone or computer: nothing to install. The page is phone-friendly, has a dark theme and an optional password. It runs on the built-in `WebServer`: **no AsyncWebServer needed**. Made for classes and Alash Electronics kits.

```cpp
#include <AlashWeb.h>
AlashWeb web;

void setup() {
  Serial.begin(115200);
  WiFi.begin("СЕТЬ", "ПАРОЛЬ");
  while (WiFi.status() != WL_CONNECTED) delay(300);
  web.title("Моя панель");
  web.addSwitch("led", "Светодиод", [](bool on) { digitalWrite(2, on); });
  web.addValue("temp", "Температура", "C", true);   // true: график
  web.begin();
  Serial.println(web.url());                         // откройте адрес / open this address
}
void loop() {
  web.update();
  web.setValue("temp", 23.5);
}
```

### Features
- **Widgets in one line:** `addSwitch`, `addButton`, `addSlider(id, label, min, max, function, initial)`, `addValue(id, label, unit, chart)`, `addText`.
- **The page builds itself:** the `web.url()` address opens on any phone or computer on the same network, values refresh every second, a chart of the last minute, a dark theme, big buttons.
- **Your functions are called on a press:** `[](bool on){ digitalWrite(2, on); }`, and a slider value is clamped to its range on the board.
- **Update from the program:** `setValue`, `setText`, `setSwitch`, `setSlider`; `getSwitch`, `getSlider`.
- **Password:** `setPassword("1234", "login")` (HTTP Basic, not encrypted: for a home network).
- **Offline:** the page shows a red "No connection" bar in the chosen language and does not freeze.
- **Clear errors** in three languages: a bad id, too many widgets, no Wi-Fi; `diagnose(Serial)` prints the address and tips.
- For **ESP32 and ESP8266**, with no AsyncWebServer and no file system.

### How it differs from other libraries
We looked at ESPAsyncWebServer, ESP-DASH and the "web server in 100 lines" examples from courses:
- ESPAsyncWebServer also needs AsyncTCP, and these libraries are often not updated; we use the built-in `WebServer`.
- In the examples the page HTML is pasted into the sketch as a hundred-line string and every change means editing JavaScript; here the page is built from `addSwitch(...)`.
- ESP-DASH needs AsyncWebServer and ArduinoJson; we have no dependencies.
- No password, no chart and no tips about why the page does not open; we have all three.

### Examples
`Basic_ESP`, `Dashboard_ESP`, `RobotPanel_ESP`, `Diagnose_ESP`

### Tests
Ids and the widget limit, the exact interface and state JSON (numbers without trailing zeros, null, Cyrillic and quotes), button, switch and slider commands with value clamping, an unknown id, a read-only value, the password on every address and diagnostics are checked without a network on a WebServer and WiFi simulator: `extras/test`. The page JavaScript is checked with Node.js on a browser simulator (building widgets, showing values, correct `/set` requests): `extras/test/page`.

Full guide: [docs/WIKI.md](docs/WIKI.md).

---

MIT License, Alash Electronics.
