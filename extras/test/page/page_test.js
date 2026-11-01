// Мини-DOM для проверки логики страницы AlashWeb без браузера
const fs = require('fs');
const code = fs.readFileSync('/tmp/page.js', 'utf8');
function node(tag) { return { tag, children: [], className: '', textContent: '', style: {}, value: '', onclick: null, oninput: null,
  appendChild(c) { this.children.push(c); return c; }, set innerHTML(v) { this.children = []; }, getContext() { return { clearRect() {}, beginPath() {}, moveTo() {}, lineTo() {}, stroke() {}, set strokeStyle(v) {}, set lineWidth(v) {} }; }, width: 0, height: 0 }; }
const ids = {}; ['h', 'off', 'm'].forEach(i => ids[i] = node('div'));
global.document = { createElement: node, getElementById: i => ids[i], title: '', body: {}, activeElement: null };
global.getComputedStyle = () => ({ getPropertyValue: () => '#0a7' });
global.location = { reload() {} };
let state = { light: 1, speed: 30, temp: 23.5, status: 'ok' }; const sent = [];
const ui = { title: 'Фобо', offline: 'Нет связи', widgets: [
  { id: 'horn', type: 'button', label: 'Сигнал', min: 0, max: 1, unit: '', chart: false },
  { id: 'light', type: 'switch', label: 'Свет', min: 0, max: 1, unit: '', chart: false },
  { id: 'speed', type: 'slider', label: 'Скорость', min: 0, max: 100, unit: '', chart: false },
  { id: 'temp', type: 'value', label: 'Темп', min: 0, max: 1, unit: 'C', chart: true },
  { id: 'status', type: 'text', label: 'Статус', min: 0, max: 1, unit: '', chart: false } ] };
global.fetch = (u) => new Promise((res, rej) => {
  if (u.startsWith('/ui')) return res({ ok: true, json: async () => ui });
  if (u.startsWith('/state')) return res({ ok: true, json: async () => state });
  if (u.startsWith('/set')) { sent.push(u); const m = u.match(/id=(\w+)&v=(\d+)/); if (m[1] == 'light') state.light = +m[2]; return res({ ok: true, json: async () => state }); }
  rej(new Error('bad'));
});
let timers = []; global.setTimeout = (f, t) => { timers.push(f); return timers.length; }; global.clearTimeout = () => {};
eval(code);
setTimeout = global.setTimeout;
(async () => {
  await new Promise(r => setImmediate(r)); await new Promise(r => setImmediate(r)); await new Promise(r => setImmediate(r));
  const main = ids['m'];
  const fail = (m) => { console.log('FAIL', m); process.exitCode = 1; };
  if (ids['h'].textContent !== 'Фобо') fail('title');
  if (main.children.length !== 5) fail('widgets ' + main.children.length);
  const sw = main.children[1].children[1];                   // кнопка-переключатель
  if (!/on/.test(sw.className)) fail('switch should be on: ' + sw.className);
  const valBox = main.children[3].children[1].children[0];   // число
  if (String(valBox.textContent) !== '23.5') fail('value ' + valBox.textContent);
  main.children[0].children[0].onclick();                    // кнопка
  sw.onclick();                                              // переключатель: был включён -> выключить
  await new Promise(r => setImmediate(r)); await new Promise(r => setImmediate(r));
  if (sent[0] !== '/set?id=horn&v=1') fail('button url ' + sent[0]);
  if (sent[1] !== '/set?id=light&v=0') fail('switch url ' + sent[1]);
  if (/ on/.test(sw.className) && sw.className.indexOf('on') > 0) fail('switch should be off after reply: ' + sw.className);
  if (!process.exitCode) console.log('PAGE JS OK');
})();
