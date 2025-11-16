# Juego SIMÓN 2 Jugadores con Arduino

Este proyecto implementa una versión mejorada del clásico **SIMÓN**, ahora con **modo de 2 jugadores**, usando Arduino, LEDs, pulsadores y un buzzer.

Cada jugador tiene su **propia secuencia**, su **propio turno** y sus **propios LEDs de indicación**.  
El objetivo es superar rondas sin equivocarse, y al fallar uno, **gana el otro**.

---

## 🎮 Componentes

- 1 Arduino (Uno, Nano, etc.)
- 4 LEDs de colores (secuencia principal)
- 4 pulsadores (secuencia principal)
- 2 LEDs adicionales (indicadores de jugadores)
- 1 buzzer
- Resistencias para LEDs y pulsadores
- Protoboard y cableado

---

## ⚡ Conexiones

### 🔹 LEDs principales
| Componente | Pin |
|-----------|------|
| LED 1     | 2    |
| LED 2     | 3    |
| LED 3     | 4    |
| LED 4     | 5    |

### 🔹 Botones principales
| Pulsador | Pin |
|----------|------|
| P1       | 6    |
| P2       | 7    |
| P3       | 8    |
| P4       | 9    |

### 🔹 Otros
| Componente | Pin |
|------------|------|
| Buzzer     | 10   |
| LED Jugador 1 | 11 |
| LED Jugador 2 | 12 |

> Todos los pines son modificables editando los arrays del código.

---

## ⏱ Configuración de tiempos

```cpp
const int NUM_LEDS = 4;
const int NUM_RONDAS = 20;
const int TIEMPO_MOSTRAR = 500;
const int TIEMPO_ENTRE_LEDS = 250;
const int TIEMPO_ESPERA = 5000;
const int TIEMPO_PAUSA_RONDA = 1000;
```

---

# Funcionamiento del modo 2 jugadores

## 🎬 Inicio del juego
- Se muestra una animación inicial.
- Se resetean las secuencias de cada jugador.

## 🔄 Gestión de turnos
Cada ronda sigue esta estructura:

1. **Turno J1**  
   - Se ilumina su LED de jugador.  
   - Se muestra su secuencia (crece 1 paso por ronda).  
   - J1 debe reproducirla con los botones.  
   - Si falla → **J2 gana automáticamente**.

2. **Turno J2**  
   - Se ilumina su LED de jugador.  
   - Se muestra su secuencia.  
   - J2 debe reproducirla.  
   - Si falla → **J1 gana**.

---

# ✔️ Verificación
El juego comprueba:

- Orden correcto
- Tiempo máximo
- Coincidencia del LED con el botón correspondiente

Si ambos aciertan su ronda:

- Se hace una animación de acierto.
- Avanza a la siguiente ronda.

---

# 🏆 Victoria
Un jugador gana cuando:

- El rival falla una pulsación.
- O se termina el máximo de rondas y uno acumula más aciertos.

El LED del ganador parpadea 6 veces.

---

## 🔧 Funciones principales

| Función | Descripción |
|--------|-------------|
| `inicioJuego()` | Efecto inicial. |
| `mostrarSecuenciaJugador1()` | Muestra la secuencia del Jugador 1. |
| `mostrarSecuenciaJugador2()` | Muestra la secuencia del Jugador 2. |
| `leerSecuenciaJugadorArray()` | Comprueba si el jugador reproduce su secuencia correctamente. |
| `reproducirSonido()` | Sonido según color. |
| `acierto()` | Efecto cuando ambos superan la ronda. |
| `perder()` | Luces y sonido al fallar un jugador. |
| `ganar()` | Animación para el ganador. |

---

# 📌 Notas finales
- Este modo permite que **ambos jugadores aumenten su secuencia independientemente**.
- El sistema gestiona turnos, fallos y ganador automáticamente.
- Se ha añadido lógica para evitar parpadeos indeseados en LEDs de jugador.

---

Disfruta mejorando y ampliando esta versión de SIMÓN 2 jugadores.
