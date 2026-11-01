// Имитация WebServer (ESP32): обработчики запоминаются, запросы подаёт тест через очередь, ответы записываются.
#pragma once
#include <map>
#include <vector>
#include "Arduino.h"
struct HttpReq { std::string path; std::map<std::string, std::string> args; };
struct HttpResp { int code; std::string type, body, location; };
inline std::vector<HttpReq> g_httpQueue;
inline std::vector<HttpResp> g_httpResp;
class WebServer {
 public:
  explicit WebServer(int) {}
  std::map<std::string, std::function<void()>> routes; std::function<void()> notFound; bool started = false;
  HttpReq cur; std::string locationHeader;
  void on(const char *path, std::function<void()> f) { routes[path] = f; }
  void onNotFound(std::function<void()> f) { notFound = f; }
  void begin() { started = true; }
  void close() { started = false; }
  String arg(const char *name) { auto it = cur.args.find(name); return String(it == cur.args.end() ? "" : it->second.c_str()); }
  void sendHeader(const char *, const String &v, bool = false) { locationHeader = v.s; }
  void send(int code, const char *type, const String &body) { g_httpResp.push_back({code, type, body.s, locationHeader}); locationHeader.clear(); }
  bool authRequired = false; std::string wantUser, wantPass; std::string authUser, authPass; bool authSent = false;
  bool authenticate(const char *u, const char *p) { wantUser = u; wantPass = p; return authSent && authUser == u && authPass == p; }
  void requestAuthentication() { g_httpResp.push_back({401, "auth", "", ""}); }
  void send_P(int code, const char *type, const char *body) { g_httpResp.push_back({code, type, body, ""}); }
  void handleClient() {
    while (!g_httpQueue.empty()) {
      cur = g_httpQueue.front(); g_httpQueue.erase(g_httpQueue.begin());
      authSent = cur.args.count("__auth") > 0; if (authSent) { std::string a = cur.args["__auth"]; size_t c = a.find(':'); authUser = a.substr(0, c); authPass = a.substr(c + 1); }
      auto it = routes.find(cur.path);
      if (it != routes.end()) it->second(); else if (notFound) notFound();
    }
  }
};
