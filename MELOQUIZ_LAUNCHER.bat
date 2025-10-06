@echo off
title MeloQuiz Launcher
color 0B

:MENU
cls
echo.
echo ========================================
echo      MeloQuiz - Ear Training Game
echo         Developed by Igor Pessoa
echo ========================================
echo.
echo  Escolha uma opcao:
echo.
echo  [1] Executar MeloQuiz (Debug)
echo  [2] Executar MeloQuiz (Release)
echo  [3] Abrir pasta do executavel
echo  [4] Abrir projeto no Visual Studio
echo  [5] Ver logs de compilacao
echo  [0] Sair
echo.
echo ========================================
echo.
set /p choice=Digite sua escolha: 

if "%choice%"=="1" goto DEBUG
if "%choice%"=="2" goto RELEASE
if "%choice%"=="3" goto FOLDER
if "%choice%"=="4" goto OPEN_VS
if "%choice%"=="5" goto LOGS
if "%choice%"=="0" goto EXIT
goto MENU

:DEBUG
cls
echo.
echo Executando versao Debug...
echo.
if exist "Builds\VisualStudio2022\x64\Debug\App\meloquiz.exe" (
    start "" "Builds\VisualStudio2022\x64\Debug\App\meloquiz.exe"
    color 0A
    echo [OK] Programa iniciado!
    timeout /t 2 >nul
    goto EXIT
) else (
    color 0C
    echo [ERRO] Executavel Debug nao encontrado!
    echo Compile o projeto primeiro.
    pause
    goto MENU
)

:RELEASE
cls
echo.
echo Executando versao Release...
echo.
if exist "Builds\VisualStudio2022\x64\Release\App\meloquiz.exe" (
    start "" "Builds\VisualStudio2022\x64\Release\App\meloquiz.exe"
    color 0A
    echo [OK] Programa iniciado!
    timeout /t 2 >nul
    goto EXIT
) else (
    color 0C
    echo [ERRO] Executavel Release nao encontrado!
    echo Compile o projeto primeiro.
    pause
    goto MENU
)

:FOLDER
cls
echo.
echo Abrindo pasta do executavel...
echo.
if exist "Builds\VisualStudio2022\x64\Debug\App\" (
    start "" "Builds\VisualStudio2022\x64\Debug\App\"
) else if exist "Builds\VisualStudio2022\x64\Release\App\" (
    start "" "Builds\VisualStudio2022\x64\Release\App\"
) else (
    color 0C
    echo [ERRO] Pasta nao encontrada!
    pause
)
goto MENU

:OPEN_VS
cls
echo.
echo Abrindo projeto no Visual Studio...
echo.
if exist "Builds\VisualStudio2022\meloquiz.sln" (
    start "" "Builds\VisualStudio2022\meloquiz.sln"
    color 0A
    echo [OK] Visual Studio abrindo...
    timeout /t 2 >nul
    goto EXIT
) else (
    color 0C
    echo [ERRO] Arquivo .sln nao encontrado!
    pause
    goto MENU
)

:LOGS
cls
echo.
echo ========================================
echo         Informacoes do Projeto
echo ========================================
echo.
echo Diretorio atual:
cd
echo.
echo Arquivos principais:
echo.
if exist "Source\MainComponent.cpp" echo [OK] MainComponent.cpp
if exist "Source\GameEngine.cpp" echo [OK] GameEngine.cpp
if exist "Source\SimpleSineVoice.h" echo [OK] SimpleSineVoice.h
echo.
if exist "Builds\VisualStudio2022\x64\Debug\App\meloquiz.exe" (
    echo [OK] Debug Build existe
) else (
    echo [X] Debug Build nao existe
)
if exist "Builds\VisualStudio2022\x64\Release\App\meloquiz.exe" (
    echo [OK] Release Build existe
) else (
    echo [X] Release Build nao existe
)
echo.
echo ========================================
pause
goto MENU

:EXIT
exit
