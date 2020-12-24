/*
 * --------------------------------------------------------------------------
-----------       Diseño de las unidades de control        ------------------
-----------     para cada articulación del Robot Educativo ------------------
-----------      PROTOTIPO SIMILAR a SCORBOT ER 9 Pro basada  ---------------
-----------     en sistemas de procesamiento de bajo costo      -------------
-----------------------------------------------------------------------------
-----------------             DESCRIPCIÓN            ------------------------
-----------------------------------------------------------------------------
----   Este programa permite controlar el brazo prototipo desde          ----
--------------     un SMARTPHONE a través              ----------------------
--------------    de una conexión Bluethoot            ----------------------
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
---------------                DETALLES                ----------------------
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
-----------                PROGRAM                 -----------------
-----------------------------------------------------------------------------
/*--LIBRARIES----------------------------------------------------------------
Se requiere incluir las librerias necesarias para el control de los servos
y de la conexión Bluethoot
*/ 

#include <SoftwareSerial.h>
#include <Servo.h>

Servo ServoM01;
Servo ServoM02;
Servo ServoM03;
Servo MicroServoM04;
Servo MicroServoM05;
Servo MicroServoM06;

SoftwareSerial Bluetooth(3, 4); // Arduino (RX, TX) - HC-05 Bluetooth (TX, RX)

/*--VARIABLES----------------------------------------------------------------
se definen las variables necesarias y los arrays para el almacenamiento de los
pasos como las posiciones previa, actual.*/

int ServoM1_Pos, ServoM2_Pos, ServoM3_Pos, MicroServoM4_Pos, MicroServoM5_Pos, MicroServoM6_Pos;                          // Posición actual
int Servo1_Pos_Prev, Servo2_Pos_Prev, Servo3_Pos_Prev, MicroServo4_Pos_Prev, MicroServo5_Pos_Prev, MicroServo6_Pos_Prev;  // Posición anterior o previa
int ServoM01SP[50], ServoM02SP[50], ServoM03SP[50], MicroServoM04SP[50], MicroServoM05SP[50], MicroServoM06SP[50];        // Almacena posiciones / pasos
int index = 0;
int velocidadDelay = 30;
String dataIn = "";

/*
-----------------------------------------------------------------------------
-------------             SETUP-CONFIGURACION          --- ------------------
-----------------------------------------------------------------------------
Aquí se encuentran las instrucciones previas al ciclo infinito bucle, 
* en esta sección del código se dan las instrucciones iniciales al bazo robot
* y se declaran variables para almacenar posiciones de los motores.
-----------------------------------------------------------------------------
*/


void setup() {
    ServoM01.attach(5);    // Este Motor se encarga de mover la Cintura del brazo robótico.
    ServoM02.attach(6);  // Este Motor se encarga de mover el Hombro del brazo robótico.
    ServoM03.attach(7);  // Este Motor se encarga de mover el Codo del brazo robótico.
    MicroServoM04.attach(8);  // Este Motor se encarga de girar la muñeca del brazo robótico, también
                                   // se conoce como brida  
    MicroServoM05.attach(9);  // Este Motor se encarga de inclinar/declinar, la muñeca del brazo robótico.
    MicroServoM06.attach(10);// Este Motor se encarga de cerrar y abrir la pinza, también se le llama
                                   // agarre.
    
    Bluetooth.begin(38400); // Tasa de baudios determinada para el módulo Bluetooth debe ser configurada previamente en el módulo bluetooth.
    Bluetooth.setTimeout(1);
    delay(30);

// Posición previa (también inicial o de reposo) del brazo robótico: Antes de ir a 
// void loop se dan las siguientes instrucciones, también llamado "Home".
// Se debe tener en cuenta que se definieron unos límites en grados de algunos 
// servomotores para evitar choques físicos que comprometan el hardware.

// Cabe especificar los rangos posibles por los servomotores es de 0° a 180°
     
    Servo1_Pos_Prev = 90;      // En contra de las manecillas del reloj cuanto más valor
                                // El brazo va hacia la derecha entre más valor e izquierda en caso
                                //contrario
    ServoM01.write(Servo1_Pos_Prev); // Cintura (Rango límite 0° a 180°)
    delay(700);
  
    Servo2_Pos_Prev = 150; // Cuanto más valor, sube el brazo y va hasta atrás por medio de la articulación del hombro, en contra de las manecillas del reloj
    ServoM02.write(Servo2_Pos_Prev); // Hombro (Rango límite 0° a 180°)
    delay(700);
      
    Servo3_Pos_Prev = 120; // Entre más valor va hacia abajo el antebrazo y entre menos valor hacia arriba
    ServoM03.write(Servo3_Pos_Prev); // Codo (Rango límite 0° a 150°)
    delay(700);
      
    MicroServo4_Pos_Prev = 90; // Entre más valor la muñeca gira a la izquierda y viceversa
    MicroServoM04.write(MicroServo4_Pos_Prev); // Rotación de Muñeca (Rango límite 0° a 180°)
    delay(700);
      
    MicroServo5_Pos_Prev = 60; //Cuanto más valor la inclinación es más hacia arriba
    MicroServoM05.write(MicroServo5_Pos_Prev); // Inclinación de Muñeca (Rango límite 0° a 180°)
    delay(700);
      
    MicroServo6_Pos_Prev = 140; // Cuanto más valor abre proporcionalmente la pinza
    MicroServoM06.write(MicroServo6_Pos_Prev); // Pinza o agarre (Rango límite 60° a 150°)
    delay(300);
    
// Se hace un saludo abriendo y cerrando las pinzas:
    
    MicroServo6_Pos_Prev = 60;
    MicroServoM06.write(MicroServo6_Pos_Prev);// Pinza o agarre
    delay(300);
    MicroServo6_Pos_Prev = 140;
    MicroServoM06.write(MicroServo6_Pos_Prev);// Pinza o agarre
    delay(300);
    MicroServo6_Pos_Prev = 60;
    MicroServoM06.write(MicroServo6_Pos_Prev);// Pinza o agarre
    delay(300);
    MicroServo6_Pos_Prev = 140;
    MicroServoM06.write(MicroServo6_Pos_Prev);// Pinza o agarre
    delay(300);
    MicroServo6_Pos_Prev = 60;
    MicroServoM06.write(MicroServo6_Pos_Prev);// Pinza o agarre
    delay(300);
    MicroServo6_Pos_Prev = 140;
    MicroServoM06.write(MicroServo6_Pos_Prev);// Pinza o agarre
    delay(300);
}

