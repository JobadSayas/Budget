float version = 3.07;

#include <Wire.h>
#include <U8g2lib.h>
#include <Keypad.h>

// Crear una instancia de la librería U8g2 para la pantalla OLED con rotación de 90 grados (U8G2_R1)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R1, U8X8_PIN_NONE, U8X8_PIN_NONE);

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

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Declaración de variables para los valores
float presupuesto = 0;
float gastado = 0;
float disponible = presupuesto - gastado;  // Calcular el valor disponible
float entryValue = 0;  // Variable para almacenar el valor ingresado en la pantalla de entrada
float lastGastos[7] = {0, 0, 0, 0, 0, 0, 0};  // Array para almacenar los últimos 5 gastos
int gastoCount = 0;  // Contador para el número de gastos ingresados
bool entryStarted = false;  // Flag para indicar si la entrada ha comenzado
bool decimalStarted = false;  // Flag para indicar si se ha ingresado un decimal
int decimalCount = 0;  // Contador para los dígitos decimales
bool pantalla = true;

// Estado de la pantalla actual
enum Screen { MAIN, MENU, ENTRY_ADD, ENTRY_SUB, ENTRY_PRE, ULTIMOS_REG };
Screen currentScreen = MAIN;

// Opciones de menú
int menuIndex = 0;  // Índice para la navegación del menú
const char* menuOptions[] = {"Registros", "Pantalla", "Reiniciar"};
const int menuOptionsCount = 3;

void setup() {
  u8g2.begin();  // Inicializar la pantalla
  delay(100);    // Añadir una pequeña pausa para asegurar que la pantalla esté lista
}

