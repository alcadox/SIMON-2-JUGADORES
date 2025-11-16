// ==================== CONFIGURACIÓN ====================
const int NUM_LEDS = 4;
const int NUM_RONDAS = 2;

const int TIEMPO_MOSTRAR = 500;     // ms que se enciende cada led en la secuencia
const int TIEMPO_ENTRE_LEDS = 250;  // ms entre cada led
const int TIEMPO_ESPERA = 5000;     // ms máximo para pulsar
const int TIEMPO_PAUSA_RONDA = 1000;

// ==================== PINES ====================
int luces[NUM_LEDS] = {2, 3, 4, 5};
int pulsadores[NUM_LEDS] = {6, 7, 8, 9};
int sonidos = 10;
int lucesJugadores[] = {11, 12};

// BOTONES
int boton1Jugador = 13;
int boton2Jugadores = A1;

// ==================== VARIABLES ====================
int secuenciaAleatoria[NUM_RONDAS];        // para 1 jugador
int secuenciaIntroducida[NUM_RONDAS];
int contadorAciertos = 0;
int contadorRonda = 0;
int jugadorJugando = 0;

// PARA 2 JUGADORES
int secuenciaJ1[NUM_RONDAS];
int secuenciaJ2[NUM_RONDAS];
int contadorAciertosJ1 = 0;
int contadorAciertosJ2 = 0;
int rondaJ1 = 0;
int rondaJ2 = 0;

int modoJuego = 0; // 1 = 1 jugador, 2 = 2 jugadores

// ==================== SETUP ====================
void setup() {
  randomSeed(analogRead(A0));
  pinMode(sonidos, OUTPUT);
  pinMode(lucesJugadores[0], OUTPUT);
  pinMode(lucesJugadores[1], OUTPUT);

  pinMode(boton1Jugador, INPUT);
  pinMode(boton2Jugadores, INPUT);
  
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(luces[i], OUTPUT);
    pinMode(pulsadores[i], INPUT);
  }
}

// ==================== PRINCIPAL ====================
void loop() {
  // Reiniciar contadores generales (para 1 jugador)
  contadorAciertos = 0;
  contadorRonda = 0;

  // Seleccionar modo: se repite hasta que pulse 1 u 2.
  seleccionarModoJuego(); // bloquea hasta elegir modo

  if (modoJuego == 1) {
    // MODO 1 JUGADOR
    digitalWrite(lucesJugadores[0], HIGH);
    digitalWrite(lucesJugadores[1], LOW);

    inicioJuego();

    while (contadorRonda < NUM_RONDAS) {
      delay(1000);
      contadorRonda++;
      int numeroRandom = random(0, NUM_LEDS);
      secuenciaAleatoria[contadorRonda - 1] = numeroRandom;

      // Mostrar secuencia
      mostrarSecuencia(contadorRonda);

      // Leer secuencia del jugador
      if (!leerSecuenciaJugador(contadorRonda)) {
        delay(300);
        perder();
        delay(500);
        parpadearAciertos(contadorAciertos);
        delay(1000);
        digitalWrite(lucesJugadores[0], LOW);
        return; // termina el juego
      }

      // Si acierta
      delay(500);
      contadorAciertos++;
      acierto();
    }

    // Si llega a 20 rondas
    ganar();
    digitalWrite(lucesJugadores[0], LOW);
  }
  else if (modoJuego == 2) {
    // MODO 2 JUGADORES
    // Reiniciar contadores de jugadores
    contadorAciertosJ1 = 0;
    contadorAciertosJ2 = 0;
    rondaJ1 = 0;
    rondaJ2 = 0;

    // Empezar juego: alterna 1 ronda por jugador
    while (true) {
      // TURNO J1
      if (rondaJ1 < NUM_RONDAS) {
        digitalWrite(lucesJugadores[0], HIGH);
        digitalWrite(lucesJugadores[1], LOW);
        delay(300);

        rondaJ1++;
        secuenciaJ1[rondaJ1 - 1] = random(0, NUM_LEDS);

        mostrarSecuenciaArray(secuenciaJ1, rondaJ1);

        if (!leerSecuenciaJugadorArray(rondaJ1, secuenciaJ1)) {
          // J1 falla -> fin del juego
          perder();
          // apagar leds de jugadores tras el fallo
          digitalWrite(lucesJugadores[0], LOW);
          digitalWrite(lucesJugadores[1], LOW);
          break;
		} else {
          contadorAciertosJ1++;
          acierto();
        }
      }

      // TURNO J2
      if (rondaJ2 < NUM_RONDAS) {
        digitalWrite(lucesJugadores[1], HIGH);
        digitalWrite(lucesJugadores[0], LOW);
        delay(300);

        rondaJ2++;
        secuenciaJ2[rondaJ2 - 1] = random(0, NUM_LEDS);

        mostrarSecuenciaArray(secuenciaJ2, rondaJ2);

        if (!leerSecuenciaJugadorArray(rondaJ2, secuenciaJ2)) {
          perder();
          digitalWrite(lucesJugadores[0], LOW);
          digitalWrite(lucesJugadores[1], LOW);
          break;
		}else {
          contadorAciertosJ2++;
          acierto();
        }
      }

      // Si ambos alcanzaron NUM_RONDAS -> terminar y comparar
      if (rondaJ1 >= NUM_RONDAS && rondaJ2 >= NUM_RONDAS) {
        break;
      }
    }

    // Determinar ganador por mayor contador de aciertos (rondas completadas)
    int ganador = 0; // 0->J1, 1->J2, 2->empate
    if (contadorAciertosJ1 > contadorAciertosJ2) ganador = 0;
    else if (contadorAciertosJ2 > contadorAciertosJ1) ganador = 1;
    else ganador = 2;

    // Si empate, hacer ambos parpadear y sonar
    if (ganador == 2) {
      tone(sonidos, 1200, 500);
      for (int i = 0; i < 6; i++) {
        digitalWrite(lucesJugadores[0], HIGH);
        digitalWrite(lucesJugadores[1], HIGH);
        delay(200);
        digitalWrite(lucesJugadores[0], LOW);
        digitalWrite(lucesJugadores[1], LOW);
        delay(200);
      }
    } else {
      tone(sonidos, 1200, 500);
      for (int i = 0; i < 6; i++) {
        digitalWrite(lucesJugadores[ganador], HIGH);
        delay(200);
        digitalWrite(lucesJugadores[ganador], LOW);
        delay(200);
      }
    }

    // Apagar leds de jugadores al final
    digitalWrite(lucesJugadores[0], LOW);
    digitalWrite(lucesJugadores[1], LOW);
  }

  // pequeño delay antes de reiniciar loop
  delay(500);
}

