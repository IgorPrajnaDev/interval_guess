# ?? Diagrama de Classes UML - MeloQuiz

**Projeto:** MeloQuiz - Jogo de Treinamento de Ouvido Musical  
**Autor:** Igor Pessoa  
**Data:** Outubro 2025

---

## ?? Vis�o Geral da Arquitetura

```
???????????????????????????????????????????????????????????????????????
?                         PADR�O MVC                                   ?
???????????????????????????????????????????????????????????????????????
?  MODEL                 CONTROLLER              VIEW                 ?
?  ?? GameEngine         ?? MainComponent        ?? MidiKeyboard      ?
?  ?? Challenge          ?? ...                  ?   Component         ?
?  ?? MelodyChallenge                            ?? VirtualKeyboard   ?
?  ?? Note                                           Component         ?
?  ?? NoteSequence                                                     ?
???????????????????????????????????????????????????????????????????????
```

---

## ?? Diagrama UML Completo

```plantuml
@startuml MeloQuiz Class Diagram

' ============================================================================
' CLASSES DO MODELO (MODEL)
' ============================================================================

package "Model" {
    
    ' Classe Note
    class Note {
        - midiNoteNumber : int
        - velocity : int
        --
        + Note(midiNote: int, noteVelocity: int)
        + getMidiNoteNumber() : int
        + getVelocity() : int
        + operator==(other: Note) : bool
    }
    
    ' Classe NoteSequence
    class NoteSequence {
        - notes : vector<Note>
        --
        + NoteSequence()
        + addNote(note: Note) : void
        + clear() : void
        + size() : size_t
        + getNotes() : vector<Note>&
    }
    
    ' Classe Challenge (Abstrata)
    abstract class Challenge {
        # targetSequence : NoteSequence
        --
        + ~Challenge() {virtual}
        + {abstract} generateSequence(difficulty: int) : void
        + {abstract} isCorrect(userSequence: NoteSequence) : bool
        + getTargetSequence() : NoteSequence&
    }
    
    ' Classe MelodyChallenge
    class MelodyChallenge {
        --
        + generateSequence(difficulty: int) : void {override}
        + isCorrect(userSequence: NoteSequence) : bool {override}
    }
    
    ' Classe GameEngine
    class GameEngine {
        - currentChallenge : unique_ptr<Challenge>
        - userInputSequence : NoteSequence
        - score : int
        - current_challenge_size : int
        --
        + GameEngine()
        + startNewGame(notes_quantity: int) : void
        + nextLevel() : void
        + processUserInput(note: Note) : void
        + checkUserSequence() : bool
        + getUserInputSequence() : NoteSequence&
        + clearUserInput() : void
        + getScore() : int
        + getCurrentTargetSequence() : NoteSequence&
    }
    
    ' Rela��es do Modelo
    NoteSequence "1" *-- "0..*" Note : cont�m
    Challenge "1" *-- "1" NoteSequence : possui targetSequence
    MelodyChallenge --|> Challenge : herda
    GameEngine "1" *-- "1" NoteSequence : possui userInputSequence
    GameEngine "1" o-- "0..1" Challenge : possui (unique_ptr)
}

' ============================================================================
' CLASSES DE �UDIO
' ============================================================================

package "Audio" {
    
    ' Classe SimpleSineSound
    class SimpleSineSound {
        --
        + SimpleSineSound()
        + appliesToNote(noteNumber: int) : bool {override}
        + appliesToChannel(midiChannel: int) : bool {override}
    }
    
    ' Classe SimpleSineVoice
    class SimpleSineVoice {
        - currentAngle : double
        - angleDelta : double
        - level : double
        - tailOff : double
        - currentNoteNumber : int
        - adsr : ADSR
        --
        + SimpleSineVoice()
        + canPlaySound(sound: SynthesiserSound*) : bool {override}
        + startNote(midiNoteNumber: int, velocity: float, ...) : void {override}
        + stopNote(velocity: float, allowTailOff: bool) : void {override}
        + pitchWheelMoved(newPitchWheelValue: int) : void {override}
        + controllerMoved(controllerNumber: int, newValue: int) : void {override}
        + renderNextBlock(outputBuffer: AudioBuffer<float>&, ...) : void {override}
    }
    
    ' Rela��es de heran�a JUCE
    SimpleSineSound --|> "juce::SynthesiserSound" : herda
    SimpleSineVoice --|> "juce::SynthesiserVoice" : herda
}

' ============================================================================
' CLASSES DE INTERFACE/CONTROLLER
' ============================================================================

package "View/Controller" {
    
    ' Classe VirtualKeyboardComponent
    class VirtualKeyboardComponent {
        - keyStates : vector<bool>
        --
        + VirtualKeyboardComponent()
        + ~VirtualKeyboardComponent() {override}
        + paint(g: Graphics&) : void {override}
        + resized() : void {override}
        + setKeyPressed(midiNote: int, isDown: bool) : void
    }
    
    ' Classe MainComponent (Controller Principal)
    class MainComponent {
        - gameEngine : GameEngine
        - keyboardState : MidiKeyboardState
        - keyboardComponent : MidiKeyboardComponent
        - midiInput : unique_ptr<MidiInput>
        - startButton : TextButton
        - replayButton : TextButton
        - numNotesSlider : Slider
        - numNotesLabel : Label
        - flashColour : Colour
        - flashOpacity : float
        - audioDeviceManager : AudioDeviceManager
        - synth : Synthesiser
        - midiCollector : MidiMessageCollector
        - audioSourcePlayer : AudioSourcePlayer
        - sequenceBuffer : MidiBuffer
        - sequencePosition : int
        - isPlayingSequence : bool
        - sampleRate : double
        --
        + MainComponent()
        + ~MainComponent() {override}
        + handleIncomingMidiMessage(source: MidiInput*, message: MidiMessage&) : void {override}
        + paint(g: Graphics&) : void {override}
        + resized() : void {override}
        + prepareToPlay(samplesPerBlockExpected: int, sampleRate: double) : void {override}
        + getNextAudioBlock(bufferToFill: AudioSourceChannelInfo&) : void {override}
        + releaseResources() : void {override}
        - timerCallback() : void {override}
        - playSequence() : void
    }
    
    ' Rela��es de heran�a JUCE
    VirtualKeyboardComponent --|> "juce::Component" : herda
    MainComponent --|> "juce::Component" : herda
    MainComponent ..|> "juce::MidiInputCallback" : implementa
    MainComponent ..|> "juce::Timer" : implementa
    MainComponent ..|> "juce::AudioSource" : implementa
    
    ' Rela��es de composi��o
    MainComponent "1" *-- "1" GameEngine : possui
    MainComponent "1" *-- "1" "juce::MidiKeyboardComponent" : possui
    MainComponent "1" o-- "0..1" VirtualKeyboardComponent : pode usar
    MainComponent "1" *-- "1" "juce::Synthesiser" : possui
}

' ============================================================================
' RELA��ES ENTRE PACOTES
' ============================================================================

MainComponent ..> Note : usa
MainComponent ..> NoteSequence : usa
MainComponent ..> SimpleSineVoice : cria
MainComponent ..> SimpleSineSound : cria
"juce::Synthesiser" ..> SimpleSineVoice : gerencia

' ============================================================================
' NOTAS E LEGENDAS
' ============================================================================

note right of Challenge
  **Padr�o Strategy**
  Classe abstrata que define
  interface para diferentes
  tipos de desafios
end note

note right of GameEngine
  **Motor do Jogo**
  Gerencia estado do jogo,
  pontua��o e desafios
  usando Smart Pointers
end note

note right of MainComponent
  **Controller MVC**
  Coordena Model e View
  Implementa m�ltiplas
  interfaces JUCE
end note

note right of SimpleSineVoice
  **Sintetizador de Piano**
  Gera som usando 7 harm�nicos
  com envelope ADSR din�mico
end note

@enduml
```

