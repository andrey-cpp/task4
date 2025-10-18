# HTTP API

База: `http://<ip_устройства>/`

## Эндпоинты
- `GET /api/relay?state=on` — включить реле/SSR  
- `GET /api/relay?state=off` — выключить  
- `GET /api/relay?state=status` — получить статус

## Ответ JSON (пример)
```json
{"status":"success","relay_state":"on"}