// ==================== FUNCIONES ====================

void inicioJuego() {
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(luces[i], HIGH);
    reproducirSonido(i);
    delay(300);
    digitalWrite(luces[i], LOW);
  }
  delay(300);
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    digitalWrite(luces[i], HIGH);
    reproducirSonido(i);
    delay(300);
    digitalWrite(luces[i], LOW);
  }
  delay(200);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < NUM_LEDS; j++) digitalWrite(luces[j], HIGH);
    tone(sonidos, 999, 200);
    delay(200);
    for (int j = 0; j < NUM_LEDS; j++) digitalWrite(luces[j], LOW);
    delay(200);
  }
}

void reproducirSonido(int indice) {
  switch (indice) {
    case 0: tone(sonidos, 200, TIEMPO_MOSTRAR); break;
    case 1: tone(sonidos, 400, TIEMPO_MOSTRAR); break;
    case 2: tone(sonidos, 600, TIEMPO_MOSTRAR); break;
    case 3: tone(sonidos, 800, TIEMPO_MOSTRAR); break;
  }
}

// Mostrar secuencia 
void mostrarSecuencia(int ronda) {
  for (int i = 0; i < ronda; i++) {
    int indice = secuenciaAleatoria[i];
    digitalWrite(luces[indice], HIGH);
    reproducirSonido(indice);
    delay(TIEMPO_MOSTRAR);
    digitalWrite(luces[indice], LOW);
    delay(TIEMPO_ENTRE_LEDS);
  }
}

// Mostrar secuencia (2 jugadores)
void mostrarSecuenciaArray(int secuencia[], int ronda) {
  for (int i = 0; i < ronda; i++) {
    int indice = secuencia[i];
    digitalWrite(luces[indice], HIGH);
    reproducirSonido(indice);
    delay(TIEMPO_MOSTRAR);
    digitalWrite(luces[indice], LOW);
    delay(TIEMPO_ENTRE_LEDS);
  }
}

