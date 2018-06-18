#include <Boards.h>
#include <Firmata.h> 

int mat_aux [10][10][3]; // crear matriz para pasar datos a luces
int mat [10][10][3];

int ancho = 10;
int alto = 10;
int color = 3;
int cantidad_esperada = 100; // num pixels

int encabezado = 255;
int cierre = 254;

int indice = 0;  // variables para indicar que es cada dato que llega
int indice_x = 0;
int indice_y = 0;
int indice_color = 0;
int pin_led = 13; // led indicador de que se estan recibiendo datos
boolean estado_pin = false;

///control luces

#include <Adafruit_NeoPixel.h> // libreria pixel led
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6 // pin donde conecto luces
Adafruit_NeoPixel strip = Adafruit_NeoPixel(100, PIN, NEO_GRB + NEO_KHZ800); // cant pixels

void setup() {
  Serial.begin(115200); // seteo velocidad a la misma que manda processing
  pinMode(pin_led, OUTPUT); 


  strip.begin(); // inicio strip
  strip.show(); // Inicio pixels en off

}

void loop() {

  while (Serial.available () > 0) {

    int inByte = Serial.read();
    if (inByte == encabezado) {
          indice=0;
      
        } //leo datos y los meto en una variable
    switch (indice) {
      case 0://indice == 0 miro que el byte sea = al encabezado
        if (inByte == encabezado) {
          indice++;
          indice_x = 0;
          indice_y = 0;
          indice_color = 0;
          
        }
        break;
      case 1://indice == 1 cantidad de mensajes
        mat_aux[indice_x][indice_y][indice_color] = inByte;
        indice_color++;
        if (indice_color >= 3) {
          indice_color = 0;
          indice_y++;
          if (indice_y >= alto) {
            indice_y = 0;
            indice_x++;
            if (indice_x >= ancho) {
              indice_x = 0;
              indice++;
              break;
            }
          }
        }


        break;

      case 2://indice == 2 miro que el byte sea el final del mensaje

        if (inByte == cierre) {

          copio_vector(); // ejecuto funcion
        }
        indice = 0;//espero un nuevo mensaje
        break;
    }
  }



}

void copio_vector() {

  for (int i = 0; i < ancho; i++) {
    for (int j = 0; j < alto; j++) {
      for (int c = 0; c < 3; c++) {
        mat[i][j][c] = mat_aux[i][j][c]; //porque
      }
    }
  }

  for (int i = 0; i < ancho ; i++) {
    for (int j = 0; j < alto; j++) {
    
      strip.setPixelColor(i+(j*ancho), strip.Color( mat_aux[i][j][0],  mat_aux[i][j][1],  mat_aux[i][j][2]));
   //strip.setPixelColor(num pixel), strip.Color( red,  green, blue));
    
    }
  }


  strip.show();


  digitalWrite(pin_led, HIGH);
  delay(2);
  digitalWrite(pin_led, LOW);//muestro que estoy recibiendo datos
  

}
