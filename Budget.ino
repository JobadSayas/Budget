// Version 1.9

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

// Declaración de variables para los valores
float presupuesto = 1300.00;
float gastado = 0;
float restante = presupuesto - gastado;  // Calcular el valor restante
float entryValue = 0;  // Variable para almacenar el valor ingresado en la pantalla de entrada
bool entryStarted = false;  // Flag to indicate if the entry has started

// Estado de la pantalla actual
enum Screen { MAIN, ENTRY };
Screen currentScreen = MAIN;

void setup() {
  u8g2.begin();  // Inicializar la pantalla
}

void loop() {
  char key = keypad.getKey();  // Leer la tecla presionada

  if (key) {
    if (key == 'B') {
      // Cambiar a la pantalla de entrada
      currentScreen = ENTRY;
      entryValue = 0;  // Inicializar el valor de entrada
      entryStarted = false;  // Resetear el flag de inicio
    } else if (key == 'D') {
      // Cambiar de vuelta a la pantalla principal
      currentScreen = MAIN;
    } else if (currentScreen == ENTRY) {
      // En la pantalla de entrada, añadir números al valor
      if (isDigit(key) || key == '.') {
        if (key == '.' && entryStarted) {
          // No permitir múltiples puntos decimales
          return;
        }
        
        if (key == '.') {
          entryStarted = true;  // Marcar que el punto decimal ha sido ingresado
        } else {
          // Construir el número en formato float
          entryValue = entryValue * 10 + (key - '0');
          if (entryStarted) {
            // Ajustar la posición del punto decimal
            entryValue = entryValue / 10;
          }
        }
      }
    }
  }

  // Calcular el valor restante
  restante = presupuesto - gastado;

  u8g2.clearBuffer();  // Limpiar la memoria interna

  if (currentScreen == MAIN) {
    // Pantalla principal
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
  } else if (currentScreen == ENTRY) {
    // Pantalla de entrada
    u8g2.setFont(u8g2_font_helvB08_tr);
    u8g2.drawStr(2, 50, "Gasto:");

    u8g2.setFont(u8g2_font_helvR10_tr);
    u8g2.setCursor(2, 70); // Ajusta la posición según necesites
    u8g2.print("$");
    u8g2.print(entryValue, 2);  // Mostrar el valor ingresado con 2 decimales
  }

  u8g2.sendBuffer();  // Transferir la memoria interna a la pantalla
  delay(100);  // Pequeño retraso para evitar rebotes en el teclado
}
