@echo off
echo Compilando Campo Minado...
g++ main.cpp -o CampoMinado.exe -mwindows -static-libgcc -static-libstdc++
if %errorlevel% neq 0 (
    echo [ERRO] Falha na compilacao!
    exit /b %errorlevel%
)
echo [SUCESSO] Compilacao finalizada com sucesso!
echo Execute CampoMinado.exe para jogar.
