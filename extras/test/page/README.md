# Проверка JavaScript страницы панели

`page_test.js` запускает скрипт страницы из `src/AlashWeb.cpp` в Node.js с мини-имитацией браузера (DOM, fetch) и проверяет,
что элементы строятся, значения подставляются, кнопки и переключатели шлют правильные запросы `/set`.

    python3 - <<'PY'
    import re; s=open('../../../src/AlashWeb.cpp',encoding='utf-8').read()
    html=re.search(r'R"HTML\((.*?)\)HTML"',s,re.S).group(1)
    open('/tmp/page.js','w').write(re.search(r'<script>(.*?)</script>',html,re.S).group(1))
    PY
    node page_test.js
