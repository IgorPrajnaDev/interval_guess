@echo off
cls
color 0B
echo.
echo ========================================
echo      MeloQuiz - Ear Training Game
echo         Developed by Igor Pessoa
echo ========================================
echo.
echo Procurando executavel...
echo.

REM Tenta executar a versao Debug
if exist "Builds\VisualStudio2022\x64\Debug\App\meloquiz.exe" (
    color 0A
    echo [OK] Encontrado: Debug Build
    echo [OK] Iniciando programa...
    echo.
    start "" "Builds\VisualStudio2022\x64\Debug\App\meloquiz.exe"
    echo.
    echo Programa iniciado com sucesso!
    echo Divirta-se tocando piano! ^_^
    timeout /t 2 >nul
    exit
)

REM Se nao encontrar Debug, tenta Release
if exist "Builds\VisualStudio2022\x64\Release\App\meloquiz.exe" (
    color 0A
    echo [OK] Encontrado: Release Build
    echo [OK] Iniciando programa...
    echo.
    start "" "Builds\VisualStudio2022\x64\Release\App\meloquiz.exe"
    echo.
    echo Programa iniciado com sucesso!
    echo Divirta-se tocando piano! ^_^
    timeout /t 2 >nul
    exit
)

REM Se nao encontrar nenhum
color 0C
echo.
echo ========================================
echo            [ERRO]
echo ========================================
echo.
echo Executavel nao encontrado!
echo.
echo Para compilar o projeto:
echo.
echo 1. Abra "meloquiz.sln" no Visual Studio 2022
echo 2. Pressione Ctrl + Shift + B para compilar
echo 3. Aguarde a compilacao terminar
echo 4. Execute este arquivo novamente
echo.
echo ========================================
echo.
echo Pressione qualquer tecla para fechar...
pause >nul
