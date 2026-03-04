# Technical Report

Project: Automated Hot Beverage Machine (Coffee and Tea)
========================================================

<h3>Repository: <a href="https://github.com/PauloBelmont/maquina_de_bebidas">PauloBelmont/maquina_de_bebidas</a></h3>

**Authors:** Andersson Silva Pereira, Matheus Rebouças de Lima, Paulo César Pereira Belmont, Sarah Evelyn do Vale Silva ([GitHub][1])
**License:** MIT ([GitHub][1])
**Date:** *January / 2026*
**CAPES Category:** Prototype, system, artifact, or computational process with functional validation.

**Report Objective:** Document technological architecture, innovation, components, and system operation.

**Justification:**

- Functional embedded system, not merely conceptual;
- Integrates hardware–software integration;
- Possesses technical documentation (report, UML diagrams, architecture);
- Is reproducible and subject to experimental validation.

---

## 📌 1. Project Overview

The &quot;<b>Automated Hot Beverage Machine</b>&quot; project consists of an **embedded system for autonomous acquisition and preparation of hot beverages** (coffee and tea), integrating sensors, actuators, and human-machine interface (HMI). The system was developed with focus on educational applications and rapid prototyping in the context of embedded systems.

---

## 📐 2. Solution Scope

This technical report addresses:

* **Technological architecture** of the system (hardware and software);
* **Control methods and user interface**;
* **Technical innovation and technological differentiation**;
* **Relevance for R&D&I**;
* **Conclusions and evolution recommendations**.

---

### 📚 2.1 Related Work & Theoretical Analysis

To validate the architectural decisions of this project, a comparative study was conducted against four academic references (A1-A4), analyzing commonalities in embedded automation and differences in application scope (3D printing, education, energy efficiency).

*   **[📄 Read the Full Literature Comparison](literature_comparison.md)**

Furthermore, the core control logic relies on a Finite State Machine (FSM). A detailed analysis of states, transitions, and safety conditions is available in the dedicated analysis document.

*   **[⚙️ Read the FSM Logic Analysis](fsm_analysis.md)**

---

## 🏗️ 3. Technological Architecture

### 🔌 3.1 Hardware

The machine utilizes a hybrid architecture combining electronic modules, detection sensors, and user interface:

| Component                          | Primary Function                            |
| ---------------------------------- | ------------------------------------------- |
| **ESP32 Devkit V1**                | Main control unit (MCU)                     |
| **Display Nextion NX3224T024_011** | Graphical interface and touchscreen (HMI)   |
| **Ultrasonic Sensors HC-SR04**     | Coin insertion and cup presence detection   |
| **Peristaltic Pumps**              | Precise liquid dispensing                   |
| **Relay Module SRD-05VDC-SL-C**    | Pump control / power management             |
| **Power Supply (12V Battery)**     | System power                                |
| **3D-Printed Reservoirs**          | Physical liquid storage                     |

**Integration Description:**
The **ESP32** orchestrates all modules, receiving sensor signals and commanding dosage pumps, ensuring the machine's operation cycle executes sequentially and safely. Event detection (such as coin insertion or cup presence) is performed by ultrasound, executing periodic readings and activations according to defined states.

---

### 🧠 3.2 Software

#### 🗂️ 3.2.1 Firmware (ESP32)

The firmware is **developed in C++ for the ESP32**, with logic based on **Finite State Machine (FSM)** to control operation phases:

1. **Initialization:** sensor and display setup
2. **Coin awaiting:** ultrasonic sensor detects insertion
3. **Cup detection:** second sensor reading
4. **Beverage selection:** via touchscreen interface
5. **Dispensing:** activation of corresponding pump
6. **Cycle completion:** cup removal detection and reset

This state pattern improves safety and reliability of the preparation process.

---

### 🎨 3.2.2 Human-Machine Interface (Nextion)

The graphical interface is designed to:

* Enable intuitive beverage selection;
* Display user feedback (e.g., progress, error messages);
* Connect via UART serial to the ESP32.

The use of **Nextion** display facilitates building complex interfaces without overloading the main microcontroller.

---

## 🚀 4. Technological Innovation

This project presents innovative aspects for educational applications and prototyping:

### ✓ Finite State Machine (FSM) Usage

FSM adoption organizes control flow and ensures robustness against asynchronous events, an essential factor in reactive embedded systems.

---

### ✓ Ultrasonic Sensing

The use of **ultrasonic sensors for coin and cup detection** is more precise and less susceptible to failures than simple mechanical switches.

---

### ✓ Agile Interface with Nextion

The separation between control (ESP32) and graphical interface (Nextion) is an architectural characteristic that **improves modularity and scalability**, especially when compared with systems implementing UI directly in firmware.

---

### ✓ 3D Printing Applied to Prototyping