// Leer secuencia original
bool leerSecuenciaJugador(int ronda) {
  int contadorLuzIntroducida = 0;
  unsigned long tiempoInicio = millis();

  while (contadorLuzIntroducida < ronda) {
    for (int i = 0; i < NUM_LEDS; i++) {
      if (digitalRead(pulsadores[i]) == HIGH) {
        digitalWrite(luces[i], HIGH);
        reproducirSonido(i);
        while (digitalRead(pulsadores[i]) == HIGH); // espera a soltar
        digitalWrite(luces[i], LOW);

        secuenciaIntroducida[contadorLuzIntroducida] = i;
        contadorLuzIntroducida++;
        delay(100);

        // Reiniciar contador de tiempo
        tiempoInicio = millis();
      }
    }

    // Si pasan más de TIEMPO_ESPERA sin pulsar pierde
    if (millis() - tiempoInicio > TIEMPO_ESPERA) {
      return false;
    }
  }

  // Comparar secuencias
  for (int i = 0; i < ronda; i++) {
    if (secuenciaIntroducida[i] != secuenciaAleatoria[i]) {
      return false;
    }
  }
  return true;
}

// Leer secuencia contra una secuencia dada (2 jugadores)
bool leerSecuenciaJugadorArray(int ronda, int secuenciaCorrecta[]) {
  int contadorLuzIntroducida = 0;
  unsigned long tiempoInicio = millis();

  while (contadorLuzIntroducida < ronda) {
    for (int i = 0; i < NUM_LEDS; i++) {
      if (digitalRead(pulsadores[i]) == HIGH) {
        digitalWrite(luces[i], HIGH);
        reproducirSonido(i);
        while (digitalRead(pulsadores[i]) == HIGH); // espera a soltar
        digitalWrite(luces[i], LOW);

        secuenciaIntroducida[contadorLuzIntroducida] = i;
        contadorLuzIntroducida++;
        delay(100);

        // Reiniciar contador de tiempo
        tiempoInicio = millis();
      }
    }

    // Si pasan más de TIEMPO_ESPERA sin pulsar pierde
    if (millis() - tiempoInicio > TIEMPO_ESPERA) {
      return false;
    }
  }

  // Comparar secuencias
  for (int i = 0; i < ronda; i++) {
    if (secuenciaIntroducida[i] != secuenciaCorrecta[i]) {
      return false;
    }
  }
  return true;
}

void acierto() {
  for (int i = 0; i < NUM_LEDS; i++) digitalWrite(luces[i], HIGH);
  tone(sonidos, 1000, 400);
  delay(400);
  for (int i = 0; i < NUM_LEDS; i++) digitalWrite(luces[i], LOW);
  delay(TIEMPO_PAUSA_RONDA);
}

void perder() {
  tone(sonidos, 150, 800);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < NUM_LEDS; j++) digitalWrite(luces[j], HIGH);
    delay(200);
    for (int j = 0; j < NUM_LEDS; j++) digitalWrite(luces[j], LOW);
    delay(200);
  }
}

void ganar() {
  for (int i = 0; i < 3; i++) {
    tone(sonidos, 500 + (i * 300), 300);
    for (int j = 0; j < NUM_LEDS; j++) digitalWrite(luces[j], HIGH);
    delay(300);
    for (int j = 0; j < NUM_LEDS; j++) digitalWrite(luces[j], LOW);
    delay(300);
  }
  tone(sonidos, 1200, 600);
  delay(600);
}

void parpadearAciertos(int veces) {
  for (int i = 0; i < veces; i++) {
    for (int j = 0; j < NUM_LEDS; j++) digitalWrite(luces[j], HIGH);
    delay(200);
    for (int j = 0; j < NUM_LEDS; j++) digitalWrite(luces[j], LOW);
    delay(200);
  }
}

// ==================== FUNCION SELECCIÓN MODO (REPITE SALUDO HASTA ELEGIR) ====================
void seleccionarModoJuego() {
  // Repetir saludo hasta que pulse boton1Jugador o boton2Jugadores
  while (true) {
    inicioJuego(); // muestra saludo

    // Comprobar botones con pequeño
    if (digitalRead(boton1Jugador) == HIGH) {
      delay(60);
      if (digitalRead(boton1Jugador) == HIGH) {
        modoJuego = 1;
        // encender led jugador 1 permanentemente
        digitalWrite(lucesJugadores[0], HIGH);
        digitalWrite(lucesJugadores[1], LOW);
        delay(200);
        return;
      }
    }
    if (digitalRead(boton2Jugadores) == HIGH) {
      delay(60);
      if (digitalRead(boton2Jugadores) == HIGH) {
        modoJuego = 2;
        // apagar leds
        digitalWrite(lucesJugadores[0], LOW);
        digitalWrite(lucesJugadores[1], LOW);
        delay(200);
        return;
      }
    }
    // si no se pulsa nada, repetir saludo
  }
}
