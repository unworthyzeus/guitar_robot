# Coses a fer (Full de Ruta)

Aquest document detalla els passos necessaris per completar el projecte, des de l'adquisició de materials fins a l'assemblatge final.

## FASE 1: Adquisició de Material (Components Electrònics)

L'objectiu és construir un sistema per a 6 cordes, amb 4 trasts actius per corda + 1 actuador de pua per corda.
**Total Actuadors: 30** (5 per corda x 6 cordes).

- [ ] **Microcontrolador**:
  - [ ] 1x ESP32 DevKit V1 (Cervell).
  - [ ] 1x Cable Micro-USB (càrrega i dades).

- [ ] **Controladors de Servos (Drivers)**:
  - [ ] 2x Plaques **PCA9685** (Controlador PWM 16 canals I2C).
    - *Nota*: Una controlarà les cordes 1-3, i l'altra les cordes 4-6.

- [ ] **Actuadors (Servos)**:
  - [ ] **Pack de 30x Servos MG90S** (Engranatges metàl·lics).
    - *Recomanació*: Compra'n alguns de recanvi (p. ex. 35 unitats). Els SG90 (blaus de plàstic) no tenen prou força/durabilitat.

- [ ] **Font d'Alimentació (CRÍTIC)**:
  - [ ] 1x **Font d'Alimentació 5V de 10A (o superior)**.
    - Els servos consumeixen molt en arrencar. L'USB de l'ordinador NO serveix.
  - [ ] Jackson Femella o bornes per connectar els cables de la font a les plaques PCA9685.

- [ ] **Material Divers**:
  - [ ] Cables Jumper (Femella-Femella i Mascle-Femella).
  - [ ] Cable elèctric de 0.5mm o 0.75mm per a l'alimentació principal.

---

## FASE 2: Disseny i Impresió 3D

No intentis fer-ho tot de cop. Dissenya i valida per a UNA sola corda.

### A. El Mecanisme de "Dit" (Trasts)
- [ ] **Disseny del suport del servo**:
  - [ ] Ha de permetre muntar els servos de forma compacta (alternats esquerra/dreta si cal per espai).
  - [ ] Sistema de fixació al mànec (abraçadora ajustable).
- [ ] **Punta del dit**:
  - [ ] Dissenya una "peça final" que va al braç del servo.
  - [ ] **Important**: Ha de tenir una superfície tova (TPU o goma enganxada) per prémer la corda sense relliscar ni fer soroll "clac".

### B. El Mecanisme de Pua (Pont)
- [ ] **Suport del pont**:
  - [ ] S'ha de fixar al cos de la guitarra o al pont existent.
- [ ] **Braç de la pua**:
  - [ ] Braç que subjecti una pua real (flexible) o una pua impresa en 3D.

---

## FASE 3: Muntatge i Prototipatge (Iteració 1)

1.  [ ] **Muntatge de prova (1 Corda)**:
    - Munta només els 5 servos de la Corda 1.
    - Connecta'ls al primer PCA9685.
2.  [ ] **Connexió Elèctrica**:
    - Connecta ESP32 -> PCA9685 (GND, SDA, SCL, 3.3V per lògica).
    - Connecta Font Externa -> PCA9685 (Terminals verds V+ i GND).
3.  [ ] **Test de Software**:
    - Configura el codi per activar només la Corda 1.
    - Ajusta els angles `SERVO_MIN` i `SERVO_MAX` a `Config.h` perquè premin la corda sense trencar res.

---

## FASE 4: Producció Final

- [ ] Imprimir les peces per a les 5 cordes restants.
- [ ] Soldar el pont d'adreça (A0) a la segona placa PCA9685 (per tenir l'adreça 0x41).
- [ ] Cablejat final i organització de cables (Cable management).
- [ ] Afinació fina dels angles per a cada servo individualment.