The use of **3D-printed components** for physical structure enables rapid adjustments and design customization, a differentiator for R&D of interactive prototypes and IoT systems.

---

## 📈 5. Innovation Characteristics and Impact

This project demonstrates **an advance in teaching embedded systems and sensory integration with actuators**. Furthermore:

* ESP32 use incorporates future connectivity (Wi-Fi/Bluetooth) for telemetry or remote control;
* Architectural modularity facilitates evolution (e.g., NFC payment, cloud integration, consumption tracking, real-time dashboards);
* Can evolve into a minimum viable product in commercial environments or events.

This foundation is comparable to beverage automation trends, where intelligent autonomous systems address automated preparation and dispensing solutions — technically relevant in the current context of automation and services. ([TecMundo][2])

---

## 🧩 6. Conceptual Diagram (Text)

```
[Ultrasonic Sensor 1]       [Ultrasonic Sensor 2]
           │                        │
           └──────> ESP32 MCU <─────┘
                    │   ↓   ↑
                    │    Nextion Display
                    ↓
           [Power Relay] ──→ [Peristaltic Pumps]
                    ↓
              Dispensing System
```

## 🔄 7. System Architecture Diagram

```
┌─────────────────────────────────────────────────────┐
│              ESP32 Firmware (FSM)                   │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐          │
│  │ Sensor   │  │ Actuator │  │ Serial   │          │
│  │ Manager  │  │Controller│  │ Comm     │          │
│  └──────────┘  └──────────┘  └──────────┘          │
└────────┬──────────────┬──────────────┬──────────────┘
         │              │              │
    ┌────▼─┐       ┌────▼──┐      ┌───▼────┐
    │Sensors│      │Relay  │      │Nextion │
    │(HC-   │      │Module │      │Display │
    │SR04)  │      │       │      │(UI)    │
    └───────┘      └───┬───┘      └────────┘
                       │
                   ┌───▼─────┐
                   │ Pumps   │
                   │(Liquid) │
                   └─────────┘
```

**Componentes:**
- **ESP32:** Orquestra sensores e atuadores via FSM
- **Sensores Ultrassônicos:** Detectam moedas e xícara
- **Módulo Relé:** Controla ativação das bombas
- **Display Nextion:** Interface gráfica via UART

## 8. Sequence Diagram (Main Scenario)

```
mermaid
sequenceDiagram
    ```
    mermaid
    sequenceDiagram
        participant U as User
        participant D as Nextion Display
        participant E as ESP32
        participant S as Sensors
        participant P as Pump

        U ->> D: Interacts with interface
        S ->> E: Coin detected
        E ->> D: Requests cup insertion
        S ->> E: Cup detected
        U ->> D: Selects Coffee
        D ->> E: Sends command
        E ->> P: Activates coffee pump
        P -->> E: Dispensing completed
        E ->> D: Displays final message
    ```
```

---

## 🧪 9. Technical Evaluation

### ✔ Strengths

* Modularity between control and interface;
* Detection robustness and operation cycle;
* Potential for IoT extension;
* Software pattern usage (FSM).

### ⚠ Limitations

* Lack of integration with modern payment methods;
* Absence of transaction logging or telemetry;
* Current scope limited to educational prototype.

---

## 📌 10. Recommendations for Evolution

| Future Improvements         | Expected Benefits                   |
| --------------------------- | ----------------------------------- |
| **NFC Payment**             | Modernizes usage and accessibility  |
| **Cloud Connectivity**      | Telemetry and consumption dashboards|
| **Mobile App Control**      | Remote interaction and personalization|
| **Machine Learning**        | Stock prediction or recommendation  |

---

## 🏁 11. Conclusion

The **Automated Beverage Machine** project is a solid example of embedded systems prototyping with **sensory integration, reactive control, and user-friendly interface**. Its modular technological architecture and use of emerging technologies make it a relevant case for **R&D&I in IoT, physical automation, and advanced prototyping**.

---

## 📚 12. References

1. Project GitHub repository: *PauloBelmont/maquina_de_bebidas* – README and code structure. ([GitHub][1])
2. Articles on ultrasonic sensors and FSM in embedded systems.
3. Comparison with autonomous beverage machines and automation trends. ([TecMundo][2])

---

[1]: https://github.com/PauloBelmont/maquina_de_bebidas "GitHub - PauloBelmont/maquina_de_bebidas: Este repositório contém todos os artefatos relacionados ao projeto maquina de chafé"
[2]: https://www.tecmundo.com.br/tecnologia/47158-10-bartenders-robos-que-fazem-drinks-tao-bem-quanto-os-humanos.htm?utm_source=chatgpt.com "10 bartenders robôs que fazem drinks tão bem quanto os humanos | Internet"