void loop() {
  char key = keypad.getKey();  // Leer la tecla presionada

  if (key) {
    if (currentScreen == MAIN) {
      if (key == 'A') {
        currentScreen = ENTRY_ADD;
        entryValue = 0;  
        entryStarted = false;
        decimalStarted = false;  
        decimalCount = 0;
      } else if (key == 'B') {
        currentScreen = ENTRY_SUB;
        entryValue = 0;  
        entryStarted = false;
        decimalStarted = false;  
        decimalCount = 0;
      } else if (key == 'C') {
        // Mostrar el menú
        currentScreen = MENU;
        menuIndex = 0;  // Iniciar en la primera opción del menú
      }
    } else if (currentScreen == MENU) {
      if (key == 'A') {
        menuIndex = (menuIndex - 1 + menuOptionsCount) % menuOptionsCount;  // Mover hacia arriba en el menú
      } else if (key == 'B') {
        menuIndex = (menuIndex + 1) % menuOptionsCount;  // Mover hacia abajo en el menú
      } else if (key == 'D') {
        if (menuIndex == 0) {
          // Selección de "Registros"
          currentScreen = ULTIMOS_REG;
        } else if (menuIndex == 1) {
          // Selección de "Pantalla"
          u8g2.setPowerSave(1);  // Apagar la pantalla
          pantalla = false;      // Actualizar el estado de la pantalla a apagada
        }
        else if (menuIndex == 2) {
          // Selección de "Reiniciar"
          currentScreen = ENTRY_PRE;
          entryValue = 0;  
          entryStarted = false;
          decimalStarted = false;  
          decimalCount = 0;
        }
      } else if (key == '#') {
          // Regresar a la pantalla principal y encender la pantalla si está apagada
          currentScreen = MAIN;
          if (!pantalla) {
              u8g2.setPowerSave(0);  // Encender la pantalla
              pantalla = true;       // Actualizar el estado de la pantalla a encendida
          }
      }
    } else if (currentScreen == ENTRY_ADD || currentScreen == ENTRY_SUB || currentScreen == ENTRY_PRE) {
      if (key == '#') {
        currentScreen = MAIN;  // Cancelar y regresar al menú principal
      } else if (key == 'D') {
        if (currentScreen == ENTRY_ADD) {
          gastado -= entryValue;
          // Guardar el nuevo gasto y actualizar el array
          if (gastoCount < 7) {
            lastGastos[gastoCount] = entryValue;
            gastoCount++;
          } else {
            for (int i = 0; i < 6; i++) {
              lastGastos[i] = lastGastos[i + 1];  // Desplazar los valores
            }
            lastGastos[6] = entryValue;  // Añadir el nuevo gasto al final
          }
        } else if (currentScreen == ENTRY_SUB) {
          gastado += entryValue;
          // Guardar el nuevo gasto y actualizar el array
          if (gastoCount < 7) {
            lastGastos[gastoCount] = entryValue;
            gastoCount++;
          } else {
            for (int i = 0; i < 6; i++) {
              lastGastos[i] = lastGastos[i + 1];  // Desplazar los valores
            }
            lastGastos[6] = entryValue;  // Añadir el nuevo gasto al final
          }
        } else if (currentScreen == ENTRY_PRE) {
          presupuesto = entryValue;
          gastado = 0;  // Resetear gastado al establecer un nuevo presupuesto
          // Limpiar los registros
          for (int i = 0; i < 7; i++) {
            lastGastos[i] = 0;
          }
          gastoCount = 0;
        }
        currentScreen = MAIN;
      } else {
        // En las pantallas de entrada, añadir números al valor
        if (isDigit(key) || key == '*') {
          if (key == '*' && decimalStarted) {
            return;  // No permitir más de un punto decimal
          }
          
          if (key == '*') {
            decimalStarted = true;  // Comenzar la parte decimal
          } else {
            if (decimalStarted) {
              decimalCount++;
              entryValue += (key - '0') / pow(10, decimalCount);  // Añadir el dígito a la parte decimal
            } else {
              entryValue = entryValue * 10 + (key - '0');
            }
          }
        }
      }
    } else if (currentScreen == ULTIMOS_REG) {
      // Pantalla de registros, regresar a la pantalla principal
      if (key == 'D' || key == '#') {
        currentScreen = MAIN;
      }
    }
  }

  // Calcular el valor disponible
  disponible = presupuesto - gastado;

  u8g2.clearBuffer();  // Limpiar la memoria interna

  if (currentScreen == MAIN) {
    // Pantalla principal
    u8g2.setFont(u8g2_font_helvB08_tr);
    u8g2.drawStr(2, 12, "Presupuesto");

    u8g2.setFont(u8g2_font_helvR12_tr);
    u8g2.setCursor(3, 30);
    u8g2.print("$");
    u8g2.print(presupuesto, 2);

    u8g2.drawStr(0, 35, "_____________________________");

    u8g2.setFont(u8g2_font_helvB08_tr);
    u8g2.drawStr(2, 52, "Gastado");

    u8g2.setFont(u8g2_font_helvR12_tr);
    u8g2.setCursor(3, 70);
    u8g2.print("$");
    u8g2.print(gastado, 2);

    u8g2.drawStr(0, 75, "_____________________________");

    u8g2.setFont(u8g2_font_helvB08_tr);
    u8g2.drawStr(2, 92, "Disponible");

    u8g2.setFont(u8g2_font_helvR12_tr);
    u8g2.setCursor(3, 110);
    u8g2.print("$");
    u8g2.print(disponible, 2);
  } else if (currentScreen == MENU) {
    // Menú de opciones
    u8g2.setFont(u8g2_font_helvB08_tr);
    u8g2.drawStr(2, 12, "Menu");

    for (int i = 0; i < menuOptionsCount; i++) {
      if (i == menuIndex) {
        u8g2.drawStr(2, 30 + (i * 16), ">");
      }
      u8g2.setCursor(12, 30 + (i * 16));
      u8g2.print(menuOptions[i]);
    }

    // Mostrar la versión en la parte inferior del menú
    u8g2.setFont(u8g2_font_6x10_tr);  // Fuente más pequeña para la versión
    u8g2.setCursor(2, 85);
    u8g2.print("Ver ");
    u8g2.print(version);
  } else if (currentScreen == ENTRY_ADD || currentScreen == ENTRY_SUB || currentScreen == ENTRY_PRE) {
    // Pantallas de entrada para agregar, restar o presupuesto
    if (currentScreen == ENTRY_ADD) {
      u8g2.setFont(u8g2_font_helvB08_tr);
      u8g2.drawStr(2, 12, "Ingreso");
    } else if (currentScreen == ENTRY_SUB) {
      u8g2.setFont(u8g2_font_helvB08_tr);
      u8g2.drawStr(2, 12, "Gasto");
    } else if (currentScreen == ENTRY_PRE) {
      u8g2.setFont(u8g2_font_helvB08_tr);
      u8g2.drawStr(2, 12, "Presupuesto");
    }

    u8g2.setFont(u8g2_font_helvR12_tr);
    u8g2.setCursor(3, 30);

    u8g2.print("$");
    u8g2.print(entryValue, 2);
  } else if (currentScreen == ULTIMOS_REG) {
    // Pantalla de registros
    u8g2.setFont(u8g2_font_helvB08_tr);
    u8g2.drawStr(2, 12, "Registros");

    u8g2.setFont(u8g2_font_helvR10_tr);
    for (int i = 0; i < 7; i++) {
      if (lastGastos[i] != 0 || (i == 0 && gastoCount == 0)) {
        u8g2.setCursor(2, 30 + (i * 15));  // Ajustar la posición de acuerdo a los gastos
        u8g2.print("$");
        u8g2.print(lastGastos[i], 2);
      }
    }
  }

  u8g2.sendBuffer();  // Enviar el buffer a la pantalla
}
