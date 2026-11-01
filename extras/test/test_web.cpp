// Тесты AlashWeb без сети: имитация WebServer и WiFi (как на ESP32); запросы страницы подаются через очередь.
//   g++ -std=c++17 -Wall -DESP32 -I mock -I ../../src test_web.cpp ../../src/AlashWeb.cpp && ./a.out
#include <stdio.h>
#include "Arduino.h"
#include "WiFi.h"
#include "WebServer.h"
#include "AlashWeb.h"

static int fails = 0;
#define CHECK(cond) do { if (!(cond)) { printf("FAIL line %d: %s\n", __LINE__, #cond); fails++; } } while (0)
static bool has(const std::string &h, const char *n) { return h.find(n) != std::string::npos; }

static int presses = 0; static bool lastSwitch = false; static int lastSlider = -1;
static void onBtn() { presses++; }
static void onSw(bool on) { lastSwitch = on; }
static void onSl(int v) { lastSlider = v; }

// Запрос к серверу: кладём в очередь, обрабатываем, возвращаем тело последнего ответа
static AlashWeb *gWeb = nullptr;
static HttpResp ask(const std::string &path, std::map<std::string, std::string> args = {}) {
  size_t before = g_httpResp.size();
  g_httpQueue.push_back({path, args});
  gWeb->update();
  return g_httpResp.size() > before ? g_httpResp.back() : HttpResp{0, "", "", ""};
}

