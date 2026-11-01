// AlashWeb: веб-панель управления на ESP32 и ESP8266 в несколько строк: кнопки, переключатели, ползунки, живые значения с графиком.
// Страница открывается с телефона и компьютера, ничего не нужно устанавливать. Без AsyncWebServer. MIT License. Alash Electronics.
#ifndef ALASH_WEB_H
#define ALASH_WEB_H

#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#include <WebServer.h>
typedef WebServer AlashHttpServer;
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
typedef ESP8266WebServer AlashHttpServer;
#endif

#ifndef ALASH_WEB_MAX_WIDGETS
#define ALASH_WEB_MAX_WIDGETS 16
#endif

class AlashWeb {
 public:
  typedef void (*ButtonCallback)();
  typedef void (*SwitchCallback)(bool on);
  typedef void (*SliderCallback)(int value);
  enum Error : uint8_t {
    ERR_OK = 0,
    ERR_TOO_MANY,        // больше элементов, чем ALASH_WEB_MAX_WIDGETS
    ERR_BAD_ID,          // идентификатор пустой, длиннее 15 символов, с недопустимыми символами или уже занят
    ERR_NO_SUCH,         // элемента с таким идентификатором нет
    ERR_NOT_CONNECTED    // нет подключения к Wi-Fi: адрес панели неизвестен
  };
  enum Language : uint8_t { LANG_RU = 0, LANG_KK = 1, LANG_EN = 2 };

  explicit AlashWeb(uint16_t port = 80) : _server(port) {}

  // ---- Запуск --------------------------------------------------------------
  void title(const char *text) { strncpy(_title, text, sizeof(_title) - 1); }
  void begin();                              // запустить сервер (после подключения к Wi-Fi, после добавления элементов)
  void update() { _server.handleClient(); }  // вызывайте в loop()
  void setPassword(const char *password, const char *user = "admin");   // защита паролем (HTTP Basic); без шифрования, для домашней сети
  String url() const;                        // адрес панели, например http://192.168.1.50/

  // ---- Элементы (добавлять до begin()) ----------------------------------------------
  // id: короткое имя из латинских букв, цифр и _ (до 15 символов); label: подпись на странице (можно по-русски)
  bool addButton(const char *id, const char *label, ButtonCallback cb);
  bool addSwitch(const char *id, const char *label, SwitchCallback cb, bool initial = false);
  bool addSlider(const char *id, const char *label, int minValue, int maxValue, SliderCallback cb, int initial = 0);
  bool addValue(const char *id, const char *label, const char *unit = "", bool chart = false);   // число с единицей; chart: рисовать график за последнюю минуту
  bool addText(const char *id, const char *label);                                              // строка текста (статус)

  // ---- Обновление значений из программы -------------------------------------------------
  bool setValue(const char *id, float value);
  bool setText(const char *id, const char *text);
  bool setSwitch(const char *id, bool on);
  bool setSlider(const char *id, int value);
  bool getSwitch(const char *id);
  int getSlider(const char *id);

  // ---- Диагностика -----------------------------------------------------------------
  Error error() const { return _err; }
  void setLanguage(Language lang) { _lang = lang; }
  const __FlashStringHelper *errorText();
  void diagnose(Print &out);
  uint8_t widgetCount() const { return _n; }

  // Построение JSON (static: проверяется тестами без сети)
  static size_t jsonEscape(const char *in, char *out, size_t size);
  static bool validId(const char *id);

 private:
  enum Type : uint8_t { BUTTON, SWITCH, SLIDER, VALUE, TEXT };
  struct Widget {
    char id[16];
    char label[40];
    char unit[8];
    Type type;
    int minV, maxV;
    float value;
    bool chart;
    char text[40];
    ButtonCallback onButton;
    SwitchCallback onSwitch;
    SliderCallback onSlider;
  };
  AlashHttpServer _server;
  Widget _w[ALASH_WEB_MAX_WIDGETS];
  uint8_t _n = 0;
  char _title[40] = "Alash";
  char _user[16] = "admin", _pass[24] = "";
  bool _started = false;
  Error _err = ERR_OK;
  Language _lang = LANG_RU;

  Widget *find(const char *id);
  Widget *add(const char *id, const char *label, Type t);
  bool authorized();
  void sendUi();
  void sendState();
  void handleSet();
  bool appendValue(String &out, const Widget &w);
};

#endif  // ALASH_WEB_H
