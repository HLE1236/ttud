@echo off
cd /d "%~dp0"
echo ===================================================
echo   TTUD Sprint 29/11 - Local C++ Learning Platform
echo   Mo trinh duyet tai: http://127.0.0.1:8765
echo   Nhan Ctrl+C trong cua so nay de tat server.
echo ===================================================
start "" http://127.0.0.1:8765
python server.py 8765
pause