---

## ?? Legenda de Nota��es UML

### Tipos de Rela��es

| S�mbolo | Significado | Exemplo no Projeto |
|---------|-------------|-------------------|
| `??>` | **Associa��o** | MainComponent usa Note |
| `*??` | **Composi��o** (forte) | NoteSequence cont�m Notes |
| `o??` | **Agrega��o** (fraca) | GameEngine possui Challenge via unique_ptr |
| `???` | **Heran�a/Generaliza��o** | MelodyChallenge herda de Challenge |
| `..?` | **Implementa��o de Interface** | MainComponent implementa MidiInputCallback |
| `..>` | **Depend�ncia** | MainComponent depende de SimpleSineVoice |

### Modificadores de Visibilidade

| S�mbolo | Significado |
|---------|-------------|
| `+` | **public** |
| `-` | **private** |
| `#` | **protected** |

### Tipos de M�todos

| Nota��o | Significado |
|---------|-------------|
| `{abstract}` | M�todo virtual puro |
| `{override}` | Sobrescreve m�todo da classe base |
| `{virtual}` | M�todo virtual |

---

## ?? Diagrama de Composi��o Simplificado

```
MainComponent
    ??? possui (composi��o)
    ?   ??? GameEngine
    ?   ?   ??? possui (composi��o)
    ?   ?   ?   ??? NoteSequence (userInputSequence)
    ?   ?   ?       ??? cont�m m�ltiplos Note
    ?   ?   ??? possui (agrega��o via unique_ptr)
    ?   ?       ??? Challenge (pode ser MelodyChallenge)
    ?   ?           ??? possui (composi��o)
    ?   ?               ??? NoteSequence (targetSequence)
    ?   ?                   ??? cont�m m�ltiplos Note
    ?   ?
    ?   ??? MidiKeyboardComponent (JUCE)
    ?   ??? MidiKeyboardState (JUCE)
    ?   ??? Synthesiser (JUCE)
    ?   ?   ??? cont�m m�ltiplas SimpleSineVoice
    ?   ?   ??? cont�m SimpleSineSound
    ?   ??? AudioDeviceManager (JUCE)
    ?   ??? AudioSourcePlayer (JUCE)
    ?   ??? MidiMessageCollector (JUCE)
    ?   ?
    ?   ??? TextButton (startButton)
    ?   ??? TextButton (replayButton)
    ?   ??? Slider (numNotesSlider)
    ?   ??? Label (numNotesLabel)
    ?
    ??? herda de
        ??? juce::Component
        ??? implementa interfaces
            ??? juce::MidiInputCallback
            ??? juce::Timer
            ??? juce::AudioSource
```

