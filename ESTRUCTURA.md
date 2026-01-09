# Estructura del Codi del Guitar Robot

El projecte està dividit en mòduls per facilitar el manteniment i la neteja:

## 📁 `include/` (Headers)
*   `Config.h`: Tota la configuració de pins, angles dels servos i noms (Edit'l per calibrar).
*   `HAL.h`: Hardware Abstraction Layer. Defineix com ens comuniquem amb els motors.
*   `MidiHandler.h`: Gestió del Bluetooth MIDI.
*   `WebUI.h`: El servidor web de configuració.
*   `Tunings.h`: Base de dades d'afinacions i el gestor de canvi d'aquestes.

## 📁 `src/` (Lògica)
*   `main.cpp`: El cervell. Només inicialitza els mòduls i corre els seus `loop()`.
*   `HAL.cpp`: Implementació real dels motors (mou el PCA9685 i gestiona el temps no bloquejant).
*   `MidiHandler.cpp`: Rep les notes del mòbil i decideix quina corda i trast tocar (Lògica intel·ligent de barra).
*   `WebUI.cpp`: El codi del servidor web i tot el disseny HTML/JS que has vist.

## 📁 `design/` (3D)
*   `barre_unit.scad`: Peça per als trasts (imprimir x4).
*   `bridge_plucker.scad`: Peça per al pont (imprimir x1).
*   `guitar_bot_assembly.scad`: Visualització de com queda tot junt.

## 📁 Altres
*   `platformio.ini`: Configuració del projecte i llista de llibreries (Arduino, MIDI, etc.).
*   `test_ui.html`: Simulador per provar la web a l'ordinador sense el robot.
