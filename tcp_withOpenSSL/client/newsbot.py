import json
import time
import urllib.request
import threading
import schedule
import sys
from datetime import datetime


# ================================================================
#  НАСТРОЙКИ
# ================================================================
SERPER_API_KEY      = "99b95adf25e001d18b36e8bb325ac3d01e50c12f"
OPENROUTER_API_KEY  = "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"
TELEGRAM_BOT_TOKEN  = "8593424035:AAFfOR2oPVbsg0kpldp2o5Kv0BtZO2Rxmgg"
TELEGRAM_CHAT_ID    = "2145450623"

# Модель в OpenRouter (можно сменить на "anthropic/claude-3-haiku" или "meta-llama/llama-3-8b-instruct")
MODEL_ID = "google/gemini-2.0-flash-001" 

def log(msg):
    print(f"[{datetime.now().strftime('%H:%M:%S')}] {msg}")

# ----------------------------------------------------------------
#  SERPER API -- Поиск по Radar.am
# ----------------------------------------------------------------

def fetch_news_from_serper():
    url = "https://google.serper.dev/search"
    query = "site:https://www.azatutyun.am/news"
    
    payload = json.dumps({
        "q": query,
        "gl": "am",
        "hl": "hy",
        
    }, ensure_ascii=False)
    
    headers = {
        'X-API-KEY': SERPER_API_KEY,
        'Content-Type': 'application/json'
    }
    
    log("Serper ищет новости на radar.am...")
    req = urllib.request.Request(url, data=payload.encode("utf-8"), headers=headers, method="POST")
    
    try:
        with urllib.request.urlopen(req, timeout=20) as resp:
            data = json.loads(resp.read().decode("utf-8"))
            results = data.get("organic", [])
            log(f"Найдено ссылок: {len(results)}")
            return results
    except Exception as e:
        log(f"Ошибка Serper: {e}")
        return []

# ----------------------------------------------------------------
#  OPENROUTER API -- Генерация конспекта
# ----------------------------------------------------------------

def get_news_digest():
    raw_news = fetch_news_from_serper()
    
    if not raw_news:
        return "Այս պահին Radar.am-ում թարմ լուրեր չեն գտնվել:"

    # Собираем контекст из найденных ссылок
    news_context = ""
    for idx, item in enumerate(raw_news[:5]):
        news_context += f"{idx+1}. {item.get('title')}\nLink: {item.get('link')}\nSnippet: {item.get('snippet')}\n\n"

    today = datetime.now().strftime("%d.%m.%Y")
    
    # Формируем запрос для OpenRouter
    # Փոխված պրոմպտ, որը AI-ին արգելում է «արդարանալ»
    prompt = (
        f"Դու պրոֆեսիոնալ լրագրող ես: Քեզ տրամադրված են հետևյալ նորությունները radar.am կայքից:\n\n"
        f"{news_context}\n"
        "ՔՈ ԽՆԴԻՐՆ Է. Օգտագործելով ՄԻԱՅՆ վերը նշված տեղեկությունները, պատրաստել հակիրճ ամփոփում հայերենով:\n"
        "Մի՛ փորձիր ինքնուրույն մտնել ինտերնետ: Պարզապես ձևավորիր այս տեքստը:\n"
        "Յուրաքանչյուր լուրի համար գրիր 📌 վերնագիր և 🔗 հղում:"
    )

    log(f"Отправка запроса в OpenRouter ({MODEL_ID})...")
    
    url = "https://openrouter.ai/api/v1/chat/completions"
    headers = {
        "Authorization": f"Bearer {OPENROUTER_API_KEY}",
        "Content-Type": "application/json",
        "HTTP-Referer": "https://github.com/mybot", # Обязательно для OpenRouter
        "X-Title": "Armenia News Bot"
    }
    
    data = json.dumps({
        "model": MODEL_ID,
        "messages": [
            {"role": "user", "content": prompt}
        ],
        "temperature": 0.3
    }, ensure_ascii=False)

    try:
        req = urllib.request.Request(url, data=data.encode("utf-8"), headers=headers, method="POST")
        with urllib.request.urlopen(req, timeout=30) as resp:
            result = json.loads(resp.read().decode("utf-8"))
            # Извлекаем текст ответа
            content = result['choices'][0]['message']['content']
            log("Конспект успешно получен от OpenRouter.")
            return content
    except Exception as e:
        log(f"Ошибка OpenRouter: {e}")
        return f"Սխալ OpenRouter-ի հետ կապի մեջ: {e}"

# ----------------------------------------------------------------
#  TELEGRAM
# ----------------------------------------------------------------

def send_msg(chat_id, text):
    url = f"https://api.telegram.org/bot{TELEGRAM_BOT_TOKEN}/sendMessage"
    payload = {
        "chat_id": chat_id,
        "text": text,
        "parse_mode": "HTML",
        "disable_web_page_preview": False
    }
    try:
        data = json.dumps(payload, ensure_ascii=False).encode("utf-8")
        req = urllib.request.Request(url, data=data, headers={"Content-Type":"application/json"}, method="POST")
        with urllib.request.urlopen(req, timeout=15) as resp:
            return json.loads(resp.read().decode("utf-8"))
    except Exception as e:
        log(f"Ошибка отправки в TG: {e}")

def handle_update(update):
    msg = update.get("message")
    if not msg or "text" not in msg: return
    
    chat_id = msg["chat"]["id"]
    if msg["text"] == "/news":
        send_msg(chat_id, "⏳ Կապվում եմ Radar.am-ի և AI-ի հետ...")
        def worker():
            digest = get_news_digest()
            send_msg(chat_id, digest)
        threading.Thread(target=worker).start()

def start_polling():
    log("Бот запущен через OpenRouter. Ожидание команд...")
    offset = 0
    while True:
        try:
            url = f"https://api.telegram.org/bot{TELEGRAM_BOT_TOKEN}/getUpdates"
            params = json.dumps({"offset": offset, "timeout": 20}).encode("utf-8")
            req = urllib.request.Request(url, data=params, headers={"Content-Type":"application/json"}, method="POST")
            with urllib.request.urlopen(req, timeout=25) as resp:
                res = json.loads(resp.read().decode("utf-8"))
                for upd in res.get("result", []):
                    offset = upd["update_id"] + 1
                    handle_update(upd)
        except Exception as e:
            time.sleep(5)

if __name__ == "__main__":
    start_polling()