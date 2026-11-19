#!/usr/bin/env bash
set -e

cd "$(dirname "$0")"

PORT="${1:-8765}"

if command -v python3 >/dev/null 2>&1; then
  PY_BIN="python3"
elif command -v python >/dev/null 2>&1; then
  PY_BIN="python"
else
  echo "Khong tim thay Python 3. Hay cai Python 3 (brew install python hoac python.org)."
  exit 1
fi

echo "==================================================="
echo "  TTUD Sprint 29/11 - Local C++ Learning Platform"
echo "  Mo trinh duyet tai: http://127.0.0.1:${PORT}"
echo "  Nhan Ctrl+C trong cua so nay de tat server."
echo "==================================================="

if command -v open >/dev/null 2>&1; then
  (sleep 1 && open "http://127.0.0.1:${PORT}") >/dev/null 2>&1 &
elif command -v xdg-open >/dev/null 2>&1; then
  (sleep 1 && xdg-open "http://127.0.0.1:${PORT}") >/dev/null 2>&1 &
fi

exec "$PY_BIN" server.py "$PORT"