/*
-----------------------------------------------------------------------------
-----------            BUCLE-LOOP            ------------------
-----------------------------------------------------------------------------
La parte principal del programa se llama LOOP y funciona indefinidamente.
-----------------------------------------------------------------------------
*/

void loop() {

// Verificar datos entrantes:

  if (Bluetooth.available() > 0) {
    dataIn = Bluetooth.readString();  // Lee los datos se almacenan como variable cadena 
    
// Si el control deslizante "Cintura" ha cambiado de valor, mueva el Servomotor 1 a la posición:

    if (dataIn.startsWith("s1")) {
      String dataInS = dataIn.substring(2, dataIn.length());  // Extrae solo el número. E.g. de "s1120" a "120"
      ServoM1_Pos = dataInS.toInt();  // Convierte la cadena en variable entero

// Se usan bucles for para poder controlar la velocidad del servo
// Si la posición anterior es mayor que la posición actual

if (Servo1_Pos_Prev > ServoM1_Pos) {
        for ( int j = Servo1_Pos_Prev; j >= ServoM1_Pos; j--) {   // Acciona el Servomotor hacia abajo
          ServoM01.write(j);
          delay(30);    // Define la velocidad a la que gira el Servomotor
        }
      }

// Si la posición anterior es más pequeña que la posición actual:

      if (Servo1_Pos_Prev < ServoM1_Pos) {
        for ( int j = Servo1_Pos_Prev; j <= ServoM1_Pos; j++) {   // Acciona el Servomotor hacia arriba
          ServoM01.write(j);
          delay(30);
        }
      }
      Servo1_Pos_Prev = ServoM1_Pos;   // Establecer la posición actual como la posición previa o anterior
    }
    
// Mueve el Servomotor 2:

    if (dataIn.startsWith("s2")) {
      String dataInS = dataIn.substring(2, dataIn.length());
      ServoM2_Pos = dataInS.toInt();

      if (Servo2_Pos_Prev > ServoM2_Pos) {
        for ( int j = Servo2_Pos_Prev; j >= ServoM2_Pos; j--) {
          ServoM02.write(j);
          delay(50);
        }
      }
      if (Servo2_Pos_Prev < ServoM2_Pos) {
        for ( int j = Servo2_Pos_Prev; j <= ServoM2_Pos; j++) {
          ServoM02.write(j);
          delay(50);
        }
      }
      Servo2_Pos_Prev = ServoM2_Pos;
    }

// Mueve Servomotor 3:

    if (dataIn.startsWith("s3")) {
      String dataInS = dataIn.substring(2, dataIn.length());
      ServoM3_Pos = dataInS.toInt();
      if (Servo3_Pos_Prev > ServoM3_Pos) {
        for ( int j = Servo3_Pos_Prev; j >= ServoM3_Pos; j--) {
          ServoM03.write(j);
          delay(30);
        }
      }
      if (Servo3_Pos_Prev < ServoM3_Pos) {
        for ( int j = Servo3_Pos_Prev; j <= ServoM3_Pos; j++) {
          ServoM03.write(j);
          delay(30);
        }
      }
      Servo3_Pos_Prev = ServoM3_Pos;
    }
    
// Mueve micro Servomotor 4
    if (dataIn.startsWith("s4")) {
      String dataInS = dataIn.substring(2, dataIn.length());
      MicroServoM4_Pos = dataInS.toInt();
      if (MicroServo4_Pos_Prev > MicroServoM4_Pos) {
        for ( int j = MicroServo4_Pos_Prev; j >= MicroServoM4_Pos; j--) {
          MicroServoM04.write(j);
          delay(30);
        }
      }
      if (MicroServo4_Pos_Prev < MicroServoM4_Pos) {
        for ( int j = MicroServo4_Pos_Prev; j <= MicroServoM4_Pos; j++) {
          MicroServoM04.write(j);
          delay(30);
        }
      }
      MicroServo4_Pos_Prev = MicroServoM4_Pos;
    }
    
// Mueve micro Servomotor 5
    if (dataIn.startsWith("s5")) {
      String dataInS = dataIn.substring(2, dataIn.length());
      MicroServoM5_Pos = dataInS.toInt();
      if (MicroServo5_Pos_Prev > MicroServoM5_Pos) {
        for ( int j = MicroServo5_Pos_Prev; j >= MicroServoM5_Pos; j--) {
          MicroServoM05.write(j);
          delay(30);
        }
      }
      if (MicroServo5_Pos_Prev < MicroServoM5_Pos) {
        for ( int j = MicroServo5_Pos_Prev; j <= MicroServoM5_Pos; j++) {
          MicroServoM05.write(j);
          delay(30);
        }
      }
      MicroServo5_Pos_Prev = MicroServoM5_Pos;
    }

// Mueve el micro Servomotor 6
    if (dataIn.startsWith("s6")) {
      String dataInS = dataIn.substring(2, dataIn.length());
      MicroServoM6_Pos = dataInS.toInt();
      if (MicroServo6_Pos_Prev > MicroServoM6_Pos) {
        for ( int j = MicroServo6_Pos_Prev; j >= MicroServoM6_Pos; j--) {
          MicroServoM06.write(j);
          delay(30);
        }
      }
      if (MicroServo6_Pos_Prev < MicroServoM6_Pos) {
        for ( int j = MicroServo6_Pos_Prev; j <= MicroServoM6_Pos; j++) {
          MicroServoM06.write(j);
          delay(30);
        }
      }
      MicroServo6_Pos_Prev = MicroServoM6_Pos; 
    }

// Si se presiona el botón "GUARDAR"  POS_G: Posición guardada

    if (dataIn.startsWith("GUARDAR")) {

      ServoM01SP[index] = Servo1_Pos_Prev;  // Guarda la posición en la variable Array
      ServoM02SP[index] = Servo2_Pos_Prev;
      ServoM03SP[index] = Servo3_Pos_Prev;
      MicroServoM04SP[index] = MicroServo4_Pos_Prev;
      MicroServoM05SP[index] = MicroServo5_Pos_Prev;
      MicroServoM06SP[index] = MicroServo6_Pos_Prev;
      index++;                        // Incrementa el índice del Array
    }

// Si se presiona el botón "ARRANCAR"
    
    if (dataIn.startsWith("ARRANCAR")) {
      ARRANCARservo();  // Modo automático (ejecuta los pasos guardados)
    }
    
// Si se presiona el botón "REINICIAR"
    if ( dataIn == "REINICIAR") {
      memset(ServoM01SP, 0, sizeof(ServoM01SP)); // Borrar los datos de la Array a 0
      memset(ServoM02SP, 0, sizeof(ServoM02SP));
      memset(ServoM03SP, 0, sizeof(ServoM03SP));
      memset(MicroServoM04SP, 0, sizeof(MicroServoM04SP));
      memset(MicroServoM05SP, 0, sizeof(MicroServoM05SP));
      memset(MicroServoM06SP, 0, sizeof(MicroServoM06SP));
      index = 0;  // Index to 0
    }
  }
}

