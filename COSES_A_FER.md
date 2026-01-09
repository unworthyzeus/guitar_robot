# Coses a fer (Full de Ruta Detallat)

Aquest document detalla exactament què necessites i com muntar-ho.

## 1. LLISTA DE LA COMPRA (Material Exacte)

### Electrònica Principal
- [ ] **1x ESP32 DevKit V1**: El cervell.
- [ ] **2x Mòduls PCA9685**: Controladors de 16 canals PWM (Interfície I2C).
  - *Important*: Necessites dos perquè tens 30 motors i una sola placa només en controla 16.
- [ ] **1x Font d'Alimentació 5V 10A (o superior)**: 
  - *Tipus recomanat*: Font commutada metàl·lica (tipus LED/Industrial) o "Power Brick" d'alta potència.
  - *NO serveix*: Carregadors de mòbil (solen ser 2A, insuficient).
- [ ] **1x Adaptador DC Femella amb bornes de cargol**: Per connectar els cables de la font fàcilment.

### Actuadors (Motors)
- [ ] **30x Servos MG90S (Metal Gear)**:
  - Necessaris 30 unitats instal·lades. Compra'n **35** per tenir recanvis.
  - *Per què MG90S?*: Els engranatges metàl·lics aguanten la tensió de prémer la corda. Els SG90 (blaus) es trencaran de seguida.

### Cablejat
- [ ] **Cable Vermell i Negre Gruixut (18 AWG o 0.75mm²)**: Per portar els 5V 10A des de la font fins a les plaques PCA9685.
- [ ] **Cables Jumper Dupont (Femella-Femella)**: Per connectar l'ESP32 a les plaques PCA.
- [ ] **Estany i Soldador**: Per canviar l'adreça de la segona placa PCA.

---

## 2. MODIFICACIÓ DE HARDWARE (Abans de muntar)

### Preparar la placa PCA9685 núm. 2
Perquè l'ESP32 pugui parlar amb dues plaques diferents, han de tenir "noms" (adreces) diferents.
1.  Agafa UNA de les plaques PCA9685.
2.  Busca els pads (gotetes d'estany) marcats com **A0** a la cantonada superior dreta.
3.  Pos'hi una gota d'estany unnt els dos pads de **A0**.
4.  Ara aquesta placa tindrà l'adreça `0x41` (l'altra sense soldar serà `0x40`).

---

## 3. LÒGICA DE CONNEXIÓ (Esquema Mental)

No connectis els servos a l'ESP32 directament. Es cremarà.

- **Dades (Control)**: ESP32 -> PCA9685
  - ESP32 3V3 -> PCA VCC (Part lògica)
  - ESP32 GND -> PCA GND
  - ESP32 D21 -> PCA SDA
  - ESP32 D22 -> PCA SCL
  - *Connecta les dues PCA en cadena (mira els pins dels costats) o en paral·lel als mateixos pins de l'ESP32.*

- **Potència (Força)**: Font 10A -> PCA9685
  - Font V+ -> PCA Borna Verda V+
  - Font GND -> PCA Borna Verda GND
  - *Has de portar cable gruixut a les bornes verdes de LES DUES plaques.*

---

## 4. ESTRUCTURA MECÀNICA (Què cal imprimir/dissenyar)

### A. Mòdul del Mànec (Trasts)
Necessites construir 6 torres (una per corda). Cada torre porta 4 servos.
- **Repte d'espai**: Els trasts estan molt junts. No pots posar 4 servos en línia recta un darrera l'altre tocant la fusta.
- **Solució Suggerida**: Fes una estructura en "V" o alternada.
  - Servo 1 (Trast 1): Muntat a l'esquerra del mànec.
  - Servo 2 (Trast 2): Muntat a la dreta del mànec.
  - Així els cossos dels motors no xoquen.

### B. Mòdul del Pont (Pua/Dits)
Necessites 6 servos alineats al pont.
- Pots dissenyar un "Rail" únic que s'enganxi darrera el pont de la guitarra.
- Cada servo mou un petit braç que "bufa" o colpeja la corda.

---

## 5. PAS A PAS PEL MUNTATGE

1.  **Validació Unitària**:
    - Connecta 1 sol servo a la placa `0x40`, canal 0.
    - Carrega el codi.
    - Envia una nota MIDI. Es mou? Perfecte.

2.  **Validació de Potència**:
    - Connecta 5 servos (una corda completa).
    - Fes-los moure tots a la vegada. Si l'ESP32 es reinicia o el LED parpelleja, tens un problema amb la Font d'Alimentació.

3.  **Instal·lació Mecànica**:
    - Comença per la **Corda 1 (E agut)**. És la més fàcil de fer sonar.
    - Munta els 4 servos de trasts i el de la pua.
    - Calibra els angles al fitxer `Config.h` (SERVO_MIN, SERVO_MAX) perquè premin la corda just el necessari per fer nota neta (sense apagar el so per massa pressió).

4.  **Escalar**:
    - Un cop la Corda 1 funciona perfecte, repeteix per a les altres 5.
