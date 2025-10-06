# 🎹 MeloQuiz - Jogo de Treinamento de Ouvido Musical

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![JUCE](https://img.shields.io/badge/JUCE-8.x-green.svg)](https://juce.com/)
[![License](https://img.shields.io/badge/License-Academic-yellow.svg)]()

**Desenvolvedor:** Igor Pessoa  
**Disciplina:** Programação Orientada a Objetos (C++)  
**Framework:** JUCE 8.x  
**Repositório:** https://github.com/IgorPrajnaDev/interval_guess

---

## 📖 Sobre o Projeto

MeloQuiz é uma aplicação interativa de treinamento de ouvido musical desenvolvida em C++ usando o framework JUCE. Inspirado nos exercícios do Duolingo, o jogo desafia o usuário a reproduzir sequências melódicas através de um piano virtual ou teclado MIDI físico.

### 🎯 Objetivos Acadêmicos

Este projeto demonstra de forma prática e aplicada os principais conceitos de **Programação Orientada a Objetos**:
- ✅ Herança e Polimorfismo (Padrão Strategy)
- ✅ Composição sobre Herança
- ✅ Encapsulamento e Abstração
- ✅ Gerenciamento Moderno de Memória (RAII, Smart Pointers)
- ✅ Padrões de Design (MVC, Strategy)
- ✅ Uso de STL (std::vector, std::unique_ptr)

---

## ✨ Funcionalidades

### 🎮 Mecânicas de Jogo
- **Geração Aleatória de Sequências**: 2-12 notas MIDI configuráveis
- **Reprodução Automática**: Sistema de playback com timing preciso (500ms/nota + 200ms gap)
- **Verificação de Resposta**: Comparação nota-a-nota em tempo real
- **Sistema de Pontuação**: +10 pontos por acerto, -5 por erro (sem negativos)
- **Tentativas Ilimitadas**: Múltiplas tentativas por desafio
- **Progressão Automática**: Novo desafio após acerto (delay de 1.5s)

### 🎹 Sistema de Áudio
- **Sintetizador de Piano Realista**: Som gerado via harmônicos múltiplos
- **8 Vozes Polifônicas**: Permite tocar várias notas simultaneamente
- **Envelope ADSR Dinâmico**: Attack e release ajustados por registro
- **7 Harmônicos**: Fundamental + 6 overtones para timbre autêntico

### 🎛️ Interface
- **Teclado Virtual**: Piano visual para usuários sem hardware MIDI
- **Suporte MIDI Físico**: Conexão automática com dispositivos externos
- **Feedback Visual em Tempo Real**:
  - Flash verde ao acertar (fade suave)
  - Flash vermelho ao errar (fade suave)
  - Contador de progresso: "Notas: X / Y"
  - Indicador de playback: "Ouvindo a sequência..."

### 🎚️ Controles
- **Botão "Iniciar Novo Jogo"**: Reseta score e inicia novo desafio
- **Botão "Repetir Sequência"**: Re-executa a melodia sem penalidade
- **Slider de Dificuldade**: Ajusta quantidade de notas (2-12)

---

## 🛠️ Requisitos

### Software Necessário
- **JUCE Framework** (versão 8.x ou superior)
- **Visual Studio 2022** (Windows) ou Xcode (macOS) ou GCC/Clang (Linux)
- **Projucer** (incluído com JUCE)
- **Git** (opcional, para clonar o repositório)

### Especificações Técnicas
- **Linguagem**: C++17
- **Padrão de Compilação**: ISO C++17 Standard
- **Plataformas Suportadas**: Windows, macOS, Linux
- **Resolução Mínima**: 800x600 pixels

---

## 🚀 Como Compilar e Executar

### Método 1: Usando o Projucer (Recomendado)

1. **Clone o Repositório**
```bash
git clone https://github.com/IgorPrajnaDev/interval_guess.git
cd interval_guess
```

2. **Abra o Projeto no Projucer**
   - Dê duplo clique em `meloquiz.jucer`

3. **Configure o Exportador**
   - Vá para a aba "Exporters"
   - Selecione "Visual Studio 2022" (Windows) ou "Xcode" (macOS)
   - Verifique as configurações

4. **Salve e Abra na IDE**
   - Clique em "Save and Open in IDE..."
   - Aguarde a IDE abrir

5. **Compile o Projeto**
   - **Visual Studio**: Pressione `Ctrl + Shift + B`
   - **Xcode**: Clique no botão "Play" (▶)

6. **Execute o Programa**
   - O executável estará em `Builds/VisualStudio2022/x64/Debug/App/meloquiz.exe`

### Método 2: Usando o Launcher (Mais Rápido)

1. **Após compilar**, navegue até a pasta raiz do projeto

2. **Execute o launcher**:
   - Duplo clique em `RUN_MELOQUIZ.bat` (execução rápida)
   - OU `MELOQUIZ_LAUNCHER.bat` (menu com opções)

3. **O programa abre automaticamente!** 🎉

---

## 📁 Estrutura do Projeto

```
meloquiz/
├── Source/                      # Código-fonte C++
│   ├── Main.cpp                 # Ponto de entrada da aplicação
│   ├── MainComponent.h/cpp      # Componente principal (Controller)
│   ├── GameEngine.h/cpp         # Lógica do jogo (Model)
│   ├── Challenge.h/cpp          # Classe base abstrata de desafios
│   ├── MelodyChallenge.h/cpp    # Desafio de melodia (Strategy)
│   ├── Note.h/cpp               # Representação de nota MIDI
│   ├── NoteSequence.h/cpp       # Sequência de notas
│   ├── SimpleSineVoice.h        # Sintetizador de piano
│   └── VirtualKeyboardComponent.h/cpp  # Piano virtual (View)
│
├── Builds/                      # Arquivos de build (gerados)
│   └── VisualStudio2022/
│       ├── meloquiz.sln         # Solução do Visual Studio
│       └── x64/Debug/App/       # Executável compilado
│
├── RUN_MELOQUIZ.bat            # Launcher rápido
├── MELOQUIZ_LAUNCHER.bat       # Launcher com menu
├── README.txt                   # Este arquivo
└── meloquiz.jucer              # Arquivo de projeto Projucer
```

---

## 🏗️ Arquitetura do Software

### Padrão MVC Simplificado

```
┌─────────────────────────────────────────────────┐
│                    VIEW                          │
│  - VirtualKeyboardComponent                     │
│  - Elementos visuais (botões, sliders)          │
│  - Feedback visual (flash, textos)              │
└──────────────────┬──────────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────────┐
│                 CONTROLLER                       │
│            MainComponent                         │
│  - Gerencia eventos MIDI                        │
│  - Coordena Model e View                        │
│  - Controla áudio e animações                   │
└──────────────────┬──────────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────────┐
│                   MODEL                          │
│  - GameEngine (lógica do jogo)                  │
│  - Challenge (hierarquia de desafios)           │
│  - Note, NoteSequence (dados)                   │
└─────────────────────────────────────────────────┘
```

### Padrão Strategy para Desafios

```cpp
Challenge (abstrata)
    ├── generateSequence(int difficulty) = 0
    └── isCorrect(const NoteSequence&) = 0
         │
         └── MelodyChallenge (concreta)
             ├── generateSequence() → sequência aleatória
             └── isCorrect() → comparação nota-a-nota
```

### Composição sobre Herança

```cpp
MainComponent HAS-A:
    ├── GameEngine
    ├── MidiKeyboardComponent
    ├── Synthesiser
    └── AudioDeviceManager

GameEngine HAS-A:
    ├── unique_ptr<Challenge>
    └── NoteSequence (user input)

NoteSequence HAS-A:
    └── vector<Note>
```

---

## 🎓 Conceitos de POO Demonstrados

### 1. Encapsulamento
```cpp
class Note {
private:
    int midiNoteNumber;  // Dados ocultos
    int velocity;
public:
    int getMidiNoteNumber() const;  // Acesso controlado
    int getVelocity() const;
};
```

### 2. Herança e Polimorfismo
```cpp
class Challenge {
public:
    virtual void generateSequence(int difficulty) = 0;  // Virtual puro
    virtual bool isCorrect(const NoteSequence&) const = 0;
};

class MelodyChallenge : public Challenge {
    void generateSequence(int difficulty) override;  // Implementação
    bool isCorrect(const NoteSequence&) const override;
};
```

### 3. Abstração
```cpp
// Interface abstrata - esconde implementação
unique_ptr<Challenge> currentChallenge;
currentChallenge->generateSequence(5);  // Polimorfismo em ação
```

### 4. RAII e Smart Pointers
```cpp
std::unique_ptr<Challenge> currentChallenge;  // Gerenciamento automático
std::unique_ptr<juce::MidiInput> midiInput;   // Sem new/delete manual
```

### 5. Sobrecarga de Operadores
```cpp
class Note {
    bool operator==(const Note& other) const {
        return midiNoteNumber == other.midiNoteNumber;
    }
};
```

---

## 🎮 Como Jogar

### Passo a Passo

1. **Inicie o Programa**
   - Execute `RUN_MELOQUIZ.bat` ou o executável diretamente

2. **Configure a Dificuldade**
   - Use o slider para escolher quantas notas (2-12)

3. **Inicie o Jogo**
   - Clique em "Iniciar Novo Jogo"
   - Ouça atentamente a sequência tocada

4. **Reproduza a Sequência**
   - Use o teclado virtual (mouse) OU
   - Use teclado MIDI físico conectado

5. **Veja o Resultado**
   - ✅ Acertou: Flash verde + nova sequência
   - ❌ Errou: Flash vermelho + nova tentativa

6. **Continue Praticando**
   - Clique em "Repetir Sequência" para ouvir novamente
   - Ajuste a dificuldade conforme melhorar

### Dicas

- 🎧 **Use fones de ouvido** para melhor percepção
- 🎹 **Conecte um teclado MIDI** para experiência mais realista
- 📊 **Acompanhe seu score** no canto superior esquerdo
- 🔄 **Use "Repetir"** quantas vezes precisar
- 📈 **Aumente gradualmente** a dificuldade conforme melhorar

---

## 🐛 Solução de Problemas

### O programa não abre
1. Verifique se compilou corretamente (veja logs de build)
2. Execute `MELOQUIZ_LAUNCHER.bat` → Opção [5] para ver status
3. Tente recompilar: Visual Studio → `Ctrl + Shift + B`

### Sem som
1. Verifique se o volume do sistema está ligado
2. No Windows, abra "Mixer de Volume" e verifique "meloquiz.exe"
3. Tente reconectar dispositivos de áudio

### Teclado MIDI não funciona
1. Conecte o dispositivo ANTES de abrir o programa
2. Veja logs de debug (Output window no VS): deve mostrar "Dispositivo MIDI aberto"
3. Use o teclado virtual como alternativa

### Erro de compilação
1. Verifique se o JUCE está instalado corretamente
2. Abra o projeto no Projucer e salve novamente
3. Limpe e recompile: Build → Clean Solution → Rebuild

---

## 📊 Estatísticas do Projeto

- **Linhas de Código**: ~800 (sem contar JUCE)
- **Classes Criadas**: 8
- **Padrões de Design**: 3 (Strategy, MVC, RAII)
- **Smart Pointers**: 3 tipos (unique_ptr)
- **Interfaces Implementadas**: 4 (Component, MidiInputCallback, Timer, AudioSource)
- **Tempo de Desenvolvimento**: ~3 semanas

---

## 🔮 Possíveis Extensões Futuras

### Novos Desafios
- `IntervalChallenge`: Identificar intervalos musicais
- `ChordChallenge`: Reproduzir acordes
- `RhythmChallenge`: Timing e ritmo preciso

### Melhorias de Interface
- Visualização de partitura
- Temas customizáveis (dark mode, etc)
- Animações avançadas
- Gráficos de progressão

### Funcionalidades Extras
- Sistema de save/load
- Leaderboard local/online
- Modos de jogo diferentes
- Suporte a escalas e modos

### Multiplataforma
- Build para iOS/Android
- Versão web (WebAssembly)

---

## 📚 Referências e Recursos

### Documentação
- [JUCE Framework](https://docs.juce.com/)
- [C++17 Reference](https://en.cppreference.com/)
- [MIDI Specification](https://www.midi.org/)

### Tutoriais JUCE
- [JUCE Tutorial](https://docs.juce.com/master/tutorial_getting_started.html)
- [Audio Programming](https://docs.juce.com/master/tutorial_synth_using_midi_input.html)

### Teoria Musical
- [Music Theory for Computer Musicians](https://www.musictheory.net/)
- [MIDI Note Numbers](https://www.inspiredacoustics.com/en/MIDI_note_numbers_and_center_frequencies)

---

## 🤝 Contribuindo

Este é um projeto acadêmico, mas sugestões e feedback são bem-vindos!

1. Fork o projeto
2. Crie uma branch para sua feature (`git checkout -b feature/NovaIdeia`)
3. Commit suas mudanças (`git commit -m 'Adiciona nova ideia'`)
4. Push para a branch (`git push origin feature/NovaIdeia`)
5. Abra um Pull Request

---

## 📄 Licença

Este projeto foi desenvolvido para fins acadêmicos na disciplina de Programação Orientada a Objetos (C++).

**Uso Educacional Livre** - Pode ser usado como referência para estudos, mas dê os devidos créditos.

---

## 👤 Autor

**Igor Pessoa**

- GitHub: [@IgorPrajnaDev](https://github.com/IgorPrajnaDev)
- Projeto: [interval_guess](https://github.com/IgorPrajnaDev/interval_guess)

---

## 🙏 Agradecimentos

- **JUCE Team** - Pelo excelente framework de áudio
- **Professor Carlos** - Pela orientação no projeto
- **Comunidade C++** - Pela documentação e recursos

---

## 📝 Changelog

### Versão 1.0 (Atual)
- ✅ Sistema de jogo completo
- ✅ Sintetizador de piano realista
- ✅ Suporte MIDI completo
- ✅ Feedback visual com flash colorido
- ✅ Múltiplas tentativas
- ✅ Botão de replay
- ✅ Launcher automático (.bat files)

---

<div align="center">

**🎹 Desenvolvido com paixão por música e código 🎵**

Divirta-se praticando! 🎶

</div>