/*
-----------------------------------------------------------------------------
--------------------             FUNCIONES            -----------------------
-----------------------------------------------------------------------------
Cuando trabajamos con código, es importante dividir el código en funciones.
Estas funciones pueden ser llamadas desde diferentes partes del programa.
-----------------------------------------------------------------------------   
*/

// Función personalizada del modo automático (ejecuta los pasos guardados)

void ARRANCARservo() {
  while (dataIn != "REINICIAR") {   // Ejecuta los pasos una y otra vez hasta que se presione el botón "REINICIAR" (Bucle)
    for (int i = 0; i <= index - 2; i++) {  // Ejecuta todos los pasos (index)
      if (Bluetooth.available() > 0) {      // Verifica los datos entrantes
        dataIn = Bluetooth.readString();
        if ( dataIn == "PAUSAR") {           // Si se presiona el botón "PAUSAR"
          while (dataIn != "ARRANCAR") {         // Espera hasta que se presione "ARRANCAR" nuevamente

            if (Bluetooth.available() > 0) {
              dataIn = Bluetooth.readString();
              if ( dataIn == "REINICIAR") {     
                break;
              }
            }
          }
        }
        
// Si se cambia el control deslizante de velocidad
        if (dataIn.startsWith("ss")) {
          String dataInS = dataIn.substring(2, dataIn.length());
          velocidadDelay = dataInS.toInt(); // Cambiar la velocidad del Servomotor (tiempo de retardo)
        }
      }

// A continuación se los cálculos necesarios para definir la nueva posición, esto restando posición anterior o guardada y posición solicitada:
      // Servomotor 1
      if (ServoM01SP[i] == ServoM01SP[i + 1]) {
      }
      if (ServoM01SP[i] > ServoM01SP[i + 1]) {
        for ( int j = ServoM01SP[i]; j >= ServoM01SP[i + 1]; j--) {
          ServoM01.write(j);
          delay(velocidadDelay);
        }
      }
      if (ServoM01SP[i] < ServoM01SP[i + 1]) {
        for ( int j = ServoM01SP[i]; j <= ServoM01SP[i + 1]; j++) {
          ServoM01.write(j);
          delay(velocidadDelay);
        }
      }

      // Servomotor 2
      if (ServoM02SP[i] == ServoM02SP[i + 1]) {
      }
      if (ServoM02SP[i] > ServoM02SP[i + 1]) {
        for ( int j = ServoM02SP[i]; j >= ServoM02SP[i + 1]; j--) {
          ServoM02.write(j);
          delay(velocidadDelay);
        }
      }
      if (ServoM02SP[i] < ServoM02SP[i + 1]) {
        for ( int j = ServoM02SP[i]; j <= ServoM02SP[i + 1]; j++) {
          ServoM02.write(j);
          delay(velocidadDelay);
        }
      }

      // Servomotor 3
      if (ServoM03SP[i] == ServoM03SP[i + 1]) {
      }
      if (ServoM03SP[i] > ServoM03SP[i + 1]) {
        for ( int j = ServoM03SP[i]; j >= ServoM03SP[i + 1]; j--) {
          ServoM03.write(j);
          delay(velocidadDelay);
        }
      }
      if (ServoM03SP[i] < ServoM03SP[i + 1]) {
        for ( int j = ServoM03SP[i]; j <= ServoM03SP[i + 1]; j++) {
          ServoM03.write(j);
          delay(velocidadDelay);
        }
      }

      // Micro Servomotor 4
      if (MicroServoM04SP[i] == MicroServoM04SP[i + 1]) {
      }
      if (MicroServoM04SP[i] > MicroServoM04SP[i + 1]) {
        for ( int j = MicroServoM04SP[i]; j >= MicroServoM04SP[i + 1]; j--) {
          MicroServoM04.write(j);
          delay(velocidadDelay);
        }
      }
      if (MicroServoM04SP[i] < MicroServoM04SP[i + 1]) {
        for ( int j = MicroServoM04SP[i]; j <= MicroServoM04SP[i + 1]; j++) {
          MicroServoM04.write(j);
          delay(velocidadDelay);
        }
      }

      // Micro Servomotor 5
      if (MicroServoM05SP[i] == MicroServoM05SP[i + 1]) {
      }
      if (MicroServoM05SP[i] > MicroServoM05SP[i + 1]) {
        for ( int j = MicroServoM05SP[i]; j >= MicroServoM05SP[i + 1]; j--) {
          MicroServoM05.write(j);
          delay(velocidadDelay);
        }
      }
      if (MicroServoM05SP[i] < MicroServoM05SP[i + 1]) {
        for ( int j = MicroServoM05SP[i]; j <= MicroServoM05SP[i + 1]; j++) {
          MicroServoM05.write(j);
          delay(velocidadDelay);
        }
      }

      // Micro Servomotor 6
      if (MicroServoM06SP[i] == MicroServoM06SP[i + 1]) {
      }
      if (MicroServoM06SP[i] > MicroServoM06SP[i + 1]) {
        for ( int j = MicroServoM06SP[i]; j >= MicroServoM06SP[i + 1]; j--) {
          MicroServoM06.write(j);
          delay(velocidadDelay);
        }
      }
      if (MicroServoM06SP[i] < MicroServoM06SP[i + 1]) {
        for ( int j = MicroServoM06SP[i]; j <= MicroServoM06SP[i + 1]; j++) {
          MicroServoM06.write(j);
          delay(velocidadDelay);
        }
      }
    }
  }
}
