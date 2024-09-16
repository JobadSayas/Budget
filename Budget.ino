// Version 1.4

#include <Wire.h>
#include <U8g2lib.h>

// Crear una instancia de la librería U8g2 para la pantalla OLED con rotación de 90 grados (U8G2_R1)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R1, U8X8_PIN_NONE, U8X8_PIN_NONE);

// Declaración de variables para los valores, ahora como float para permitir decimales
float presupuesto = 1300.00;
float gastado = 205.30;
float restante = presupuesto - gastado;  // Calcular el valor restante

void setup() {
  u8g2.begin();  // Inicializar la pantalla
}

void loop() {
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
  delay(1000);  // Esperar 1 segundo
}