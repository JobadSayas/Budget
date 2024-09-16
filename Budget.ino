// Version 1.5

#include <Wire.h>
#include <U8g2lib.h>
#include <Keypad.h>

// Crear una instancia de la librería U8g2 para la pantalla OLED con rotación de 90 grados (U8G2_R1)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R1, U8X8_PIN_NONE, U8X8_PIN_NONE);

// Configuración del teclado de membrana
const byte ROWS = 4;  // 4 filas
const byte COLS = 4;  // 4 columnas
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};    // Pines de las filas
byte colPins[COLS] = {5, 4, 3, 2};    // Pines de las columnas

// Crear una instancia de Keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Declaración de variables para los valores, ahora como float para permitir decimales
float presupuesto = 1300.00;
float gastado = 0;
float restante = presupuesto - gastado;  // Calcular el valor restante

void setup() {
  u8g2.begin();  // Inicializar la pantalla
}

void loop() {
  char key = keypad.getKey();  // Leer la tecla presionada
  
  // Si se presiona una tecla
  if (key) {
    // Verificar si la tecla es un número o un punto decimal
    if (isDigit(key) || key == '.') {
      static String gastadoInput = "";
      
      // Agregar la tecla presionada al input del valor "gastado"
      gastadoInput += key;
      
      // Convertir la entrada a un valor float
      gastado = gastadoInput.toFloat();
      
      // Calcular el nuevo valor restante
      restante = presupuesto - gastado;
    }
    
    // Opcional: Implementar un carácter especial como '#' para resetear la entrada
    if (key == '#') {
      gastado = 0.0;
      restante = presupuesto;
    }
  }

  u8g2.clearBuffer();  // Limpiar la memoria interna

  // Presupuesto
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(2, 12, "Presupuesto");

  u8g2.setFont(u8g2_font_helvR10_tr);
  u8g2.setCursor(2, 30);
  u8g2.print("$");
  u8g2.print(presupuesto, 2);  // Mostrar con 2 decimales

  // Línea divisoria
  u8g2.drawStr(0, 35, "_____________________________");

  // Gastado
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(2, 52, "Gastado");

  u8g2.setFont(u8g2_font_helvR10_tr);
  u8g2.setCursor(2, 70);
  u8g2.print("$");
  u8g2.print(gastado, 2);  // Mostrar con 2 decimales

  // Línea divisoria
  u8g2.drawStr(0, 75, "_____________________________");

  // Restante
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(2, 92, "Restante");

  u8g2.setFont(u8g2_font_helvR10_tr);
  u8g2.setCursor(2, 110);
  u8g2.print("$");
  u8g2.print(restante, 2);  // Mostrar con 2 decimales

  u8g2.sendBuffer();  // Transferir la memoria interna a la pantalla
  delay(100);  // Pequeño retraso para evitar rebotes en el teclado
}
