# Full de Ruta i Manual d'Assemblatge (Guitar Robot)

> **Arquitectura**: 10 Motors (4 Barres de Trast + 6 Pues)

## FASE 1: Aprovisionament (Components i Material)

### Electrònica
- [ ] **1x ESP32 DevKit V1**.
- [ ] **1x Mòdul PCA9685** (Controlador PWM 16 canals).
- [ ] **1x Font d'Alimentació 5V 5A (mínim)**.
  - *Tipus*: Font commutada (preferible) o "Power Brick".
  - *Cable*: Jack DC femella amb bornes o adaptador.
- [ ] **Cables**:
  - Cable 22AWG (vermell/negre) per alimentació.
  - Cables Jumper (Femella-Femella) per a l'ESP32.

### Actuadors
- [ ] **10x Servos MG90S (Metal Gear) - Versió 180º**.
  - *Molt Important*: Han de ser de **180 graus (Posicionals)**.
  - Els de 360 graus (rotació contínua) **NO** serveixen.
  - *Nota*: Compra'n 12-14 per tenir recanvis.
  - *Nota2*: Han de ser petits per cabre-hi

### Ferreteria i Diversos
- [ ] **Cargols M2 o M2.5** (per muntar els servos a les peces impreses).
- [ ] **Cargols M3 o M4 llargs** (per tancar l'abraçadora del mànec).
- [ ] **Goma EVA o Feltre** (adhesiu): Per posar a la cara interna de les abraçadores i protegir la fusta de la guitarra.
- [ ] **Goma duresa mitjana**: Per posar sota les "Barres de Trast" i que premin bé les cordes sense fer soroll metàl·lic.

---

## FASE 2: Impressió 3D

Tens els fitxers font a la carpeta `design/`. Utilitza OpenSCAD per generar els STL.

### A. Unitats de Barra (Trasts)
- [ ] **Generar STL**: Obre `design/barre_unit.scad`.
- [ ] **Imprimir X4 unitats**:
  - *Material*: PLA o PETG (millor PETG per resistència).
  - *Emplenat (Infill)*: 40% mínim (han de ser rígides).
  - *Suports*: Sí, necessaris per a l'arc.

### B. Unitat del Pont (Pues)
- [ ] **Generar STL**: Obre `design/bridge_plucker.scad`.
- [ ] **Imprimir X1 unitat**:
  - *Material*: PLA està bé.

---

## FASE 3: Muntatge Mecànic

1.  **Preparació Pesa Trasts**:
    - [ ] Enganxa el feltre/goma EVA a l'interior de l'arc del `BarreUnit`.
    - [ ] Munta el Servo MG90S a la torre lateral.
    - [ ] Fabrica la "Barra" física (pot ser un tros de plàstic imprès o un pal de gelat reforçat) i uneix-la al braç del servo. Enganxa-hi goma a sota.

2.  **Instal·lació al Mànec**:
    - [ ] Col·loca la **Unitat 1** sobre el Trast 1.
    - [ ] Col·loca la **Unitat 2** sobre el Trast 2...
    - [ ] Assegura les abraçadores amb els cargols M3/M4, però sense escanyar la guitarra.

3.  **Preparació Pesa Pont**:
    - [ ] Munta els 6 servos al `BridgePluckerArray`.
    - [ ] Posa'ls-hi un petit "dit" o pua al braç del servo.

---

## FASE 4: Cablejat Electrònic

Seguim l'esquema de pins del programa (`Config.h`):

**Connexions al PCA9685:**
| Servo | Port PCA |
|-------|----------|
| Pua C1 (Aguda) | 0 |
| Pua C2 | 1 |
| Pua C3 | 2 |
| Pua C4 | 3 |
| Pua C5 | 4 |
| Pua C6 (Greu) | 5 |
| **Barra Trast 1** | **6** |
| Barra Trast 2 | 7 |
| Barra Trast 3 | 8 |
| Barra Trast 4 | 9 |

**Connexions MCU:**
- ESP32 (GND/3V3/SDA/SCL) -> PCA9685.
- Font Alimentació (5V/GND) -> Borna Verda PCA9685.

---

## FASE 5: Calibració Software

1.  **Càrrega Inicial**:
    - [ ] Puja el firmware amb PlatformIO.
    - [ ] Tots les barres s'haurien d'aixecar (`BAR_UP`) i les pues centrar-se (`PLUCK_REST`).

2.  **Ajust d'Angles (`Config.h`)**:
    - Si les barres no baixen prou per prémer la corda, augmenta `BAR_DOWN`.
    - Si les barres baixen massa i xoquen amb la fusta, redueix `BAR_DOWN`.
    - Fes el mateix per a `PLUCK_HIT` fins que la pua toqui la corda netament.

3.  **Prova de So**:
    - [ ] Connecta el mòbil per Bluetooth ("GuitarBot_ESP32").
    - [ ] Envia una nota MIDI i gaudeix!
