# Millores i Evolució del Projecte

Un cop tinguis la versió bàsica (MVP) funcionant, aquí tens idees per portar el Guitar Robot al següent nivell.

## 1. Software i Firmware (Cost 0€)

### 🎛️ Calibració via WiFi (Molt Recomanat)
- **Problema**: Ara, per canviar l'angle d'un servo (si prem massa fort o fluix), has de modificar `Config.h` i tornar a pujar el codi. És lent.
- **Solució**: Crear una petita web (WebServer) dins l'ESP32.
- **Com funciona**: Et connectes amb el mòbil a "GuitarBot_Setup", obres una web i tens lliscadors (sliders) per ajustar cada servo en temps real. Els valors es guarden a la memòria permanent (EEPROM).

### 🎹 Sensibilitat (Velocity)
- **Problema**: Ara el robot sempre toca amb la mateixa força.
- **Solució**: Utilitzar el paràmetre `velocity` del MIDI.
- **Implementació**: Si la velocitat és 127 (màxim), la pua es mou més angle (`PLUCK_HIT_HARD`). Si és 40, es mou menys (`PLUCK_HIT_SOFT`). Això donarà "vida" a la música.

### 🎸 Mode "Strumming" (Rasgueo)
- **Problema**: El MIDI sol disparar notes individuals.
- **Solució**: Detectar acords i fer que els servos de les pues es disparin en cascada ràpida (Corda 6 -> 1) per simular un rasgueo de pua real, en lloc de sonar totes perfectament alhora com un piano.

## 2. Electrònica (Cost Baix/Mitjà)

### 🔋 Portabilitat (Bateries)
- Afegir un suport per a 2x Bateries 18650 (Li-Ion) i un mòdul BMS.
- Això permetria tocar al carrer sense endollar-lo a la paret.

### 📟 Pantalla d'Estat (OLED)
- Afegir una petita pantalla OLED I2C (0.96").
- **Funcions**: Mostrar la IP del WiFi, l'acord que està sonant (ex: "C Major") o l'estat de la connexió Bluetooth.

### 🌈 Llums Reactius (LEDs)
- Posar una tira de LED RGB (WS2812B) al costat de les barres.
- Il·luminar la barra que està prement el robot. Queda espectacular en directe/vídeos.

## 3. Mecànica (Cost Alt / Dificultat Alta)

### 🔇 Sistema d'Apagat (Damping)
- **El repte**: Una guitarra real necessita que paris les cordes amb la mà dreta perquè no es barregin els sons. El robot actual no sap "callar" una corda.
- **Solució**: Afegir una barra tova accionada per un servo extra que toqui lleugerament totes les cordes per silenciar-les (Mute).

### 🤖 Capo Automàtic (Eix Z)
- En lloc de tenir 4 barres fixes, tenir un sol robot que es mogui amunt i avall del mànec sobre uns rails (Motor Pas a Pas + Cargol sense fi).
- **Avantatge**: Pots tocar en qualsevol trast (0-12).
- **Desaventatge**: És lent canviant de posició (no pots fer solos ràpids) i mecànicament molt complex.