---

## ?? Padr�es de Design Identificados

### 1. **Strategy Pattern** ?
```
Challenge (interface abstrata)
    ??? MelodyChallenge (implementa��o concreta)
    ??? [futuro] IntervalChallenge
    ??? [futuro] ChordChallenge
```

**Benef�cio:** Facilita adi��o de novos tipos de desafios sem modificar GameEngine

### 2. **MVC Pattern** ?
```
Model: GameEngine, Challenge, Note, NoteSequence
View: MidiKeyboardComponent, VirtualKeyboardComponent
Controller: MainComponent
```

**Benef�cio:** Separa��o clara de responsabilidades

### 3. **RAII (Resource Acquisition Is Initialization)** ?
```cpp
std::unique_ptr<Challenge> currentChallenge;
std::unique_ptr<juce::MidiInput> midiInput;
```

**Benef�cio:** Gerenciamento autom�tico de mem�ria, sem leaks

### 4. **Composition over Inheritance** ?
```
MainComponent HAS-A GameEngine
NoteSequence HAS-A vector<Note>
GameEngine HAS-A Challenge
```

**Benef�cio:** Maior flexibilidade e menor acoplamento

---

## ?? Diagrama de Sequ�ncia - Fluxo do Jogo

```
Usu�rio ? MainComponent ? GameEngine ? Challenge ? NoteSequence
   |           |              |            |            |
   |  click   |              |            |            |
   |--------->|startNewGame()|            |            |
   |          |------------->|generateSeq()|           |
   |          |              |----------->|addNote()   |
   |          |              |            |---------->|
   |          |playSequence()|            |            |
   |          |<-------------|            |            |
   |          |              |            |            |
   | toca nota|              |            |            |
   |--------->|processInput()|            |            |
   |          |------------->|addNote()   |            |
   |          |              |---------->|            |
   |          |              |checkSeq()  |            |
   |          |              |<-----------|            |
   |  flash   |              |            |            |
   |<---------|repaint()     |            |            |
```

