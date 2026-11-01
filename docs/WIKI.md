# AlashWeb: руководство / нұсқаулық / guide

[Русский](#русский) | [Қазақша](#қазақша) | [English](#english)

---

## Русский

Простая библиотека, которая делает из **ESP32 или ESP8266 веб-панель управления**: `web.addSwitch("led", "Свет", функция)`, кнопки, **ползунки**, **живые значения с графиком**. Откройте адрес платы на телефоне или компьютере: ничего устанавливать не нужно. Страница удобна на телефоне, имеет тёмную тему и необязательный пароль. Работает на встроенном `WebServer`: **AsyncWebServer не нужен**. Сделана для уроков и наборов Alash Electronics.

### Установка
Arduino IDE: Скетч, Подключить библиотеку, Управлять библиотеками, найти **AlashWeb** и нажать Установить. Или скачайте ZIP с GitHub: Скетч, Подключить библиотеку, Добавить .ZIP библиотеку.

### Платы
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 и другие, где работает Arduino-ядро.

### Быстрый старт
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

### Справочник по методам
| Метод | Что делает |
|---|---|
| `AlashWeb(port)` | Панель на порту (80) |
| `title(s) / begin() / update()` | Заголовок, запуск, обслуживание в loop() |
| `addButton(id, label, fn)` | Кнопка |
| `addSwitch(id, label, fn, initial)` | Переключатель |
| `addSlider(id, label, min, max, fn, initial)` | Ползунок |
| `addValue(id, label, unit, chart)` | Значение с графиком |
| `addText(id, label) / setText(id, s)` | Строка текста |
| `setValue / setSwitch / setSlider / getSwitch / getSlider` | Значения из программы |
| `setPassword(pass, user) / url()` | Пароль и адрес |
| `errorText() / diagnose(Serial)` | Причина / самопроверка |

### Если что-то не работает
**Страница не открывается**

Телефон должен быть в той же сети Wi-Fi, что и плата; мобильный интернет не подходит. Адрес печатается в монитор порта: `Serial.println(web.url())`. Проверьте, что в `loop()` вызывается `web.update()`. Запустите Diagnose_ESP.

**Страница пустая или «Нет связи»**

Элементы нужно добавить до `web.begin()`. Если плата перезагрузилась, обновите страницу. Слишком долгий `delay()` в `loop()` тоже даёт «Нет связи»: не блокируйте цикл.

**Пароль не защищает от взлома**

Логин и пароль передаются без шифрования (HTTP Basic). Они останавливают случайных гостей в домашней сети, но не защита для интернета: не выводите панель наружу.

### Самопроверка
В библиотеке есть `diagnose(Serial)`: она печатает, что видит плата, и подсказывает, что проверить. Язык подсказок: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Примеры
`Basic_ESP`, `Dashboard_ESP`, `RobotPanel_ESP`, `Diagnose_ESP`

---

## Қазақша

**ESP32 не ESP8266-ны веб-басқару панеліне** айналдыратын қарапайым кітапхана: `web.addSwitch("led", "Жарық", функция)`, түймелер, **жүгірткілер**, **графигі бар тірі мәндер**. Тақта мекенжайын телефонда не компьютерде ашыңыз: ештеңе орнатудың қажеті жоқ. Қараңғы тақырып пен міндетті емес құпиясөз бар. Кірістірілген `WebServer`: **AsyncWebServer қажет емес**.

### Орнату
Arduino IDE: Sketch, Include Library, Manage Libraries, **AlashWeb** табып, Install басыңыз. Немесе GitHub-тан ZIP жүктеп, Sketch, Include Library, Add .ZIP Library.

### Тақталар
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 және Arduino ядросы бар басқалар.

### Жылдам бастау
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

### Әдістер анықтамасы
| Әдіс | Не істейді |
|---|---|
| `AlashWeb(port)` | Портта панель |
| `title(s) / begin() / update()` | Тақырып, іске қосу, loop() |
| `addButton(id, label, fn)` | Түйме |
| `addSwitch(id, label, fn, initial)` | Ауыстырғыш |
| `addSlider(id, label, min, max, fn, initial)` | Жүгірткі |
| `addValue(id, label, unit, chart)` | Графигі бар мән |
| `addText(id, label) / setText(id, s)` | Мәтін жолы |
| `setValue / setSwitch / setSlider / getSwitch / getSlider` | Бағдарламадан мәндер |
| `setPassword(pass, user) / url()` | Құпиясөз және мекенжай |
| `errorText() / diagnose(Serial)` | Себеп / өзін тексеру |

### Бірдеңе жұмыс істемесе
**Бет ашылмайды**

Телефон тақтамен бір Wi-Fi желісінде болуы керек. Мекенжай монитор портына басылады. loop() ішінде web.update() шақырылуы керек.

**Бет бос не «Байланыс жоқ»**

Элементтер web.begin() алдында қосылады. loop() ішінде ұзақ delay() болмасын.

**Құпиясөз бұзудан қорғамайды**

Логин мен құпиясөз шифрланбай жіберіледі. Панельді интернетке шығармаңыз.

### Өзін-өзі тексеру
Кітапханада `diagnose(Serial)` бар: тақта не көргенін басып шығарады және нені тексеру керегін айтады. Кеңес тілі: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Мысалдар
`Basic_ESP`, `Dashboard_ESP`, `RobotPanel_ESP`, `Diagnose_ESP`

---

## English

A simple library that turns an **ESP32 or ESP8266 into a web control panel**: `web.addSwitch("led", "Light", function)`, buttons, **sliders**, **live values with a chart**. Open the board address on a phone or computer: nothing to install. The page is phone-friendly, has a dark theme and an optional password. It runs on the built-in `WebServer`: **no AsyncWebServer needed**. Made for classes and Alash Electronics kits.

### Installation
Arduino IDE: Sketch, Include Library, Manage Libraries, search **AlashWeb** and click Install. Or download the ZIP from GitHub: Sketch, Include Library, Add .ZIP Library.

### Boards
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 and others with an Arduino core.

### Quick start
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

### Method reference
| Method | What it does |
|---|---|
| `AlashWeb(port)` | A panel on a port (80) |
| `title(s) / begin() / update()` | Title, start, serving in loop() |
| `addButton(id, label, fn)` | A button |
| `addSwitch(id, label, fn, initial)` | A switch |
| `addSlider(id, label, min, max, fn, initial)` | A slider |
| `addValue(id, label, unit, chart)` | A value with a chart |
| `addText(id, label) / setText(id, s)` | A line of text |
| `setValue / setSwitch / setSlider / getSwitch / getSlider` | Values from the program |
| `setPassword(pass, user) / url()` | Password and address |
| `errorText() / diagnose(Serial)` | Reason / self-check |

### If something does not work
**The page does not open**

The phone must be on the same Wi-Fi network as the board; mobile data does not work. The address is printed to the serial monitor: `Serial.println(web.url())`. Make sure `loop()` calls `web.update()`. Run Diagnose_ESP.

**The page is empty or says "No connection"**

Add widgets before `web.begin()`. If the board rebooted, refresh the page. A long `delay()` in `loop()` also gives "No connection": do not block the loop.

**The password is not real security**

The login and password travel unencrypted (HTTP Basic). They stop casual guests on a home network but are no protection for the internet: do not expose the panel outside.

### Self-check
The library has `diagnose(Serial)`: it prints what the board sees and tells what to check. Tips language: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Examples
`Basic_ESP`, `Dashboard_ESP`, `RobotPanel_ESP`, `Diagnose_ESP`

---

MIT License, Alash Electronics. https://github.com/Alash-electronics/AlashWeb
