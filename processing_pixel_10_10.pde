int numFrames = 2142; // variable cant de imagenes
PImage[] images = new PImage[numFrames]; //vector cant de imagenes
color mat[][] = new color[10][10]; // vector colores
int ancho = 10; // variables tamanio matriz
int alto = 10;
import processing.serial.*; // libreria comunicacion serie
Serial puerto;  // Crea objeto clase serial
int encabezado = 255; // valores para mandar paquete de datos
int cierre = 254;

void setup() {
size(420, 420);
frameRate(30); 
println(Serial.list()); // para identificar en que puerto esta arduino
String portName = Serial.list()[1];
puerto = new Serial(this, portName, 115200); //115200 velocidad adecuada para los pixel led
delay(1000); // le doy un tiempo entre que abre el puerto y empieza a tirar datos
for (int i = 0; i < images.length; i++) {  //creo vector con todas las imagenes
String imageName = + i + ".jpg";
images[i] = loadImage(imageName);
}
}
void draw() {
  background(0);
int frame = frameCount % numFrames; // variable del sistema que contiene el numero de frames que pasaron desde que arranco el programa
image(images[frame], 0, 0);  //muestro imagen 
  for (int i=0; i<ancho; i++) {
    for (int j=0; j<alto; j++) {
      color c = images[frame].get(i, j);
      mat[i][j] = c;
      fill(c);
      rect(i*42, j*42, 40, 40);
    }
  }// este for captura y guarda en una matriz los valores de color de cada pixel de cada imagen
  
  //ecribo en el puerto serie
  puerto.write(encabezado);
  println(encabezado);
  
  for (int i=0; i<ancho; i++) {
    for (int j=0; j<alto; j++) {
     
      color aux_color =mat[i][j];
      int r = int(red(aux_color));
      int g = int(green(aux_color));
      int b = int(blue(aux_color));
      
      r =constrain(r,0,253);//para que no llegue a los valores que estoy usando como encabezado y cierre
      g =constrain(g,0,253);
      b =constrain(b,0,253);
      
      puerto.write(r);
        println(r);
      puerto.write(g);
           println(g);
      puerto.write(b);
           println(b);
  
    }
  }
  puerto.write(cierre);
       println(cierre);
  
}