---

## ?? Depend�ncias entre Classes

### Depend�ncias Fortes (Composi��o)
- `NoteSequence` ? `Note` (cont�m)
- `GameEngine` ? `NoteSequence` (possui)
- `Challenge` ? `NoteSequence` (possui)
- `MainComponent` ? `GameEngine` (possui)

### Depend�ncias Fracas (Agrega��o/Uso)
- `GameEngine` ? `Challenge` (via unique_ptr)
- `MainComponent` ? `Note` (cria temporariamente)
- `MainComponent` ? `SimpleSineVoice` (cria e adiciona ao Synthesiser)

### Heran�a
- `MelodyChallenge` ? `Challenge`
- `SimpleSineVoice` ? `juce::SynthesiserVoice`
- `SimpleSineSound` ? `juce::SynthesiserSound`
- `MainComponent` ? `juce::Component`
- `VirtualKeyboardComponent` ? `juce::Component`

---

## ?? Cardinalidade das Rela��es

| Rela��o | Cardinalidade | Descri��o |
|---------|---------------|-----------|
| NoteSequence ? Note | 1 para 0..* | Uma sequ�ncia cont�m zero ou mais notas |
| GameEngine ? Challenge | 1 para 0..1 | GameEngine possui zero ou um desafio ativo |
| MainComponent ? GameEngine | 1 para 1 | MainComponent sempre possui um GameEngine |
| Synthesiser ? SimpleSineVoice | 1 para 8 | Sintetizador tem exatamente 8 vozes |

---

## ?? Cores e Organiza��o (para diagramas visuais)

```
????????????????????
? MODEL (Azul)     ? ? Classes de l�gica e dados
????????????????????
? VIEW (Verde)     ? ? Classes de interface visual
????????????????????
? CONTROLLER (Roxo)? ? Classes de controle e coordena��o
????????????????????
? AUDIO (Laranja)  ? ? Classes de s�ntese de �udio
????????????????????
```

---

## ?? Como Usar Este Diagrama

### Para visualizar em ferramentas UML:

1. **PlantUML** (Recomendado)
   - Instale a extens�o PlantUML no VS Code
   - Copie o c�digo entre `@startuml` e `@enduml`
   - Visualize o diagrama renderizado

2. **Draw.io / Lucidchart**
   - Use o diagrama textual como refer�ncia
   - Recrie manualmente com as ferramentas visuais

3. **Visual Paradigm / StarUML**
   - Importar c�digo PlantUML diretamente

---

## ?? Refer�ncias

- [UML Class Diagram Tutorial](https://www.visual-paradigm.com/guide/uml-unified-modeling-language/uml-class-diagram-tutorial/)
- [PlantUML Documentation](https://plantuml.com/class-diagram)
- [C++ Design Patterns](https://refactoring.guru/design-patterns/cpp)

---

**Diagrama criado por:** Igor Pessoa  
**�ltima atualiza��o:** Outubro 2025  
**Vers�o:** 1.0
