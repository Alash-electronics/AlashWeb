// MIT License. Alash Electronics.
#include "AlashWeb.h"

#if defined(ESP32) || defined(ESP8266)

#define T(ru, kk, en) (_lang == LANG_KK ? F(kk) : _lang == LANG_EN ? F(en) : F(ru))

// Страница панели. Элементы приходят с адреса /ui, значения опрашиваются с /state раз в секунду, команды уходят на /set.
static const char PAGE[] PROGMEM = R"HTML(<!doctype html><html><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Alash</title><style>
:root{--bg:#f4f6f8;--card:#fff;--ink:#16202a;--mut:#6b7886;--acc:#0a7d6b;--line:#dde3e8}
@media(prefers-color-scheme:dark){:root{--bg:#10171d;--card:#18222b;--ink:#e8eef3;--mut:#93a1ae;--acc:#2dc3a8;--line:#2a3743}}
*{box-sizing:border-box}body{margin:0;font-family:system-ui,sans-serif;background:var(--bg);color:var(--ink)}
header{padding:16px;font-size:20px;font-weight:600}#off{display:none;margin:0 16px 8px;padding:10px;border-radius:8px;background:#b3261e;color:#fff}
main{display:grid;gap:12px;padding:0 16px 24px;max-width:560px;margin:0 auto}
.c{background:var(--card);border:1px solid var(--line);border-radius:12px;padding:14px}
.c label{display:block;color:var(--mut);font-size:14px;margin-bottom:8px}
button.b{width:100%;padding:16px;font-size:18px;border:0;border-radius:10px;background:var(--acc);color:#fff}button.b:active{opacity:.7}
.s{display:flex;align-items:center;justify-content:space-between}.s span{font-size:18px}
.t{width:56px;height:30px;border-radius:15px;background:var(--line);position:relative;border:0}.t:after{content:"";position:absolute;top:3px;left:3px;width:24px;height:24px;border-radius:50%;background:#fff;transition:left .15s}
.t.on{background:var(--acc)}.t.on:after{left:29px}
input[type=range]{width:100%;accent-color:var(--acc)}.v{font-size:32px;font-weight:600}.u{font-size:16px;color:var(--mut);margin-left:4px}
canvas{width:100%;height:60px;margin-top:8px}.x{font-size:18px}
</style></head><body><header id="h"></header><div id="off"></div><main id="m"></main><script>
var W=[],H={},D=1000;
function E(t,c,x){var e=document.createElement(t);if(c)e.className=c;if(x!=null)e.textContent=x;return e}
function J(u){return fetch(u,{cache:'no-store'}).then(function(r){if(!r.ok)throw 0;return r.json()})}
function off(m){var o=document.getElementById('off');o.style.display=m?'block':'none';if(m)o.textContent=m}
function set(id,v){J('/set?id='+id+'&v='+v).then(upd).catch(function(){off(OFF)})}
var OFF='offline';
function build(u){document.getElementById('h').textContent=u.title;document.title=u.title;OFF=u.offline;var m=document.getElementById('m');m.innerHTML='';
u.widgets.forEach(function(w){var c=E('div','c'),n;
if(w.type=='button'){n=E('button','b',w.label);n.onclick=function(){set(w.id,1)};c.appendChild(n)}
else if(w.type=='switch'){c.className+=' s';c.appendChild(E('span','',w.label));n=E('button','t');n.onclick=function(){set(w.id,n.className.indexOf('on')>0?0:1)};c.appendChild(n)}
else if(w.type=='slider'){var l=E('label','',w.label);n=E('input');n.type='range';n.min=w.min;n.max=w.max;var t=0;
n.oninput=function(){l.textContent=w.label+': '+n.value;clearTimeout(t);t=setTimeout(function(){set(w.id,n.value)},150)};c.appendChild(l);c.appendChild(n);w.l=l}
else if(w.type=='value'){c.appendChild(E('label','',w.label));var v=E('div','v');n=E('span');v.appendChild(n);v.appendChild(E('span','u',w.unit));c.appendChild(v);
if(w.chart){var k=E('canvas');k.width=520;k.height=120;c.appendChild(k);w.k=k;H[w.id]=[]}}
else{c.appendChild(E('label','',w.label));n=E('div','x');c.appendChild(n)}
w.n=n;m.appendChild(c);W.push(w)})}
function draw(w){var a=H[w.id],k=w.k,g=k.getContext('2d');g.clearRect(0,0,k.width,k.height);if(a.length<2)return;
var lo=Math.min.apply(0,a),hi=Math.max.apply(0,a);if(hi==lo){hi=lo+1}g.strokeStyle=getComputedStyle(document.body).getPropertyValue('--acc');g.lineWidth=3;g.beginPath();
a.forEach(function(y,i){var px=i*k.width/59,py=k.height-6-(y-lo)*(k.height-12)/(hi-lo);i?g.lineTo(px,py):g.moveTo(px,py)});g.stroke()}
function upd(s){off(0);W.forEach(function(w){var v=s[w.id];if(v===undefined)return;
if(w.type=='switch')w.n.className='t'+(v?' on':'');
else if(w.type=='slider'){if(document.activeElement!==w.n){w.n.value=v;w.l.textContent=w.label+': '+v}}
else if(w.type=='value'){w.n.textContent=v===null?'—':v;if(w.k&&v!==null){var a=H[w.id];a.push(v);if(a.length>60)a.shift();draw(w)}}
else if(w.type=='text')w.n.textContent=v})}
function poll(){J('/state').then(upd).catch(function(){off(OFF)}).then(function(){setTimeout(poll,D)})}
J('/ui').then(function(u){build(u);poll()}).catch(function(){document.getElementById('h').textContent='Alash';off('offline');setTimeout(function(){location.reload()},3000)});
</script></body></html>)HTML";

// ---- Строки и JSON ---------------------------------------------------------------------------------------

size_t AlashWeb::jsonEscape(const char *in, char *out, size_t size) {
  size_t o = 0;
  for (size_t i = 0; in[i]; i++) {
    unsigned char c = in[i];
    char tmp[8];
    size_t n;
    if (c == '"') { tmp[0] = '\\'; tmp[1] = '"'; n = 2; }
    else if (c == '\\') { tmp[0] = '\\'; tmp[1] = '\\'; n = 2; }
    else if (c == '\n') { tmp[0] = '\\'; tmp[1] = 'n'; n = 2; }
    else if (c < 0x20) { n = snprintf(tmp, sizeof tmp, "\\u%04x", c); }
    else { tmp[0] = (char)c; n = 1; }
    if (o + n + 1 > size) break;
    memcpy(out + o, tmp, n);
    o += n;
  }
  out[o] = 0;
  return o;
}

bool AlashWeb::validId(const char *id) {
  if (!id || !id[0] || strlen(id) > 15) return false;
  for (const char *p = id; *p; p++)
    if (!((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9') || *p == '_')) return false;
  return true;
}

// ---- Элементы ---------------------------------------------------------------------------------------------------

AlashWeb::Widget *AlashWeb::find(const char *id) {
  for (uint8_t i = 0; i < _n; i++)
    if (strcmp(_w[i].id, id) == 0) return &_w[i];
  return nullptr;
}

AlashWeb::Widget *AlashWeb::add(const char *id, const char *label, Type t) {
  if (!validId(id) || find(id)) { _err = ERR_BAD_ID; return nullptr; }
  if (_n >= ALASH_WEB_MAX_WIDGETS) { _err = ERR_TOO_MANY; return nullptr; }
  Widget &w = _w[_n++];
  memset(&w, 0, sizeof w);
  strncpy(w.id, id, sizeof(w.id) - 1);
  strncpy(w.label, label, sizeof(w.label) - 1);
  w.type = t;
  w.maxV = 1;
  _err = ERR_OK;
  return &w;
}

bool AlashWeb::addButton(const char *id, const char *label, ButtonCallback cb) {
  Widget *w = add(id, label, BUTTON);
  if (w) w->onButton = cb;
  return w != nullptr;
}

bool AlashWeb::addSwitch(const char *id, const char *label, SwitchCallback cb, bool initial) {
  Widget *w = add(id, label, SWITCH);
  if (!w) return false;
  w->onSwitch = cb;
  w->value = initial ? 1 : 0;
  return true;
}

bool AlashWeb::addSlider(const char *id, const char *label, int minValue, int maxValue, SliderCallback cb, int initial) {
  Widget *w = add(id, label, SLIDER);
  if (!w) return false;
  if (maxValue < minValue) { int t = minValue; minValue = maxValue; maxValue = t; }
  w->minV = minValue;
  w->maxV = maxValue;
  w->onSlider = cb;
  w->value = initial < minValue ? minValue : initial > maxValue ? maxValue : initial;
  return true;
}

bool AlashWeb::addValue(const char *id, const char *label, const char *unit, bool chart) {
  Widget *w = add(id, label, VALUE);
  if (!w) return false;
  strncpy(w->unit, unit, sizeof(w->unit) - 1);
  w->chart = chart;
  w->value = NAN;
  return true;
}

bool AlashWeb::addText(const char *id, const char *label) { return add(id, label, TEXT) != nullptr; }

bool AlashWeb::setValue(const char *id, float v) { Widget *w = find(id); if (!w || w->type != VALUE) { _err = ERR_NO_SUCH; return false; } w->value = v; return true; }
bool AlashWeb::setText(const char *id, const char *t) { Widget *w = find(id); if (!w || w->type != TEXT) { _err = ERR_NO_SUCH; return false; } strncpy(w->text, t, sizeof(w->text) - 1); w->text[sizeof(w->text) - 1] = 0; return true; }
bool AlashWeb::setSwitch(const char *id, bool on) { Widget *w = find(id); if (!w || w->type != SWITCH) { _err = ERR_NO_SUCH; return false; } w->value = on ? 1 : 0; return true; }
bool AlashWeb::setSlider(const char *id, int v) {
  Widget *w = find(id);
  if (!w || w->type != SLIDER) { _err = ERR_NO_SUCH; return false; }
  w->value = v < w->minV ? w->minV : v > w->maxV ? w->maxV : v;
  return true;
}
bool AlashWeb::getSwitch(const char *id) { Widget *w = find(id); return w && w->type == SWITCH && w->value != 0; }
int AlashWeb::getSlider(const char *id) { Widget *w = find(id); return (w && w->type == SLIDER) ? (int)w->value : 0; }

// ---- Сервер -----------------------------------------------------------------------------------------------------

void AlashWeb::setPassword(const char *password, const char *user) {
  strncpy(_pass, password, sizeof(_pass) - 1);
  strncpy(_user, user, sizeof(_user) - 1);
}

bool AlashWeb::authorized() {
  if (!_pass[0]) return true;
  if (_server.authenticate(_user, _pass)) return true;
  _server.requestAuthentication();
  return false;
}

String AlashWeb::url() const {
  return String("http://") + WiFi.localIP().toString() + "/";
}

// Значение элемента в JSON: число без хвостовых нулей, null для NAN, строка в кавычках
bool AlashWeb::appendValue(String &out, const Widget &w) {
  if (w.type == TEXT) {
    char esc[90];
    jsonEscape(w.text, esc, sizeof esc);
    out += '"'; out += esc; out += '"';
  } else if (isnan(w.value)) {
    out += F("null");
  } else if (w.type == SWITCH || w.type == SLIDER) {
    out += (int)w.value;
  } else {
    char b[24];
    snprintf(b, sizeof b, "%.2f", w.value);
    size_t n = strlen(b);
    while (n > 1 && b[n - 1] == '0') b[--n] = 0;       // 23.50 -> 23.5, 7.00 -> 7
    if (n > 1 && b[n - 1] == '.') b[--n] = 0;
    out += b;
  }
  return true;
}

void AlashWeb::sendUi() {
  if (!authorized()) return;
  static const char *types[] = {"button", "switch", "slider", "value", "text"};
  String j;
  j.reserve(160 + 110 * _n);
  char esc[90];
  jsonEscape(_title, esc, sizeof esc);
  j += F("{\"title\":\""); j += esc; j += F("\",\"offline\":\"");
  j += (_lang == LANG_KK ? "Байланыс жоқ" : _lang == LANG_EN ? "No connection" : "Нет связи");
  j += F("\",\"widgets\":[");
  for (uint8_t i = 0; i < _n; i++) {
    const Widget &w = _w[i];
    if (i) j += ',';
    j += F("{\"id\":\""); j += w.id;
    j += F("\",\"type\":\""); j += types[w.type];
    jsonEscape(w.label, esc, sizeof esc);
    j += F("\",\"label\":\""); j += esc;
    j += F("\",\"min\":"); j += w.minV;
    j += F(",\"max\":"); j += w.maxV;
    jsonEscape(w.unit, esc, sizeof esc);
    j += F(",\"unit\":\""); j += esc;
    j += F("\",\"chart\":"); j += (w.chart ? "true" : "false");
    j += '}';
  }
  j += F("]}");
  _server.send(200, "application/json; charset=utf-8", j);
}

void AlashWeb::sendState() {
  String j;
  j.reserve(20 + 24 * _n);
  j += '{';
  bool first = true;
  for (uint8_t i = 0; i < _n; i++) {
    if (_w[i].type == BUTTON) continue;
    if (!first) j += ',';
    first = false;
    j += '"'; j += _w[i].id; j += F("\":");
    appendValue(j, _w[i]);
  }
  j += '}';
  _server.sendHeader("Cache-Control", "no-store");
  _server.send(200, "application/json", j);
}

void AlashWeb::handleSet() {
  if (!authorized()) return;
  String id = _server.arg("id"), v = _server.arg("v");
  Widget *w = find(id.c_str());
  if (!w) {
    _server.send(404, "application/json", "{\"error\":\"no such id\"}");
    return;
  }
  long n = v.toInt();
  if (w->type == BUTTON) {
    if (w->onButton) w->onButton();
  } else if (w->type == SWITCH) {
    w->value = n != 0 ? 1 : 0;
    if (w->onSwitch) w->onSwitch(n != 0);
  } else if (w->type == SLIDER) {
    int c = n < w->minV ? w->minV : n > w->maxV ? w->maxV : (int)n;
    w->value = c;
    if (w->onSlider) w->onSlider(c);
  }
  sendState();
}

void AlashWeb::begin() {
  _server.on("/", [this]() {
    if (!authorized()) return;
    _server.send_P(200, "text/html; charset=utf-8", PAGE);
  });
  _server.on("/ui", [this]() { sendUi(); });
  _server.on("/state", [this]() { if (authorized()) sendState(); });
  _server.on("/set", [this]() { handleSet(); });
  _server.onNotFound([this]() { _server.send(404, "text/plain", "Not found"); });
  _server.begin();
  _started = true;
  _err = WiFi.status() == WL_CONNECTED ? ERR_OK : ERR_NOT_CONNECTED;
}

// ---- Диагностика -----------------------------------------------------------------------------------------------------

const __FlashStringHelper *AlashWeb::errorText() {
  switch (_err) {
    case ERR_OK: return F("OK");
    case ERR_TOO_MANY:
      return T("Слишком много элементов: добавьте #define ALASH_WEB_MAX_WIDGETS 24 до #include (памяти платы должно хватить)",
               "Элемент тым көп: #include алдында #define ALASH_WEB_MAX_WIDGETS 24 қосыңыз",
               "Too many widgets: add #define ALASH_WEB_MAX_WIDGETS 24 before #include (the board memory must allow it)");
    case ERR_BAD_ID:
      return T("Идентификатор элемента: до 15 латинских букв, цифр и _, уникальный (например, \"led1\")", "Элемент идентификаторы: 15 латын әрпіне, санға және _ дейін, бірегей", "Widget id: up to 15 Latin letters, digits and _, unique (for example \"led1\")");
    case ERR_NO_SUCH:
      return T("Нет элемента с таким идентификатором или он другого типа (setValue для значения, setSwitch для переключателя)", "Мұндай идентификаторлы элемент жоқ не түрі басқа", "No widget with that id, or it has another type (setValue for a value, setSwitch for a switch)");
    case ERR_NOT_CONNECTED:
      return T("Нет подключения к Wi-Fi: панель недоступна. Сначала подключитесь к сети (WiFi.begin) и только потом вызывайте web.begin()", "Wi-Fi-ға қосылмаған: панель қолжетімсіз. Алдымен желіге қосылыңыз, содан кейін web.begin()", "Not connected to Wi-Fi: the panel is unreachable. Connect to the network first (WiFi.begin) and only then call web.begin()");
  }
  return F("");
}

void AlashWeb::diagnose(Print &out) {
  out.println(T("AlashWeb: диагностика", "AlashWeb: диагностика", "AlashWeb: diagnostics"));
  if (WiFi.status() == WL_CONNECTED) {
    out.print(T("Откройте на телефоне или компьютере (в той же сети Wi-Fi): ", "Телефонда не компьютерде ашыңыз (сол Wi-Fi желісінде): ", "Open on a phone or computer (on the same Wi-Fi network): "));
    out.println(url());
  } else {
    out.println(T("Wi-Fi НЕ подключён: адрес панели появится после подключения", "Wi-Fi қосылмаған: панель мекенжайы қосылғаннан кейін шығады", "Wi-Fi is NOT connected: the panel address appears after connecting"));
  }
  out.print(T("Элементов: ", "Элементтер: ", "Widgets: "));
  out.print((int)_n);
  out.print(F(" / "));
  out.println((int)ALASH_WEB_MAX_WIDGETS);
  out.println(_started ? T("Сервер запущен", "Сервер іске қосылған", "The server is running") : T("Сервер не запущен: вызовите web.begin()", "Сервер іске қосылмаған: web.begin() шақырыңыз", "The server is not running: call web.begin()"));
  if (_err != ERR_OK) out.println(errorText());
  out.println(T("Телефон должен быть в той же сети Wi-Fi, что и плата (мобильный интернет не подойдёт). Если страница не открывается, проверьте адрес и что в loop() вызывается web.update().",
                "Телефон тақтамен бір Wi-Fi желісінде болуы керек (мобильді интернет жарамайды). Бет ашылмаса, мекенжайды және loop() ішінде web.update() шақырылғанын тексеріңіз.",
                "The phone must be on the same Wi-Fi network as the board (mobile data will not work). If the page does not open, check the address and that web.update() is called in loop()."));
}

#endif  // ESP32 || ESP8266
