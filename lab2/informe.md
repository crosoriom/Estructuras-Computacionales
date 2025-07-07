# **Informe: Laboratorio 2 - Medición y Optimización de Drivers Embebidos**
**Curso:** 4100901 - Estructuras Computacionales
**Autores:**
*   Cristian Camilo Osorio Mesa
*   Santiago Bustamante Montoya

**Fecha:** [Fecha de Entrega]

---

### **1. Introducción**

En este laboratorio se aplicaron conceptos prácticos de medición y optimización de drivers en un sistema embebido basado en el microcontrolador STM32L476RG. El objetivo fue cuantificar el rendimiento de nuestro código base ("Room Control Final 2025_1") y explorar técnicas para mejorar su eficiencia en tiempo de ejecución y consumo de energía. Para ello, se utilizaron herramientas como un analizador lógico para mediciones temporales precisas, un script de Python para la evaluación de latencia de comunicación y un multímetro para analizar el consumo de corriente en diferentes modos de operación del microcontrolador.

### **2. Objetivos**

#### **2.1. Objetivo General**
*   Entender cómo funcionan, se miden y se optimizan los drivers embebidos en un microcontrolador.

#### **2.2. Objetivos Específicos**
*   Medir el tiempo de transmisión de datos a través de un periférico I2C.
*   Analizar cómo los diferentes niveles de optimización del compilador afectan el tiempo de respuesta a eventos.
*   Comparar la latencia de comunicación entre USB y WiFi.
*   Medir y comparar el consumo de corriente del microcontrolador en diferentes modos de operación.

### **3. Herramientas y Configuración**

*   **Hardware:**
    *   Placa NUCLEO-L476RG
    *   Display OLED SSD1306 (conectado a I2C1: PB8-SCL, PB9-SDA)
    *   Teclado matricial 4x4
    *   Analizador Lógico
    *   Multímetro
*   **Software:**
    *   Proyecto base "Room Control Final 2025_1"
    *   Python 3.x (`pyserial`, `socket`)
    *   Software de análisis lógico (ej. Saleae Logic 2)

---

### **4. Procedimientos y Resultados**

A continuación, se detallan los procedimientos y resultados obtenidos para cada una de las tres partes del laboratorio.

#### **4.1. Parte 1: Medición de Tiempo de Transmisión I2C**

**Procedimiento:**
Se utilizó el proyecto base, que actualiza periódicamente un display OLED SSD1306. Se conectaron dos canales del analizador lógico a las líneas I2C1 (PB8 para SCL y PB9 para SDA). Se configuró el software del analizador para decodificar el protocolo I2C y se capturó una transacción completa, correspondiente a la actualización de un *frame* en la pantalla.

**Resultados:**
Se midió la duración total de la transmisión del frame, desde el primer bit de START hasta el último bit de STOP.

*   **Tiempo de transmisión del frame I2C:** **`[Insertar valor medido en ms o µs]`**

**Gráfica Obtenida:**
*La siguiente captura de pantalla muestra la trama I2C completa y la medición del tiempo total.*

![Captura de la transmisión I2C completa](nombre_imagen_i2c.jpg)

#### **4.2. Parte 2: Medición de Latencia y Optimización del Compilador**

**Procedimiento:**
Se utilizó el script `timing_comparison.py` para enviar datos al microcontrolador y medir el tiempo de respuesta (latencia) a través de dos medios: el puerto serie virtual del ST-Link (USB) y un módulo WiFi ESP01 (opcional). Este proceso se repitió para cuatro niveles de optimización del compilador (`-O0`, `-Og`, `-O3`, `-Os`) para evaluar su impacto en el rendimiento del sistema.

**Resultados:**
Se recopilaron los datos de latencia para cada protocolo y nivel de optimización.

**Tabla 1: Comparativa de Latencia por Protocolo (con optimización `-O3`)**
| Protocolo | Latencia Promedio (ms) | Desviación Estándar (ms) | Min (ms) | Max (ms) | Observaciones |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **USB (ST-Link)** | `[Valor]` | `[Valor]` | `[Valor]` | `[Valor]` | `[Observación]` |
| **WiFi (ESP01)** | `[Valor]` | `[Valor]` | `[Valor]` | `[Valor]` | `[Observación]` |

**Tabla 2: Impacto del Nivel de Optimización en la Latencia (USB)**
| Nivel de Optimización | Latencia Promedio (ms) | Diferencia vs. -O0 (%) | Observaciones |
| :--- | :--- | :--- | :--- |
| **-O0** (Sin optimización) | `[Valor]` | 0% | Línea base |
| **-Og** (Debug optimizado) | `[Valor]` | `[Valor]` | `[Observación]` |
| **-O3** (Máxima velocidad) | `[Valor]` | `[Valor]` | `[Observación]` |
| **-Os** (Tamaño optimizado) | `[Valor]` | `[Valor]` | `[Observación]` |

#### **4.3. Parte 3: Medición de Consumo de Corriente**

**Procedimiento:**
Se conectó un multímetro en modo amperímetro en serie con la línea de alimentación de 3.3V de la placa Nucleo. Se midió el consumo de corriente en dos escenarios:
1.  **Modo RUN:** Con el microcontrolador ejecutando activamente el bucle principal del proyecto a 80 MHz.
2.  **Modo SLEEP:** Modificando el código para que el microcontrolador entre en modo de bajo consumo (`WFI` - Wait For Interrupt) y solo se despierte al recibir una interrupción externa (ej. pulsación de tecla).

**Resultados:**
Se observó una diferencia significativa en el consumo de energía entre los dos modos de operación.

| Modo de Operación | Descripción | Consumo de Corriente Medido (mA) |
| :--- | :--- | :--- |
| **Run Mode** | CPU activa a 80 MHz, periféricos encendidos. | `[Insertar valor medido en mA]` |
| **Sleep Mode** | CPU detenida, esperando interrupción externa. | `[Insertar valor medido en mA o µA]` |

---

### **5. Análisis y Conclusiones**

*   **Rendimiento I2C:** *[Análisis pendiente. Se discutirá si el tiempo de transmisión medido es adecuado para la aplicación, y cómo este valor establece una línea base para futuras optimizaciones del driver gráfico.]*

*   **Latencia y Optimización:** *[Análisis pendiente. Se comparará la latencia entre USB y WiFi, explicando las posibles causas de la diferencia (ej. sobrecarga de red). Se analizará el impacto de cada nivel de optimización, destacando la mejora drástica entre -O0 y -O3 y discutiendo el balance entre velocidad (-O3) y tamaño de código (-Os).]*

*   **Consumo de Energía:** *[Análisis pendiente. Se cuantificará la reducción porcentual del consumo al pasar de Modo RUN a Modo SLEEP. Se concluirá sobre la importancia crítica de utilizar modos de bajo consumo en aplicaciones alimentadas por batería y cómo una simple instrucción de software puede tener un impacto masivo en la eficiencia energética del sistema.]*