int main() {
  // 1) идентификаторы и лимит элементов
  CHECK(AlashWeb::validId("led1") && AlashWeb::validId("A_b9") && !AlashWeb::validId("") && !AlashWeb::validId("1234567890123456") && !AlashWeb::validId("a b") && !AlashWeb::validId("свет") && !AlashWeb::validId(nullptr));
  WiFi = WiFiClass(); WiFi.st = WL_CONNECTED; WiFi.doneAt = 1; g_ms += 10;
  AlashWeb web; gWeb = &web;
  web.title("Робот Фобо");
  CHECK(web.addButton("horn", "Сигнал", onBtn));
  CHECK(web.addSwitch("light", "Свет", onSw, true));
  CHECK(web.addSlider("speed", "Скорость", 0, 100, onSl, 30));
  CHECK(web.addValue("temp", "Температура", "C", true));
  CHECK(web.addText("status", "Статус"));
  CHECK(web.widgetCount() == 5);
  CHECK(!web.addButton("horn", "Дубль", onBtn) && web.error() == AlashWeb::ERR_BAD_ID);                 // занятый идентификатор
  CHECK(!web.addButton("bad id", "x", onBtn) && web.error() == AlashWeb::ERR_BAD_ID);
  AlashWeb many;
  for (int i = 0; i < ALASH_WEB_MAX_WIDGETS; i++) { char id[8]; snprintf(id, sizeof id, "w%d", i); CHECK(many.addText(id, "x")); }
  CHECK(!many.addText("extra", "x") && many.error() == AlashWeb::ERR_TOO_MANY);

  web.begin();
  CHECK(web.error() == AlashWeb::ERR_OK);

  // 2) страница и описание интерфейса
  HttpResp r = ask("/");
  CHECK(r.code == 200 && has(r.type, "text/html") && has(r.body, "<!doctype html>") && has(r.body, "J('/ui')") && has(r.body, "/state") && has(r.body, "prefers-color-scheme:dark"));
  r = ask("/ui");
  CHECK(r.code == 200 && has(r.type, "application/json"));
  CHECK(r.body == "{\"title\":\"Робот Фобо\",\"offline\":\"Нет связи\",\"widgets\":["
                  "{\"id\":\"horn\",\"type\":\"button\",\"label\":\"Сигнал\",\"min\":0,\"max\":1,\"unit\":\"\",\"chart\":false},"
                  "{\"id\":\"light\",\"type\":\"switch\",\"label\":\"Свет\",\"min\":0,\"max\":1,\"unit\":\"\",\"chart\":false},"
                  "{\"id\":\"speed\",\"type\":\"slider\",\"label\":\"Скорость\",\"min\":0,\"max\":100,\"unit\":\"\",\"chart\":false},"
                  "{\"id\":\"temp\",\"type\":\"value\",\"label\":\"Температура\",\"min\":0,\"max\":1,\"unit\":\"C\",\"chart\":true},"
                  "{\"id\":\"status\",\"type\":\"text\",\"label\":\"Статус\",\"min\":0,\"max\":1,\"unit\":\"\",\"chart\":false}]}");

  // 3) состояние: начальные значения, число без хвоста нулей, null для NAN, строка
  r = ask("/state"); CHECK(r.body == "{\"light\":1,\"speed\":30,\"temp\":null,\"status\":\"\"}");
  web.setValue("temp", 23.5f); web.setText("status", "Едем \"вперёд\"");
  r = ask("/state"); CHECK(r.body == "{\"light\":1,\"speed\":30,\"temp\":23.5,\"status\":\"Едем \\\"вперёд\\\"\"}");
  web.setValue("temp", 7.0f); CHECK(ask("/state").body.find("\"temp\":7,") != std::string::npos);
  web.setValue("temp", -0.125f); CHECK(ask("/state").body.find("\"temp\":-0.13,") != std::string::npos || ask("/state").body.find("\"temp\":-0.12,") != std::string::npos);

  // 4) команды со страницы: кнопка, переключатель, ползунок (с ограничением), ответ содержит новое состояние
  r = ask("/set", {{"id", "horn"}, {"v", "1"}}); CHECK(presses == 1 && r.code == 200);
  r = ask("/set", {{"id", "light"}, {"v", "0"}}); CHECK(!lastSwitch && !web.getSwitch("light") && has(r.body, "\"light\":0"));
  r = ask("/set", {{"id", "light"}, {"v", "1"}}); CHECK(lastSwitch && web.getSwitch("light"));
  r = ask("/set", {{"id", "speed"}, {"v", "75"}}); CHECK(lastSlider == 75 && web.getSlider("speed") == 75 && has(r.body, "\"speed\":75"));
  r = ask("/set", {{"id", "speed"}, {"v", "999"}}); CHECK(lastSlider == 100 && web.getSlider("speed") == 100);        // выше предела: зажимается
  r = ask("/set", {{"id", "speed"}, {"v", "-5"}}); CHECK(lastSlider == 0);
  r = ask("/set", {{"id", "nope"}, {"v", "1"}}); CHECK(r.code == 404 && has(r.body, "no such id"));
  r = ask("/set", {{"v", "1"}}); CHECK(r.code == 404);
  int before = presses; ask("/set", {{"id", "temp"}, {"v", "5"}}); CHECK(presses == before);                       // значение только для чтения не меняется командой
  r = ask("/missing"); CHECK(r.code == 404);

  // 5) значения из программы
  CHECK(web.setSwitch("light", false) && !web.getSwitch("light") && web.setSlider("speed", 40) && web.getSlider("speed") == 40);
  CHECK(web.setSlider("speed", 500) && web.getSlider("speed") == 100);
  CHECK(!web.setValue("light", 1) && web.error() == AlashWeb::ERR_NO_SUCH);                                       // не тот тип
  CHECK(!web.setText("nope", "x") && !web.setSwitch("temp", true));

  // 6) пароль: без него 401, с неверным 401, с верным пускает; на странице, в состоянии и на /set
  web.setPassword("secret", "kid");
  r = ask("/"); CHECK(r.code == 401);
  r = ask("/state", {{"__auth", "kid:wrong"}}); CHECK(r.code == 401);
  r = ask("/state", {{"__auth", "kid:secret"}}); CHECK(r.code == 200);
  int p0 = presses; r = ask("/set", {{"id", "horn"}, {"v", "1"}}); CHECK(r.code == 401 && presses == p0);          // без пароля команда не выполняется
  r = ask("/set", {{"id", "horn"}, {"v", "1"}, {"__auth", "kid:secret"}}); CHECK(presses == p0 + 1);
  r = ask("/ui"); CHECK(r.code == 401);

  // 7) адрес и диагностика; без Wi-Fi
  CHECK(web.url() == String("http://192.168.1.50/"));
  Print o; web.diagnose(o); CHECK(has(o.log, "http://192.168.1.50/") && has(o.log, "5 / 16") && has(o.log, "Сервер запущен"));
  WiFiClass off; WiFi = off; AlashWeb w2; w2.begin(); CHECK(w2.error() == AlashWeb::ERR_NOT_CONNECTED);
  w2.setLanguage(AlashWeb::LANG_KK); CHECK(strlen((const char *)w2.errorText()) > 10);

  // 8) экранирование
  char e[32]; AlashWeb::jsonEscape("a\"b\\c\nd", e, sizeof e); CHECK(strcmp(e, "a\\\"b\\\\c\\nd") == 0);
  AlashWeb::jsonEscape("Свет", e, sizeof e); CHECK(strcmp(e, "Свет") == 0);

  if (fails) { printf("FAILED: %d\n", fails); return 1; }
  printf("ALL TESTS PASSED\n");
  return 0;
}
