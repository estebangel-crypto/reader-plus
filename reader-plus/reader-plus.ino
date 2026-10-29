//Sticky Tiempo V1.0.2 - TACTIL - Detalle max info Open-Meteo + Gestos
// Base V1.0.1 + tactil GT911 (astro V9) - 6 idiomas
// Tap en EXTERIOR y 3 dias -> detalle con borde fino, 2 columnas con icono
// Swipe izq/der navega entre detalles, tap vuelve a resumen
// Open-Meteo ampliado a todos los datos posibles

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <FS.h>
#include <SdFat.h>
// SdFat para exFAT 64GB (como CrossPoint)
SdFs sdFat;
FsFile dirFile;
bool usandoSdFat = false;
#define USE_PARTIAL_EPAPER
#include <Seeed_GFX.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <time.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>
#pragma once
#include <Arduino.h>

// tiempo - naturaleza.png 80x80
const unsigned char icon_tiempo_80x80[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x7e, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x7f, 0x81, 0xfc, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0f, 0xc1, 0xff, 0xf0, 0xfc, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0f, 0x87, 0xff, 0xfc, 0x7c, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0x1f, 0xff, 0xfe, 0x3c, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x06, 0x3f, 0xff, 0xff, 0x18, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x04, 0x7f, 0xff, 0xff, 0x88, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xe0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xe0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xf0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xf0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xf0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x23, 0xff, 0xff, 0xff, 0xf9, 0x80,
  0x00, 0x00, 0x00, 0x00, 0xe7, 0xff, 0xff, 0xff, 0xf9, 0xc0,
  0x00, 0x00, 0x00, 0x00, 0xe7, 0xff, 0xff, 0xff, 0xf9, 0xf0,
  0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xf9, 0xf8,
  0x00, 0x00, 0x00, 0x1f, 0x81, 0xff, 0xff, 0xff, 0xf9, 0xf8,
  0x00, 0x00, 0x01, 0xff, 0xf8, 0x7f, 0xff, 0xff, 0xf9, 0xf0,
  0x00, 0x00, 0x0f, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xf9, 0xc0,
  0x00, 0x00, 0x3f, 0xff, 0xff, 0x8f, 0xff, 0xff, 0xf9, 0x80,
  0x00, 0x00, 0x7f, 0xff, 0xff, 0xc7, 0xff, 0xff, 0xf0, 0x00,
  0x00, 0x00, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xf0, 0x00,
  0x00, 0x01, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xf0, 0x00,
  0x00, 0x03, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xe0, 0x00,
  0x00, 0x07, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xc0, 0x00,
  0x00, 0x0f, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xff, 0xc0, 0x00,
  0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0x88, 0x00,
  0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0x18, 0x00,
  0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xfe, 0x3c, 0x00,
  0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xf8, 0x7c, 0x00,
  0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xe0, 0xfc, 0x00,
  0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xcf, 0x01, 0xfc, 0x00,
  0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x3e, 0x00,
  0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00,
  0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x78, 0x00, 0x00,
  0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe3, 0xff, 0x00, 0x00,
  0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xff, 0xc0, 0x00,
  0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00,
  0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00,
  0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00,
  0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00,
  0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00,
  0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00,
  0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00,
  0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00,
  0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00,
  0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00,
  0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00,
  0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00,
  0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00,
  0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00,
  0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00,
  0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00,
  0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00,
  0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#define ICON_TIEMPO_80X80_W 80
#define ICON_TIEMPO_80X80_H 80

const unsigned char icon_astro_80x80[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xE0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xFC, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xC0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x1F, 0xFF, 0xF0, 0x04, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x3F, 0xFF, 0xE0, 0x1E, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x3F, 0xFF, 0xC0, 0x1E, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xC0, 0x7F, 0xFF, 0x80, 0x3F, 0x01, 0xC0, 0x00,
  0x00, 0x0E, 0x00, 0xFF, 0xFF, 0x0F, 0xFF, 0x00, 0x70, 0x00,
  0x00, 0x38, 0x00, 0xFF, 0xFE, 0x0F, 0xFF, 0x00, 0x1C, 0x00,
  0x00, 0x70, 0x01, 0xFF, 0xFE, 0x0F, 0xFF, 0xE0, 0x0E, 0x00,
  0x00, 0xE0, 0x01, 0xFF, 0xFC, 0x07, 0xFF, 0xF0, 0x03, 0x00,
  0x01, 0xC0, 0x03, 0xFF, 0xFC, 0x07, 0xFF, 0xF8, 0x01, 0x80,
  0x03, 0x80, 0x03, 0xFF, 0xF8, 0x03, 0xFF, 0xF0, 0x01, 0xC0,
  0x03, 0x00, 0x03, 0xFF, 0xF8, 0x03, 0xFF, 0xE0, 0x00, 0xC0,
  0x03, 0x00, 0x07, 0xFF, 0xF8, 0x07, 0xFF, 0x80, 0x00, 0xC0,
  0x03, 0x00, 0x07, 0xFF, 0xF8, 0x07, 0xFF, 0x00, 0x00, 0xC0,
  0x03, 0x80, 0x07, 0xFF, 0xF0, 0x07, 0xFF, 0x00, 0x01, 0xC0,
  0x01, 0xC0, 0x07, 0xFF, 0xF0, 0x07, 0xFF, 0x00, 0x03, 0xC0,
  0x01, 0xE0, 0x07, 0xFF, 0xF0, 0x07, 0x9F, 0x00, 0x07, 0x80,
  0x00, 0xF8, 0x07, 0xFF, 0xF0, 0x00, 0x0F, 0x00, 0x0F, 0x00,
  0x00, 0x7E, 0x07, 0xFF, 0xF0, 0x00, 0x02, 0x00, 0x7E, 0x00,
  0x00, 0x3F, 0xC7, 0xFF, 0xF0, 0x00, 0x00, 0x01, 0xFC, 0x00,
  0x00, 0x0F, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x1F, 0xF0, 0x00,
  0x00, 0x03, 0xFF, 0xFF, 0xF8, 0x00, 0x03, 0xFF, 0xC0, 0x00,
  0x00, 0x00, 0xFF, 0xFF, 0xFC, 0x3F, 0xFF, 0xFF, 0x00, 0x00,
  0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x43, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xE1, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x07, 0xE1, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x07, 0xF8, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x03, 0xFC, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x03, 0xF8, 0x7F, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x07, 0xE0, 0x7F, 0xFF, 0xFC, 0x00, 0x01, 0x80, 0x00,
  0x00, 0x07, 0xE0, 0x3F, 0xFF, 0xFF, 0x00, 0x0F, 0x80, 0x00,
  0x00, 0x00, 0x60, 0x1F, 0xFF, 0xFF, 0xF0, 0xFF, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0xFF, 0xFE, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xE0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xC0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xFE, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xF0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
#define ICON_TIEMPO_80X80_W 80
#define ICON_TIEMPO_80X80_H 80


// actualizar - actualizar.png 80x80
const unsigned char icon_actualizar_80x80[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x7F, 0xFC, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00,
  0x00, 0x07, 0xFF, 0xFF, 0xE0, 0x00, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x3F,
  0xFF, 0xFF, 0xFC, 0x00, 0x1F, 0x80, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
  0xFF, 0x00, 0x1F, 0x80, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0,
  0x1F, 0x80, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x1F, 0x80,
  0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x1F, 0x80, 0x00, 0x00,
  0x7F, 0xFF, 0x80, 0x01, 0xFF, 0xFE, 0x1F, 0x80, 0x00, 0x01, 0xFF, 0xF8,
  0x00, 0x00, 0x1F, 0xFF, 0x9F, 0x80, 0x00, 0x03, 0xFF, 0xE0, 0x00, 0x00,
  0x07, 0xFF, 0xDF, 0x80, 0x00, 0x07, 0xFF, 0x80, 0x00, 0x00, 0x01, 0xFF,
  0xFF, 0x80, 0x00, 0x0F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0x80,
  0x00, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0x80, 0x00, 0x3F,
  0xF0, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0x80, 0x00, 0x7F, 0xE0, 0x00,
  0x00, 0x00, 0x00, 0x07, 0xFF, 0x80, 0x00, 0xFF, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xFF, 0x80, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0xFF, 0x80, 0x01, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0x80,
  0x03, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0x80, 0x03, 0xFC,
  0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0x80, 0x07, 0xF8, 0x00, 0x00,
  0x00, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00,
  0x3F, 0xFF, 0xFF, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF,
  0xFC, 0x00, 0x0F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x1F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xC0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFC,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF8, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0xF0, 0x00, 0x3F, 0xFF, 0xF8, 0x00, 0x00,
  0x00, 0x00, 0x0F, 0xF0, 0x00, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00,
  0x0F, 0xE0, 0x00, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xE0,
  0x01, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x01, 0xFF,
  0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xC0, 0x01, 0xFF, 0xFF, 0xF8,
  0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x01, 0xFF, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xFF, 0x00, 0x01, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01,
  0xFF, 0x00, 0x01, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFE, 0x00,
  0x01, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFC, 0x00, 0x01, 0xFF,
  0xF8, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x01, 0xFF, 0xFE, 0x00,
  0x00, 0x00, 0x00, 0x7F, 0xF0, 0x00, 0x01, 0xFF, 0xFF, 0x80, 0x00, 0x00,
  0x00, 0xFF, 0xE0, 0x00, 0x01, 0xFB, 0xFF, 0xE0, 0x00, 0x00, 0x07, 0xFF,
  0xC0, 0x00, 0x01, 0xF9, 0xFF, 0xF8, 0x00, 0x00, 0x1F, 0xFF, 0x80, 0x00,
  0x01, 0xF8, 0x7F, 0xFF, 0x80, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x01, 0xF8,
  0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x01, 0xF8, 0x0F, 0xFF,
  0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x01, 0xF8, 0x03, 0xFF, 0xFF, 0xFF,
  0xFF, 0xC0, 0x00, 0x00, 0x01, 0xF8, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
  0x00, 0x00, 0x01, 0xF8, 0x00, 0x3F, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x00,
  0x01, 0xF8, 0x00, 0x07, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0xF0,
  0x00, 0x00, 0x3F, 0xFE, 0x00, 0x00, 0x00, 0x00,
};
#define ICON_ACTUALIZAR_80X80_W 80
#define ICON_ACTUALIZAR_80X80_H 80

// buscar - busqueda.png 80x80
const unsigned char icon_buscar_80x80[] PROGMEM = {
  0x00, 0x00, 0x00, 0x1F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF,
  0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xFF,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00,
  0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00,
  0x00, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x0F,
  0xFF, 0xE0, 0x00, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0x00,
  0x00, 0x1F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF8, 0x00, 0x00, 0x07,
  0xFF, 0x80, 0x00, 0x00, 0x00, 0x7F, 0xF0, 0x00, 0x00, 0x01, 0xFF, 0xC0,
  0x00, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x7F, 0xE0, 0x00, 0x00,
  0x01, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x3F, 0xE0, 0x00, 0x00, 0x03, 0xFF,
  0x00, 0x00, 0x00, 0x00, 0x1F, 0xF0, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x00,
  0x00, 0x00, 0x0F, 0xF8, 0x00, 0x00, 0x07, 0xFC, 0x00, 0x00, 0x00, 0x00,
  0x07, 0xFC, 0x00, 0x00, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFC,
  0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFE, 0x00, 0x00,
  0x1F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x1F, 0xE0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x7F, 0x80, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
  0x80, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00,
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x7F, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xC0, 0x00, 0x7F, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x1F, 0xC0, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x1F, 0xC0, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F,
  0xC0, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xC0, 0x00,
  0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0xFE, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0xFE, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0F, 0xE0, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F,
  0xE0, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00,
  0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0xFE, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xC0, 0x00, 0xFE, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x1F, 0xC0, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x1F, 0xC0, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F,
  0xC0, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xC0, 0x00,
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x3F, 0x80,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x3F, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x7F, 0x00, 0x00, 0x1F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F,
  0x00, 0x00, 0x1F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00,
  0x0F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x0F, 0xF0,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x07, 0xFC, 0x00, 0x00, 0x00, 0x00,
  0x07, 0xF8, 0x00, 0x00, 0x03, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF8,
  0x00, 0x00, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF0, 0x00, 0x00,
  0x00, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x3F, 0xE0, 0x00, 0x00, 0x00, 0xFF,
  0xE0, 0x00, 0x00, 0x00, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x7F, 0xF0, 0x00,
  0x00, 0x01, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x3F, 0xFC, 0x00, 0x00, 0x07,
  0xFF, 0xFC, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0x80, 0x00, 0x3F, 0xFF, 0xFE,
  0x00, 0x00, 0x00, 0x07, 0xFF, 0xF0, 0x03, 0xFF, 0xFD, 0xFF, 0x00, 0x00,
  0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0xFF, 0x80, 0x00, 0x00, 0x01,
  0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x7F, 0xC0, 0x00, 0x00, 0x00, 0x7F, 0xFF,
  0xFF, 0xFF, 0x80, 0x3F, 0xE0, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xFE,
  0x00, 0x1F, 0xF0, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0xF8, 0x00, 0x0F,
  0xF8, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xC0, 0x00, 0x07, 0xFC, 0x00,
  0x00, 0x00, 0x00, 0x03, 0xF8, 0x00, 0x00, 0x03, 0xFE, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x7F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x3F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF8, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFC, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C,
};
#define ICON_BUSCAR_80X80_W 80
#define ICON_BUSCAR_80X80_H 80

// sd - tarjeta-sd.png 80x80
const unsigned char icon_sd_80x80[] PROGMEM = {
  0x00, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3F,
  0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFF,
  0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xF0, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00,
  0x00, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x00,
  0x07, 0xFE, 0x00, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x00, 0x0F, 0xFC,
  0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00,
  0x00, 0x01, 0xFC, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x01,
  0xFC, 0x00, 0x00, 0x00, 0x7F, 0xE0, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00,
  0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x01,
  0xFF, 0x80, 0x70, 0xFC, 0x3F, 0x01, 0xFC, 0x00, 0x00, 0x03, 0xFF, 0x00,
  0xF8, 0xFE, 0x3F, 0x81, 0xFC, 0x00, 0x00, 0x07, 0xFE, 0x01, 0xF8, 0xFE,
  0x3F, 0x81, 0xFC, 0x00, 0x00, 0x0F, 0xFC, 0x03, 0xF8, 0xFE, 0x3F, 0x81,
  0xFC, 0x00, 0x00, 0x0F, 0xF8, 0x03, 0xF8, 0xFE, 0x3F, 0x81, 0xFC, 0x00,
  0x00, 0x1F, 0xF0, 0x03, 0xF8, 0xFE, 0x3F, 0x81, 0xFC, 0x00, 0x00, 0x1F,
  0xE0, 0x03, 0xF8, 0xFE, 0x3F, 0x81, 0xFC, 0x00, 0x00, 0x1F, 0xC0, 0x03,
  0xF8, 0xFE, 0x3F, 0x81, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x03, 0xF8, 0xFE,
  0x3F, 0x81, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x03, 0xF8, 0xFE, 0x3F, 0x81,
  0xFC, 0x00, 0x00, 0x3F, 0x80, 0x03, 0xF8, 0xFE, 0x3F, 0x81, 0xFC, 0x00,
  0x00, 0x3F, 0x80, 0x03, 0xF8, 0xFE, 0x3F, 0x81, 0xFC, 0x00, 0x00, 0x3F,
  0x80, 0x03, 0xF8, 0xFE, 0x3F, 0x81, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x03,
  0xF0, 0xFC, 0x3F, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01,
  0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00,
  0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01,
  0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00,
  0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01,
  0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00,
  0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01,
  0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00,
  0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01,
  0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00,
  0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01,
  0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00,
  0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01,
  0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00,
  0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01,
  0xFC, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00,
  0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x3F, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFC, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00,
  0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x3F,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00,
};
#define ICON_SD_80X80_W 80
#define ICON_SD_80X80_H 80

// config - ajustes.png 80x80
const unsigned char icon_config_80x80[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x7F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x03, 0xFC, 0x3F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
  0xF8, 0x1F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF8, 0x1F,
  0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF8, 0x1F, 0xC0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF8, 0x1F, 0xC0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0F, 0xF8, 0x1F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x3F, 0xF8, 0x1F, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
  0xF8, 0x1F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x81, 0xFF, 0xF0, 0x0F,
  0xFF, 0x81, 0xF8, 0x00, 0x00, 0x7F, 0xE7, 0xFF, 0xF0, 0x0F, 0xFF, 0xE7,
  0xFE, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xC0, 0x03, 0xFF, 0xFF, 0xFF, 0x80,
  0x03, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xC0, 0x03, 0xFF,
  0xFF, 0xF8, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xC0, 0x07, 0xFF, 0xFF, 0xE0,
  0x00, 0x00, 0x07, 0xFF, 0xFF, 0xE0, 0x07, 0xFF, 0xFF, 0xC0, 0x00, 0x00,
  0x03, 0xFF, 0xFF, 0xE0, 0x0F, 0xF0, 0xFF, 0x80, 0x00, 0x00, 0x01, 0xFF,
  0x0F, 0xF0, 0x0F, 0xE0, 0x3F, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x07, 0xF0,
  0x0F, 0xE0, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x30, 0x07, 0xF0, 0x0F, 0xE0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF0, 0x0F, 0xE0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0xF0, 0x0F, 0xF0, 0x00, 0x00, 0x1F, 0xF8,
  0x00, 0x00, 0x0F, 0xF0, 0x07, 0xF8, 0x00, 0x00, 0x7F, 0xFE, 0x00, 0x00,
  0x1F, 0xE0, 0x07, 0xFE, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x7F, 0xE0,
  0x03, 0xFF, 0x80, 0x01, 0xFF, 0xFF, 0x80, 0x01, 0xFF, 0xC0, 0x03, 0xFF,
  0x80, 0x03, 0xFF, 0xFF, 0xC0, 0x01, 0xFF, 0xC0, 0x01, 0xFF, 0xC0, 0x07,
  0xFF, 0xFF, 0xE0, 0x03, 0xFF, 0x80, 0x00, 0x7F, 0xC0, 0x0F, 0xFE, 0x7F,
  0xF0, 0x03, 0xFE, 0x00, 0x00, 0x3F, 0x80, 0x0F, 0xF0, 0x0F, 0xF0, 0x01,
  0xFC, 0x00, 0x00, 0x3F, 0x80, 0x1F, 0xE0, 0x07, 0xF8, 0x01, 0xFC, 0x00,
  0x00, 0x3F, 0x80, 0x1F, 0xC0, 0x03, 0xF8, 0x01, 0xFC, 0x00, 0x00, 0x3F,
  0x80, 0x1F, 0xC0, 0x03, 0xF8, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x1F,
  0xC0, 0x03, 0xF8, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x1F, 0x80, 0x01,
  0xF8, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x1F, 0x80, 0x01, 0xF8, 0x01,
  0xFC, 0x00, 0x00, 0x3F, 0x80, 0x1F, 0xC0, 0x03, 0xF8, 0x01, 0xFC, 0x00,
  0x00, 0x3F, 0x80, 0x1F, 0xC0, 0x03, 0xF8, 0x01, 0xFC, 0x00, 0x00, 0x3F,
  0x80, 0x1F, 0xC0, 0x03, 0xF8, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x1F,
  0xE0, 0x07, 0xF8, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0x80, 0x0F, 0xF0, 0x0F,
  0xF0, 0x01, 0xFC, 0x00, 0x00, 0x7F, 0xC0, 0x0F, 0xFE, 0x7F, 0xF0, 0x03,
  0xFE, 0x00, 0x01, 0xFF, 0xC0, 0x07, 0xFF, 0xFF, 0xE0, 0x03, 0xFF, 0x80,
  0x03, 0xFF, 0x80, 0x03, 0xFF, 0xFF, 0xC0, 0x01, 0xFF, 0xC0, 0x03, 0xFF,
  0x80, 0x01, 0xFF, 0xFF, 0x80, 0x01, 0xFF, 0xC0, 0x07, 0xFE, 0x00, 0x00,
  0xFF, 0xFF, 0x00, 0x00, 0x7F, 0xE0, 0x07, 0xF8, 0x00, 0x00, 0x7F, 0xFE,
  0x00, 0x00, 0x1F, 0xE0, 0x0F, 0xF0, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00,
  0x0F, 0xF0, 0x0F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF0,
  0x0F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF0, 0x0F, 0xE0,
  0x0C, 0x00, 0x00, 0x00, 0x00, 0x30, 0x07, 0xF0, 0x0F, 0xE0, 0x3F, 0x00,
  0x00, 0x00, 0x00, 0xFC, 0x07, 0xF0, 0x0F, 0xF0, 0xFF, 0x80, 0x00, 0x00,
  0x01, 0xFF, 0x0F, 0xF0, 0x07, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x03, 0xFF,
  0xFF, 0xE0, 0x07, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0xE0,
  0x03, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xC0, 0x03, 0xFF,
  0xFF, 0xFE, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xC0, 0x01, 0xFF, 0xFF, 0xFF,
  0xC0, 0x03, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x7F, 0xE7, 0xFF, 0xF0, 0x0F,
  0xFF, 0xE7, 0xFE, 0x00, 0x00, 0x1F, 0x81, 0xFF, 0xF0, 0x0F, 0xFF, 0x81,
  0xF8, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xF8, 0x1F, 0xFF, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x3F, 0xF8, 0x1F, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0F, 0xF8, 0x1F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
  0xF8, 0x1F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF8, 0x1F,
  0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF8, 0x1F, 0xC0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF8, 0x1F, 0xC0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x03, 0xFC, 0x3F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFE, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00,
};
#define ICON_CONFIG_80X80_W 80
#define ICON_CONFIG_80X80_H 80




// === LOGO READER+ 2x2cm ~150x150 ===
// Logo Reader+ 150x150 ~2x2cm @133dpi
#define LOGO_READER_W 150
#define LOGO_READER_H 150
const unsigned char logo_reader_150x150[] PROGMEM = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f,
 0xfd, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xfc, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0xc0, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff,
 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x01,
 0xe0, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x3f,
 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00,
 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xef, 0xfe, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x03, 0x00, 0x07, 0x00, 0x00,
 0x03, 0xff, 0xff, 0xff, 0xe7, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
 0x00, 0x00, 0x03, 0x00, 0x01, 0xc0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xc7,
 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x08, 0x03, 0x00, 0x00,
 0xe0, 0x00, 0x1f, 0xfe, 0x7f, 0xff, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x3b, 0x00, 0x08, 0x02, 0x00, 0x00, 0x70, 0x00, 0x3f, 0xff, 0x7f,
 0xff, 0x81, 0xff, 0x70, 0x00, 0x00, 0x00, 0x00, 0x73, 0x00, 0x0c, 0x02,
 0x00, 0xc0, 0x18, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xe7, 0xff, 0x38, 0x00,
 0x00, 0x00, 0x00, 0x72, 0x00, 0x04, 0x02, 0x01, 0x80, 0x0c, 0x00, 0xff,
 0xff, 0xff, 0xff, 0xe7, 0xff, 0x38, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00,
 0x02, 0x02, 0x01, 0x00, 0x07, 0x01, 0xff, 0xff, 0xff, 0xff, 0xef, 0xff,
 0xb8, 0x00, 0x00, 0x00, 0x00, 0xe6, 0x00, 0x03, 0x02, 0x03, 0x00, 0x03,
 0x03, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0x9c, 0x00, 0x00, 0x00, 0x00,
 0xe6, 0x00, 0x01, 0x00, 0x06, 0x00, 0x01, 0x87, 0xff, 0xff, 0xfe, 0x3f,
 0xff, 0xff, 0x9c, 0x00, 0x00, 0x00, 0x00, 0xec, 0x04, 0x00, 0x0e, 0x04,
 0x00, 0x00, 0xcf, 0xff, 0xff, 0xf8, 0x7f, 0xff, 0xff, 0x9c, 0x00, 0x00,
 0x00, 0x00, 0xec, 0x06, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x4f, 0xfc, 0xff,
 0xe0, 0xff, 0xff, 0xff, 0xdc, 0x00, 0x00, 0x00, 0x01, 0xcc, 0x01, 0x81,
 0xff, 0xf0, 0x00, 0x00, 0x4f, 0xfc, 0xff, 0xc1, 0xff, 0xff, 0xff, 0xce,
 0x00, 0x00, 0x00, 0x01, 0xc8, 0x00, 0xe3, 0xff, 0xf8, 0x07, 0x00, 0x4f,
 0xf0, 0x3f, 0x81, 0xff, 0xff, 0x7f, 0xce, 0x00, 0x00, 0x00, 0x01, 0xd8,
 0x00, 0x07, 0xff, 0xfc, 0x1c, 0x00, 0x4f, 0xf8, 0xff, 0x03, 0xff, 0xff,
 0xff, 0xee, 0x00, 0x00, 0x00, 0x03, 0x98, 0x00, 0x0f, 0xff, 0xfe, 0x70,
 0x00, 0x4f, 0xfc, 0xfe, 0x03, 0xff, 0xff, 0xff, 0xe7, 0x00, 0x00, 0x00,
 0x03, 0x98, 0x00, 0x1f, 0xff, 0xff, 0x00, 0x00, 0x4f, 0xff, 0xfe, 0x07,
 0xff, 0xff, 0xff, 0xe7, 0x00, 0x00, 0x00, 0x03, 0xb0, 0x00, 0x1f, 0xff,
 0xff, 0x00, 0x00, 0x4f, 0xff, 0xfc, 0x07, 0xff, 0xff, 0xff, 0xe7, 0x00,
 0x00, 0x00, 0x07, 0x30, 0x00, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x4f, 0xff,
 0xfc, 0x07, 0xff, 0x7f, 0xff, 0xf3, 0x80, 0x00, 0x00, 0x07, 0x30, 0x00,
 0x07, 0x07, 0xff, 0x80, 0x00, 0x4f, 0xff, 0xfc, 0x07, 0xfe, 0x3f, 0xff,
 0xf3, 0x80, 0x00, 0x00, 0x07, 0x20, 0x1e, 0x3f, 0xe3, 0xff, 0x80, 0x00,
 0x4f, 0xff, 0xf8, 0x07, 0xfe, 0x3f, 0xff, 0xf3, 0x80, 0x00, 0x00, 0x07,
 0x60, 0x3c, 0x7f, 0xf9, 0xff, 0x80, 0x00, 0x4f, 0xff, 0xf8, 0x07, 0xff,
 0xff, 0xff, 0xfb, 0xc0, 0x00, 0x00, 0x0e, 0x60, 0x01, 0xff, 0xfc, 0xff,
 0x9f, 0x80, 0x4f, 0xff, 0xf8, 0x07, 0xff, 0xff, 0xf7, 0xf9, 0xc0, 0x00,
 0x00, 0x0e, 0x40, 0x01, 0xff, 0xfc, 0xff, 0x81, 0xc0, 0x4f, 0xff, 0xf8,
 0x07, 0xff, 0xff, 0xe7, 0xf9, 0xc0, 0x00, 0x00, 0x0e, 0xc0, 0x03, 0xff,
 0xfe, 0x03, 0x00, 0x00, 0x4f, 0xcf, 0xf8, 0x07, 0xff, 0xff, 0x81, 0xf9,
 0xc0, 0x00, 0x00, 0x1c, 0xc0, 0x07, 0xff, 0xff, 0x00, 0x00, 0x00, 0x4f,
 0xcf, 0xf8, 0x03, 0xff, 0xff, 0xc3, 0xfc, 0xe0, 0x00, 0x00, 0x1c, 0xc0,
 0x37, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x4f, 0xff, 0xf8, 0x03, 0xff, 0xff,
 0xe7, 0xfc, 0xe0, 0x00, 0x00, 0x1c, 0x80, 0xff, 0xff, 0xff, 0xff, 0x00,
 0x00, 0x4f, 0xff, 0xf8, 0x01, 0xff, 0xff, 0xf7, 0xfc, 0xe0, 0x00, 0x00,
 0x39, 0x83, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x4f, 0xff, 0xf8, 0x01,
 0xff, 0xff, 0xff, 0xfc, 0x70, 0x00, 0x00, 0x39, 0x83, 0xff, 0xff, 0xff,
 0xff, 0xc0, 0x00, 0x4f, 0xff, 0xfc, 0x00, 0xff, 0xff, 0xff, 0xfe, 0x70,
 0x00, 0x00, 0x39, 0x87, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x4f, 0xff,
 0xfc, 0x00, 0x7f, 0xff, 0xff, 0xfe, 0x70, 0x00, 0x00, 0x7b, 0x07, 0xff,
 0xff, 0xff, 0xff, 0xe0, 0x00, 0x4f, 0xff, 0xfe, 0x00, 0x3f, 0xff, 0xff,
 0xfe, 0x78, 0x00, 0x00, 0x73, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
 0x4f, 0xff, 0xfe, 0x00, 0x1f, 0xff, 0xff, 0xff, 0x38, 0x00, 0x00, 0x73,
 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x4f, 0xff, 0xff, 0x00, 0x03,
 0xf7, 0xff, 0xff, 0x38, 0x00, 0x00, 0x72, 0x07, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xc0, 0x4f, 0xff, 0xff, 0x80, 0x00, 0x07, 0xff, 0xff, 0x38, 0x00,
 0x00, 0xe6, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x4f, 0xfe, 0xff,
 0xc0, 0x00, 0x1f, 0xff, 0xff, 0x9c, 0x00, 0x00, 0xe6, 0x07, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xc0, 0x4f, 0xfc, 0xff, 0xf0, 0x00, 0x3f, 0xff, 0xff,
 0x9c, 0x00, 0x00, 0xe4, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x4f,
 0xfc, 0x7f, 0xfc, 0x00, 0xff, 0xdf, 0xe7, 0x9c, 0x00, 0x01, 0xcc, 0x01,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x4f, 0xf0, 0x3f, 0xff, 0xff, 0xff,
 0x8f, 0xe7, 0xce, 0x00, 0x01, 0xcc, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff,
 0xc0, 0x4f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xce, 0x00, 0x01,
 0xcc, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xc0, 0x4f, 0xfc, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xce, 0x00, 0x03, 0xd8, 0x00, 0x00, 0x00, 0x00,
 0x07, 0xff, 0x80, 0x4f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xcf,
 0x00, 0x03, 0x98, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x4f, 0xff,
 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xe7, 0x00, 0x03, 0x99, 0xff, 0xff,
 0xfc, 0x00, 0x00, 0x00, 0x00, 0x4f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xe7, 0x00, 0x03, 0x9f, 0x80, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00,
 0x4f, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x67, 0x00, 0x07, 0x38,
 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x4f, 0xff, 0xff, 0xff, 0xfc,
 0x00, 0x00, 0x00, 0x03, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0,
 0x00, 0x00, 0x4f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x03, 0x80,
 0x07, 0x00, 0xff, 0xff, 0xc0, 0x00, 0xf8, 0x00, 0x00, 0x4f, 0x3f, 0xff,
 0xfc, 0x00, 0x0f, 0xff, 0xfc, 0x03, 0x80, 0x0e, 0x3f, 0xff, 0xff, 0xff,
 0x80, 0x1e, 0x00, 0x00, 0x4f, 0x3f, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xff,
 0xf3, 0xc0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x03, 0xc0, 0x00, 0x4f,
 0xff, 0xff, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xf0,
 0x00, 0x0f, 0xff, 0x80, 0xe0, 0x00, 0x4f, 0xff, 0xfc, 0x03, 0xff, 0xc0,
 0x00, 0x3f, 0xff, 0xe0, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x38,
 0x00, 0x4f, 0xff, 0xf0, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x1c,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x0e, 0x00, 0x4f, 0xff, 0xc0, 0xf8,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0xff, 0xff, 0xff, 0x80,
 0x07, 0x83, 0x00, 0x4f, 0xff, 0x03, 0xc0, 0x07, 0xff, 0xff, 0xfc, 0x00,
 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0x00, 0xe1, 0xc0, 0x4f, 0xfc,
 0x1e, 0x03, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x3f, 0xff, 0xff,
 0xff, 0xff, 0xf0, 0x38, 0x60, 0x4f, 0xf8, 0x70, 0x3f, 0xff, 0xff, 0xff,
 0xff, 0xf0, 0x00, 0x00, 0x7f, 0xff, 0x80, 0x00, 0x00, 0xfe, 0x0c, 0x30,
 0x4f, 0xe0, 0xc1, 0xfc, 0x00, 0x00, 0x07, 0xff, 0xf8, 0x00, 0x00, 0x7f,
 0xe0, 0x00, 0x00, 0x00, 0x07, 0xc3, 0x18, 0x4f, 0xc3, 0x0f, 0x80, 0x00,
 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0xfe, 0x00, 0x0f, 0xff, 0xff, 0xc0,
 0x78, 0x8c, 0x4f, 0x84, 0x78, 0x0f, 0xff, 0xff, 0xc0, 0x01, 0xfc, 0x00,
 0x00, 0xe0, 0x07, 0xff, 0xff, 0xff, 0xff, 0x0e, 0x06, 0x4f, 0x01, 0xc3,
 0xff, 0xff, 0xff, 0xff, 0x80, 0x0c, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xe1, 0x02, 0x4e, 0x02, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xfc,
 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x01, 0x4c,
 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0xc8, 0x07, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xfe, 0x00, 0x03, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x03, 0xff,
 0xe0, 0x48, 0x1f, 0xff, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0x80, 0x07,
 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x7f, 0xc0, 0x00,
 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0xfc, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff,
 0xc0, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x01, 0xe0,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x1e, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x0f, 0x87, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf8, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1e, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0xe0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
 0x07, 0x01, 0xc0, 0x03, 0xc0, 0x01, 0xc7, 0x01, 0xc0, 0x00, 0x80, 0xc0,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x07, 0xf8, 0x0f, 0xf7,
 0x07, 0xf7, 0x07, 0xf8, 0x37, 0x80, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0xe0, 0x07, 0x0f, 0xfc, 0x1f, 0xff, 0x1f, 0xff, 0x0f, 0xfc, 0x3f,
 0x80, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x1e, 0x1e,
 0x3e, 0x3f, 0x1e, 0x1f, 0x1e, 0x1e, 0x3e, 0x00, 0xc0, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0xe0, 0x0e, 0x3c, 0x0e, 0x38, 0x0f, 0x38, 0x0f, 0x3c,
 0x0e, 0x38, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe,
 0x38, 0x07, 0x78, 0x0f, 0x38, 0x07, 0x38, 0x07, 0x38, 0x3f, 0xff, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfc, 0x3b, 0xcf, 0x70, 0x07, 0x30,
 0x07, 0x3b, 0xff, 0x38, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0xff, 0xf0, 0x7f, 0xff, 0x70, 0x07, 0x70, 0x07, 0x3f, 0xff, 0x30, 0x01,
 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xf0, 0x7f, 0xff, 0x70,
 0x07, 0x70, 0x07, 0x3f, 0xff, 0x30, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0xe0, 0x78, 0x38, 0x00, 0x78, 0x07, 0x38, 0x07, 0x38, 0x00,
 0x30, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x3c, 0x38,
 0x0e, 0x38, 0x0f, 0x38, 0x0f, 0x38, 0x0e, 0x30, 0x00, 0xc0, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x1c, 0x1e, 0x3c, 0x1f, 0x1e, 0x1f,
 0x1c, 0x1e, 0x30, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
 0x0e, 0x1f, 0xfc, 0x1f, 0xff, 0x1f, 0xff, 0x1f, 0xfc, 0x30, 0x00, 0xc0,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x0f, 0x0f, 0xf8, 0x0f, 0xf7,
 0x0f, 0xf7, 0x0f, 0xf8, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0xc0, 0x07, 0x03, 0xe0, 0x03, 0xc7, 0x03, 0xe3, 0x03, 0xe0, 0x30,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff,
 0xff, 0xff, 0xc0, 0xfe, 0x0f, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xfc, 0x0f,
 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


void dibujarLogoEspera(int y); // forward

#include "driver/rtc_io.h"
#include <esp_wifi.h>
#include <Update.h>
#include "esp_ota_ops.h"
#include "esp_partition.h"

#define VERSION_FIRMWARE "1.0.0"
#define INTERVALO_OTA_DIAS 7
#define TIEMPO_PULSACION_OTA_MS 2000
#define OTA_REFRESH_CADA_PCT 1
#define OTA_BUFFER_BYTES 8192
const char* OTA_VERSION_URL = "https://estebangel-crypto.github.io/sticky-weather/version.json";

const uint64_t TIEMPO_DESPTERTA_MIN = 3; // legacy, no usado en V1.0.6
const uint64_t INTERVALO_DATOS_MIN = 120;
const uint64_t INTERVALO_MIN = 120;
const uint64_t TIEMPO_BASE_SEG = 40; // ventana corta si no hay interaccion
const uint64_t TIEMPO_EXT_SEG = 180; // ventana larga si hay toque/boton
#define HORA_NOCHE_INI 23
#define HORA_NOCHE_FIN 7
#define SIMULAR_BATERIA -1
#define UMBRAL_BATERIA_BAJA 10
#define UMBRAL_BATERIA_RECUPERA 15
const uint64_t INTERVALO_BATERIA_BAJA_MIN = 60;
const uint64_t TIEMPO_PULSACION_LARGA_MS = 1500;
const char* AP_NOMBRE = "Sticky-Tiempo";
const unsigned long TIEMPO_PORTAL_MS = 15UL * 60UL * 1000UL;
#define PRELIMPIAR true
#define PIN_BTN_AI 5 // CENTRO fisico (antes 4)
#define PIN_BTN_UP 4 // ARRIBA fisico - el que despierta (antes 5)
#define PIN_BTN_DOWN 6 // ABAJO fisico menu
#define USAR_SHT40 true
#define Serial Serial0
#define DEBUG_SERIAL Serial0

#define PIN_PWR_HOLD 45
#define PIN_PWR_LOCK 46
#define PIN_EPD_PWR 47
#define PIN_TOUCH_PWR 42
#define PIN_TOUCH_SDA 3
#define PIN_TOUCH_SCL 2
#define PIN_TOUCH_RST 41
#define PIN_TOUCH_INT 21
#define PIN_SDA 1
#define PIN_SCL 0

// SD Card - reTerminal Sticky: CS=8 MISO=12 SCK=13 MOSI=14 (comparte SCK/MOSI con EPD)
#define PIN_SD_CS   8
#define PIN_SD_MISO 12
#define PIN_SD_SCK  13
#define PIN_SD_MOSI 14


void stickyPowerLatch(){
  pinMode(PIN_PWR_HOLD, OUTPUT);
  pinMode(PIN_PWR_LOCK, OUTPUT);
  digitalWrite(PIN_PWR_HOLD, HIGH); delay(5);
  digitalWrite(PIN_PWR_LOCK, HIGH); delay(5);
  digitalWrite(PIN_PWR_LOCK, LOW); delay(5);
  digitalWrite(PIN_PWR_LOCK, HIGH); delay(5);
  pinMode(PIN_EPD_PWR, OUTPUT); digitalWrite(PIN_EPD_PWR, HIGH);
  pinMode(PIN_TOUCH_PWR, OUTPUT); digitalWrite(PIN_TOUCH_PWR, HIGH);
}

Seeed_GFX display(Seeed_Product::reTerminal_Sticky);
#define NEGRO TFT_BLACK
#define BLANCO TFT_WHITE

// Definicion real del logo (usa display ya declarado) - ~2x2cm
void dibujarLogoEspera(int y){
  int x = (800 - LOGO_READER_W)/2;
  display.drawBitmap(x, y, logo_reader_150x150, LOGO_READER_W, LOGO_READER_H, NEGRO);
}

enum WeatherIcon { WICON_SOL=0, WICON_POCO_NUBLADO=1, WICON_NUBLADO=2, WICON_LLUVIA=3, WICON_NIEVE=4, WICON_TORMENTA=5, WICON_NIEBLA=6 };

struct Config { String ssid; String pass; String ciudad; float lat=NAN; float lon=NAN; int lang=0; int uni=0; int fmt=0; bool valida=false; } cfg;
Preferences prefs;
TwoWire &sensorWire = Wire1;
bool sdIniciada = false;

TwoWire I2C_SENS = Wire1;

// ---------- TACTIL GT911 del astro V9 ----------
#define TOUCH_SWAP  1
#define TOUCH_INV_X 1
#define TOUCH_INV_Y 1
uint8_t touchAddr = 0x5D;
bool gtLeer(uint16_t reg, uint8_t* buf, uint8_t n){
  Wire.beginTransmission(touchAddr); Wire.write(reg>>8); Wire.write(reg&0xFF);
  if(Wire.endTransmission(true)!=0) return false;
  if(Wire.requestFrom((int)touchAddr,(int)n)!=n) return false;
  for(uint8_t i=0;i<n;i++) buf[i]=Wire.read();
  return true;
}
void gtEscribir(uint16_t reg, uint8_t v){
  Wire.beginTransmission(touchAddr); Wire.write(reg>>8); Wire.write(reg&0xFF); Wire.write(v); Wire.endTransmission(true);
}
void gtReset(bool addr14){
  pinMode(PIN_TOUCH_INT,OUTPUT); pinMode(PIN_TOUCH_RST,OUTPUT);
  digitalWrite(PIN_TOUCH_INT,LOW); digitalWrite(PIN_TOUCH_RST,LOW); delay(10);
  digitalWrite(PIN_TOUCH_INT, addr14?HIGH:LOW); delay(1);
  digitalWrite(PIN_TOUCH_RST,HIGH); delay(5);
  digitalWrite(PIN_TOUCH_INT,LOW); delay(50);
  pinMode(PIN_TOUCH_INT,INPUT);
}
void tactilInit(){
  Wire.begin(PIN_TOUCH_SDA, PIN_TOUCH_SCL); Wire.setClock(100000);
  gtReset(false); Wire.beginTransmission(0x5D);
  if(Wire.endTransmission()==0) touchAddr=0x5D; else { gtReset(true); touchAddr=0x14; }
  Serial.printf("GT911 en 0x%02X\n", touchAddr);
}
void mapearToque(int rx,int ry,int &sx,int &sy){
#if TOUCH_SWAP
  sx=ry; sy=rx; const int W=800,H=480;
#else
  sx=rx; sy=ry; const int W=480,H=800;
#endif
#if TOUCH_INV_X
  sx=W-1-sx;
#endif
#if TOUCH_INV_Y
  sy=H-1-sy;
#endif
}
bool gDown=false; int gx0=0,gy0=0,gxl=0,gyl=0; unsigned long gTUlt=0;
void resetGesto(){ gDown=false; }
int evaluarGesto(int &x,int &y){
  int dx=gxl-gx0, dy=gyl-gy0; x=gx0; y=gy0; gDown=false;
  if(abs(dx)>=100 && abs(dx)>abs(dy)*3/2) return dx<0 ? 2 : 3;
  if(abs(dx)<40 && abs(dy)<40) return 1;
  return 0;
}
int leerGesto(int &x,int &y){
  uint8_t st=0; if(!gtLeer(0x814E,&st,1)) return 0;
  if(!(st&0x80)){
    if(gDown && millis()-gTUlt>150) return evaluarGesto(x,y);
    return 0;
  }
  uint8_t n=st&0x0F; int res=0;
  if(n>0 && n<=5){
    uint8_t p[8];
    if(gtLeer(0x814F,p,8)){
      int rx=p[1]|(p[2]<<8), ry=p[3]|(p[4]<<8), sx, sy;
      mapearToque(rx,ry,sx,sy);
      if(!gDown){ gDown=true; gx0=sx; gy0=sy; }
      gxl=sx; gyl=sy; gTUlt=millis();
    }
  } else if(gDown){ res=evaluarGesto(x,y); }
  gtEscribir(0x814E,0);
  return res;
}
void vaciarToques(){
  resetGesto(); unsigned long t=millis(), ultimo=millis(); int a,b; uint8_t st;
  while(millis()-t<1500){
    if(gtLeer(0x814E,&st,1) && (st&0x80)) { gtEscribir(0x814E,0); ultimo=millis(); }
    if(millis()-ultimo>300) break;
    delay(20);
  }
}

// ---------- ESTRUCTURAS EXTENDIDAS ----------
struct Dia{ 
  int code; int tmax; int tmin; int lluvia; 
  int tmaxAp; int tminAp;
  float precipSum; float rainSum; float showerSum; float snowSum;
  int vientoMax; int rachaMax; int dirDom;
  int uvMax; int solMin; 
  String sunrise; String sunset;
};
struct CurrExt{
  float precip; float rain; float shower; float snow;
  int cloud; int dirViento; int racha;
  float pres; float presSea; float rocio; float uv; int isDay;
};

RTC_DATA_ATTR time_t ultimoUpdateWeb = 0;
RTC_DATA_ATTR bool datosValidosRTC = false;
RTC_DATA_ATTR bool rtc_avisoBateriaMostrado = false;
RTC_DATA_ATTR int rtc_codeAhora=0, rtc_tempAhora=0, rtc_sensacion=0, rtc_humedad=0, rtc_viento=0;
RTC_DATA_ATTR Dia rtc_dias[4];
RTC_DATA_ATTR time_t ultimoCheckOTA = 0;
RTC_DATA_ATTR char rtc_versionRemotaStr[20] = "0.0.0";
RTC_DATA_ATTR bool rtc_hayUpdate = false;
RTC_DATA_ATTR CurrExt rtc_currExt;
RTC_DATA_ATTR time_t proximaAct = 0;
int codeAhora; int tempAhora; int sensacion; int humedad; int viento; Dia dias[4]; CurrExt currExt;
bool hayInterior=false; float tempInt=0; float humInt=0;
int nivelBateria=-1; bool cargando=false;
bool avisoBateriaBaja=false;
bool iconoLunaVisible=false;
bool hayActualizacion=false;
String versionRemotaDisponible="0.0.0";

// ================= INTEGRACION ASTRO (de Sticky Astro V9) =================
enum Pant { P_TIEMPO=0, P_DET_TIEMPO=1, P_MENU=2, P_ASTRO=3, P_SOL=4, P_LUNA=5, P_ESTRELLAS=6 };
Pant pantalla = P_MENU;   // pantalla que se esta mostrando
int menuPagina = 0;       // 0 = menu principal, 1 = segunda pagina (otra aplicacion)
int detalleActual = -1;     // 0-3 solo cuando pantalla==P_DET_TIEMPO

String horaSalidaSol="--:--", horaPuestaSol="--:--", horaSalidaLuna="--:--", horaPuestaLuna="--:--";
float duracionDiaHoras=0; float faseLuna=0; int iluminacionLuna=0;
float astroSunshineHoras=0; float astroUvMax=0; // nuevos de Open-Meteo

String issLat="0", issLon="0"; time_t issTs=0;   // ISS: ultima posicion descargada y su hora (unix)
// Copia de los datos descargados en memoria RTC (sobrevive al sueno profundo; no se pueden guardar String)
struct AstroRTC { char sr[8]; char ss[8]; float dur; char la[12]; char lo[12]; time_t ts; };
RTC_DATA_ATTR AstroRTC rtc_astro;
RTC_DATA_ATTR bool rtc_astroValido = false;
void guardarAstroRTC(){
  strlcpy(rtc_astro.sr, horaSalidaSol.c_str(), sizeof(rtc_astro.sr)); strlcpy(rtc_astro.ss, horaPuestaSol.c_str(), sizeof(rtc_astro.ss));
  rtc_astro.dur=duracionDiaHoras; strlcpy(rtc_astro.la, issLat.c_str(), sizeof(rtc_astro.la)); strlcpy(rtc_astro.lo, issLon.c_str(), sizeof(rtc_astro.lo));
  rtc_astro.ts=issTs; rtc_astroValido=true;
}
void restaurarAstroRTC(){
  horaSalidaSol=String(rtc_astro.sr); horaPuestaSol=String(rtc_astro.ss); duracionDiaHoras=rtc_astro.dur;
  issLat=String(rtc_astro.la); issLon=String(rtc_astro.lo); issTs=rtc_astro.ts;
}
void guardarTiempoRTC(){
  rtc_codeAhora = codeAhora; rtc_tempAhora = tempAhora; rtc_sensacion = sensacion; rtc_humedad = humedad; rtc_viento = viento;
  for(int i=0;i<4;i++) rtc_dias[i]=dias[i];
  rtc_currExt=currExt; ultimoUpdateWeb = time(nullptr); datosValidosRTC = true;
}
int idioma(){ return (cfg.lang>=0 && cfg.lang<=5) ? cfg.lang : 0; }
static const char* const T_ACTUAL[6] = {"Actual: %s -> %s","Current: %s -> %s","Atual: %s -> %s","Actuelle: %s -> %s","Attuale: %s -> %s","Aktuell: %s -> %s"};
Pant prevPantalla = P_TIEMPO;   // pantalla a la que se vuelve al cerrar el menu
bool otaSinRed = false;         // true = checkVersionOTA no hace consulta de red (solo usa lo guardado)
// Generador pseudoaleatorio propio: las estrellas salen siempre igual al redibujar la misma pantalla
static uint32_t seedEst = 1;
uint32_t rndE(uint32_t n){ seedEst = seedEst*1664525u + 1013904223u; return (seedEst>>8) % n; }
String urlFirmwareRemoto="";

void cargarConfig(){ prefs.begin("tiempo",true); cfg.ssid=prefs.getString("ssid",""); cfg.pass=prefs.getString("pass",""); cfg.ciudad=prefs.getString("ciudad",""); cfg.lat=prefs.getFloat("lat",NAN); cfg.lon=prefs.getFloat("lon",NAN); cfg.lang=prefs.getInt("lang",0); cfg.uni=prefs.getInt("uni",0); cfg.fmt=prefs.getInt("fmt",0); prefs.end(); cfg.valida=cfg.ssid.length()>0 && cfg.ciudad.length()>0 && !isnan(cfg.lat) && !isnan(cfg.lon); }
void guardarWifi(const String& s,const String& p){ prefs.begin("tiempo",false); prefs.putString("ssid",s); prefs.putString("pass",p); prefs.end(); cfg.ssid=s; cfg.pass=p; }
void guardarCiudad(const String& n,float la,float lo, int ln, int uni, int fmt){ prefs.begin("tiempo",false); prefs.putString("ciudad",n); prefs.putFloat("lat",la); prefs.putFloat("lon",lo); prefs.putInt("lang",ln); prefs.putInt("uni",uni); prefs.putInt("fmt",fmt); prefs.end(); cfg.ciudad=n; cfg.lat=la; cfg.lon=lo; cfg.lang=ln; cfg.uni=uni; cfg.fmt=fmt; cfg.valida=cfg.ssid.length()>0; }

void txt(const char* s,int x,int y,int size,bool negrita,int color=NEGRO);
void txtCentrado(const char* s,int cx,int y,int size,bool negrita,int color=NEGRO);
void txtDerecha(const char* s,int xd,int y,int size,bool negrita,int color=NEGRO);
void txtSimple(const char* s,int x,int y,int size,bool negrita,int color=NEGRO);
int iconoDe(int c);
const char * textoDe(int c);

RTC_DATA_ATTR int rtc_nivelBateria = -1;
bool leerBateriaReal(){
  bool ok=false; int voltaje=0; int socLeido=-1;
  for(int i=0;i<10;i++){
    sensorWire.beginTransmission(0x55); sensorWire.write(0x2C);
    if(sensorWire.endTransmission(false)==0){
      if(sensorWire.requestFrom((uint8_t)0x55, (uint8_t)2)==2){
        uint8_t lo = sensorWire.read(); uint8_t hi = sensorWire.read();
        int soc = lo;
        if(soc>=0 && soc<=100){
          if(soc < 5 && i < 5){ delay(120); continue; }
          socLeido=soc; nivelBateria=soc; ok=true; break;
        }
      }
    }
    delay(120);
  }
  sensorWire.beginTransmission(0x55); sensorWire.write(0x08);
  if(sensorWire.endTransmission(false)==0){
    if(sensorWire.requestFrom((uint8_t)0x55, (uint8_t)2)==2){
      uint8_t vLo = sensorWire.read(); uint8_t vHi = sensorWire.read();
      voltaje = (vHi<<8)|vLo;
    }
  }
  int16_t corriente=0;
  sensorWire.beginTransmission(0x55); sensorWire.write(0x0C);
  if(sensorWire.endTransmission(false)==0){
    if(sensorWire.requestFrom((uint8_t)0x55, (uint8_t)2)==2){
      corriente = (int16_t)(sensorWire.read() | (sensorWire.read()<<8));
    }
  }
  if(voltaje > 3800) cargando = true;
  else if(voltaje > 3600 && abs(corriente) > 10) cargando = true;
  else cargando = false;
  if(!ok){
    if(rtc_nivelBateria>=0 && rtc_nivelBateria<=100){ nivelBateria=rtc_nivelBateria; ok=true; }
    else if(nivelBateria<0) nivelBateria=50;
  } else {
    if(socLeido>=0) rtc_nivelBateria=socLeido;
    if(voltaje>4000 && nivelBateria<15){ nivelBateria=20; rtc_nivelBateria=20; }
  }
  return ok;
}
bool leerBateria(){
  bool ok = leerBateriaReal();
  if(!ok){ delay(250); ok=leerBateriaReal(); }
#if SIMULAR_BATERIA >= 0
  nivelBateria = SIMULAR_BATERIA; cargando = false; return true;
#else
  return ok;
#endif
}
bool leerBateriaForzada(){
  for(int i=0;i<3;i++){ if(leerBateriaReal()) return true; delay(300); }
  return false;
}
void leerSHT40(){
#if USAR_SHT40
  sensorWire.beginTransmission(0x44); sensorWire.write(0xFD);
  if(sensorWire.endTransmission()!=0) return; delay(15);
  if(sensorWire.requestFrom((uint8_t)0x44, (uint8_t)6)!=6) return;
  uint8_t b[6]; for(int i=0;i<6;i++) b[i]=sensorWire.read();
  uint16_t rt=(b[0]<<8)|b[1]; uint16_t rh=(b[3]<<8)|b[4];
  tempInt=-45.0f+175.0f*rt/65535.0f; humInt=constrain(-6.0f+125.0f*rh/65535.0f,0.0f,100.0f); hayInterior=true;
#endif
}
int anchoTxt(const char* s,int size){ int n=0; for(;*s;s++) if(((uint8_t)*s & 0xC0)!=0x80) n++; return n*6*size; }
void txt(const char* s,int x,int y,int size,bool negrita,int color){
  display.setTextSize(size); display.setTextColor(color); int cw=6*size; int g=max(1,size/3);
  while(*s){
    uint8_t c=(uint8_t)*s; char base=0; int marca=0; bool esComa=false; bool esPunto=false;
    if(c<0x80){ if(c==','){ esComa=true; s++; } else if(c=='.'){ esPunto=true; s++; } else { base=c; s++; } }
    else if(c==0xC3 && s[1]){ uint8_t d=(uint8_t)s[1]; s+=2;
      switch(d){ case 0xA1: base='a'; marca=1; break; case 0xA9: base='e'; marca=1; break; case 0xAD: base='i'; marca=1; break; case 0xB3: base='o'; marca=1; break; case 0xBA: base='u'; marca=1; break; case 0xB1: base='n'; marca=2; break; case 0x81: base='A'; marca=1; break; case 0x89: base='E'; marca=1; break; case 0x8D: base='I'; marca=1; break; case 0x93: base='O'; marca=1; break; case 0x9A: base='U'; marca=1; break; case 0x91: base='N'; marca=2; break; case 0xA0: base='a'; marca=3; break; case 0xA2: base='a'; marca=4; break; case 0xA4: base='a'; marca=5; break; case 0xA7: base='c'; marca=6; break; case 0xA8: base='e'; marca=3; break; case 0xAA: base='e'; marca=4; break; case 0xAB: base='e'; marca=5; break; case 0xAE: base='i'; marca=4; break; case 0xAF: base='i'; marca=5; break; case 0xB4: base='o'; marca=4; break; case 0xB6: base='o'; marca=5; break; case 0xB9: base='u'; marca=3; break; case 0xBB: base='u'; marca=4; break; case 0xBC: base='u'; marca=5; break; case 0x84: base='A'; marca=5; break; case 0x96: base='O'; marca=5; break; case 0x9C: base='U'; marca=5; break; }
    } else { s++; continue; }
    if(esComa){ display.fillRect(x+2*size, y+5*size, 2*size, 2*size, color); display.fillRect(x+1*size, y+6*size, 3*size, 1*size, color); }
    else if(esPunto){ display.fillRect(x+2*size, y+5*size, 2*size, 2*size, color); }
    else if(base){ char b[2]={base,0}; display.drawString(b,x,y); if(negrita) display.drawString(b,x+g,y); int dy=0; if(base>='A' && base<='Z') dy=-2*size; if(marca==1){ display.fillRect(x+3*size,y+dy,size,size,color); display.fillRect(x+2*size,y+dy+size,size,size,color); } else if(marca==2){ display.fillRect(x+1*size,y+dy+size,size,size,color); display.fillRect(x+2*size,y+dy,size,size,color); display.fillRect(x+3*size,y+dy+size,size,size,color); display.fillRect(x+4*size,y+dy,size,size,color); } else if(marca==3){ display.fillRect(x+1*size,y+dy,size,size,color); display.fillRect(x+2*size,y+dy+size,size,size,color); } else if(marca==4){ display.fillRect(x+2*size,y+dy,size,size,color); display.fillRect(x+1*size,y+dy+size,size,size,color); display.fillRect(x+3*size,y+dy+size,size,size,color); } else if(marca==5){ display.fillRect(x+1*size,y+dy,size,size,color); display.fillRect(x+3*size,y+dy,size,size,color); } else if(marca==6){ display.fillRect(x+2*size,y+6*size,size,size,color); display.fillRect(x+1*size,y+7*size,2*size,size,color); } }
    x+=cw;
  }
}
void txtCentrado(const char* s,int cx,int y,int size,bool negrita,int color){ int w=anchoTxt(s,size); txt(s,cx-w/2,y,size,negrita,color); }
void txtDerecha(const char* s,int xd,int y,int size,bool negrita,int color){ int w=anchoTxt(s,size); txt(s,xd-w,y,size,negrita,color); }
void txtSimple(const char* s,int x,int y,int size,bool negrita,int color){ txt(s,x,y,size,negrita,color); }
int iconoDe(int c){ if(c==0) return WICON_SOL; if(c<=2) return WICON_POCO_NUBLADO; if(c==3) return WICON_NUBLADO; if(c==45||c==48) return WICON_NIEBLA; if((c>=51&&c<=67)||(c>=80&&c<=82)) return WICON_LLUVIA; if((c>=71&&c<=77)||c==85||c==86) return WICON_NIEVE; if(c>=95) return WICON_TORMENTA; return WICON_NUBLADO; }
const char* textoDe(int c){
  if(cfg.lang == 0){ switch(c){ case 0: return "Despejado"; case 1: return "Mayormente despejado"; case 2: return "Parcialmente nublado"; case 3: return "Nublado"; case 45: case 48: return "Niebla"; case 51: case 53: case 55: return "Llovizna"; case 56: case 57: return "Llovizna helada"; case 61: return "Lluvia ligera"; case 63: return "Lluvia"; case 65: return "Lluvia fuerte"; case 66: case 67: return "Lluvia helada"; case 71: return "Nieve ligera"; case 73: return "Nieve"; case 75: return "Nieve fuerte"; case 77: return "Granizo fino"; case 80: case 81: case 82: return "Chubascos"; case 85: case 86: return "Chubascos de nieve"; case 95: return "Tormenta"; case 96: case 99: return "Tormenta con granizo"; } }
  else if(cfg.lang == 1){ switch(c){ case 0: return "Clear sky"; case 1: return "Mainly clear"; case 2: return "Partly cloudy"; case 3: return "Overcast"; case 45: case 48: return "Fog"; case 51: case 53: case 55: return "Drizzle"; case 56: case 57: return "Freezing drizzle"; case 61: return "Slight rain"; case 63: return "Moderate rain"; case 65: return "Heavy rain"; case 66: case 67: return "Freezing rain"; case 71: return "Slight snow"; case 73: return "Moderate snow"; case 75: return "Heavy snow"; case 77: return "Snow grains"; case 80: case 81: case 82: return "Rain showers"; case 85: case 86: return "Snow showers"; case 95: return "Thunderstorm"; case 96: case 99: return "Thunderstorm with hail"; } }
  else if(cfg.lang == 2){ switch(c){ case 0: return "Ceu limpo"; case 1: return "Predominantemente limpo"; case 2: return "Parcialmente nublado"; case 3: return "Encoberto"; case 45: case 48: return "Nevoa"; case 51: case 53: case 55: return "Chuvisco"; case 56: case 57: return "Chuvisco congelante"; case 61: return "Chuva leve"; case 63: return "Chuva moderada"; case 65: return "Chuva forte"; case 66: case 67: return "Chuva congelante"; case 71: return "Neve leve"; case 73: return "Neve moderada"; case 75: return "Neve forte"; case 77: return "Graos de neve"; case 80: case 81: case 82: return "Aguaceiros"; case 85: case 86: return "Aguaceiros de neve"; case 95: return "Trovoada"; case 96: case 99: return "Trovoada com granizo"; } }
  else if(cfg.lang == 3){ switch(c){ case 0: return "Ciel degage"; case 1: return "Principalement degage"; case 2: return "Partiellement nuageux"; case 3: return "Couvert"; case 45: case 48: return "Brouillard"; case 51: case 53: case 55: return "Bruine"; case 56: case 57: return "Bruine verglacante"; case 61: return "Pluie legere"; case 63: return "Pluie moderee"; case 65: return "Pluie forte"; case 66: case 67: return "Pluie verglacante"; case 71: return "Neige legere"; case 73: return "Neige moderee"; case 75: return "Neige forte"; case 77: return "Neige en grains"; case 80: case 81: case 82: return "Averses de pluie"; case 85: case 86: return "Averses de neige"; case 95: return "Orage"; case 96: case 99: return "Orage avec grele"; } }
  else if(cfg.lang == 4){ switch(c){ case 0: return "Cielo sereno"; case 1: return "Prevalentemente sereno"; case 2: return "Parzialmente nuvoloso"; case 3: return "Nuvoloso"; case 45: case 48: return "Nebbia"; case 51: case 53: case 55: return "Pioviggine"; case 56: case 57: return "Pioviggine gelata"; case 61: return "Pioggia leggera"; case 63: return "Pioggia moderata"; case 65: return "Pioggia forte"; case 66: case 67: return "Pioggia gelata"; case 71: return "Neve leggera"; case 73: return "Neve moderata"; case 75: return "Neve forte"; case 77: return "Granello di neve"; case 80: case 81: case 82: return "Rovesci di pioggia"; case 85: case 86: return "Rovesci di neve"; case 95: return "Temporale"; case 96: case 99: return "Temporale con grandine"; } }
  else { switch(c){ case 0: return "Wolkenlos"; case 1: return "Leicht bewolkt"; case 2: return "Teilweise bewolkt"; case 3: return "Bedeckt"; case 45: case 48: return "Nebel"; case 51: case 53: case 55: return "Nieselregen"; case 56: case 57: return "Gefrierender Niesel"; case 61: return "Leichter Regen"; case 63: return "Massiger Regen"; case 65: return "Starker Regen"; case 66: case 67: return "Gefrierender Regen"; case 71: return "Leichter Schneefall"; case 73: return "Schneefall"; case 75: return "Starker Schneefall"; case 77: return "Schneegriesel"; case 80: case 81: case 82: return "Regenschauer"; case 85: case 86: return "Schneschauer"; case 95: return "Gewitter"; case 96: case 99: return "Gewitter mit Hagel"; } }
  return (cfg.lang == 0) ? "Sin datos" : "No data";
}
void lineaGruesa(int x1,int y1,int x2,int y2,int g){ for(int o=-g;o<=g;o++){ display.drawLine(x1+o,y1,x2+o,y2,NEGRO); display.drawLine(x1,y1+o,x2,y2+o,NEGRO);} }
void formaNube(int cx,int cy,int s,int inset,uint32_t col){ display.fillCircle(cx-s*0.55,cy+s*0.10,max(1,(int)(s*0.35)-inset),col); display.fillCircle(cx-s*0.10,cy-s*0.20,max(1,(int)(s*0.50)-inset),col); display.fillCircle(cx+s*0.45,cy+s*0.05,max(1,(int)(s*0.40)-inset),col); display.fillRect(cx-s*0.55,cy+s*0.10,s,max(1,(int)(s*0.35)-inset),col); }
void nube(int cx,int cy,int s){ int inset=max(3,s/12); formaNube(cx,cy,s,0,NEGRO); formaNube(cx,cy,s,inset,BLANCO); }
void sol(int cx,int cy,int r){ int g=max(1,r/14); display.fillCircle(cx,cy,r*0.55,NEGRO); for(int i=0;i<8;i++){ float a=i*PI/4; lineaGruesa(cx+cos(a)*r*0.75,cy+sin(a)*r*0.75,cx+cos(a)*r*1.05,cy+sin(a)*r*1.05,g);} }
void dibujarIcono(int t,int cx,int cy,int s){
  int g=max(1,s/16);
  switch(t){
    case WICON_SOL: sol(cx,cy,s); break;
    case WICON_POCO_NUBLADO: sol(cx-s*0.30,cy-s*0.30,s*0.65); nube(cx+s*0.10,cy+s*0.15,s*0.85); break;
    case WICON_NUBLADO: nube(cx,cy,s); break;
    case WICON_LLUVIA: nube(cx,cy-s*0.25,s*0.9); for(int i=-1;i<=1;i++) lineaGruesa(cx+i*s*0.40,cy+s*0.45,cx+i*s*0.40-s*0.15,cy+s*0.90,g); break;
    case WICON_NIEVE: nube(cx,cy-s*0.25,s*0.9); for(int i=-1;i<=1;i++){ display.fillCircle(cx+i*s*0.40,cy+s*0.55,max(2,s/12),NEGRO); display.fillCircle(cx+i*s*0.40-s*0.12,cy+s*0.85,max(2,s/12),NEGRO);} break;
    case WICON_TORMENTA: nube(cx,cy-s*0.25,s*0.9); display.fillTriangle(cx+s*0.05,cy+s*0.30,cx-s*0.25,cy+s*0.70,cx+s*0.05,cy+s*0.65,NEGRO); display.fillTriangle(cx+s*0.05,cy+s*0.65,cx+s*0.25,cy+s*0.65,cx-s*0.10,cy+s*1.05,NEGRO); break;
    case WICON_NIEBLA: nube(cx,cy-s*0.30,s*0.8); for(int i=0;i<3;i++) lineaGruesa(cx-s*0.7,cy+s*0.30+i*s*0.28,cx+s*0.7,cy+s*0.30+i*s*0.28,g); break;
  }
}
void engranaje(int cx,int cy,int r){ display.fillCircle(cx,cy,r*0.70,NEGRO); for(int i=0;i<8;i++){ float a=i*PI/4; display.fillCircle(cx+cos(a)*r*0.85,cy+sin(a)*r*0.85,max(2,(int)(r*0.24)),NEGRO);} display.fillCircle(cx,cy,r*0.30,BLANCO); }
bool conectarWifi(const String& ssid,const String& pass){
  if(ssid.length()==0) return false;
  WiFi.mode(WIFI_STA); WiFi.setSleep(false); WiFi.setAutoReconnect(true);
  WiFi.begin(ssid.c_str(),pass.c_str());
  for(int i=0;i<40 && WiFi.status()!=WL_CONNECTED;i++){ delay(500); Serial.printf("WiFi conectando %d status %d\n", i, WiFi.status()); }
  return WiFi.status()==WL_CONNECTED;
}
String urlEnc(const String& s){ static const char* hex="0123456789ABCDEF"; String o; for(size_t i=0;i<s.length();i++){ uint8_t c=s[i]; if(isalnum(c)||c=='-'||c=='_'||c=='.'||c=='~') o+=(char)c; else { o+='%'; o+=hex[c>>4]; o+=hex[c&15]; } } return o; }

bool obtenerTiempo(){
  float lat=cfg.valida?cfg.lat:42.4627; float lon=cfg.valida?cfg.lon:-2.4449;
  String url="https://api.open-meteo.com/v1/forecast?latitude="+String(lat,4)+"&longitude="+String(lon,4);
  url+="&current=temperature_2m,relative_humidity_2m,apparent_temperature,is_day,precipitation,rain,showers,snowfall,weather_code,cloud_cover,wind_speed_10m,wind_direction_10m,wind_gusts_10m,surface_pressure,pressure_msl,dew_point_2m,uv_index";
  url+="&daily=weather_code,temperature_2m_max,temperature_2m_min,apparent_temperature_max,apparent_temperature_min,sunrise,sunset,precipitation_sum,rain_sum,showers_sum,snowfall_sum,precipitation_probability_max,wind_speed_10m_max,wind_gusts_10m_max,wind_direction_10m_dominant,uv_index_max,sunshine_duration";
  url+="&timezone=Europe%2FMadrid&forecast_days=4";
  WiFiClientSecure client; client.setInsecure(); HTTPClient http; http.begin(client,url);
  int cod=http.GET(); if(cod!=200){ http.end(); return false; }
  DynamicJsonDocument doc(16384); DeserializationError err=deserializeJson(doc,http.getString()); http.end(); if(err) return false;
  JsonObject cur=doc["current"]; 
  codeAhora=cur["weather_code"]; tempAhora=round((float)cur["temperature_2m"]); sensacion=round((float)cur["apparent_temperature"]); humedad=cur["relative_humidity_2m"]; viento=round((float)cur["wind_speed_10m"]);
  currExt.precip = cur["precipitation"]|0.0f; currExt.rain=cur["rain"]|0.0f; currExt.shower=cur["showers"]|0.0f; currExt.snow=cur["snowfall"]|0.0f;
  currExt.cloud=cur["cloud_cover"]|0; currExt.dirViento=cur["wind_direction_10m"]|0; currExt.racha=round((float)(cur["wind_gusts_10m"]|0.0f));
  currExt.pres=cur["pressure_msl"]|0.0f; currExt.presSea=cur["surface_pressure"]|0.0f; currExt.rocio=cur["dew_point_2m"]|0.0f; currExt.uv=cur["uv_index"]|0.0f; currExt.isDay=cur["is_day"]|1;
  JsonObject d=doc["daily"];
  for(int i=0;i<4;i++){
    dias[i].code=d["weather_code"][i]; dias[i].tmax=round((float)d["temperature_2m_max"][i]); dias[i].tmin=round((float)d["temperature_2m_min"][i]);
    dias[i].lluvia=d["precipitation_probability_max"][i]|0;
    dias[i].tmaxAp=round((float)(d["apparent_temperature_max"][i]|0.0f)); dias[i].tminAp=round((float)(d["apparent_temperature_min"][i]|0.0f));
    dias[i].precipSum=d["precipitation_sum"][i]|0.0f; dias[i].rainSum=d["rain_sum"][i]|0.0f; dias[i].showerSum=d["showers_sum"][i]|0.0f; dias[i].snowSum=d["snowfall_sum"][i]|0.0f;
    dias[i].vientoMax=round((float)(d["wind_speed_10m_max"][i]|0.0f)); dias[i].rachaMax=round((float)(d["wind_gusts_10m_max"][i]|0.0f)); dias[i].dirDom=d["wind_direction_10m_dominant"][i]|0;
    dias[i].uvMax=round((float)(d["uv_index_max"][i]|0.0f)); dias[i].solMin=round((float)(d["sunshine_duration"][i]|0.0f))/60;
    dias[i].sunrise = String((const char*)d["sunrise"][i]); dias[i].sunset = String((const char*)d["sunset"][i]);
  }
  return true;
}

// ---------- DESCARGA ASTRO + ISS ----------
// Open-Meteo: orto, ocaso y duracion del dia. (La Luna, planetas y constelaciones se calculan en el aparato.)
bool obtenerAstro(){
  float lat=cfg.valida?cfg.lat:42.4627; float lon=cfg.valida?cfg.lon:-2.4449;
  String url="https://api.open-meteo.com/v1/forecast?latitude="+String(lat,4)+"&longitude="+String(lon,4)+"&daily=sunrise,sunset,daylight_duration,sunshine_duration,uv_index_max&timezone=Europe%2FMadrid&forecast_days=1";
  WiFiClientSecure client; client.setInsecure(); HTTPClient http; if(!http.begin(client,url)) return false;
  int cod=http.GET(); if(cod!=200){ Serial.printf("Astro HTTP %d\n",cod); http.end(); return false; }
  String payload=http.getString(); http.end();
  DynamicJsonDocument doc(4096); if(deserializeJson(doc,payload)) return false;
  auto soloHora = [](String iso)->String{ int t=iso.indexOf('T'); if(t<0) return String("--:--"); return iso.substring(t+1, t+6); };
  String sr = doc["daily"]["sunrise"][0] | ""; String ss = doc["daily"]["sunset"][0] | "";
  if(!sr.length() || !ss.length()) return false;
  horaSalidaSol=soloHora(sr); horaPuestaSol=soloHora(ss);
  long secs = doc["daily"]["daylight_duration"][0] | 0L; if(secs>0) duracionDiaHoras = secs/3600.0f;
  long sunSec = doc["daily"]["sunshine_duration"][0] | 0L; astroSunshineHoras = sunSec/3600.0f;
  astroUvMax = doc["daily"]["uv_index_max"][0] | 0.0f;
  Serial.printf("Astro OK: Sol %s-%s %.1fh sun %.1fh uv %.1f\n", horaSalidaSol.c_str(), horaPuestaSol.c_str(), duracionDiaHoras, astroSunshineHoras, astroUvMax);
  return true;
}
// open-notify: posicion de la ISS y la hora (unix) a la que se midio
bool obtenerISS(){
  HTTPClient http; if(!http.begin("http://api.open-notify.org/iss-now.json")) return false;
  int cod=http.GET(); if(cod!=200){ http.end(); return false; }
  String payload=http.getString(); http.end();
  DynamicJsonDocument doc(1024); if(deserializeJson(doc,payload)) return false;
  const char* la = doc["iss_position"]["latitude"] | ""; const char* lo = doc["iss_position"]["longitude"] | "";
  if(!la[0] || !lo[0]) return false;
  issLat=String(la); issLon=String(lo);
  long ts = doc["timestamp"] | 0L; issTs = (ts>1700000000) ? (time_t)ts : time(nullptr);
  return true;
}

void dibujarLunaMenguante(int cx, int cy, int r){ display.fillCircle(cx, cy, r, NEGRO); display.fillCircle(cx+2, cy, r-2, BLANCO); }
void dibujarBateria(int x, int y, int pct, bool chg){ int w=32; int h=16; int borde=2; display.drawRect(x,y,w,h,NEGRO); display.fillRect(x+w,y+4,3,h-8,NEGRO); if(pct>=0){ int fillW = map(constrain(pct,0,100),0,100,0,w-borde*2); if(fillW>0) display.fillRect(x+borde,y+borde,fillW,h-borde*2,NEGRO); } if(chg){ int rx = x + w/2 - 2; int ry = y + 1; display.fillTriangle(rx+4, ry, rx-2, ry+7, rx+2, ry+7, NEGRO); display.fillTriangle(rx+2, ry+7, rx+6, ry+7, rx+0, ry+14, NEGRO); } char buf[8]; snprintf(buf,sizeof(buf),"%d%%",pct>=0?pct:0); txtSimple(buf,x+w+8,y,2,true); }
void dibujarBateria(int x,int y,int pct){ dibujarBateria(x,y,pct,cargando); }

int compararVersiones(String v1, String v2){ int a1=0,b1=0,c1=0,a2=0,b2=0,c2=0; sscanf(v1.c_str(), "%d.%d.%d", &a1,&b1,&c1); sscanf(v2.c_str(), "%d.%d.%d", &a2,&b2,&c2); if(a1!=a2) return (a1>a2)?1:-1; if(b1!=b2) return (b1>b2)?1:-1; if(c1!=c2) return (c1>c2)?1:-1; return 0; }
bool checkVersionOTA(){
  time_t ahora = time(nullptr);
  Serial.printf("checkVersionOTA ahora=%ld ultimoCheckOTA=%ld\n", (long)ahora, (long)ultimoCheckOTA);
  if(ahora < 1700000000){
    Serial.println("checkVersionOTA: sin hora valida (<1700000000)");
    return false;
  }
  prefs.begin("tiempo", true); String verIgnorada = prefs.getString("ota_ignore", ""); prefs.end();
  if(INTERVALO_OTA_DIAS>0 && ultimoCheckOTA != 0 && (ahora - ultimoCheckOTA) < (INTERVALO_OTA_DIAS * 86400)){
    Serial.println("checkVersionOTA: usando cache RTC");
    if(rtc_hayUpdate && compararVersiones(String(rtc_versionRemotaStr), String(VERSION_FIRMWARE)) <= 0){ rtc_hayUpdate = false; }
    if(rtc_hayUpdate && String(rtc_versionRemotaStr) == verIgnorada){ rtc_hayUpdate = false; }
    if(rtc_hayUpdate){ hayActualizacion = true; versionRemotaDisponible = String(rtc_versionRemotaStr); }
    return rtc_hayUpdate;
  }
  if(otaSinRed){
    Serial.println("checkVersionOTA: otaSinRed=true, no red");
    return false;
  }
  WiFiClientSecure client; client.setInsecure(); client.setTimeout(15000);
  HTTPClient http; http.setTimeout(15000); http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  Serial.printf("checkVersionOTA: GET %s\n", OTA_VERSION_URL);
  if(!http.begin(client, OTA_VERSION_URL)){
    Serial.println("checkVersionOTA: http.begin fallo");
    ultimoCheckOTA = ahora; // marca intento aunque falle begin
    return false;
  }
  int code = http.GET();
  Serial.printf("checkVersionOTA: HTTP code %d\n", code);
  if(code != 200){
    Serial.printf("checkVersionOTA: code !=200 payload? %s\n", http.getString().substring(0,100).c_str());
    http.end();
    ultimoCheckOTA = ahora; // marca que se intento, evita bucle ERROR WIFI DATOS generico
    return false;
  }
  String payload = http.getString(); http.end();
  Serial.printf("checkVersionOTA payload: %s\n", payload.c_str());
  DynamicJsonDocument doc(1024); if(deserializeJson(doc, payload)){
    Serial.println("checkVersionOTA: JSON parse fallo");
    ultimoCheckOTA = ahora;
    return false;
  }
  String verRemota = doc["version"] | "0.0.0"; String url = doc["url"] | "";
  prefs.begin("tiempo", true); String verIgnoradaDbg = prefs.getString("ota_ignore", "(ninguna)"); prefs.end();
  Serial.printf("checkVersionOTA verRemota=%s local=%s ignorada=%s url=%s\n", verRemota.c_str(), VERSION_FIRMWARE, verIgnoradaDbg.c_str(), url.c_str());
  int cmp = compararVersiones(verRemota, String(VERSION_FIRMWARE));
  Serial.printf("checkVersionOTA cmp=%d (1=remota mayor, -1=local mayor, 0=igual)\n", cmp);
  if(verRemota == verIgnorada){ ultimoCheckOTA = ahora; rtc_hayUpdate = false; hayActualizacion = false; strncpy(rtc_versionRemotaStr, verRemota.c_str(), sizeof(rtc_versionRemotaStr)-1); return false; }
  if(compararVersiones(verRemota, String(VERSION_FIRMWARE)) <= 0){ ultimoCheckOTA = ahora; rtc_hayUpdate = false; strncpy(rtc_versionRemotaStr, verRemota.c_str(), sizeof(rtc_versionRemotaStr)-1); hayActualizacion = false; return false; }
  ultimoCheckOTA = ahora; strncpy(rtc_versionRemotaStr, verRemota.c_str(), sizeof(rtc_versionRemotaStr)-1); rtc_hayUpdate = true; versionRemotaDisponible = verRemota; urlFirmwareRemoto = url; hayActualizacion = true; return true;
}
void pantallaUpdateDisponible(String verNueva){
  int l = (cfg.lang>=0 && cfg.lang<=5) ? cfg.lang : 0;
  const char* t_updateDisp[6] = {"Update %s disponible","Update %s available","Update %s disponivel","Mise a jour %s dispo","Aggiornamento %s disponibile","Update %s verfugbar"};
  const char* t_hold[6]       = {"MANTEN PULSADO BOTON CENTRAL","HOLD CENTER BUTTON","MANTENHA BOTAO CENTRAL","MAINTENEZ BOUTON CENTRAL","TIENI PREMUTO TASTO CENTRALE","MITTLERE TASTE HALTEN"};
  const char* t_2seg[6]        = {"2 SEG PARA ACTUALIZAR","2 SEC TO UPDATE","2 SEG PARA ATUALIZAR","2 SEC POUR MAJ","2 SEC PER AGGIORNARE","2 SEK ZUM UPDATEN"};
  const char* t_reboot[6]      = {"Se reiniciara solo","Will reboot alone","Reiniciara sozinho","Redemarrage auto","Riavvio automatico","Neustart automatisch"};
  display.fillScreen(BLANCO);
  dibujarLogoEspera(10);
  char b1[60], b2[50]; snprintf(b1,sizeof(b1),t_updateDisp[l], verNueva.c_str()); snprintf(b2,sizeof(b2),T_ACTUAL[l], VERSION_FIRMWARE, verNueva.c_str());
  txtCentrado(b1,400,180,3,true); txtCentrado(b2,400,230,2,false); display.fillRect(0,320,800,60,NEGRO); txtCentrado(t_hold[l],400,330,2,false); display.setTextColor(BLANCO); txtCentrado(t_2seg[l],400,355,2,true); display.setTextColor(NEGRO); txtCentrado(t_reboot[l],400,420,2,false); display.refresh();
}
bool realizarOTA(const String& url){
  int l = (cfg.lang>=0 && cfg.lang<=5) ? cfg.lang : 0;
  const char* t_updating[6]  = {"Actualizando firmware...","Updating firmware...","Atualizando firmware...","Mise a jour firmware...","Aggiornamento firmware...","Aktualisiere Firmware..."};
  const char* t_connecting[6] = {"Conectando GitHub...","Connecting GitHub...","Conectando GitHub...","Connexion GitHub...","Connessione GitHub...","Verbinde GitHub..."};
  const char* t_ok[6]         = {"Update OK!","Update OK!","Update OK!","Mise a jour OK!","Aggiornamento OK!","Update OK!"};
  const char* t_rebooting[6]  = {"Reiniciando...","Rebooting...","Reiniciando...","Redemarrage...","Riavvio...","Neustart..."};
  const char* t_fail[6]       = {"OTA fallo","OTA failed","OTA falhou","OTA echec","OTA fallito","OTA fehlgeschlagen"};
  display.fillScreen(BLANCO); display.update();
  display.fillScreen(BLANCO);
  dibujarLogoEspera(10);
  txtCentrado(t_updating[l],400,180,3,true); txtCentrado(t_connecting[l],400,225,2,false); int bx=100, by=280, bw=600, bh=40; display.drawRect(bx, by, bw, bh, NEGRO); display.drawRect(bx+1, by+1, bw-2, bh-2, NEGRO); txtCentrado("0%",400,by+bh+20,2,true); display.update();
  WiFiClientSecure client; client.setInsecure(); client.setTimeout(15000); HTTPClient http; http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS); http.setTimeout(25000);
  if(!http.begin(client, url)){ display.fillScreen(BLANCO); txtCentrado(t_fail[l],400,150,4,true); display.refresh(); delay(5000); return false; }
  int code = http.GET(); int size = http.getSize(); if(code != 200){ display.fillScreen(BLANCO); txtCentrado(t_fail[l],400,150,4,true); display.refresh(); delay(5000); http.end(); return false; }
  if(!Update.begin(size > 0 ? (size_t)size : UPDATE_SIZE_UNKNOWN)){ display.fillScreen(BLANCO); txtCentrado(t_fail[l],400,150,4,true); display.refresh(); delay(5000); http.end(); return false; }
  WiFiClient *stream = http.getStreamPtr(); stream->setNoDelay(true); WiFi.setSleep(false); setCpuFrequencyMhz(240);
  uint8_t* buff = (uint8_t*)malloc(OTA_BUFFER_BYTES); size_t buffSize = OTA_BUFFER_BYTES; if(!buff){ static uint8_t buffRespaldo[2048]; buff = buffRespaldo; buffSize = sizeof(buffRespaldo); } bool buffEnHeap = (buffSize == OTA_BUFFER_BYTES);
  size_t written = 0; unsigned long lastData = millis(); unsigned long t0 = millis(); bool unknownLen = (size <= 0); int proxRefreshPct = (OTA_REFRESH_CADA_PCT > 0) ? OTA_REFRESH_CADA_PCT : 101;
  // Barra continua sin ghosting: fondo blanco solido + barra negra
  while(http.connected() || stream->available()){ size_t avail = stream->available(); if(avail){ lastData = millis(); size_t toRead = (avail > buffSize) ? buffSize : avail; int c = stream->readBytes(buff, toRead); if(c > 0){ size_t w = Update.write(buff, c); if(w != (size_t)c) break; written += c; if(!unknownLen){ int pctNow = (int)(((uint64_t)written * 100) / size); if(pctNow >= proxRefreshPct && pctNow < 100){ char pbuf[50]; snprintf(pbuf, sizeof(pbuf), "%d%%  %u KB / %u KB", pctNow, (unsigned)(written/1024), (unsigned)(size/1024)); 
        // Limpia toda la franja a blanco solido primero para evitar transparencia
        display.fillRect(0, by, 800, bh+45, BLANCO);
        // Redibuja borde de la barra
        display.drawRect(bx, by, bw, bh, NEGRO);
        display.drawRect(bx+1, by+1, bw-2, bh-2, NEGRO);
        // Relleno negro segun %
        display.fillRect(bx+4, by+4, (bw-8)*pctNow/100, bh-8, NEGRO);
        txtCentrado(pbuf, 400, by+bh+10, 2, true); 
        display.updatePartial(0, by, 800, bh+45);
        lastData = millis(); proxRefreshPct += 2; // 2% fluido y sin tanto ghosting
      } } } } else { delay(1); if(millis() - lastData > 20000) break; } }
  if(buffEnHeap) free(buff); http.end();
  if(written>0 && Update.end(true)){ display.fillScreen(BLANCO); txtCentrado(t_ok[l],400,200,4,true); txtCentrado(t_rebooting[l],400,260,2,false); display.refresh(); rtc_hayUpdate = false; ultimoCheckOTA = 0; hayActualizacion = false; prefs.begin("tiempo", false); prefs.putString("ota_ignore", String(rtc_versionRemotaStr)); prefs.putString("ota_last_ok", String(rtc_versionRemotaStr)); prefs.end(); delay(2000); ESP.restart(); return true; } else { display.fillScreen(BLANCO); txtCentrado(t_fail[l],400,150,4,true); display.refresh(); delay(5000); Update.abort(); return false; }
}

// ==== DIBUJO RESUMEN (igual que V1.0.1) ====
// ==== CABECERA COMPARTIDA (resumen del tiempo y resumen de astro) ====
static const char* dsem[6][7] = {{"domingo","lunes","martes","miercoles","jueves","viernes","sabado"},{"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"},{"Dom","Seg","Ter","Qua","Qui","Sex","Sab"},{"dimanche","lundi","mardi","mercredi","jeudi","vendredi","samedi"},{"domenica","lunedi","martedi","mercoledi","giovedi","venerdi","sabato"},{"Sonntag","Montag","Dienstag","Mittwoch","Donnerstag","Freitag","Samstag"}};
static const char* meses[6][12] = {{"enero","febrero","marzo","abril","mayo","junio","julio","agosto","septiembre","octubre","noviembre","diciembre"},{"January","February","March","April","May","June","July","August","September","October","November","December"},{"janeiro","fevereiro","marco","abril","maio","junho","julho","agosto","setembro","outubro","novembro","dezembro"},{"janvier","fevrier","mars","avril","mai","juin","juillet","aout","septembre","octobre","novembre","decembre"},{"gennaio","febbraio","marzo","aprile","maggio","giugno","luglio","agosto","setembre","ottobre","novembre","dicembre"},{"Januar","Februar","Marz","April","Mai","Juni","Juli","August","September","Oktober","November","Dezember"}};
static const char* dcor[6][7] = {{"Dom","Lun","Mar","Mie","Jue","Vie","Sab"},{"Sun","Mon","Tue","Wed","Thu","Fri","Sat"},{"Dom","Seg","Ter","Qua","Qui","Sex","Sab"},{"Dim","Lun","Mar","Mer","Jeu","Ven","Sam"},{"Dom","Lun","Mar","Mer","Gio","Ven","Sab"},{"Son","Mon","Die","Mit","Don","Fre","Sam"}};
static const char* const T_BTN_TIEMPO[6] = {"TIEMPO","WEATHER","TEMPO","METEO","METEO","WETTER"};
static const char* const T_BTN_ASTRO[6]  = {"ASTRO","ASTRO","ASTRO","ASTRO","ASTRO","ASTRO"};
void iconoMenu(int x,int y){ for(int i=0;i<3;i++) display.fillRect(x,y+i*8,26,3,NEGRO); }
// Recuadro pequeno: fondo blanco, borde y texto negros (esquina superior derecha, bajo la bateria)
void botonCambio(const char* t){ display.fillRect(680,46,100,34,BLANCO); display.drawRect(680,46,100,34,NEGRO); display.drawRect(681,47,98,32,NEGRO); txtCentrado(t,730,55,2,true); }
void cabeceraResumen(const char* textoBoton){
  time_t ahora=time(nullptr); struct tm ti; localtime_r(&ahora,&ti); bool horaOk=ti.tm_year>120;
  String fStr = ""; if(horaOk){ int anio = ti.tm_year + 1900; if(cfg.fmt == 1){ fStr = String(meses[cfg.lang][ti.tm_mon]) + " . " + String(dsem[cfg.lang][ti.tm_wday]) + " " + String(ti.tm_mday) + " . " + String(anio); } else if(cfg.fmt == 2){ char iso[16]; snprintf(iso,sizeof(iso),"%04d-%02d-%02d",anio,ti.tm_mon+1,ti.tm_mday); fStr = String(iso) + " " + String(dsem[cfg.lang][ti.tm_wday]); } else { if(cfg.lang == 0) fStr = String(dsem[0][ti.tm_wday]) + ", " + String(ti.tm_mday) + " de " + String(meses[0][ti.tm_mon]) + " de " + String(anio); else if(cfg.lang == 1) fStr = String(dsem[1][ti.tm_wday]) + ", " + String(meses[1][ti.tm_mon]) + " " + String(ti.tm_mday) + ", " + String(anio); else if(cfg.lang == 2) fStr = String(dsem[2][ti.tm_wday]) + ". " + String(ti.tm_mday) + " de " + String(meses[2][ti.tm_mon]) + " de " + String(anio); else if(cfg.lang == 3) fStr = String(dsem[3][ti.tm_wday]) + " " + String(ti.tm_mday) + " " + String(meses[3][ti.tm_mon]) + " " + String(anio); else if(cfg.lang == 4) fStr = String(dsem[4][ti.tm_wday]) + " " + String(ti.tm_mday) + " " + String(meses[4][ti.tm_mon]) + " " + String(anio); else fStr = String(dsem[5][ti.tm_wday]) + ", " + String(ti.tm_mday) + ". " + String(meses[5][ti.tm_mon]) + " " + String(anio); } }
  const char* ciudad = cfg.valida ? cfg.ciudad.c_str() : "Logrono";
  if(iconoLunaVisible){ dibujarLunaMenguante(32,18,8); } else { iconoMenu(20,10); }
  if(horaOk){ txtCentrado(fStr.c_str(), 400, 14, 3, false); }
  dibujarBateria(700, 10, nivelBateria, cargando);
  int sizeCiudad = 4; while(sizeCiudad > 2 && anchoTxt(ciudad, sizeCiudad) > 630){ sizeCiudad--; }
  txtSimple(ciudad, 20, 52, sizeCiudad, true);
  display.fillRect(20, 85, 760, 3, NEGRO);
  botonCambio(textoBoton);
}
void dibujar() {
  char buf[128]; time_t ahora=time(nullptr); struct tm ti; localtime_r(&ahora,&ti); bool horaOk=ti.tm_year>120;
  display.fillScreen(BLANCO);
  cabeceraResumen(T_BTN_ASTRO[idioma()]);
  const int HEADER_H = 85;
  int mitadX = 400; int yTopCentral = HEADER_H + 12; int hCentral = 190; display.drawFastVLine(mitadX, yTopCentral, hCentral, NEGRO);
  const char* lblExt = "EXTERIOR"; const char* lblInt = "INTERIOR"; const char* fmtSens = "Sens %d  Hum %d%%"; const char* fmtVntBase = "Viento"; const char* fmtHumI = "Humedad %.0f%%"; const char* lblSinS = "Sin sensor"; const char* fmtLlu = "Lluvia %d%%";
  if(cfg.lang == 1) { lblExt = "OUTDOOR"; lblInt = "INDOOR"; fmtSens = "Feels %d  Hum %d%%"; fmtVntBase = "Wind"; fmtHumI = "Humidity %.0f%%"; lblSinS = "No sensor"; fmtLlu = "Rain %d%%"; }
  else if(cfg.lang == 2) { lblExt = "EXTERIOR"; lblInt = "INTERIOR"; fmtSens = "Sens %d  Hum %d%%"; fmtVntBase = "Vento"; fmtHumI = "Humidade %.0f%%"; lblSinS = "Sem sensor"; fmtLlu = "Chuva %d%%"; }
  else if(cfg.lang == 3) { lblExt = "EXTERIEUR"; lblInt = "INTERIEUR"; fmtSens = "Ressenti %d Humi %d%%"; fmtVntBase = "Vent"; fmtHumI = "Humidite %.0f%%"; lblSinS = "Pas de capteur"; fmtLlu = "Pluie %d%%"; }
  else if(cfg.lang == 4) { lblExt = "ESTERNO"; lblInt = "INTERNO"; fmtSens = "Percep %d  Umid %d%%"; fmtVntBase = "Vento"; fmtHumI = "Umidita %.0f%%"; lblSinS = "Senza sensore"; fmtLlu = "Pioggia %d%%"; }
  else if(cfg.lang == 5) { lblExt = "AUSSEN"; lblInt = "INNEN"; fmtSens = "Gefuhlt %d  Feuchte %d%%"; fmtVntBase = "Wind"; fmtHumI = "Feuchtigkeit %.0f%%"; lblSinS = "Kein Sensor"; fmtLlu = "Regen %d%%"; }
  int centroY = yTopCentral + hCentral/2; int tempMostrar = tempAhora; int sensMostrar = sensacion; int vientoMostrar = viento; float tempIntMostrar = tempInt; char unidadTemp = 'C';
  if(cfg.uni == 1){ tempMostrar = round(tempAhora * 9.0/5.0 + 32); sensMostrar = round(sensacion * 9.0/5.0 + 32); vientoMostrar = round(viento * 0.621371); tempIntMostrar = tempInt * 9.0/5.0 + 32; unidadTemp = 'F'; }
  txtCentrado(lblExt, 200, yTopCentral+8, 3, true); int yTemp = centroY - 24; dibujarIcono(iconoDe(codeAhora), 60, yTemp, 36); snprintf(buf,sizeof(buf),"%d",tempMostrar); txtSimple(buf, 150, yTemp, 6, true); char uStr[2] = {unidadTemp, 0}; txtSimple(uStr, 150+anchoTxt(buf,6)+6, yTemp, 4, true); txtCentrado(textoDe(codeAhora), 200, centroY+36, 2, true); snprintf(buf,sizeof(buf),fmtSens,sensMostrar,humedad); txtCentrado(buf, 200, centroY+58, 2, false); { char unitV[6]; strcpy(unitV, cfg.uni==1?"mph":"km/h"); char fmtVnt[32]; snprintf(fmtVnt,sizeof(fmtVnt),"%s %%d %s", fmtVntBase, unitV); snprintf(buf,sizeof(buf),fmtVnt,vientoMostrar); txtCentrado(buf, 200, centroY+78, 2, false); }
  txtCentrado(lblInt, 600, yTopCentral+8, 3, true);
  if(hayInterior){ int casaX=410; int casaY=yTemp; display.drawTriangle(casaX+25, casaY, casaX, casaY+25, casaX+50, casaY+25, NEGRO); display.drawTriangle(casaX+25, casaY+2, casaX+3, casaY+25, casaX+47, casaY+25, NEGRO); display.fillRect(casaX+4, casaY+25, 42, 30, NEGRO); display.fillRect(casaX+12, casaY+35, 18, 20, BLANCO); char bufInt[16]; snprintf(bufInt,sizeof(bufInt),"%.1f",tempIntMostrar); if(cfg.uni != 1) for(char *p=bufInt; *p; p++) if(*p=='.') *p=','; txtSimple(bufInt, 510, yTemp, 6, true); char uStr2[2] = {unidadTemp, 0}; txtSimple(uStr2, 510+anchoTxt(bufInt,6)+6, yTemp, 4, true); snprintf(buf,sizeof(buf),fmtHumI,humInt); txtCentrado(buf, 600, centroY+58, 2, false); } else { txtCentrado(lblSinS, 600, centroY-10, 3, true); }
  int yPrevTop = yTopCentral + hCentral + 18; display.fillRect(20, yPrevTop, 760, 2, NEGRO);
  for(int i=1;i<=3;i++){ int cx=20+(i-1)*253+126; if(i>1) display.drawFastVLine(20+(i-1)*253, yPrevTop+18, 150, NEGRO); const char* nombre=""; if(horaOk){ time_t t=ahora+86400L*i; struct tm tf; localtime_r(&t,&tf); nombre = dcor[cfg.lang][tf.tm_wday]; } txtCentrado(nombre,cx,yPrevTop+20,3,true); dibujarIcono(iconoDe(dias[i].code),cx,yPrevTop+80,36); snprintf(buf,sizeof(buf),"%d / %d",dias[i].tmax,dias[i].tmin); txtCentrado(buf,cx,yPrevTop+122,2,false); snprintf(buf,sizeof(buf),fmtLlu,dias[i].lluvia); txtCentrado(buf,cx,yPrevTop+144,2,false); }
  display.update();
}

// ==== NUEVO: PANTALLA DETALLE CON BORDE FINO + 2 COLUMNAS ====

void dibujarMarcoFino(){ display.drawRect(4,4,792,472,NEGRO); display.drawRect(6,6,788,468,NEGRO); }

void miniIcono(int tipo,int x,int y,int s){
  // s=20
  int g=max(1,s/12);
  switch(tipo){
    case 0: // temp termometro
      display.drawRect(x+7,y+2,6,s-8,NEGRO);
      display.fillCircle(x+10,y+s-4,6,NEGRO);
      display.fillCircle(x+10,y+s-4,3,BLANCO);
      break;
    case 1: // sensacion
      display.drawRect(x+7,y+2,6,s-8,NEGRO);
      display.fillCircle(x+10,y+s-4,6,NEGRO);
      display.fillRect(x+2,y+4,3,3,NEGRO);
      break;
    case 2: // humedad gota
      display.fillCircle(x+10,y+10,7,NEGRO);
      display.fillTriangle(x+10,y+2,x+2,y+14,x+18,y+14,NEGRO);
      break;
    case 3: // viento
      for(int i=0;i<3;i++) display.fillRect(x, y+4+i*6, 16, 2, NEGRO);
      display.fillTriangle(x+14,y+4,x+14,y+18,x+20,y+11,NEGRO);
      break;
    case 4: // racha
      for(int i=0;i<3;i++) display.fillRect(x, y+4+i*5, 14, 2, NEGRO);
      display.fillCircle(x+16,y+10,3,NEGRO);
      break;
    case 5: // direccion
      display.drawCircle(x+10,y+10,8,NEGRO);
      display.fillTriangle(x+10,y+1,x+7,y+8,x+13,y+8,NEGRO);
      break;
    case 6: // nubes
      display.fillCircle(x+4,y+10,5,NEGRO); display.fillCircle(x+10,y+6,7,NEGRO); display.fillCircle(x+16,y+10,5,NEGRO);
      display.fillRect(x+4,y+10,12,4,NEGRO);
      break;
    case 7: // precipitacion
      display.fillCircle(x+4,y+8,4,NEGRO); display.fillCircle(x+9,y+5,5,NEGRO); display.fillCircle(x+15,y+8,4,NEGRO);
      display.fillRect(x,y+12,3,6,NEGRO); display.fillRect(x+8,y+12,3,6,NEGRO); display.fillRect(x+16,y+12,3,6,NEGRO);
      break;
    case 8: // presion
      display.drawCircle(x+10,y+10,8,NEGRO);
      display.fillRect(x+9,y+6,2,8,NEGRO);
      break;
    case 9: // rocio
      display.fillCircle(x+10,y+12,5,NEGRO); display.fillRect(x+10,y+4,2,6,NEGRO);
      break;
    case 10: // uv sol
      display.fillCircle(x+10,y+10,5,NEGRO);
      for(int i=0;i<8;i++){ float a=i*3.14159/4; int x1=x+10+cos(a)*7; int y1=y+10+sin(a)*7; int x2=x+10+cos(a)*10; int y2=y+10+sin(a)*10; display.drawLine(x1,y1,x2,y2,NEGRO); }
      break;
    case 11: // amanecer
      display.fillCircle(x+10,y+14,8,NEGRO); display.fillRect(x,y+14,20,4,BLANCO); display.fillRect(x,y+14,20,2,NEGRO);
      display.drawLine(x+10,y+4,x+14,y+8,NEGRO); display.drawLine(x+14,y+8,x+10,y+8,NEGRO);
      break;
    case 12: // anochecer
      display.fillCircle(x+10,y+14,8,NEGRO); display.fillRect(x,y+14,20,4,BLANCO); display.fillRect(x,y+14,20,2,NEGRO);
      display.drawLine(x+10,y+4,x+6,y+8,NEGRO); display.drawLine(x+6,y+8,x+10,y+8,NEGRO);
      break;
    case 13: // sol horas
      display.drawCircle(x+10,y+12,8,NEGRO); display.drawLine(x+10,y+12,x+16,y+12,NEGRO);
      break;
    case 14: // estado / lluvia prob
      display.fillCircle(x+4,y+8,4,NEGRO); display.fillCircle(x+10,y+6,5,NEGRO); display.fillRect(x+4,y+10,12,4,NEGRO);
      break;
  }
}

void lineaDatoIcon(int icono,int x,int y,const char* label,const char* valor){
  miniIcono(icono,x,y,20);
  char buf[80]; snprintf(buf,sizeof(buf),"%s: %s", label, valor);
  txtSimple(buf, x+28, y+2, 2, false);
}

void dibujarDetalle(int idx){
  display.fillScreen(BLANCO);
  dibujarMarcoFino();
  int l = (cfg.lang>=0 && cfg.lang<=5)?cfg.lang:0;
  time_t ahora=time(nullptr); struct tm ti; localtime_r(&ahora,&ti);
  // titulos HOY y dias
  const char* TIT_HOY[6]={"HOY","TODAY","HOJE","AUJ.","OGGI","HEUTE"};
  const char* dcor[6][7] = {{"Dom","Lun","Mar","Mie","Jue","Vie","Sab"},{"Sun","Mon","Tue","Wed","Thu","Fri","Sat"},{"Dom","Seg","Ter","Qua","Qui","Sex","Sab"},{"Dim","Lun","Mar","Mer","Jeu","Ven","Sam"},{"Dom","Lun","Mar","Mer","Gio","Ven","Sab"},{"Son","Mon","Die","Mit","Don","Fre","Sam"}};
  char titulo[32];
  if(idx==0) snprintf(titulo,sizeof(titulo),"%s", TIT_HOY[l]);
  else {
    time_t t=ahora+86400L*idx; struct tm tf; localtime_r(&t,&tf);
    snprintf(titulo,sizeof(titulo),"%s", dcor[l][tf.tm_wday]);
  }
  // Dia a la izquierda para dejar icono mas visible, HOY mas grande
  int sizeTit = (idx==0)?5:4;
  if(iconoLunaVisible) dibujarLunaMenguante(32,18,8);   // lunita de suspension (arriba izquierda)
  txtSimple(titulo, 56, 14, sizeTit, true);              // titulo desplazado a la derecha de la luna
  dibujarBateria(700, 10, nivelBateria, cargando);        // bateria con su % (arriba derecha)
  // icono centrado y subido un poco
  int iconCx = 400; int iconCy = 70;
  if(idx==0) dibujarIcono(iconoDe(codeAhora), iconCx, iconCy, 52);
  else dibujarIcono(iconoDe(dias[idx].code), iconCx, iconCy, 52);
  txtCentrado(textoDe(idx==0?codeAhora:dias[idx].code), 400, iconCy+62, 2, true);

  // etiquetas traducidas completas (solo idioma config)
  const char* L_TEMP[6]={"Temperatura","Temperature","Temperatura","Temperature","Temperatura","Temperatur"};
  const char* L_SENS[6]={"Sensacion","Feels like","Sensacao","Ressenti","Percepita","Gefuehlt"};
  const char* L_HUM[6]={"Humedad","Humidity","Umidade","Humidite","Umidita","Feuchte"};
  const char* L_VIENTO[6]={"Viento","Wind","Vento","Vent","Vento","Wind"};
  const char* L_RACHA[6]={"Racha","Gust","Rajada","Rafale","Raffica","Boe"};
  const char* L_DIR[6]={"Direccion","Direction","Direcao","Direction","Direzione","Richtung"};
  const char* L_NUBES[6]={"Nubes","Clouds","Nuvens","Nuages","Nuvole","Wolken"};
  const char* L_PREC[6]={"Precipitacion","Precipitation","Precipitacao","Precipitation","Precipitazione","Niederschlag"};
  const char* L_PROB[6]={"Prob. lluvia","Rain prob.","Prob. chuva","Prob. pluie","Prob. pioggia","Regen-Wahrs."};
  const char* L_PRES[6]={"Presion mar","Sea pressure","Pressao mar","Pression mer","Pressione mare","Luftdruck Meer"};
  const char* L_PRES_SUP[6]={"Presion sup.","Surface pres.","Pressao sup.","Pression surf.","Press. sup.","Druck Boden"};
  const char* L_ROCIO[6]={"Rocio","Dew point","Orvalho","Rosee","Rugiada","Taupunkt"};
  const char* L_UV[6]={"Indice UV","UV index","Indice UV","Indice UV","Indice UV","UV Index"};
  const char* L_SALIDA[6]={"Amanecer","Sunrise","Nascer do sol","Lever soleil","Alba","Sonnenaufgang"};
  const char* L_PUESTA[6]={"Anochecer","Sunset","Por do sol","Coucher soleil","Tramonto","Sonnenuntergang"};
  const char* L_SOL_H[6]={"Horas sol","Sunshine","Horas sol","Ensoleillement","Ore sole","Sonnenschein"};
  const char* L_LLUVIA[6]={"Lluvia","Rain","Chuva","Pluie","Pioggia","Regen"};
  const char* L_CHUB[6]={"Chubascos","Showers","Aguaceiros","Averses","Rovesci","Schauer"};
  const char* L_NIEVE[6]={"Nieve","Snow","Neve","Neige","Neve","Schnee"};

  char v[40];
  int col1x=18; int col2x=410; int y0=165; int lh=32;
  int y= y0;
  bool esF = (cfg.uni==1);
  char unitT[3]; strcpy(unitT, esF?"F":"C");
  char unitV[6]; strcpy(unitV, esF?"mph":"km/h");

  if(idx==0){
    int tShow = esF? round(tempAhora*9.0/5.0+32): tempAhora;
    int sShow = esF? round(sensacion*9.0/5.0+32): sensacion;
    int vShow = esF? round(viento*0.621371): viento;
    int rShow = esF? round(currExt.racha*0.621371): currExt.racha;
    float rocioShow = esF? currExt.rocio*9.0/5.0+32 : currExt.rocio;
    snprintf(v,sizeof(v),"%d %s", tShow, unitT); lineaDatoIcon(0,col1x,y,L_TEMP[l],v); y+=lh;
    snprintf(v,sizeof(v),"%d %s", sShow, unitT); lineaDatoIcon(1,col1x,y,L_SENS[l],v); y+=lh;
    snprintf(v,sizeof(v),"%d %%", humedad); lineaDatoIcon(2,col1x,y,L_HUM[l],v); y+=lh;
    snprintf(v,sizeof(v),"%d %s", vShow, unitV); lineaDatoIcon(3,col1x,y,L_VIENTO[l],v); y+=lh;
    snprintf(v,sizeof(v),"%d %s", rShow, unitV); lineaDatoIcon(4,col1x,y,L_RACHA[l],v); y+=lh;
    snprintf(v,sizeof(v),"%d deg", currExt.dirViento); lineaDatoIcon(5,col1x,y,L_DIR[l],v); y+=lh;
    snprintf(v,sizeof(v),"%d %%", currExt.cloud); lineaDatoIcon(6,col1x,y,L_NUBES[l],v); y+=lh;
    snprintf(v,sizeof(v),"%.1f mm", currExt.precip); lineaDatoIcon(7,col1x,y,L_PREC[l],v); y+=lh;

    y=y0;
    snprintf(v,sizeof(v),"%.0f hPa", currExt.pres); lineaDatoIcon(8,col2x,y,L_PRES[l],v); y+=lh;
    snprintf(v,sizeof(v),"%.0f hPa", currExt.presSea); lineaDatoIcon(8,col2x,y,L_PRES_SUP[l],v); y+=lh;
    snprintf(v,sizeof(v),"%.0f %s", rocioShow, unitT); lineaDatoIcon(9,col2x,y,L_ROCIO[l],v); y+=lh;
    snprintf(v,sizeof(v),"%.1f", currExt.uv); lineaDatoIcon(10,col2x,y,L_UV[l],v); y+=lh;
    snprintf(v,sizeof(v),"%.1f mm", currExt.rain); lineaDatoIcon(7,col2x,y,L_LLUVIA[l],v); y+=lh;
    snprintf(v,sizeof(v),"%.1f mm", currExt.shower); lineaDatoIcon(7,col2x,y,L_CHUB[l],v); y+=lh;
    snprintf(v,sizeof(v),"%.1f cm", currExt.snow); lineaDatoIcon(14,col2x,y,L_NIEVE[l],v); y+=lh;
  } else {
    Dia &d = dias[idx];
    int tmaxS = esF? round(d.tmax*9.0/5.0+32): d.tmax; int tminS = esF? round(d.tmin*9.0/5.0+32): d.tmin;
    int tmaxApS = esF? round(d.tmaxAp*9.0/5.0+32): d.tmaxAp; int tminApS = esF? round(d.tminAp*9.0/5.0+32): d.tminAp;
    int vMaxS = esF? round(d.vientoMax*0.621371): d.vientoMax; int rMaxS = esF? round(d.rachaMax*0.621371): d.rachaMax;
    snprintf(v,sizeof(v),"%d / %d %s", tmaxS, tminS, unitT); lineaDatoIcon(0,col1x,y,L_TEMP[l],v); y+=lh;
    snprintf(v,sizeof(v),"%d / %d %s", tmaxApS, tminApS, unitT); lineaDatoIcon(1,col1x,y,L_SENS[l],v); y+=lh;
    snprintf(v,sizeof(v),"%d %%", d.lluvia); lineaDatoIcon(14,col1x,y,L_PROB[l],v); y+=lh;
    snprintf(v,sizeof(v),"%.1f mm", d.precipSum); lineaDatoIcon(7,col1x,y,L_PREC[l],v); y+=lh;
    snprintf(v,sizeof(v),"%.1f mm", d.rainSum); lineaDatoIcon(7,col1x,y,L_LLUVIA[l],v); y+=lh;
    snprintf(v,sizeof(v),"%.1f mm", d.showerSum); lineaDatoIcon(7,col1x,y,L_CHUB[l],v); y+=lh;
    snprintf(v,sizeof(v),"%.1f cm", d.snowSum); lineaDatoIcon(14,col1x,y,L_NIEVE[l],v); y+=lh;
    snprintf(v,sizeof(v),"%d", d.uvMax); lineaDatoIcon(10,col1x,y,L_UV[l],v); y+=lh;

    y=y0;
    snprintf(v,sizeof(v),"%d %s", vMaxS, unitV); lineaDatoIcon(3,col2x,y,L_VIENTO[l],v); y+=lh;
    snprintf(v,sizeof(v),"%d %s", rMaxS, unitV); lineaDatoIcon(4,col2x,y,L_RACHA[l],v); y+=lh;
    snprintf(v,sizeof(v),"%d deg", d.dirDom); lineaDatoIcon(5,col2x,y,L_DIR[l],v); y+=lh;
    snprintf(v,sizeof(v),"%d min", d.solMin); lineaDatoIcon(13,col2x,y,L_SOL_H[l],v); y+=lh;
    char sr[12]; strncpy(sr, d.sunrise.c_str()+11,5); sr[5]=0; lineaDatoIcon(11,col2x,y,L_SALIDA[l],sr); y+=lh;
    char ss[12]; strncpy(ss, d.sunset.c_str()+11,5); ss[5]=0; lineaDatoIcon(12,col2x,y,L_PUESTA[l],ss); y+=lh;
  }
  const char* t_tap[6] = {"Toca para volver - Desliza para navegar","Tap to return - Swipe to navigate","Toque para voltar - Deslize","Toucher retour - Glisser","Tocca per tornare - Scorri","Tippen zurueck - Wischen"};
  txtCentrado(t_tap[l], 400, 450, 2, false);
  display.update();
}

// ================= ASTRO CALCULADO EN EL APARATO (Sol, Luna, planetas, constelaciones) =================
// Nada de texto fijo: Luna, planetas y constelaciones se calculan con efemerides a partir de la hora y tu ubicacion.
// ===== CALCULO ASTRONOMICO LOCAL (efemerides de baja precision, P. Schlyter) =====
// Precision: Sol/planetas ~1-2 minutos de arco, Luna ~0.1-0.2 grados (suficiente para horas al minuto y visibilidad)
#define AC_DEG 0.017453292519943295
static double ac_rev(double x){ x=fmod(x,360.0); if(x<0) x+=360.0; return x; }
static double ac_dayNum(double unixT){ return unixT/86400.0 - 10956.0; }   // dias desde 1999-12-31 0h UT
static void ac_eclToEq(double xg,double yg,double zg,double d,double &ra,double &dec){
  double ecl=(23.4393-3.563E-7*d)*AC_DEG;
  double ye=yg*cos(ecl)-zg*sin(ecl), ze=yg*sin(ecl)+zg*cos(ecl);
  ra=ac_rev(atan2(ye,xg)/AC_DEG); dec=atan2(ze,sqrt(xg*xg+ye*ye))/AC_DEG;
}
static double ac_kepler(double M,double e){          // M en grados -> E en grados
  double E=M+e/AC_DEG*sin(M*AC_DEG)*(1.0+e*cos(M*AC_DEG));
  for(int k=0;k<6;k++){ double dE=(E-e/AC_DEG*sin(E*AC_DEG)-M)/(1.0-e*cos(E*AC_DEG)); E-=dE; if(fabs(dE)<1e-6) break; }
  return E;
}
// Sol: longitud ecliptica, distancia (UA), RA/Dec
static void ac_sun(double d,double &lon,double &r,double &ra,double &dec){
  double w=282.9404+4.70935E-5*d, e=0.016709-1.151E-9*d, M=ac_rev(356.0470+0.9856002585*d);
  double E=ac_kepler(M,e);
  double xv=cos(E*AC_DEG)-e, yv=sqrt(1.0-e*e)*sin(E*AC_DEG);
  r=sqrt(xv*xv+yv*yv); lon=ac_rev(atan2(yv,xv)/AC_DEG+w);
  ac_eclToEq(r*cos(lon*AC_DEG), r*sin(lon*AC_DEG), 0.0, d, ra, dec);
}
// Luna: longitud/latitud ecliptica geocentrica, RA/Dec (con las perturbaciones principales)
static void ac_moon(double d,double &lon,double &lat,double &ra,double &dec){
  double N=ac_rev(125.1228-0.0529538083*d), i=5.1454, w=ac_rev(318.0634+0.1643573223*d), a=60.2666, e=0.054900, M=ac_rev(115.3654+13.0649929509*d);
  double E=ac_kepler(M,e);
  double xv=a*(cos(E*AC_DEG)-e), yv=a*sqrt(1.0-e*e)*sin(E*AC_DEG);
  double v=atan2(yv,xv)/AC_DEG, r=sqrt(xv*xv+yv*yv);
  double vw=(v+w)*AC_DEG, Nr=N*AC_DEG, ir=i*AC_DEG;
  double xg=r*(cos(Nr)*cos(vw)-sin(Nr)*sin(vw)*cos(ir)), yg=r*(sin(Nr)*cos(vw)+cos(Nr)*sin(vw)*cos(ir)), zg=r*sin(vw)*sin(ir);
  double lo=atan2(yg,xg)/AC_DEG, la=atan2(zg,sqrt(xg*xg+yg*yg))/AC_DEG;
  double Ms=ac_rev(356.0470+0.9856002585*d), ws=282.9404+4.70935E-5*d;
  double Ls=ac_rev(Ms+ws), Lm=ac_rev(M+w+N), D=ac_rev(Lm-Ls), F=ac_rev(Lm-N);
  double Mr=M*AC_DEG, Msr=Ms*AC_DEG, Dr=D*AC_DEG, Fr=F*AC_DEG;
  lo += -1.274*sin(Mr-2*Dr) +0.658*sin(2*Dr) -0.186*sin(Msr) -0.059*sin(2*Mr-2*Dr) -0.057*sin(Mr-2*Dr+Msr) +0.053*sin(Mr+2*Dr)
        +0.046*sin(2*Dr-Msr) +0.041*sin(Mr-Msr) -0.035*sin(Dr) -0.031*sin(Mr+Msr) -0.015*sin(2*Fr-2*Dr) +0.011*sin(Mr-4*Dr);
  la += -0.173*sin(Fr-2*Dr) -0.055*sin(Mr-Fr-2*Dr) -0.046*sin(Mr+Fr-2*Dr) +0.033*sin(Fr+2*Dr) +0.017*sin(2*Mr+Fr);
  r  += -0.58*cos(Mr-2*Dr) -0.46*cos(2*Dr);
  lon=ac_rev(lo); lat=la;
  double lr=lon*AC_DEG, br=lat*AC_DEG;
  ac_eclToEq(r*cos(lr)*cos(br), r*sin(lr)*cos(br), r*sin(br), d, ra, dec);
}
// Planetas: 0 Mercurio, 1 Venus, 2 Marte, 3 Jupiter, 4 Saturno  -> RA/Dec geocentricas
static void ac_planet(int idx,double d,double &ra,double &dec){
  static const double EL[5][12]={
   {48.3313,3.24587E-5, 7.0047,5.00E-8,  29.1241,1.01444E-5, 0.387098,0, 0.205635,5.59E-10,  168.6562,4.0923344368},
   {76.6799,2.46590E-5, 3.3946,2.75E-8,  54.8910,1.38374E-5, 0.723330,0, 0.006773,-1.302E-9,  48.0052,1.6021302244},
   {49.5574,2.11081E-5, 1.8497,-1.78E-8, 286.5016,2.92961E-5,1.523688,0, 0.093405,2.516E-9,   18.6021,0.5240207766},
   {100.4542,2.76854E-5,1.3030,-1.557E-7,273.8777,1.64505E-5,5.20256,0,  0.048498,4.469E-9,   19.8950,0.0830853001},
   {113.6634,2.38980E-5,2.4886,-1.081E-7,339.3939,2.97661E-5,9.55475,0,  0.055546,-9.499E-9,  316.9670,0.0334442282}};
  const double* p=EL[idx];
  double N=p[0]+p[1]*d, i=p[2]+p[3]*d, w=p[4]+p[5]*d, a=p[6], e=p[8]+p[9]*d, M=ac_rev(p[10]+p[11]*d);
  double E=ac_kepler(M,e);
  double xv=a*(cos(E*AC_DEG)-e), yv=a*sqrt(1.0-e*e)*sin(E*AC_DEG);
  double v=atan2(yv,xv)/AC_DEG, r=sqrt(xv*xv+yv*yv);
  double vw=(v+w)*AC_DEG, Nr=N*AC_DEG, ir=i*AC_DEG;
  double xh=r*(cos(Nr)*cos(vw)-sin(Nr)*sin(vw)*cos(ir)), yh=r*(sin(Nr)*cos(vw)+cos(Nr)*sin(vw)*cos(ir)), zh=r*sin(vw)*sin(ir);
  if(idx>=3){   // perturbaciones Jupiter-Saturno
    double Mj=ac_rev(19.8950+0.0830853001*d)*AC_DEG, Ms=ac_rev(316.9670+0.0334442282*d)*AC_DEG;
    double lo=atan2(yh,xh)/AC_DEG, la=atan2(zh,sqrt(xh*xh+yh*yh))/AC_DEG;
    if(idx==3){
      lo += -0.332*sin(2*Mj-5*Ms-67.6*AC_DEG) -0.056*sin(2*Mj-2*Ms+21*AC_DEG) +0.042*sin(3*Mj-5*Ms+21*AC_DEG) -0.036*sin(Mj-2*Ms)
            +0.022*cos(Mj-Ms) +0.023*sin(2*Mj-3*Ms+52*AC_DEG) -0.016*sin(Mj-5*Ms-69*AC_DEG);
    } else {
      lo += +0.812*sin(2*Mj-5*Ms-67.6*AC_DEG) -0.229*cos(2*Mj-4*Ms-2*AC_DEG) +0.119*sin(Mj-2*Ms-3*AC_DEG) +0.046*sin(2*Mj-6*Ms-69*AC_DEG) +0.014*sin(Mj-3*Ms+32*AC_DEG);
      la += -0.020*cos(2*Mj-4*Ms-2*AC_DEG) +0.018*sin(2*Mj-6*Ms-49*AC_DEG);
    }
    double lr=lo*AC_DEG, br=la*AC_DEG;
    xh=r*cos(lr)*cos(br); yh=r*sin(lr)*cos(br); zh=r*sin(br);
  }
  double sl,sr,sra,sdec; ac_sun(d,sl,sr,sra,sdec);
  double xs=sr*cos(sl*AC_DEG), ys=sr*sin(sl*AC_DEG);
  ac_eclToEq(xh+xs, yh+ys, zh, d, ra, dec);
}
// Altura y acimut (grados; acimut desde el Norte hacia el Este) para un observador (lat, lon Este positiva)
static void ac_altaz(double ra,double dec,double unixT,double latO,double lonO,double &alt,double &az){
  double jd=unixT/86400.0+2440587.5;
  double lst=ac_rev(280.46061837+360.98564736629*(jd-2451545.0)+lonO);
  double H=(lst-ra)*AC_DEG, dc=dec*AC_DEG, ph=latO*AC_DEG;
  double sa=sin(dc)*sin(ph)+cos(dc)*cos(ph)*cos(H);
  double al=asin(sa);
  double y=-cos(dc)*sin(H), x=sin(dc)-sin(al)*sin(ph);   // sinA*cos(alt)*cos(lat)... (proporcional)
  alt=al/AC_DEG; az=ac_rev(atan2(y, x/cos(ph))/AC_DEG);
}
static double ac_sunAlt(double unixT,double latO,double lonO){
  double d=ac_dayNum(unixT),l,r,ra,dec,alt,az; ac_sun(d,l,r,ra,dec); ac_altaz(ra,dec,unixT,latO,lonO,alt,az); return alt;
}
static double ac_moonAlt(double unixT,double latO,double lonO){
  double d=ac_dayNum(unixT),lo,la,ra,dec,alt,az; ac_moon(d,lo,la,ra,dec); ac_altaz(ra,dec,unixT,latO,lonO,alt,az); return alt;
}
// Elongacion Luna-Sol (0 = nueva, 180 = llena), en grados [0,360)
static double ac_elong(double unixT){
  double d=ac_dayNum(unixT),sl,sr,sra,sdec,ml,mb,mra,mdec; ac_sun(d,sl,sr,sra,sdec); ac_moon(d,ml,mb,mra,mdec); return ac_rev(ml-sl);
}
// Salida y puesta del Sol (body=0, umbral -0.833) o la Luna (body=1, umbral +0.125) entre t0 y t0+86400. 0 si no hay.
static void ac_riseset(int body,double t0,double latO,double lonO,double &rise,double &set){
  const double h0 = body==0 ? -0.833 : 0.125; const double step = body==0 ? 900.0 : 1200.0;
  rise=0; set=0;
  double tPrev=t0, aPrev=(body==0?ac_sunAlt(tPrev,latO,lonO):ac_moonAlt(tPrev,latO,lonO))-h0;
  for(double t=t0+step; t<=t0+86400.0+1; t+=step){
    double a=(body==0?ac_sunAlt(t,latO,lonO):ac_moonAlt(t,latO,lonO))-h0;
    if(aPrev<0 && a>=0 && rise==0) rise = tPrev + step*(-aPrev)/(a-aPrev);
    if(aPrev>=0 && a<0 && set==0)  set  = tPrev + step*( aPrev)/(aPrev-a);
    tPrev=t; aPrev=a;
  }
}
// Version con umbral personalizado para crepusculos
static void ac_riseset_h0(double t0,double latO,double lonO,double h0,double &rise,double &set){
  const double step=900.0;
  rise=0; set=0;
  double tPrev=t0, aPrev=ac_sunAlt(tPrev,latO,lonO)-h0;
  for(double t=t0+step; t<=t0+86400.0+1; t+=step){
    double a=ac_sunAlt(t,latO,lonO)-h0;
    if(aPrev<0 && a>=0 && rise==0) rise = tPrev + step*(-aPrev)/(a-aPrev);
    if(aPrev>=0 && a<0 && set==0)  set  = tPrev + step*( aPrev)/(aPrev-a);
    tPrev=t; aPrev=a;
  }
}
static void ac_sunAltAz(double unixT,double latO,double lonO,double &alt,double &az){
  double d=ac_dayNum(unixT),l,r,ra,dec; ac_sun(d,l,r,ra,dec); ac_altaz(ra,dec,unixT,latO,lonO,alt,az);
}
static const char* ac_brujula(double az){
  if(az<11.25) return "N"; if(az<33.75) return "NNE"; if(az<56.25) return "NE"; if(az<78.75) return "ENE";
  if(az<101.25) return "E"; if(az<123.75) return "ESE"; if(az<146.25) return "SE"; if(az<168.75) return "SSE";
  if(az<191.25) return "S"; if(az<213.75) return "SSO"; if(az<236.25) return "SO"; if(az<258.75) return "OSO";
  if(az<281.25) return "O"; if(az<303.75) return "ONO"; if(az<326.25) return "NO"; if(az<348.75) return "NNO"; return "N";
}

// Proximo instante (despues de t0) en que la elongacion alcanza 'objetivo' (0 nueva, 180 llena). 0 si no se encuentra en 32 dias.
static double ac_nextPhase(double t0,double objetivo){
  const double step=6*3600.0;
  double tPrev=t0, gPrev=ac_rev(ac_elong(tPrev)-objetivo+180.0)-180.0;
  for(double t=t0+step; t<=t0+32*86400.0; t+=step){
    double g=ac_rev(ac_elong(t)-objetivo+180.0)-180.0;
    if(gPrev<0 && g>=0 && fabs(g-gPrev)<90.0){
      double lo=tPrev, hi=t; for(int k=0;k<24;k++){ double m=0.5*(lo+hi); double gm=ac_rev(ac_elong(m)-objetivo+180.0)-180.0; if(gm<0) lo=m; else hi=m; }
      return 0.5*(lo+hi);
    }
    tPrev=t; gPrev=g;
  }
  return 0;
}
// Distancia sobre la superficie (km) entre dos puntos lat/lon (grados)
static double ac_distKm(double la1,double lo1,double la2,double lo2){
  double p1=la1*AC_DEG,p2=la2*AC_DEG,dl=(lo2-lo1)*AC_DEG,dp=p2-p1;
  double a=sin(dp/2)*sin(dp/2)+cos(p1)*cos(p2)*sin(dl/2)*sin(dl/2);
  return 2.0*6371.0*atan2(sqrt(a),sqrt(1.0-a));
}

double obsLat(){ return cfg.valida ? (double)cfg.lat : 42.4627; }
double obsLon(){ return cfg.valida ? (double)cfg.lon : -2.4449; }
bool astroHoraOk(){ return time(nullptr) >= 1700000000; }
static String hmDesdeUnix(double t){ if(t<=0) return String("--:--"); time_t tt=(time_t)(t+30); struct tm x; localtime_r(&tt,&x); char b[8]; snprintf(b,sizeof(b),"%02d:%02d",x.tm_hour,x.tm_min); return String(b); }
int hmAMin(const String& s){ if(s.length()<5 || s[2]!=':') return -1; return s.substring(0,2).toInt()*60 + s.substring(3,5).toInt(); }
String minAHM(int m){ m=((m%1440)+1440)%1440; char b[8]; snprintf(b,sizeof(b),"%02d:%02d",m/60,m%60); return String(b); }

// ---- Luna (calculada al momento) ----
bool lunaOk=false; double diasLlena=-1, diasNueva=-1; float lunaMes[30]; static time_t lunaCalcT=0;
void calcularAstroLocal(){
  time_t ahoraT=time(nullptr);
  if(ahoraT<1700000000){ lunaOk=false; return; }          // sin hora real no se calcula (ni se inventa) nada
  if(lunaOk && lunaCalcT && (ahoraT-lunaCalcT)<300 && (ahoraT-lunaCalcT)>=0) return;
  int f0=getCpuFrequencyMhz(); setCpuFrequencyMhz(240);
  double t=(double)ahoraT, la=obsLat(), lo=obsLon();
  double el=ac_elong(t); faseLuna=(float)(el/360.0); iluminacionLuna=(int)floor((1.0-cos(el*AC_DEG))/2.0*100.0+0.5);
  double tl=ac_nextPhase(t,180.0), tn=ac_nextPhase(t,0.0);
  diasLlena = tl>0 ? (tl-t)/86400.0 : -1; diasNueva = tn>0 ? (tn-t)/86400.0 : -1;
  struct tm lt; localtime_r(&ahoraT,&lt); lt.tm_hour=0; lt.tm_min=0; lt.tm_sec=0; double t0=(double)mktime(&lt);
  double ri,se; ac_riseset(1,t0,la,lo,ri,se); horaSalidaLuna=hmDesdeUnix(ri); horaPuestaLuna=hmDesdeUnix(se);
  // Sol: solo si no llegaron de internet
  if(horaSalidaSol=="--:--" || horaPuestaSol=="--:--"){
    double sr,ss; ac_riseset(0,t0,la,lo,sr,ss);
    if(sr>0) horaSalidaSol=hmDesdeUnix(sr); if(ss>0) horaPuestaSol=hmDesdeUnix(ss);
    if(sr>0 && ss>sr && duracionDiaHoras<=0) duracionDiaHoras=(float)((ss-sr)/3600.0);
  }
  for(int i=0;i<30;i++){ double e=ac_elong(t+i*86400.0); lunaMes[i]=(float)((1.0-cos(e*AC_DEG))/2.0); }
  lunaOk=true; lunaCalcT=ahoraT; setCpuFrequencyMhz(f0);
}

// ---- Cielo de esta noche: planetas y constelaciones sobre el horizonte ----
struct Cons { float ra; float dec; const char* n[6]; };   // centro aproximado de cada constelacion (RA en horas, Dec en grados)
static const Cons CONS[] = {
 {5.6f,   0.0f,{"Orion","Orion","Orion","Orion","Orione","Orion"}},
 {4.7f,  16.0f,{"Tauro","Taurus","Touro","Taureau","Toro","Stier"}},
 {7.0f,  22.0f,{"Geminis","Gemini","Gemeos","Gemeaux","Gemelli","Zwillinge"}},
 {10.7f, 13.0f,{"Leo","Leo","Leao","Lion","Leone","Loewe"}},
 {13.2f, -4.0f,{"Virgo","Virgo","Virgem","Vierge","Vergine","Jungfrau"}},
 {16.9f,-27.0f,{"Escorpio","Scorpius","Escorpiao","Scorpion","Scorpione","Skorpion"}},
 {19.0f,-28.0f,{"Sagitario","Sagittarius","Sagitario","Sagittaire","Sagittario","Schuetze"}},
 {21.0f,-18.0f,{"Capricornio","Capricornus","Capricornio","Capricorne","Capricorno","Steinbock"}},
 {22.3f,-11.0f,{"Acuario","Aquarius","Aquario","Verseau","Acquario","Wassermann"}},
 {0.5f,  10.0f,{"Piscis","Pisces","Peixes","Poissons","Pesci","Fische"}},
 {2.6f,  20.0f,{"Aries","Aries","Aries","Belier","Ariete","Widder"}},
 {11.3f, 56.0f,{"Osa Mayor","Ursa Major","Ursa Maior","Grande Ourse","Orsa Maggiore","Grosser Baer"}},
 {15.0f, 78.0f,{"Osa Menor","Ursa Minor","Ursa Menor","Petite Ourse","Orsa Minore","Kleiner Baer"}},
 {1.0f,  60.0f,{"Casiopea","Cassiopeia","Cassiopeia","Cassiopee","Cassiopea","Kassiopeia"}},
 {20.6f, 42.0f,{"Cisne","Cygnus","Cisne","Cygne","Cigno","Schwan"}},
 {18.85f,36.0f,{"Lira","Lyra","Lira","Lyre","Lira","Leier"}},
 {19.7f,  3.0f,{"Aguila","Aquila","Aguia","Aigle","Aquila","Adler"}},
 {17.4f, 30.0f,{"Hercules","Hercules","Hercules","Hercule","Ercole","Herkules"}},
 {14.7f, 31.0f,{"Boyero","Bootes","Boieiro","Bouvier","Boote","Baerenhueter"}},
 {3.2f,  45.0f,{"Perseo","Perseus","Perseu","Persee","Perseo","Perseus"}},
 {5.2f,  42.0f,{"Auriga","Auriga","Auriga","Cocher","Auriga","Fuhrmann"}},
 {6.8f, -22.0f,{"Can Mayor","Canis Major","Cao Maior","Grand Chien","Cane Maggiore","Grosser Hund"}},
 {22.7f, 20.0f,{"Pegaso","Pegasus","Pegaso","Pegase","Pegaso","Pegasus"}},
 {0.8f,  38.0f,{"Andromeda","Andromeda","Andromeda","Andromede","Andromeda","Andromeda"}},
 {17.5f, 65.0f,{"Dragon","Draco","Dragao","Dragon","Drago","Drache"}},
 {22.0f, 70.0f,{"Cefeo","Cepheus","Cefeu","Cephee","Cefeo","Kepheus"}},
};
#define NCONS ((int)(sizeof(CONS)/sizeof(CONS[0])))
static const char* const PLN[6][5] = {
 {"Mercurio","Venus","Marte","Jupiter","Saturno"},{"Mercury","Venus","Mars","Jupiter","Saturn"},{"Mercurio","Venus","Marte","Jupiter","Saturno"},
 {"Mercure","Venus","Mars","Jupiter","Saturne"},{"Mercurio","Venere","Marte","Giove","Saturno"},{"Merkur","Venus","Mars","Jupiter","Saturn"}};
static const char* const BRUJ[6][8] = {
 {"N","NE","E","SE","S","SO","O","NO"},{"N","NE","E","SE","S","SW","W","NW"},{"N","NE","L","SE","S","SO","O","NO"},
 {"N","NE","E","SE","S","SO","O","NO"},{"N","NE","E","SE","S","SO","O","NO"},{"N","NO","O","SO","S","SW","W","NW"}};
struct CieloNoche { bool ok; time_t ref; int nPl; uint8_t pl[5]; int16_t plAlt[5]; int16_t plAz[5]; int nCon; uint8_t con[32]; int16_t conAlt[32]; };
CieloNoche cielo = {false,0,0,{0},{0},{0},0,{0},{0}}; static time_t cieloCalcT=0;
void calcularCielo(){
  time_t ahoraT=time(nullptr);
  if(ahoraT<1700000000){ cielo.ok=false; return; }
  if(cielo.ok && cieloCalcT && (ahoraT-cieloCalcT)<300 && (ahoraT-cieloCalcT)>=0) return;
  int f0=getCpuFrequencyMhz(); setCpuFrequencyMhz(240);
  double la=obsLat(), lo=obsLon(), t=(double)ahoraT, ref=t;
  if(ac_sunAlt(t,la,lo) >= -6.0){ for(int k=1;k<=144;k++){ double tk=t+k*600.0; if(ac_sunAlt(tk,la,lo) < -6.0){ ref=tk; break; } } }   // proxima hora en que el cielo esta oscuro
  cielo.ref=(time_t)ref; cielo.nPl=0; cielo.nCon=0;
  double d=ac_dayNum(ref);
  for(int i=0;i<5;i++){
    double ra,dec,alt,az; ac_planet(i,d,ra,dec); ac_altaz(ra,dec,ref,la,lo,alt,az);
    if(alt>=5.0){ int p=cielo.nPl++; while(p>0 && cielo.plAlt[p-1]<(int)alt){ cielo.pl[p]=cielo.pl[p-1]; cielo.plAlt[p]=cielo.plAlt[p-1]; cielo.plAz[p]=cielo.plAz[p-1]; p--; } cielo.pl[p]=i; cielo.plAlt[p]=(int16_t)alt; cielo.plAz[p]=(int16_t)az; }
  }
  for(int i=0;i<NCONS;i++){
    double alt,az; ac_altaz(CONS[i].ra*15.0,CONS[i].dec,ref,la,lo,alt,az);
    if(alt>=20.0){ int p=cielo.nCon++; while(p>0 && cielo.conAlt[p-1]<(int)alt){ cielo.con[p]=cielo.con[p-1]; cielo.conAlt[p]=cielo.conAlt[p-1]; p--; } cielo.con[p]=i; cielo.conAlt[p]=(int16_t)alt; }
  }
  cielo.ok=true; cieloCalcT=ahoraT; setCpuFrequencyMhz(f0);
}

// ================= PANTALLAS ASTRO + MENU =================
// Idiomas: 0=es 1=en 2=pt 3=fr 4=it 5=de
static const char* const A_SOL[6]   = {"SOL","SUN","SOL","SOLEIL","SOLE","SONNE"};
static const char* const A_LUNA[6]  = {"LUNA","MOON","LUA","LUNE","LUNA","MOND"};
static const char* const A_ESTR[6]  = {"ESTRELLAS","STARS","ESTRELAS","ETOILES","STELLE","STERNE"};
static const char* const A_LUZ[6]   = {"LUZ DEL DIA","DAYLIGHT","LUZ DO DIA","LUMIERE DU JOUR","LUCE DEL GIORNO","TAGESLICHT"};
static const char* const A_SALE[6]  = {"Sale","Rise","Nasce","Lever","Sorge","Aufgang"};
static const char* const A_PONE[6]  = {"Pone","Set","Poe","Coucher","Tramonta","Untergang"};
static const char* const A_DIA[6]   = {"Dia %.1fh","Day %.1fh","Dia %.1fh","Jour %.1fh","Giorno %.1fh","Tag %.1fh"};
static const char* const A_LUZP[6]  = {"%d%% luz","%d%% lit","%d%% luz","%d%% eclairee","%d%% luce","%d%% hell"};
static const char* const A_TRAY[6]  = {"TRAYECTORIA SOLAR HOY","SOLAR PATH TODAY","TRAJETORIA SOLAR HOJE","TRAJET DU SOLEIL AUJ.","PERCORSO SOLARE OGGI","SONNENBAHN HEUTE"};
static const char* const A_MEDIO[6] = {"Mediodia solar","Solar noon","Meio-dia solar","Midi solaire","Mezzogiorno solare","Sonnenhoechststand"};
static const char* const A_DURDIA[6]= {"Duracion del dia: %.1f horas","Day length: %.1f hours","Duracao do dia: %.1f horas","Duree du jour: %.1f heures","Durata del giorno: %.1f ore","Tageslaenge: %.1f Stunden"};
static const char* const A_DORADA[6]= {"Hora dorada: %s-%s y %s-%s","Golden hour: %s-%s and %s-%s","Hora dourada: %s-%s e %s-%s","Heure doree: %s-%s et %s-%s","Ora d'oro: %s-%s e %s-%s","Goldene Stunde: %s-%s und %s-%s"};
static const char* const A_TOCA[6]  = {"Toca para volver  -  Desliza para cambiar","Tap to go back  -  Swipe to change","Toque para voltar  -  Deslize para mudar","Toucher: retour  -  Glisser: changer","Tocca per tornare  -  Scorri per cambiare","Tippen: zurueck  -  Wischen: wechseln"};
static const char* const A_FASE[6][8] = {
  {"Nueva","Creciente","Cuarto crec.","Creciente","Llena","Menguante","Cuarto meng.","Menguante"},
  {"New","Waxing crescent","First quarter","Waxing gibbous","Full","Waning gibbous","Last quarter","Waning crescent"},
  {"Nova","Crescente","Quarto cresc.","Gibosa cresc.","Cheia","Gibosa mingu.","Quarto mingu.","Minguante"},
  {"Nouvelle","Croissant","Premier quartier","Gibbeuse croiss.","Pleine","Gibbeuse decr.","Dernier quartier","Decroissant"},
  {"Nuova","Crescente","Primo quarto","Gibbosa cresc.","Piena","Gibbosa calan.","Ultimo quarto","Calante"},
  {"Neumond","Sichel zun.","Erstes Viertel","Zunehm. 3/4","Vollmond","Abnehm. 3/4","Letztes Viertel","Sichel abn."}
};
static const char* const A_ILUM[6]  = {"%d%% iluminada","%d%% illuminated","%d%% iluminada","%d%% eclairee","%d%% illuminata","%d%% beleuchtet"};
static const char* const A_LLENA[6] = {"Llena en %.0f dias - Nueva en %.0f dias","Full in %.0f days - New in %.0f days","Cheia em %.0f dias - Nova em %.0f dias","Pleine dans %.0f j - Nouvelle dans %.0f j","Piena tra %.0f gg - Nuova tra %.0f gg","Voll in %.0f Tg. - Neu in %.0f Tg."};
static const char* const A_MES[6]   = {"Iluminacion proximos 30 dias","Illumination next 30 days","Iluminacao proximos 30 dias","Illumination 30 prochains jours","Illuminazione prossimi 30 giorni","Beleuchtung naechste 30 Tage"};
static const char* const A_CIELO[6] = {"CIELO DE ESTA NOCHE","TONIGHT'S SKY","CEU DESTA NOITE","CIEL DE CETTE NUIT","CIELO DI STANOTTE","HIMMEL HEUTE NACHT"};
static const char* const A_CALC[6]  = {"Calculado para las %02d:%02d","Calculated for %02d:%02d","Calculado para as %02d:%02d","Calcule pour %02d:%02d","Calcolato per le %02d:%02d","Berechnet fuer %02d:%02d"};
static const char* const A_CONSTV[6]= {"Constelaciones visibles:","Visible constellations:","Constelacoes visiveis:","Constellations visibles:","Costellazioni visibili:","Sichtbare Sternbilder:"};
static const char* const A_CONSP[6] = {"Const:","Const:","Const:","Const:","Cost:","Sternb:"};
static const char* const A_PLANETAS[6]={"Planetas:","Planets:","Planetas:","Planetes:","Pianeti:","Planeten:"};
static const char* const A_NINGUNO[6]={"ninguno visible","none visible","nenhum visivel","aucune visible","nessuno visibile","keiner sichtbar"};
static const char* const A_ALT[6]   = {"alt","alt","alt","haut.","alt","Hoehe"};
static const char* const A_ISSDIST[6]={"a %d km de ti","%d km from you","a %d km de si","a %d km de vous","a %d km da te","%d km entfernt"};
static const char* const A_ISSNOTA[6]={"Posicion en el momento de la descarga","Position at the time of download","Posicao no momento do download","Position au moment du telechargement","Posizione al momento del download","Position zum Zeitpunkt des Downloads"};
static const char* const A_NOHORA[6]= {"Sin hora sincronizada","Time not synced","Hora nao sincronizada","Heure non synchronisee","Ora non sincronizzata","Zeit nicht synchronisiert"};
static const char* const A_NOCHE[6] = {"Noche","Night","Noite","Nuit","Notte","Nacht"};
static const char* const A_AHORA[6] = {"Ahora","Now","Agora","Maint.","Ora","Jetzt"};
static const char* const A_BAJO_H[6] = {"Bajo horizonte","Below horiz.","Abaixo horiz.","Sous horizon","Sotto orizz.","Unter Horiz."};
static const char* const A_CIVIL[6] = {"Civil","Civil","Civil","Civil","Civile","Buergerl."};
static const char* const A_NAUT[6] = {"Naut.","Naut.","Naut.","Naut.","Naut.","Naut."};
static const char* const A_ASTRO_LBL[6] = {"Astro","Astro","Astro","Astro","Astro","Astro"};
static const char* const A_DIANOCHE[6] = {"Dia %.1fh / %s %.1fh","Day %.1fh / %s %.1fh","Dia %.1fh / %s %.1fh","Jour %.1fh / %s %.1fh","Giorno %.1fh / %s %.1fh","Tag %.1fh / %s %.1fh"};
static const char* const A_VSAYER[6] = {"%+0.0fm vs ayer","%+0.0fm vs yest.","%+0.0fm vs ontem","%+0.0fm vs hier","%+0.0fm vs ieri","%+0.0fm vs gest."};

// ---- Menu ----
static const char* const M_LBL[6][6] = {
  {"Tiempo","Astro","Actualizar datos","Buscar actualizaciones","Actualizar desde SD","Configuracion"},
  {"Weather","Astro","Update data","Check for updates","Update from SD","Settings"},
  {"Tempo","Astro","Atualizar dados","Procurar atualizacoes","Atualizar do SD","Configuracao"},
  {"Meteo","Astro","Actualiser donnees","Chercher mises a jour","Mise a jour depuis SD","Configuration"},
  {"Meteo","Astro","Aggiorna dati","Cerca aggiornamenti","Aggiorna da SD","Impostazioni"},
  {"Wetter","Astro","Daten aktualisieren","Nach Updates suchen","Update von SD","Einstellungen"}
};
static const char* const M_TIT[6] = {"MENU","MENU","MENU","MENU","MENU","MENU"};
// Pagina 1 del menu: casilla -> accion (0 tiempo, 1 astro, 2 actualizar datos, 3 buscar OTA, 4 SD, 5 configuracion, 6 CrossPoint)
static const int MENU_P0[6] = {0, 1, 6, 2, 3, 4};

const unsigned char icon_crosspoint_80x80[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x07, 0x80,
  0x01, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x1e, 0xff, 0x80,
  0x01, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xde, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xde, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xde, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xde, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xde, 0xff, 0x80,
  0x01, 0xf0, 0x07, 0xff, 0xfc, 0x3f, 0xff, 0xde, 0x0f, 0x80,
  0x01, 0xf0, 0x00, 0x03, 0xfc, 0x3f, 0xc0, 0x12, 0x0f, 0x80,
  0x01, 0xff, 0xf0, 0x00, 0xfc, 0x3f, 0x00, 0x00, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xf8, 0xfc, 0x3f, 0x1f, 0xdc, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0x80,
  0x01, 0xf0, 0x03, 0xff, 0xfc, 0x3f, 0xff, 0xc0, 0x0f, 0x80,
  0x01, 0xf0, 0x00, 0x01, 0xfc, 0x3f, 0x80, 0x00, 0x0f, 0x80,
  0x01, 0xff, 0xf8, 0x00, 0xfc, 0x3f, 0x00, 0x1f, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xfc, 0xfc, 0x3f, 0x3f, 0xff, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0x80,
  0x01, 0xf0, 0x01, 0xff, 0xfc, 0x3f, 0xff, 0x80, 0x0f, 0x80,
  0x01, 0xf8, 0x00, 0x00, 0xfc, 0x3f, 0x00, 0x00, 0x0f, 0x80,
  0x01, 0xff, 0xfc, 0x00, 0xfc, 0x3f, 0x00, 0x3f, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xfe, 0xfc, 0x3f, 0x7f, 0xff, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0x80,
  0x01, 0xf3, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xcf, 0x80,
  0x01, 0xf0, 0x00, 0xff, 0xfc, 0x3f, 0xff, 0x00, 0x0f, 0x80,
  0x01, 0xf8, 0x00, 0x00, 0xfc, 0x3f, 0x00, 0x00, 0x1f, 0x80,
  0x01, 0xff, 0xfe, 0x00, 0xfc, 0x3f, 0x00, 0x7f, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0x80,
  0x01, 0xf1, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0x8f, 0x80,
  0x01, 0xf0, 0x00, 0x7f, 0xfc, 0x3f, 0xfe, 0x00, 0x0f, 0x80,
  0x01, 0xfe, 0x00, 0x00, 0xfc, 0x3f, 0x00, 0x00, 0x3f, 0x80,
  0x01, 0xff, 0xff, 0x00, 0xfc, 0x3f, 0x00, 0xff, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0x80,
  0x01, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0x80,
  0x00, 0x0f, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xf8, 0x00,
  0x00, 0x00, 0x1f, 0xff, 0xfc, 0x3f, 0xff, 0xf8, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x3f, 0xfc, 0x3f, 0xfc, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x3c, 0x3c, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// ---- Otra aplicacion (p.ej. CrossPoint) en el otro hueco OTA ----
static const char* const T_CP_ETQ[6]     = {"CrossPoint","CrossPoint","CrossPoint","CrossPoint","CrossPoint","CrossPoint"};
static const char* const T_CP_VACIO[6]   = {"Hueco vacio","Empty slot","Espaco vazio","Emplacement vide","Slot vuoto","Leerer Slot"};
static const char* const T_CP_IGUAL[6]   = {"Solo AstroWeather","Only AstroWeather","So AstroWeather","Que AstroWeather","Solo AstroWeather","Nur AstroWeather"};
static const char* const T_CP_NOAPP[6]   = {"No hay otra app","No other app","Sem outra app","Pas d'autre app","Nessuna altra app","Keine andere App"};
static const char* const T_CP_INSTALA[6] = {"Instala CrossPoint desde la SD","Install CrossPoint from SD","Instale o CrossPoint pelo SD","Installe CrossPoint depuis SD","Installa CrossPoint da SD","CrossPoint von SD installieren"};
static const char* const T_CP_MISMA[6]   = {"El otro hueco es esta misma app","Other slot is this same app","Outro slot e a mesma app","Autre slot = meme app","Altro slot = stessa app","Anderer Slot = gleiche App"};
static const char* const T_CP_ABRIENDO[6]= {"Abriendo...","Opening...","Abrindo...","Ouverture...","Apertura...","Oeffne..."};

// Lee el descriptor de una particion de aplicacion con esp_partition_read (sin usar mmap).
// Imagen ESP: cabecera 24 B + cabecera de segmento 8 B -> el esp_app_desc_t empieza en 0x20.
bool leerDescApp(const esp_partition_t* p, esp_app_desc_t* d){
  if(!p || !d) return false;
  uint8_t hdr[0x20 + sizeof(esp_app_desc_t)];
  esp_err_t e = esp_partition_read(p, 0, hdr, sizeof(hdr));
  if(e != ESP_OK){
    Serial.printf("leerDescApp: esp_partition_read(%s @0x%x) error %s\n", p->label, (unsigned)p->address, esp_err_to_name(e)); Serial.flush();
    return false;
  }
  if(hdr[0] != 0xE9){
    Serial.printf("leerDescApp: %s no empieza por 0xE9 (0x%02x)\n", p->label, hdr[0]); Serial.flush();
    return false;
  }
  memcpy(d, hdr + 0x20, sizeof(esp_app_desc_t));
  if(d->magic_word != ESP_APP_DESC_MAGIC_WORD){
    Serial.printf("leerDescApp: %s sin descriptor (magic 0x%08x)\n", p->label, (unsigned)d->magic_word); Serial.flush();
    return false;
  }
  return true;
}

// Lee nombre y version de la aplicacion del OTRO hueco. true si hay una imagen valida.
// esMia = true si es exactamente el mismo programa (mismo nombre y version) que el que esta corriendo.
bool otraAppInfo(char* nombre, size_t nn, char* version, size_t nv, bool* esMia){
  nombre[0]=0; version[0]=0; if(esMia) *esMia=false;
  const esp_partition_t* p = esp_ota_get_next_update_partition(NULL);
  if(!p) return false;
  esp_app_desc_t d;
  if(!leerDescApp(p, &d)) return false;
  strlcpy(nombre, d.project_name, nn);
  snprintf(version, nv, "%.11s %.5s", d.date, d.time);   // fecha y hora de compilacion de la otra app
  const esp_app_desc_t* yo = esp_ota_get_app_description();
  if(esMia && yo) *esMia = (strcmp(d.project_name, yo->project_name)==0 && strcmp(d.version, yo->version)==0 &&
                            strcmp(d.date, yo->date)==0 && strcmp(d.time, yo->time)==0);
  return true;
}

// Aviso en las pantallas de instalacion: dice que aplicacion del otro hueco se va a sustituir
void dibujarAvisoSustituye(int y){
  char on[24], ov[24]; bool igual=false;
  if(otraAppInfo(on,sizeof(on),ov,sizeof(ov),&igual) && !igual){
    char w[72]; snprintf(w,sizeof(w),"Sustituira / Replaces: %s",ov);
    txtCentrado(w,400,y,2,false);
  }
}

void dibujarPuntosMenu(int pag){
  if(pag==0){ display.fillCircle(388,472,4,NEGRO); display.drawCircle(412,472,4,NEGRO); }
  else      { display.drawCircle(388,472,4,NEGRO); display.fillCircle(412,472,4,NEGRO); }
}

void dibujarMenuPagina2(){
  int l=idioma();
  display.fillScreen(BLANCO);
  txtCentrado(M_TIT[l], 400, 14, 3, true);
  if(iconoLunaVisible) dibujarLunaMenguante(32,18,8);   // lunita de suspension (arriba izquierda)
  dibujarBateria(700, 10, nivelBateria, cargando);
  int x=20, y=58, w=370, h=124;                          // misma casilla que la primera de la pagina 1
  display.drawRect(x,y,w,h,NEGRO); display.drawRect(x+1,y+1,w-2,h-2,NEGRO);
  display.drawBitmap(x+12, y+(h-80)/2, icon_config_80x80, 80, 80, NEGRO);
  txtBoton(M_LBL[l][5], x, y, w, h);                     // Configuracion
  dibujarPuntosMenu(1);
  display.update();
}


void solIcon(int cx,int cy,int r){ int g=max(1,r/14); display.fillCircle(cx,cy,r*0.55,NEGRO); for(int i=0;i<8;i++){ float a=i*PI/4; lineaGruesa(cx+cos(a)*r*0.75,cy+sin(a)*r*0.75,cx+cos(a)*r*1.05,cy+sin(a)*r*1.05,g);} }
// Luna con la fase real: parte iluminada en blanco, parte oscura en negro (se invierte el lado en el hemisferio sur)
void dibujarLuna(int cx,int cy,int r,float fase){
  float k=cos(fase*2.0f*PI);                       // 1 = nueva, 0 = cuartos, -1 = llena
  bool sur = obsLat()<0;
  display.fillCircle(cx,cy,r,NEGRO);
  for(int dy=-r+1; dy<=r-1; dy++){
    float w=sqrt((float)r*r-(float)dy*dy), x0, x1;
    if(fase<0.5f){ x0=k*w; x1=w; } else { x0=-w; x1=-k*w; }       // hemisferio norte: creciente ilumina la derecha
    int xa=(int)floor(x0+0.5f), xb=(int)floor(x1+0.5f);
    if(sur){ int ta=-xb, tb=-xa; xa=ta; xb=tb; }
    if(xb>xa) display.drawFastHLine(cx+xa, cy+dy, xb-xa, BLANCO);
  }
  display.drawCircle(cx,cy,r,NEGRO);
}
const char* nombreFase(float f){
  int i; if(f<0.03 || f>0.97) i=0; else if(f<0.22) i=1; else if(f<0.28) i=2; else if(f<0.47) i=3; else if(f<0.53) i=4; else if(f<0.72) i=5; else if(f<0.78) i=6; else i=7;
  return A_FASE[idioma()][i];
}
// Cabecera de las pantallas de detalle de astro (ciudad, fecha/hora, bateria)
void cabeceraAstro(){
  dibujarBateria(700,10,nivelBateria,cargando);
  if(iconoLunaVisible) dibujarLunaMenguante(32,18,8);   // lunita de suspension (arriba izquierda)
}
// ---- utilidades de texto ----
String issLinea(){
  if(issTs<=0) return String("ISS --");
  time_t tt=issTs; struct tm x; localtime_r(&tt,&x); char b[56]; snprintf(b,sizeof(b),"ISS %02d:%02d  %s, %s",x.tm_hour,x.tm_min,issLat.c_str(),issLon.c_str()); return String(b);
}
String listaPlanetas(int l,int maxPx){
  String s; for(int i=0;i<cielo.nPl;i++){ String t = s.length() ? s+", "+String(PLN[l][cielo.pl[i]]) : String(PLN[l][cielo.pl[i]]); if(anchoTxt(t.c_str(),2)>maxPx) break; s=t; }
  return s.length() ? s : String(A_NINGUNO[l]);
}
String listaConstelaciones(int l,int maxItems,int maxPx){
  String s; for(int i=0;i<cielo.nCon && i<maxItems;i++){ String t = s.length() ? s+", "+String(CONS[cielo.con[i]].n[l]) : String(CONS[cielo.con[i]].n[l]); if(anchoTxt(t.c_str(),2)>maxPx) break; s=t; }
  return s.length() ? s : String(A_NINGUNO[l]);
}
// Escribe nombres separados por coma ajustando a varias lineas. Devuelve la y siguiente.
int txtListaConstelaciones(int l,int x,int y,int size,int maxW,int lineH,int maxLines){
  String linea=""; int lines=0;
  for(int i=0;i<cielo.nCon;i++){
    String n=String(CONS[cielo.con[i]].n[l]); String t = linea.length() ? linea+", "+n : n;
    if(anchoTxt(t.c_str(),size)>maxW && linea.length()){ if(lines>=maxLines) return y; txtSimple((linea+",").c_str(),x,y,size,false); y+=lineH; lines++; linea=n; } else linea=t;
  }
  if(linea.length() && lines<maxLines){ txtSimple(linea.c_str(),x,y,size,false); y+=lineH; }
  return y;
}

void dibujarAstroResumen(){
  int l=idioma(); char buf[96];
  calcularAstroLocal(); calcularCielo();
  display.fillScreen(BLANCO);
  cabeceraResumen(T_BTN_TIEMPO[l]);
  // SOL
  int y0=96;
  display.drawRect(12, y0, 376, 140, NEGRO);
  txtSimple(A_SOL[l], 26, y0+8, 3, true);
  solIcon(345, y0+55, 26);
  snprintf(buf,sizeof(buf),"%s %s",A_SALE[l],horaSalidaSol.c_str()); txtSimple(buf, 26, y0+40, 3, false);
  snprintf(buf,sizeof(buf),"%s %s",A_PONE[l],horaPuestaSol.c_str()); txtSimple(buf, 26, y0+72, 3, false);
  if(duracionDiaHoras>0){ snprintf(buf,sizeof(buf),A_DIA[l],duracionDiaHoras); txtSimple(buf, 26, y0+108, 2, true); }
  // LUNA
  display.drawRect(412, y0, 376, 140, NEGRO);
  txtSimple(A_LUNA[l], 426, y0+8, 3, true);
  if(lunaOk){
    dibujarLuna(468, y0+62, 26, faseLuna);
    txtSimple(nombreFase(faseLuna), 516, y0+34, 2, true);
    snprintf(buf,sizeof(buf),A_LUZP[l],iluminacionLuna); txtSimple(buf, 516, y0+56, 3, false);
    snprintf(buf,sizeof(buf),"%s %s",A_SALE[l],horaSalidaLuna.c_str()); txtSimple(buf, 426, y0+92, 3, false);
    snprintf(buf,sizeof(buf),"%s %s",A_PONE[l],horaPuestaLuna.c_str()); txtSimple(buf, 426, y0+114, 3, false);
  } else txtSimple(A_NOHORA[l], 426, y0+60, 2, false);
  // ESTRELLAS (planetas y constelaciones calculados; ISS descargada)
  int y1=244;
  display.drawRect(12, y1, 776, 102, NEGRO);
  txtSimple(A_ESTR[l], 26, y1+8, 3, true);
  txtSimple(issLinea().c_str(), 26, y1+38, 2, false);
  if(cielo.ok){
    struct tm r; time_t rr=cielo.ref; localtime_r(&rr,&r); snprintf(buf,sizeof(buf),A_CALC[l],r.tm_hour,r.tm_min); txtDerecha(buf, 772, y1+12, 2, false);
    String s1=String(A_PLANETAS[l])+" "+listaPlanetas(l,730-anchoTxt(A_PLANETAS[l],2)); txtSimple(s1.c_str(), 26, y1+60, 2, false);
    String s2=String(A_CONSP[l])+" "+listaConstelaciones(l,3,730-anchoTxt(A_CONSP[l],2)); txtSimple(s2.c_str(), 26, y1+82, 2, false);
  } else txtSimple(A_NOHORA[l], 26, y1+60, 2, false);
  // LUZ DEL DIA
  int y2=354;
  display.drawRect(12, y2, 776, 118, NEGRO);
  txtSimple(A_LUZ[l], 26, y2+8, 3, true);
  int bx=26, bw=740, by=y2+46, bh=18;
  display.drawRect(bx,by,bw,bh,NEGRO);
  int m1=hmAMin(horaSalidaSol), m2=hmAMin(horaPuestaSol);
  if(m1>=0 && m2>m1){
    int x1=bx+(long)m1*bw/1440, x2=bx+(long)m2*bw/1440;
    display.fillRect(x1,by,x2-x1,bh,NEGRO);
    txtSimple(horaSalidaSol.c_str(), x1-12, by+bh+6, 2, true);
    txtSimple(horaPuestaSol.c_str(), x2-12, by+bh+6, 2, true);
  }
  display.update();
}

void dibujarSolDetallado(){
  int l=idioma(); char buf[140];
  calcularAstroLocal();
  display.fillScreen(BLANCO); cabeceraAstro();
  txtCentrado(A_TRAY[l], 400, 58, 3, true);
  int cx=400, cy=270, rx=300, ry=130;
  time_t ahoraT = time(nullptr);
  double lat=obsLat(), lon=obsLon();
  // Calculo tiempos base de hoy
  struct tm lt; localtime_r(&ahoraT,&lt); lt.tm_hour=0; lt.tm_min=0; lt.tm_sec=0; time_t t0mid = mktime(&lt);
  int m1=hmAMin(horaSalidaSol), m2=hmAMin(horaPuestaSol);
  double tRise = (m1>=0)? t0mid + m1*60.0 : 0;
  double tSet  = (m2>=0)? t0mid + m2*60.0 : 0;
  double tNoon = (m1>=0 && m2>m1)? t0mid + (m1+m2)/2.0*60.0 : t0mid+12*3600.0;
  // Altitudes y azimuts
  double altMax= -90, azRise=0, azSet=0, altAhora=-90, azAhora=0;
  double alt, az;
  if(tRise>0){ ac_sunAltAz(tRise,lat,lon,alt,az); azRise=az; }
  if(tSet>0){ ac_sunAltAz(tSet,lat,lon,alt,az); azSet=az; }
  if(tNoon>0){ ac_sunAltAz(tNoon,lat,lon,altMax,az); }
  else altMax = ac_sunAlt(tNoon,lat,lon);
  if(ahoraT>=1700000000){ ac_sunAltAz((double)ahoraT,lat,lon,altAhora,azAhora); }
  // Busca alt max real entre salida y puesta
  if(m1>=0 && m2>m1){
    double maxA=-90; for(double t=tRise; t<=tSet; t+=300){ double a=ac_sunAlt(t,lat,lon); if(a>maxA) maxA=a; }
    if(maxA>-90) altMax=maxA;
  }
  // Crepusculos
  double rc6,sc6, rc12,sc12, rc18,sc18;
  ac_riseset_h0(t0mid,lat,lon,-6.0,rc6,sc6);
  ac_riseset_h0(t0mid,lat,lon,-12.0,rc12,sc12);
  ac_riseset_h0(t0mid,lat,lon,-18.0,rc18,sc18);
  // Ayer para diferencia
  double rAyer,sAyer; ac_riseset_h0(t0mid-86400,lat,lon,-0.833,rAyer,sAyer);
  float durAyerH = (rAyer>0 && sAyer>0)? (sAyer-rAyer)/3600.0f : 0;
  float diffMin = (duracionDiaHoras>0 && durAyerH>0)? (duracionDiaHoras-durAyerH)*60.0f : 0;

  // --- GRAFICO ARCO ---
  for(float a=PI; a<2*PI; a+=0.01){ int x=cx+cos(a)*rx; int y=cy+sin(a)*ry; display.drawPixel(x,y,NEGRO); }
  // marca civil
  display.drawCircle(cx-rx+20, cy, 3, NEGRO);
  display.drawCircle(cx+rx-20, cy, 3, NEGRO);
  // salida / puesta base
  display.fillCircle(80, cy, 7, NEGRO);
  display.fillCircle(720, cy, 7, NEGRO);
  display.fillCircle(cx, cy-ry, 9, NEGRO); display.fillCircle(cx, cy-ry, 4, BLANCO);

  // Textos alrededor arco
  if(m1>=0) { snprintf(buf,sizeof(buf),"%s %s %d %s",A_SALE[l],horaSalidaSol.c_str(), (int)round(azRise), ac_brujula(azRise)); txtSimple(buf, 30, cy+16, 2, true); }
  if(m2>=0) { snprintf(buf,sizeof(buf),"%d %s %s %s", (int)round(azSet), ac_brujula(azSet), A_PONE[l], horaPuestaSol.c_str()); txtDerecha(buf, 770, cy+16, 2, true); }

  // Mediodia
  if(m1>=0 && m2>m1){ snprintf(buf,sizeof(buf),"%s %s  %.0f",A_MEDIO[l],minAHM((m1+m2)/2).c_str(), altMax); }
  else snprintf(buf,sizeof(buf),"%s %.0f",A_MEDIO[l], altMax);
  txtCentrado(buf, cx, cy-ry-28, 2, true);

  // Posicion actual del sol sobre el arco (si es de dia)
  if(altAhora>=-0.833 && m1>=0 && m2>m1 && ahoraT>=tRise && ahoraT<=tSet){
    double frac = (ahoraT - tRise) / (tSet - tRise); // 0..1
    double ang = PI + frac*PI; // PI .. 2PI
    int sx = cx + cos(ang)*rx;
    int sy = cy + sin(ang)*ry;
    display.fillCircle(sx, sy, 6, NEGRO); display.fillCircle(sx, sy, 3, BLANCO);
  }

  // --- DATOS INFERIORES MAXIMOS --- traducido a 6 idiomas
  int yBase = 320;
  float nocheH = (duracionDiaHoras>0)? 24.0f-duracionDiaHoras : 0;
  if(duracionDiaHoras>0){
    char tmpDiaNoche[80];
    snprintf(tmpDiaNoche,sizeof(tmpDiaNoche),A_DIANOCHE[l], duracionDiaHoras, A_NOCHE[l], nocheH);
    if(fabs(diffMin)>=0.5){
      char tmpVs[32]; snprintf(tmpVs,sizeof(tmpVs),A_VSAYER[l], diffMin);
      snprintf(buf,sizeof(buf),"%s %s  UV %.0f Sol %.1fh", tmpDiaNoche, tmpVs, astroUvMax, astroSunshineHoras);
    } else {
      snprintf(buf,sizeof(buf),"%s  UV %.0f Sol %.1fh", tmpDiaNoche, astroUvMax, astroSunshineHoras);
    }
    txtCentrado(buf, 400, yBase, 2, false);
  }
  // Linea 2: ahora
  if(altAhora>-90){
    if(altAhora>= -0.8) snprintf(buf,sizeof(buf),"%s %02d:%02d  %.0f / %d %s  Max %.0f", A_AHORA[l], lt.tm_hour, lt.tm_min, altAhora, (int)round(azAhora), ac_brujula(azAhora), altMax);
    else snprintf(buf,sizeof(buf),"%s %02d:%02d  %s %.0f  Max %.0f", A_AHORA[l], lt.tm_hour, lt.tm_min, A_BAJO_H[l], altAhora, altMax);
    txtCentrado(buf, 400, yBase+22, 2, true);
  }
  // Linea 3: dorada (usa A_DORADA que ya esta traducida)
  if(m1>=0 && m2>m1){
    snprintf(buf,sizeof(buf),"%s %s-%s / %s-%s", A_DORADA[l], horaSalidaSol.c_str(), minAHM(m1+60).c_str(), minAHM(m2-60).c_str(), horaPuestaSol.c_str());
    txtCentrado(buf, 400, yBase+44, 2, false);
  }
  // Linea 4 y 5: crepusculos traducidos
  {
    String cCiv = (rc6>0 && sc6>0)? hmDesdeUnix(rc6)+" - "+hmDesdeUnix(sc6) : "--";
    String cNau = (rc12>0 && sc12>0)? hmDesdeUnix(rc12)+" - "+hmDesdeUnix(sc12) : "--";
    String cAst = (rc18>0 && sc18>0)? hmDesdeUnix(rc18)+" - "+hmDesdeUnix(sc18) : "--";
    snprintf(buf,sizeof(buf),"%s %s", A_CIVIL[l], cCiv.c_str());
    txtCentrado(buf, 400, yBase+66, 2, false);
    snprintf(buf,sizeof(buf),"%s %s  %s %s", A_NAUT[l], cNau.c_str(), A_ASTRO_LBL[l], cAst.c_str());
    txtCentrado(buf, 400, yBase+84, 2, false);
  }

  txtCentrado(A_TOCA[l], 400, 456, 2, false);
  display.update();
}

void dibujarLunaDetallada(){
  int l=idioma(); char buf[112];
  calcularAstroLocal();
  display.fillScreen(BLANCO); cabeceraAstro();
  if(!lunaOk){ txtCentrado(A_NOHORA[l], 400, 220, 3, true); txtCentrado(A_TOCA[l], 400, 454, 2, false); display.update(); return; }
  dibujarLuna(180, 170, 75, faseLuna);
  txtSimple(nombreFase(faseLuna), 300, 84, 4, true);
  snprintf(buf,sizeof(buf),A_ILUM[l],iluminacionLuna); txtSimple(buf, 300, 126, 3, false);
  snprintf(buf,sizeof(buf),"%s: %s",A_SALE[l],horaSalidaLuna.c_str()); txtSimple(buf, 300, 162, 3, false);
  snprintf(buf,sizeof(buf),"%s: %s",A_PONE[l],horaPuestaLuna.c_str()); txtSimple(buf, 300, 194, 3, false);
  if(diasLlena>=0 && diasNueva>=0){ snprintf(buf,sizeof(buf),A_LLENA[l], diasLlena, diasNueva); txtSimple(buf, 20, 262, 2, true); }
  display.drawRect(12, 292, 776, 154, NEGRO);
  txtSimple(A_MES[l], 26, 300, 3, true);
  for(int i=0;i<30;i++){ int h = (int)(lunaMes[i]*90); display.fillRect(26+i*25, 436-h, 18, h, NEGRO); }
  txtCentrado(A_TOCA[l], 400, 454, 2, false);
  display.update();
}

void dibujarEstrellas(){
  int l=idioma(); char buf[128];
  calcularCielo();
  display.fillScreen(BLANCO); cabeceraAstro();
  txtCentrado(A_CIELO[l], 400, 58, 3, true);
  if(!cielo.ok){ txtCentrado(A_NOHORA[l], 400, 220, 3, true); txtCentrado(A_TOCA[l], 400, 454, 2, false); display.update(); return; }
  struct tm r; time_t rr=cielo.ref; localtime_r(&rr,&r); snprintf(buf,sizeof(buf),A_CALC[l],r.tm_hour,r.tm_min); txtCentrado(buf, 400, 90, 2, false);
  txtSimple(A_CONSTV[l], 26, 118, 3, true);
  if(cielo.nCon>0) txtListaConstelaciones(l, 26, 148, 2, 750, 22, 3); else txtSimple(A_NINGUNO[l], 26, 148, 2, false);
  txtSimple(A_PLANETAS[l], 26, 226, 3, true);
  if(cielo.nPl==0) txtSimple(A_NINGUNO[l], 26, 256, 2, false);
  for(int i=0;i<cielo.nPl;i++){
    int br=((int)((cielo.plAz[i]+22.5f)/45.0f))%8;
    snprintf(buf,sizeof(buf),"%s   %s %d   %s",PLN[l][cielo.pl[i]],A_ALT[l],cielo.plAlt[i],BRUJ[l][br]);
    txtSimple(buf, 26, 256+i*22, 2, false);
  }
  // ISS: ultima posicion descargada (instantanea)
  txtSimple(issLinea().c_str(), 26, 380, 2, true);
  if(issTs>0){
    int km=(int)ac_distKm(obsLat(),obsLon(),issLat.toFloat(),issLon.toFloat());
    snprintf(buf,sizeof(buf),A_ISSDIST[l],km); txtSimple(buf, 26, 402, 2, false);
    txtSimple(A_ISSNOTA[l], 26, 424, 2, false);
  }
  // --- Constelacion decorativa en hueco centro-derecha (no estorba, alegra vista) ---
  {
    int dcx = 620; int dcy = 260; int esc = 18;
    // Osa Mayor / Ursa Major - 7 estrellas del carro
    // Coordenadas relativas del asterismo
    const int pts[7][2] = {{-60,-20},{-35,-15},{-10,-10},{10,-5},{20,20},{45,25},{60,15}};
    // Brillo
    for(int i=0;i<7;i++){
      int x = dcx + pts[i][0];
      int y = dcy + pts[i][1];
      display.fillCircle(x, y, 3, NEGRO);
      display.fillCircle(x, y, 1, BLANCO);
      // destello
      if(i==0 || i==3 || i==6){ display.drawLine(x-5,y,x+5,y,NEGRO); display.drawLine(x,y-5,x,y+5,NEGRO); }
    }
    // Lineas del carro
    for(int i=0;i<6;i++){
      int x1 = dcx + pts[i][0];
      int y1 = dcy + pts[i][1];
      int x2 = dcx + pts[i+1][0];
      int y2 = dcy + pts[i+1][1];
      display.drawLine(x1,y1,x2,y2,NEGRO);
    }
    // Estrellitas de fondo alrededor
    display.fillCircle(dcx-50, dcy-45, 2, NEGRO);
    display.fillCircle(dcx+70, dcy-30, 2, NEGRO);
    display.fillCircle(dcx-30, dcy+40, 1, NEGRO);
    display.fillCircle(dcx+40, dcy+50, 2, NEGRO);
    // Etiqueta pequeña
    txtSimple("UMa", dcx-15, dcy+45, 2, false);
  }

  txtCentrado(A_TOCA[l], 400, 456, 2, false);
  display.update();
}

// ---- Menu: 6 botones en 2 columnas x 3 filas ----
// Posiciones: columna x=20 / x=410 (ancho 370); fila y=58 / 198 / 338 (alto 124)
// Iconos PNG - texto todos igual tam 3 y mas separado
void txtBoton(const char* s,int x,int y,int w,int h){
  int iconW = 90; // un poco menos ancho para dar mas sitio al texto
  int tx = x + iconW;
  int tw = w - iconW - 10;
  int cx = tx + tw/2;
  int maxW = tw - 4;
  int sz = 3; // Todos exactamente igual - grande
  int n=strlen(s), mid=n/2, best=-1;
  for(int i=0;i<n;i++) if(s[i]==' ' && (best<0 || abs(i-mid)<abs(best-mid))) best=i;
  if(best<0){
    txtCentrado(s, cx, y + h/2 - 12, sz, true);
    return;
  }
  char a[40], b[40];
  strlcpy(a,s,(best<39?best:39)+1);
  strlcpy(b,s+best+1,sizeof(b));
  // Forzar tam 3 siempre, aunque sea largo, se parte igual en 2 lineas
  // Si la segunda parte sigue siendo muy larga, intentar partirla tambien
  if(anchoTxt(b,sz) > maxW){
    // Intentar partir b en dos si tiene espacio
    int n2=strlen(b), mid2=n2/2, best2=-1;
    for(int i=0;i<n2;i++) if(b[i]==' ' && (best2<0 || abs(i-mid2)<abs(best2-mid2))) best2=i;
    if(best2>0){
      char b1[40], b2[40];
      strlcpy(b1,b,(best2<39?best2:39)+1);
      strlcpy(b2,b+best2+1,sizeof(b2));
      // 3 lineas centradas
      int y0 = y + h/2 - 32;
      txtCentrado(a, cx, y0, sz, true);
      txtCentrado(b1, cx, y0 + 24, sz, true);
      txtCentrado(b2, cx, y0 + 48, sz, true);
      return;
    }
  }
  int y0 = y + h/2 - 22;
  txtCentrado(a, cx, y0, sz, true);
  txtCentrado(b, cx, y0 + 30, sz, true);
}

void dibujarMenu(){
  if(menuPagina==1){ dibujarMenuPagina2(); return; }
  int l=idioma();
  display.fillScreen(BLANCO);
  txtCentrado(M_TIT[l], 400, 14, 3, true);
  if(iconoLunaVisible) dibujarLunaMenguante(32,18,8);   // lunita de suspension (arriba izquierda)
  dibujarBateria(700, 10, nivelBateria, cargando);
  for(int i=0;i<6;i++){
    int a = MENU_P0[i];   // accion que va en esta casilla
    int x=20+(i%2)*390, y=58+(i/2)*140, w=370, h=124;
    display.drawRect(x,y,w,h,NEGRO); display.drawRect(x+1,y+1,w-2,h-2,NEGRO);
    int icx = x + 12;
    int icy = y + (h - 80)/2;
    // Dibujar icono izquierda grande 80x80
    switch(a){
      case 0: display.drawBitmap(icx, icy, icon_tiempo_80x80, 80, 80, NEGRO); break; // tiempo - naturaleza
      case 1: display.drawBitmap(icx, icy, icon_astro_80x80, 80, 80, NEGRO); break; // astro - noche
      case 2: display.drawBitmap(icx, icy, icon_actualizar_80x80, 80, 80, NEGRO); break; // actualizar datos
      case 3: display.drawBitmap(icx, icy, icon_buscar_80x80, 80, 80, NEGRO); break; // buscar actualizaciones
      case 4: display.drawBitmap(icx, icy, icon_sd_80x80, 80, 80, NEGRO); break; // sd
      case 6: display.drawBitmap(icx, icy, icon_crosspoint_80x80, 80, 80, NEGRO); break; // CrossPoint
    }
    txtBoton((a==6) ? T_CP_ETQ[l] : M_LBL[l][a], x, y, w, h);
  }
  dibujarPuntosMenu(0);
  display.update();
}
// Devuelve la accion: 0 tiempo, 1 astro, 2 actualizar datos, 3 buscar actualizaciones, 4 SD, 5 configuracion, 6 CrossPoint (o -1)
int menuBoton(int tx,int ty){
  int col=-1, fila=-1;
  if(tx>=20 && tx<390) col=0; else if(tx>=410 && tx<780) col=1;
  if(ty>=58 && ty<182) fila=0; else if(ty>=198 && ty<322) fila=1; else if(ty>=338 && ty<462) fila=2;
  if(col<0 || fila<0) return -1;
  int idx = fila*2+col;
  if(menuPagina==1) return (idx==0) ? 5 : -1;   // pagina 2: solo Configuracion
  return (idx>=0 && idx<6) ? MENU_P0[idx] : -1;
}

void redibujar(){
  switch(pantalla){
    case P_TIEMPO: dibujar(); break;
    case P_DET_TIEMPO: dibujarDetalle(detalleActual); break;
    case P_MENU: dibujarMenu(); break;
    case P_ASTRO: dibujarAstroResumen(); break;
    case P_SOL: dibujarSolDetallado(); break;
    case P_LUNA: dibujarLunaDetallada(); break;
    case P_ESTRELLAS: dibujarEstrellas(); break;
  }
}
void pantallaMensaje(const char* l1,const char* l2){ display.fillScreen(BLANCO); engranaje(752,26,17); display.fillTriangle(744,46,760,46,752,54,NEGRO); txtCentrado(l1,400,180,4,true); txtCentrado(l2,400,250,2,false); txtCentrado("Configuracion activa / Configuration mode",400,330,2,false); display.update(); }
void pantallaConfig(){ display.fillScreen(BLANCO); engranaje(70,70,40); txtSimple("Configuracion / Setup",140,40,5,true); display.fillRect(20,118,760,3,NEGRO); txtSimple("1. Conecta a la red / Connect to WiFi:",40,145,3,false); txtSimple(AP_NOMBRE,80,185,4,true); txtSimple("2. Abre el navegador / Open browser:",40,245,3,false); txtSimple("192.168.4.1",80,285,4,true); txtSimple("3. Elige WiFi, Idioma y Ciudad",40,345,3,false); display.update(); }

WebServer servidor(80); DNSServer dns; String tmpSsid, tmpPass; int wifiEstado=0; unsigned long wifiInicio=0; bool terminado=false;
const char PAGINA[] PROGMEM = R"rawliteral(<!DOCTYPE html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'><title>Sticky Tiempo</title><style>body{font-family:sans-serif;max-width:600px;margin:20px auto;padding:0 16px;background:#fafafa}h2{border-bottom:2px solid #333;padding-bottom:8px}button,select,input{padding:10px;margin:6px 0;width:100%;box-sizing:border-box}.sec{background:#fff;border:1px solid #ddd;padding:12px;margin:12px 0;border-radius:8px}.msg{color:#080;margin:8px 0}</style></head><body><h2>Sticky Tiempo + Astro V1.1.0</h2><div class='sec'><h3>1. WiFi</h3><select id='redes'></select><button onclick='buscarRedes()' style='background:#eee'>🔄 Re-escanear redes</button><input id='clave' placeholder='Contrasena WiFi' type='password'><button onclick='conectar()'>Conectar WiFi</button><div class='msg' id='mWifi'></div></div><div class='sec'><h3>2. Idioma / Language</h3><select id='lang'><option value='0'>Espanol</option><option value='1'>English</option><option value='2'>Portugues</option><option value='3'>Francais</option><option value='4'>Italiano</option><option value='5'>Deutsch</option></select><h3>3. Unidades / Units</h3><select id='unidad'><option value='0'>Celsius C - km/h</option><option value='1'>Fahrenheit F - mph</option></select><h3>4. Formato Fecha / Date</h3><select id='fmtFecha'><option value='0'>ES: lunes, 14 de octubre</option><option value='1'>US: October - Wednesday 14</option><option value='2'>ISO: 2026-10-14</option></select><h3>5. Ciudad / City</h3><input id='q' placeholder='Escribe tu ciudad'><button onclick='buscarCiudad()'>Buscar ciudad</button><div id='lista'></div><div class='msg' id='mCiudad'></div><script>let ciudades=[], t=null;const $=id=>document.getElementById(id);async function buscarRedes(){$('redes').innerHTML='<option>Buscando redes...</option>';try{const r=await fetch('/redes');const l=await r.json();const sel=$('redes');sel.innerHTML='';if(!l.length){sel.innerHTML='<option>No se encontraron redes</option>';return;}l.forEach(x=>{const o=document.createElement('option');o.value=x.s;o.textContent=x.s+(x.l?' (con clave)':' (abierta)')+'  '+x.r+' dBm';sel.appendChild(o);});}catch(e){$('redes').innerHTML='<option>Error al buscar</option>';}}async function conectar(){const s=$('redes').value;if(!s)return;$('mWifi').textContent='Conectando...';try{await fetch('/wifi',{method:'POST',body:new URLSearchParams({ssid:s,pass:$('clave').value})});}catch(e){}sondeo();}function sondeo(){clearInterval(t);t=setInterval(async()=>{try{const j=await (await fetch('/estado')).json();if(j.wifi==1){$('mWifi').textContent='Conectando...';}if(j.wifi==2){$('mWifi').textContent='Conectada a '+j.ssid;clearInterval(t);}if(j.wifi==3){$('mWifi').textContent='No se pudo conectar.';clearInterval(t);}}catch(e){}},1500);}async function buscarCiudad(){const q=$('q').value.trim();if(!q)return;$('mCiudad').textContent='Buscando...';$('lista').innerHTML='';try{const ln=$('lang').value;const langs=['es','en','pt','fr','it','de'];const j=await (await fetch('/ciudades?q='+encodeURIComponent(q)+'&l='+langs[ln])).json();if(j.error){$('mCiudad').textContent=j.error;return;}ciudades=j;if(!j.length){$('mCiudad').textContent='Sin resultados';return;}$('mCiudad').textContent='Elige:';j.forEach((c,i)=>{const b=document.createElement('button');b.className='sec';b.textContent=[c.n,c.r,c.p].filter(Boolean).join(', ');b.onclick=()=>elegir(i);$('lista').appendChild(b);});}catch(e){$('mCiudad').textContent='Error';}}async function elegir(i){const c=ciudades[i];const ln=$('lang').value;const uni=$('unidad').value;const fmt=$('fmtFecha').value;try{await fetch('/guardar',{method:'POST',body:new URLSearchParams({n:c.n,la:c.la,lo:c.lo,lang:ln,unidad:uni,fmtFecha:fmt})});$('mCiudad').textContent='Guardado. Reiniciando...';$('lista').innerHTML='';}catch(e){$('mCiudad').textContent='Error al guardar';}}buscarRedes();sondeo();</script></body></html>)rawliteral";
void hRaiz(){ servidor.send(200,"text/html; charset=utf-8", PAGINA); }
void hRedes(){ int n=WiFi.scanNetworks(false,false,false,300); DynamicJsonDocument doc(8192); JsonArray arr=doc.to<JsonArray>(); for(int i=0;i<n && (int)arr.size()<15;i++){ String s=WiFi.SSID(i); if(!s.length()) continue; bool dup=false; for(JsonVariant v:arr){ JsonObject o=v.as<JsonObject>(); if(o["s"]==s){dup=true;break;}} if(dup) continue; JsonObject o=arr.add<JsonObject>(); o["s"]=s; o["r"]=WiFi.RSSI(i); o["l"]=WiFi.encryptionType(i)!=WIFI_AUTH_OPEN; } WiFi.scanDelete(); String out; serializeJson(doc,out); servidor.send(200,"application/json; charset=utf-8", out); }
void hWifi(){ tmpSsid=servidor.arg("ssid"); tmpPass=servidor.arg("pass"); WiFi.begin(tmpSsid.c_str(), tmpPass.c_str()); wifiEstado=1; wifiInicio=millis(); servidor.send(200,"application/json","{\"ok\":true}"); }
void hEstado(){ String s="{\"wifi\":"+String(wifiEstado)+",\"ssid\":\""; s+=(wifiEstado==2?tmpSsid:cfg.ssid); s+="\"}"; servidor.send(200,"application/json; charset=utf-8",s); }
void hCiudades(){ if(WiFi.status()!=WL_CONNECTED){ servidor.send(200,"application/json; charset=utf-8","{\"error\":\"Conecta primero la WiFi\"}"); return; } String apiLang = servidor.arg("l"); if(apiLang.length() == 0) apiLang = "es"; String url="https://geocoding-api.open-meteo.com/v1/search?name="+urlEnc(servidor.arg("q"))+"&count=20&language="+apiLang+"&format=json"; WiFiClientSecure cl; cl.setInsecure(); HTTPClient http; http.begin(cl,url); int cod=http.GET(); if(cod!=200){http.end(); servidor.send(200,"application/json; charset=utf-8","{\"error\":\"No se pudo buscar.\"}"); return;} DynamicJsonDocument in(4096); deserializeJson(in,http.getString()); http.end(); DynamicJsonDocument out(4096); JsonArray arr=out.to<JsonArray>(); if(in.containsKey("results")) { JsonArray _results = in["results"].as<JsonArray>(); for(JsonObject r: _results){ JsonObject o=arr.add<JsonObject>(); o["n"]=r["name"]|""; o["r"]=r["admin1"]|""; o["p"]=r["country"]|""; o["la"]=r["latitude"]; o["lo"]=r["longitude"]; } } String s; serializeJson(out,s); servidor.send(200,"application/json; charset=utf-8",s); }
void hGuardar(){ String n=servidor.arg("n"); float la=servidor.arg("la").toFloat(); float lo=servidor.arg("lo").toFloat(); int ln=servidor.arg("lang").toInt(); int uni=servidor.arg("unidad").toInt(); if(servidor.hasArg("uni")) uni=servidor.arg("uni").toInt(); int fmt=servidor.arg("fmtFecha").toInt(); if(servidor.hasArg("fmt")) fmt=servidor.arg("fmt").toInt(); if(n.length()==0){servidor.send(400,"text/plain","falta ciudad"); return;} guardarCiudad(n,la,lo,ln,uni,fmt); servidor.send(200,"application/json","{\"ok\":true}"); terminado=true; }
void hOtros(){ servidor.sendHeader("Location","http://192.168.4.1/"); servidor.send(302,"text/plain",""); }
void procesarWifi(){ if(wifiEstado!=1) return; if(WiFi.status()==WL_CONNECTED){wifiEstado=2; guardarWifi(tmpSsid,tmpPass);} else if(millis()-wifiInicio>20000){wifiEstado=3; WiFi.disconnect(false);} }
void modoConfiguracion(){
  pantallaConfig(); WiFi.mode(WIFI_AP_STA); WiFi.softAP(AP_NOMBRE); delay(500); dns.start(53,"*",WiFi.softAPIP()); servidor.on("/",hRaiz); servidor.on("/redes",hRedes); servidor.on("/wifi",HTTP_POST,hWifi); servidor.on("/estado",hEstado); servidor.on("/ciudades",hCiudades); servidor.on("/guardar",HTTP_POST,hGuardar); servidor.onNotFound(hOtros); servidor.begin();
  if(cfg.ssid.length()){ tmpSsid=cfg.ssid; tmpPass=cfg.pass; WiFi.begin(tmpSsid.c_str(), tmpPass.c_str()); wifiEstado=1; wifiInicio=millis(); }
  unsigned long t0=millis(); while(!terminado && millis()-t0<TIEMPO_PORTAL_MS){ dns.processNextRequest(); servidor.handleClient(); procesarWifi(); delay(5); }
  servidor.stop(); dns.stop(); WiFi.softAPdisconnect(true); if(terminado){delay(2000); ESP.restart();}
}
void apagarTodoAntesDormir(){
  // Apagado real para bajo consumo
  Serial.println("Apagando perifericos...");
  // EPD OFF
  pinMode(PIN_EPD_PWR, OUTPUT); digitalWrite(PIN_EPD_PWR, LOW);
  // TOUCH OFF
  pinMode(PIN_TOUCH_PWR, OUTPUT); digitalWrite(PIN_TOUCH_PWR, LOW);
  pinMode(PIN_TOUCH_RST, OUTPUT); digitalWrite(PIN_TOUCH_RST, LOW);
  pinMode(PIN_TOUCH_INT, INPUT_PULLDOWN);
  // I2C OFF
  Wire.end();
  sensorWire.end();
  // SDA/SCL en flotante para no chupar
  pinMode(PIN_TOUCH_SDA, INPUT); pinMode(PIN_TOUCH_SCL, INPUT);
  pinMode(PIN_SDA, INPUT); pinMode(PIN_SCL, INPUT);
  // WiFi OFF total
  WiFi.disconnect(true, true);
  WiFi.mode(WIFI_OFF);
  esp_wifi_stop();
  delay(100);
}

uint64_t calcularSegHastaManana(){
  time_t ahora=time(nullptr);
  if(ahora<1700000000) return INTERVALO_MIN*60ULL;
  struct tm ti; localtime_r(&ahora,&ti);
  int h=ti.tm_hour;
  if(h>=HORA_NOCHE_INI || h<HORA_NOCHE_FIN){
    int segHoy = ti.tm_hour*3600 + ti.tm_min*60 + ti.tm_sec;
    int seg7 = 7*3600;
    uint64_t seg;
    if(h>=HORA_NOCHE_INI) seg = (24*3600 - segHoy) + seg7;
    else seg = seg7 - segHoy;
    if(seg<60) seg=60;
    return seg;
  }
  return INTERVALO_MIN*60ULL;
}

void dormirSeg(uint64_t seg){
  if(seg<30) seg=30;
  uint64_t segReal = seg;
  time_t ahora=time(nullptr);
  if(ahora>1700000000){
    struct tm ti; localtime_r(&ahora,&ti);
    if(ti.tm_hour>=HORA_NOCHE_INI || ti.tm_hour<HORA_NOCHE_FIN){
      segReal = calcularSegHastaManana();
      Serial.printf("Noche (%dh) durmiendo hasta las 7h: %llu s\n", ti.tm_hour, segReal);
    }
    proximaAct = ahora+(time_t)segReal;
  } else {
    proximaAct=0;
  }
  Serial.printf("Durmiendo %llu s - wake ARRIBA (GPIO%d) largo\n", segReal, PIN_BTN_UP);
  Serial.flush();
  apagarTodoAntesDormir();
  { unsigned long tw=millis(); while((digitalRead(PIN_BTN_UP)==LOW || digitalRead(PIN_BTN_DOWN)==LOW || digitalRead(PIN_BTN_AI)==LOW) && millis()-tw<5000) delay(20); }
  delay(150);
  // Wake SOLO con ARRIBA (ahora GPIO4) - con hold
  pinMode(PIN_BTN_UP, INPUT_PULLUP);
  rtc_gpio_init((gpio_num_t)PIN_BTN_UP);
  rtc_gpio_set_direction((gpio_num_t)PIN_BTN_UP, RTC_GPIO_MODE_INPUT_ONLY);
  rtc_gpio_pullup_en((gpio_num_t)PIN_BTN_UP);
  rtc_gpio_pulldown_dis((gpio_num_t)PIN_BTN_UP);
  rtc_gpio_hold_dis((gpio_num_t)PIN_BTN_UP);
  rtc_gpio_hold_en((gpio_num_t)PIN_BTN_UP);
  uint64_t mascara = (1ULL<<PIN_BTN_UP);
  uint64_t status = esp_sleep_get_ext1_wakeup_status();
  Serial.printf("Mascara wake: 0x%llx status previo: 0x%llx\n", mascara, status);
  esp_sleep_enable_ext1_wakeup_io(mascara, ESP_EXT1_WAKEUP_ALL_LOW);
  esp_sleep_enable_timer_wakeup(segReal*1000000ULL);
  esp_deep_sleep_start();
}
void dormir(uint64_t minutos){ dormirSeg(minutos*60ULL); }
void esperarSoltar(int pin){ unsigned long t=millis(); while(digitalRead(pin)==LOW && millis()-t<5000) delay(20); delay(60); }

// ================= SD - Explorador de firmwares =================
// ---- Flujo "Actualizar desde SD" por FASES con reinicio ----
// La EPD y la SD comparten SCK/MOSI y Seeed_GFX no se recupera si se le quita el bus
// a mitad de ejecucion. Solucion: nunca usar pantalla y SD en el mismo arranque
// despues de tocar la SD. La EPD conserva la ultima imagen aunque reiniciemos.
//   fase 1: (arranque) leer SD -> guardar lista en NVS -> reiniciar
//   fase 2: (arranque normal) dibujar explorador desde NVS; si se elige .bin -> fase 3
//   fase 3: (arranque) montar SD e instalar el .bin sin tocar la pantalla -> reiniciar
//   fase 4: (arranque normal) mostrar el error de la fase 3
int sdFaseLeer(){ Preferences pp; pp.begin("sdmode", true); int f = pp.getInt("fase", 0); pp.end(); return f; }
void sdFaseGuardar(int f){ Preferences pp; pp.begin("sdmode", false); pp.putInt("fase", f); pp.end(); }

// Sonda: intenta leer el sector 0 en crudo y dice si la tarjeta responde en este punto
static bool probeSD(const char* tag){
  static uint8_t pb[512];
  bool ok = sdFat.card()->readSector(0, pb);
  Serial.printf("PROBE %s: readSector(0) %s  err=0x%x data=0x%x\n", tag, ok ? "OK" : "FALLO", sdFat.sdErrorCode(), sdFat.sdErrorData());
  Serial.flush();
  return ok;
}

bool iniciarSD(){
  if(sdIniciada) return true;
  Serial.println("Iniciando SD (FAT32/exFAT)...");
  pinMode(15, OUTPUT); digitalWrite(15, HIGH);
  pinMode(PIN_SD_CS, OUTPUT); digitalWrite(PIN_SD_CS, HIGH);
  pinMode(PIN_SD_MISO, INPUT_PULLUP);
  SPI.end(); delay(50);
  SPI.begin(PIN_SD_SCK, PIN_SD_MISO, PIN_SD_MOSI, PIN_SD_CS);
  delay(50);

  // 1) Intenta SdFat (soporta FAT32 y exFAT 64GB) - como CrossPoint
  Serial.println("Try SdFat exFAT/FAT32...");
  SdSpiConfig cfg(PIN_SD_CS, DEDICATED_SPI, SD_SCK_MHZ(4), &SPI);
  if(sdFat.begin(cfg)){
    sdIniciada = true;
    usandoSdFat = true;
    Serial.println("SdFat OK - FAT32/exFAT OK");
    probeSD("1 tras-begin");
    // info
    uint64_t total = (uint64_t)sdFat.clusterCount()*sdFat.bytesPerCluster();
    Serial.printf("Card size: %llu MB\n", total/(1024*1024));
    probeSD("2 tras-size");
    // OJO: NO tocar pinMode()/digitalWrite() de CS (ni otros pines del bus) con la SD montada:
    // el pinMode() sobre un pin ya asignado a SPI hace que el core suelte el bus y SdFat deja de leer.
    // El flujo por fases (sdFase1/sdFase3Instalar) reinicia el ESP antes de volver a usar la pantalla.
    probeSD("3 final-iniciarSD");
    return true;
  }
  Serial.printf("SdFat fail: 0x%x 0x%x\n", sdFat.sdErrorCode(), sdFat.sdErrorData());

  // 2) Fallback libreria SD oficial (solo FAT32)
  SPI.end(); delay(50);
  SPI.begin(PIN_SD_SCK, PIN_SD_MISO, PIN_SD_MOSI, PIN_SD_CS);
  delay(50);
  Serial.println("Try SD.h FAT32...");
  if(SD.begin(PIN_SD_CS, SPI, 1000000)){
    sdIniciada = true;
    usandoSdFat = false;
    Serial.println("SD.h OK FAT32");
    return true;
  }
  SPI.end();
  sdIniciada = false;
  usandoSdFat = false;
  Serial.println("SD FAIL - no card");
  return false;
}

static void hexDumpSD(const uint8_t* b, int n){
  for(int i=0;i<n;i+=16){
    Serial.printf("  %03x: ", i);
    for(int j=0;j<16 && i+j<n;j++) Serial.printf("%02x ", b[i+j]);
    Serial.println();
  }
  Serial.flush();
}

// Diagnostico: lee sectores en crudo y decodifica el directorio raiz exFAT
void diagExfatRoot(){
  uint8_t* b=(uint8_t*)malloc(512);
  if(!b){ Serial.println("DIAG: malloc fail"); return; }
  auto rd32=[&](int o)->uint32_t{ return (uint32_t)b[o]|((uint32_t)b[o+1]<<8)|((uint32_t)b[o+2]<<16)|((uint32_t)b[o+3]<<24); };

  Serial.println("DIAG: leyendo sector 0 de la tarjeta"); Serial.flush();
  if(!sdFat.card()->readSector(0, b)){
    Serial.printf("DIAG: readSector(0) FALLO err=0x%x data=0x%x\n", sdFat.sdErrorCode(), sdFat.sdErrorData()); Serial.flush();
    free(b); return;
  }
  hexDumpSD(b, 64);
  Serial.printf("DIAG: bytes 510/511 = %02x %02x (esperado 55 aa) | tipo particion1=0x%02x\n", b[510], b[511], b[450]); Serial.flush();

  uint32_t part = 0;
  if(b[3]=='E' && b[4]=='X' && b[5]=='F'){ part = 0; Serial.println("DIAG: sin MBR, el sector 0 ya es boot exFAT"); }
  else { part = rd32(446+8); Serial.printf("DIAG: particion1 empieza en sector %u, tamano %u sectores\n", (unsigned)part, (unsigned)rd32(446+12)); }
  Serial.flush();

  if(!sdFat.card()->readSector(part, b)){
    Serial.printf("DIAG: readSector(boot=%u) FALLO err=0x%x\n", (unsigned)part, sdFat.sdErrorCode()); Serial.flush();
    free(b); return;
  }
  char sig[9]; memcpy(sig, b+3, 8); sig[8]=0;
  Serial.printf("DIAG: boot sector firma='%s'\n", sig);
  uint32_t fatOff   = rd32(80);
  uint32_t fatLen   = rd32(84);
  uint32_t heapOff  = rd32(88);
  uint32_t nClust   = rd32(92);
  uint32_t rootClu  = rd32(96);
  uint8_t  bpsShift = b[108];
  uint8_t  spcShift = b[109];
  Serial.printf("DIAG: fatOff=%u fatLen=%u heapOff=%u clusters=%u rootCluster=%u bpsShift=%u spcShift=%u\n",
                (unsigned)fatOff,(unsigned)fatLen,(unsigned)heapOff,(unsigned)nClust,(unsigned)rootClu,(unsigned)bpsShift,(unsigned)spcShift);
  Serial.flush();

  uint64_t rootSector = (uint64_t)part + heapOff + ((uint64_t)(rootClu-2) << spcShift);
  Serial.printf("DIAG: primer sector del directorio raiz = %u\n", (unsigned)rootSector); Serial.flush();
  if(!sdFat.card()->readSector((uint32_t)rootSector, b)){
    Serial.printf("DIAG: readSector(raiz) FALLO err=0x%x\n", sdFat.sdErrorCode()); Serial.flush();
    free(b); return;
  }
  Serial.println("DIAG: primeros 128 bytes del directorio raiz:"); hexDumpSD(b, 128);
  Serial.print("DIAG: tipos de entrada (1er byte de cada 32): ");
  for(int i=0;i<512;i+=32) Serial.printf("%02x ", b[i]);
  Serial.println(); Serial.flush();
  free(b);
}

std::vector<String> listarBinsSD(){
  std::vector<String> lista;
  if(!iniciarSD()) return lista;
  auto esBin = [](String n){ n.toLowerCase(); return n.endsWith(".bin"); };
  Serial.println("listarBinsSD start");
  Serial.flush();

  if(usandoSdFat){
    Serial.printf("LST: fatType=%d (64=exFAT, 32=FAT32)\n", (int)sdFat.fatType()); Serial.flush();
    probeSD("4 pre-ls");
    Serial.println("LST: contenido de la raiz segun SdFat.ls():"); Serial.flush();
    sdFat.ls(&Serial, "/", LS_SIZE);
    Serial.println("LST: fin ls()"); Serial.flush();
    FsFile root = sdFat.open("/", O_RDONLY);
    Serial.printf("LST: root.isOpen=%d isDir=%d\n", (int)root.isOpen(), (int)root.isDir()); Serial.flush();
    if(root.isOpen()){
      FsFile f;
      int cnt=0;
      while(f.openNext(&root, O_RDONLY)){
        cnt++;
        if(cnt>100){ Serial.println("limit 100 files"); f.close(); break; }
        if(!f.isDir()){
          char name[128];
          if(f.getName(name, sizeof(name))){
            String sName = String(name);
            // ignora archivos del sistema
            if(sName.startsWith("System Volume") || sName.startsWith(".")) { f.close(); continue; }
            Serial.printf("File %d: %s\n", cnt, name);
            if(esBin(sName)){
              String path = "/";
              path += sName;
              lista.push_back(path);
              Serial.printf(" -> BIN added\n");
            }
          }
        }
        f.close();
        yield();
        delay(5);
      }
      if(cnt==0){
        // Plan B: si openNext no devuelve nada, rebobinar y usar openNextFile()
        Serial.println("LST: openNext dio 0 entradas, probando rewind + openNextFile"); Serial.flush();
        root.rewind();
        FsFile g = root.openNextFile();
        while(g){
          cnt++;
          char nm[128]; nm[0]=0;
          g.getName(nm, sizeof(nm));
          bool esDir = g.isDir();
          Serial.printf("LST2: %s %s\n", esDir ? "[DIR] " : "[FILE]", nm); Serial.flush();
          if(!esDir){
            String sName = String(nm);
            if(!(sName.startsWith("System Volume") || sName.startsWith(".")) && esBin(sName)){
              lista.push_back(String("/") + sName);
              Serial.println("LST2:  -> BIN added"); Serial.flush();
            }
          }
          g.close();
          if(cnt>100) break;
          g = root.openNextFile();
        }
        Serial.printf("LST: tras plan B cnt=%d sdErrorCode=0x%x data=0x%x\n", cnt, sdFat.sdErrorCode(), sdFat.sdErrorData()); Serial.flush();
        if(cnt==0) diagExfatRoot();
      }
      root.close();
      Serial.printf("Root done %d files %d bins\n", cnt, (int)lista.size()); Serial.flush();
    } else {
      Serial.println("root open FAIL");
    }
  } else {
    File root = SD.open("/");
    if(root){
      File f = root.openNextFile();
      while(f){
        if(!f.isDirectory()){
          String name = String(f.name());
          if(name.startsWith("/")) name = name.substring(1);
          if(esBin(name)){
            lista.push_back("/"+name);
          }
        }
        f = root.openNextFile();
      }
      root.close();
    }
  }
  Serial.println("LST: antes de sort"); Serial.flush();
  std::sort(lista.begin(), lista.end());
  Serial.println("LST: despues de sort"); Serial.flush();
  Serial.printf("listarBinsSD end %d bins\n", (int)lista.size());
  Serial.flush();
  return lista;
}

// FASE 1: se ejecuta al arrancar (tras display.begin) SIN dibujar nada. No vuelve.
void sdFase1(){
  Serial.println("SDF1: leyendo SD sin tocar la pantalla"); Serial.flush();
  setCpuFrequencyMhz(240);
  String lista="", tamS="", msg="";
  sdIniciada = false;
  if(!iniciarSD()){
    msg = "nosd";
  } else {
    std::vector<String> bins = listarBinsSD();
    if(bins.empty()) msg = "nobin";
    for(size_t i=0; i<bins.size() && i<30; i++){
      uint32_t fsz=0;
      if(usandoSdFat){
        FsFile tf = sdFat.open(bins[i].c_str(), O_RDONLY);
        if(tf.isOpen()){ fsz=(uint32_t)tf.fileSize(); tf.close(); }
      } else {
        File tf = SD.open(bins[i], FILE_READ);
        if(tf){ fsz=(uint32_t)tf.size(); tf.close(); }
      }
      lista += bins[i]; lista += "\n";
      tamS  += String(fsz); tamS += "\n";
    }
  }
  sdFat.end();
  Preferences pp; pp.begin("sdmode", false);
  pp.putInt("fase", 2); pp.putString("lista", lista); pp.putString("tam", tamS); pp.putString("msg", msg);
  pp.end();
  Serial.printf("SDF1: guardado (msg='%s'), reiniciando a fase 2\n", msg.c_str()); Serial.flush();
  delay(300);
  ESP.restart();
}

// FASE 3: se ejecuta al arrancar SIN usar la pantalla (conserva la imagen "Instalando").
// Si todo va bien reinicia con el firmware nuevo; si falla guarda el error y reinicia (fase 4).
void sdFase3Instalar(){
  Preferences pp; pp.begin("sdmode", true);
  String path = pp.getString("ruta", "");
  pp.end();
  Serial.printf("SDF3: instalar '%s'\n", path.c_str()); Serial.flush();
  setCpuFrequencyMhz(240);
  String errTxt = "";

  do {
    if(path.length() < 2){ errTxt = "Sin ruta"; break; }
    sdIniciada = false;
    if(!iniciarSD()){ errTxt = "No monta la SD"; break; }

    File f; FsFile ff; size_t sz = 0; bool useFat = usandoSdFat;
    if(useFat){
      ff = sdFat.open(path.c_str(), O_RDONLY);
      if(!ff.isOpen()){ String p2=path; if(p2.startsWith("/")) p2=p2.substring(1); ff = sdFat.open(p2.c_str(), O_RDONLY); }
      if(!ff.isOpen()){ errTxt = "No abre el archivo"; break; }
      sz = ff.fileSize();
    } else {
      f = SD.open(path, FILE_READ);
      if(!f){ String p2=path; if(p2.startsWith("/")) p2=p2.substring(1); f = SD.open(p2, FILE_READ); }
      if(!f){ errTxt = "No abre el archivo"; break; }
      sz = f.size();
    }
    if(sz < 100000){ if(f) f.close(); if(ff.isOpen()) ff.close(); errTxt = "Archivo muy pequeno"; break; }
    Serial.printf("SDF3: %u bytes\n", (unsigned)sz); Serial.flush();

    // Fix para bins grandes + OTA dual: usa UNKNOWN y comprueba particiones
    if(!Update.begin(UPDATE_SIZE_UNKNOWN)){
      if(f) f.close(); if(ff.isOpen()) ff.close();
      int e = Update.getError();
      if(e==5) errTxt = "Particion 3MB pequena! Usa 2x7MB OTA";
      else if(e==10) errTxt = "NO_PARTITION: falta ota_1! Usa partitions 2x7MB OTA";
      else errTxt = String("Update.begin err ") + String(e);
      break;
    }
    uint8_t* buf = (uint8_t*)malloc(OTA_BUFFER_BYTES);
    if(!buf){ Update.abort(); if(f) f.close(); if(ff.isOpen()) ff.close(); errTxt = "Sin memoria"; break; }

    size_t written = 0; int lastPct = -1;
    while(true){
      size_t toRead = 0;
      if(useFat){ if(!ff.isOpen() || ff.available()==0) break; toRead = ff.read(buf, OTA_BUFFER_BYTES); }
      else      { if(!f || !f.available()) break;               toRead = f.read(buf, OTA_BUFFER_BYTES);  }
      if(toRead == 0) break;
      size_t w = Update.write(buf, toRead);
      if(w != toRead){ Serial.println("SDF3: Update.write fallo"); break; }
      written += w;
      int pct = (int)(written*100 / sz);
      if(pct != lastPct && pct % 10 == 0){
        Serial.printf("SDF3 progreso %d%%  %u/%u KB\n", pct, (unsigned)(written/1024), (unsigned)(sz/1024)); Serial.flush();
        lastPct = pct;
      }
      yield();
    }
    free(buf);
    if(f) f.close();
    if(ff.isOpen()) ff.close();
    Serial.printf("SDF3: copia terminada written=%u sz=%u\n", (unsigned)written, (unsigned)sz); Serial.flush();

    if(written == sz && Update.end(true)){
      Serial.println("SDF3: OK, reiniciando con el firmware nuevo"); Serial.flush();
      delay(500);
      ESP.restart();
    }
    if(written != sz) Update.abort();
        if(Update.getError()==9){
      errTxt = "BIN factory, usa USB. Solo OTA vale por SD";
    } else {
      errTxt = String("Fallo Update err ") + String((int)Update.getError());
    }
  } while(false);

  Serial.printf("SDF3: FALLO '%s'\n", errTxt.c_str()); Serial.flush();
  sdFat.end();
  Preferences p2; p2.begin("sdmode", false); p2.putInt("fase", 4); p2.putString("msg", errTxt); p2.end();
  delay(300);
  ESP.restart();
}

// Pinta la pantalla "Instalando" (con el tiempo estimado) y reinicia a la fase 3 (la imagen se conserva).
// Mientras se copia el archivo la pantalla no puede actualizarse (comparte bus con la SD), por eso se avisa del tiempo.
void sdIniciarInstalacion(const String& path, uint32_t sizeBytes){
  // Estimacion: ~8 s de arranque y verificacion + copia a unos 160 KB/s; se redondea hacia arriba a multiplos de 5 s
  int seg = 8 + (int)(sizeBytes / (160UL * 1024UL));
  seg = ((seg + 4) / 5) * 5;
  char estim[64]; snprintf(estim, sizeof(estim), "Tardara unos %d s / About %d s", seg, seg);
  display.fillScreen(BLANCO);
  dibujarLogoEspera(10);
  char tit[64]; snprintf(tit, sizeof(tit), "%s", path.c_str());
  if(strlen(tit) > 28){ String s2=String(tit); s2=s2.substring(s2.length()-28); snprintf(tit, sizeof(tit), "%s", s2.c_str()); }
  txtCentrado("Instalando desde SD", 400, 180, 3, true);
  txtCentrado(tit, 400, 220, 2, false);
  txtCentrado("No apagues / Do not power off", 400, 265, 2, false);
  txtCentrado(estim, 400, 310, 2, true);
  txtCentrado("La pantalla no cambiara hasta terminar", 400, 350, 2, false);
  txtCentrado("Se reiniciara solo / Will reboot", 400, 390, 2, false);
  display.update();
  Serial.printf("SDI: pantalla Instalando pintada (estimado %d s), reiniciando a fase 3\n", seg); Serial.flush();
  Preferences pp; pp.begin("sdmode", false); pp.putInt("fase", 3); pp.putString("ruta", path); pp.end();
  delay(500);
  ESP.restart();
}

// Explorador de firmwares en SD con seleccion tactil
void explorarSDFase2(){
  int l=idioma(); (void)l;
  sdFaseGuardar(0);   // evita bucles de arranque si algo falla
  Preferences pp; pp.begin("sdmode", true);
  String lista = pp.getString("lista", "");
  String tamS  = pp.getString("tam", "");
  String msg   = pp.getString("msg", "");
  pp.end();
  std::vector<String> bins; std::vector<uint32_t> tam;
  { int i0=0; while(true){ int i1=lista.indexOf('\n', i0); if(i1<0) break; bins.push_back(lista.substring(i0, i1)); i0=i1+1; } }
  { int i0=0; while(true){ int i1=tamS.indexOf('\n', i0);  if(i1<0) break; tam.push_back((uint32_t)tamS.substring(i0, i1).toInt()); i0=i1+1; } }
  while(tam.size() < bins.size()) tam.push_back(0);
  Serial.printf("SDF2: %d bins cargados, msg='%s'\n", (int)bins.size(), msg.c_str()); Serial.flush();
  if(msg == "nosd"){
    pantallaAviso("No hay SD / No SD card","Inserta tarjeta");
    delay(2500);
    return;
  }

  if(bins.empty()){
    pantallaAviso("No hay .bin en SD","No .bin on SD");
    delay(2500);
    setCpuFrequencyMhz(80);
    return;
  }

  int pagina=0;
  const int porPagina=5;
  int totalPag = (bins.size()+porPagina-1)/porPagina;
  int sel=-1;
  while(true){
    display.fillScreen(BLANCO);
    txtSimple("Firmwares SD / SD Firmwares", 20, 10, 3, true);
    char pg[32]; snprintf(pg,sizeof(pg),"%d/%d", pagina+1, totalPag);
    txtDerecha(pg, 780, 12, 2, false);
    // lista
    int y0=50;
    for(int i=0;i<porPagina;i++){
      int idx = pagina*porPagina + i;
      if(idx >= (int)bins.size()) break;
      int y = y0 + i*72;
      int x=20, w=760, h=62;
      // caja
      display.drawRect(x,y,w,h,NEGRO);
      display.drawRect(x+1,y+1,w-2,h-2,NEGRO);
      // nombre corto
      String full = bins[idx];
      String nombre = full;
      int slash = nombre.lastIndexOf('/');
      if(slash>=0) nombre = nombre.substring(slash+1);
      // recorta si muy largo
      while(anchoTxt(nombre.c_str(),2) > 700 && nombre.length()>10){
        nombre = nombre.substring(0, nombre.length()-1);
      }
      txtSimple(nombre.c_str(), 30, y+8, 2, true);
      // tamano
      size_t fsz = tam[idx];
      if(fsz>0){
        char tb[64]; snprintf(tb,sizeof(tb),"%u KB  %s", (unsigned)(fsz/1024), full.c_str());
        txtSimple(tb, 30, y+34, 2, false);
      }
    }
    // botones navegacion
    if(totalPag>1){
      if(pagina>0){ display.fillRect(20, y0+porPagina*72+10, 180, 40, NEGRO); txtCentrado("< ANTERIOR",110, y0+porPagina*72+18, 2, true, BLANCO); }
      if(pagina<totalPag-1){ display.fillRect(600, y0+porPagina*72+10, 180, 40, NEGRO); txtCentrado("SIGUIENTE >",690, y0+porPagina*72+18, 2, true, BLANCO); }
      display.drawRect(220, y0+porPagina*72+10, 360, 40, NEGRO); txtCentrado("CANCELAR / CANCEL",400, y0+porPagina*72+18, 2, true, NEGRO);
    } else {
      display.drawRect(220, y0+porPagina*72+10, 360, 40, NEGRO); txtCentrado("CANCELAR / CANCEL",400, y0+porPagina*72+18, 2, true, NEGRO);
    }
    Serial.printf("DBG 13: dibujando lista pagina %d\n", pagina+1); Serial.flush();
    display.update();
    Serial.println("DBG 14: lista pintada, esperando toque"); Serial.flush();
    // espera toque
    vaciarToques();
    unsigned long t0=millis();
    bool salir=false;
    while(millis()-t0 < 30000){
      int tx,ty; int g=leerGesto(tx,ty);
      if(g==1){
        // cancelar
        if(ty >= y0+porPagina*72+10 && ty <= y0+porPagina*72+50){
          if(tx>=220 && tx<=580){ sel=-1; salir=true; break; }
          if(totalPag>1){
            if(tx>=20 && tx<=200 && pagina>0){ pagina--; break; }
            if(tx>=600 && tx<=780 && pagina<totalPag-1){ pagina++; break; }
          }
        }
        // seleccion de fila
        for(int i=0;i<porPagina;i++){
          int idx = pagina*porPagina + i;
          if(idx >= (int)bins.size()) break;
          int y = y0 + i*72;
          if(ty>=y && ty<=y+62 && tx>=20 && tx<=780){
            sel=idx;
            salir=true;
            break;
          }
        }
        if(salir) break;
      } else if(g==2){ // swipe izq -> siguiente
        if(pagina<totalPag-1){ pagina++; break; }
      } else if(g==3){ // swipe der -> anterior
        if(pagina>0){ pagina--; break; }
      }
      delay(30);
    }
    if(sel>=0) break;
    if(salir && sel==-1){ // cancelado
      setCpuFrequencyMhz(80);
      return;
    }
    // si timeout, sigue
  }

  // Confirmacion
  String elegido = bins[sel];
  String nombreCorto = elegido; int sl = nombreCorto.lastIndexOf('/'); if(sl>=0) nombreCorto = nombreCorto.substring(sl+1);
  display.fillScreen(BLANCO);
  char b1[64]; snprintf(b1,sizeof(b1),"%s ?", nombreCorto.c_str());
  txtCentrado("Instalar firmware?",400,100,3,true);
  txtCentrado(b1,400,150,2,true);
  txtCentrado(elegido.c_str(),400,185,2,false);
  dibujarAvisoSustituye(235);
  display.fillRect(40,300,350,100,NEGRO);
  display.drawRect(410,300,350,100,NEGRO); display.drawRect(411,301,348,98,NEGRO);
  txtCentrado("INSTALAR",215,338,3,true,BLANCO);
  txtCentrado("CANCELAR",585,338,3,true,NEGRO);
  txtCentrado("Se reiniciara solo / Will reboot",400,430,2,false);
  display.update();
  vaciarToques();
  unsigned long t0=millis(); int decision=0;
  while(millis()-t0<30000 && !decision){
    int tx,ty; int g=leerGesto(tx,ty);
    if(g==1 && ty>=290 && ty<=410){ if(tx>=40 && tx<390) decision=1; else if(tx>=410 && tx<=760) decision=2; }
    delay(30);
  }
  vaciarToques();
  Serial.printf("DBG 15: decision=%d archivo=%s\n", decision, elegido.c_str()); Serial.flush();
  if(decision==1){
    sdIniciarInstalacion(elegido, tam[sel]);   // reinicia: no vuelve
  }
  setCpuFrequencyMhz(80);
}

// Abre la aplicacion del otro hueco OTA (p.ej. CrossPoint): la marca como arranque y reinicia
void accionAbrirOtraApp(){
  int l=idioma();
  const esp_partition_t* p = esp_ota_get_next_update_partition(NULL);
  esp_app_desc_t d;
  Serial.printf("OTRA APP: otro hueco = %s @0x%x\n", p ? p->label : "NULL", p ? (unsigned)p->address : 0u); Serial.flush();
  if(!p || !leerDescApp(p, &d)){
    Serial.println("OTRA APP: el otro hueco no tiene una imagen valida"); Serial.flush();
    pantallaAviso(T_CP_NOAPP[l], T_CP_INSTALA[l]);
    delay(3000);
    return;
  }
  const esp_app_desc_t* yo = esp_ota_get_app_description();
  if(yo && strcmp(d.project_name, yo->project_name)==0 && strcmp(d.version, yo->version)==0 &&
     strcmp(d.date, yo->date)==0 && strcmp(d.time, yo->time)==0){
    Serial.println("OTRA APP: el otro hueco contiene este mismo programa"); Serial.flush();
    pantallaAviso(T_CP_MISMA[l], "");
    delay(3000);
    return;
  }
  char sub[56]; snprintf(sub,sizeof(sub),"%.11s %.5s", d.date, d.time);
  Serial.printf("OTRA APP: abriendo '%s' v%s en %s\n", d.project_name, d.version, p->label); Serial.flush();
  pantallaAviso(T_CP_ABRIENDO[l], sub);
  esp_err_t e = esp_ota_set_boot_partition(p);
  if(e != ESP_OK){
    Serial.printf("OTRA APP: esp_ota_set_boot_partition error %d (%s)\n", (int)e, esp_err_to_name(e)); Serial.flush();
    pantallaAviso("Error", esp_err_to_name(e));
    delay(3000);
    return;
  }
  delay(800);
  ESP.restart();
}

// Boton "Actualizar desde SD" del menu: marca la fase 1 y reinicia
void accionActualizarDesdeSD(){
  Serial.println("accionActualizarDesdeSD -> fase 1 (reinicio)"); Serial.flush();
  pantallaAviso("Conectando SD... / Connecting SD...","SD...");
  sdFaseGuardar(1);
  delay(300);
  ESP.restart();
}

// FASE 4: mostrar el error que dejo la fase 3
void sdMostrarErrorFase4(){
  Preferences pp; pp.begin("sdmode", false);
  String msg = pp.getString("msg", "?");
  pp.putInt("fase", 0);
  pp.end();
  display.fillScreen(BLANCO);
  txtCentrado("Fallo SD Update", 400, 200, 4, true);
  txtCentrado(msg.c_str(), 400, 260, 2, false);
  display.update();
  delay(4000);
}

// ================= ACCIONES DEL MENU =================
static const char* const T_CONECTANDO
[6]  = {"Conectando WiFi...","Connecting WiFi...","Conectando WiFi...","Connexion WiFi...","Connessione WiFi...","Verbinde WLAN..."};
static const char* const T_DESCARGANDO[6] = {"Descargando datos...","Downloading data...","Baixando dados...","Telechargement...","Scaricando dati...","Lade Daten..."};
static const char* const T_ITEMS[6]       = {"Tiempo + Astro + ISS","Weather + Astro + ISS","Tempo + Astro + ISS","Meteo + Astro + ISS","Meteo + Astro + ISS","Wetter + Astro + ISS"};
static const char* const T_ACTUALIZANDO[6]= {"Actualizando datos...","Updating data...","Atualizando dados...","Mise a jour des donnees...","Aggiornamento dati...","Daten werden aktualisiert..."};
static const char* const T_ACTUALIZADO[6] = {"Actualizado!","Updated!","Atualizado!","Mis a jour!","Aggiornato!","Aktualisiert!"};
static const char* const T_ERROR[6]       = {"Error WiFi/datos","WiFi/data error","Erro WiFi/dados","Erreur WiFi/donnees","Errore WiFi/dati","WLAN/Datenfehler"};
static const char* const T_ERRFALLO[6]    = {"Error en:","Error in:","Erro em:","Erreur:","Errore in:","Fehler bei:"};
static const char* const T_SINWIFI[6]     = {"Sin WiFi","No WiFi","Sem WiFi","Pas de WiFi","Nessun WiFi","Kein WLAN"};
static const char* const T_BUSCANDO[6]    = {"Buscando actualizaciones...","Checking for updates...","Procurando atualizacoes...","Recherche de mises a jour...","Ricerca aggiornamenti...","Suche nach Updates..."};
static const char* const T_ULTIMA[6]      = {"Ya tienes la ultima version","You have the latest version","Voce tem a ultima versao","Version a jour","Versione aggiornata","Neueste Version vorhanden"};
static const char* const T_UPD_DISP[6]    = {"Update %s disponible","Update %s available","Update %s disponivel","Mise a jour %s dispo","Aggiornamento %s disponibile","Update %s verfuegbar"};
static const char* const T_BTN_ACT[6]     = {"ACTUALIZAR","UPDATE","ATUALIZAR","METTRE A JOUR","AGGIORNA","UPDATEN"};
static const char* const T_BTN_CANC[6]    = {"CANCELAR","CANCEL","CANCELAR","ANNULER","ANNULLA","ABBRECHEN"};
static const char* const T_REBOOT[6]      = {"Se reiniciara solo","Will reboot alone","Reiniciara sozinho","Redemarrage auto","Riavvio automatico","Neustart automatisch"};

void pantallaAviso(const char* l1,const char* l2){
  display.fillScreen(BLANCO);
  dibujarLogoEspera(20);
  int s=4; while(s>2 && anchoTxt(l1,s)>760) s--;
  txtCentrado(l1,400,220,s,true); if(l2 && l2[0]) txtCentrado(l2,400,280,2,false);
  display.update();
}
void wifiOffSiProcede(){
  if(!hayActualizacion){ WiFi.disconnect(true,true); WiFi.mode(WIFI_OFF); esp_wifi_stop(); }
}
// Descarga TIEMPO + ASTRO + ISS con la misma conexion.
// Devuelve bits: 1=tiempo OK, 2=astro OK, 4=ISS OK; 0x80 = no hay WiFi.
int descargarTodo(bool avisos){
  int l=idioma(); int r=0;
  if(WiFi.status()!=WL_CONNECTED){
    if(avisos) pantallaAviso(T_CONECTANDO[l],"");
    if(!conectarWifi(cfg.ssid,cfg.pass)) return 0x80;
  }
  configTzTime("CET-1CEST,M3.5.0,M10.5.0/3", "pool.ntp.org", "time.google.com"); struct tm ti; getLocalTime(&ti, 800);
  if(avisos) pantallaAviso(T_DESCARGANDO[l],T_ITEMS[l]);
  if(obtenerTiempo()){ guardarTiempoRTC(); r|=1; }
  if(obtenerAstro()) r|=2;
  if(obtenerISS()) r|=4;
  if(r&6) guardarAstroRTC();
  if(r&1) checkVersionOTA();
  Serial.printf("descargarTodo: tiempo=%d astro=%d iss=%d\n", (r&1)?1:0, (r&2)?1:0, (r&4)?1:0);
  return r;
}
// Boton "Actualizar datos". No cambia de pantalla; el llamador decide.
void accionActualizarDatos(){
  int l=idioma(); setCpuFrequencyMhz(240);
  int r=descargarTodo(true);
  wifiOffSiProcede(); setCpuFrequencyMhz(80);
  if(r==7){ pantallaAviso(T_ACTUALIZADO[l],""); delay(1000); }
  else if(r==0x80){ pantallaAviso(T_ERROR[l],""); delay(1500); }
  else {
    char lst[64]; lst[0]=0;
    if(!(r&1)){ strlcat(lst,M_LBL[l][0],sizeof(lst)); }
    if(!(r&2)){ if(lst[0]) strlcat(lst,", ",sizeof(lst)); strlcat(lst,"Astro",sizeof(lst)); }
    if(!(r&4)){ if(lst[0]) strlcat(lst,", ",sizeof(lst)); strlcat(lst,"ISS",sizeof(lst)); }
    pantallaAviso(T_ERRFALLO[l],lst); delay(2500);
  }
  leerBateria(); leerSHT40();
}
// Boton "Configuracion": portal desde el movil. Si se guarda algo, el aparato se reinicia solo.
void accionConfiguracion(){
  modoConfiguracion(); cargarConfig();
  accionActualizarDatos();
}
// Pantalla "hay actualizacion" con dos botones tactiles: ACTUALIZAR (izquierda) / CANCELAR (derecha)
void pantallaUpdateBotones(String ver){
  int l=idioma(); char b1[64], b2[64];
  snprintf(b1,sizeof(b1),T_UPD_DISP[l],ver.c_str()); snprintf(b2,sizeof(b2),T_ACTUAL[l],VERSION_FIRMWARE,ver.c_str());
  display.fillScreen(BLANCO);
  int s=3; while(s>2 && anchoTxt(b1,s)>760) s--;
  txtCentrado(b1,400,110,s,true); txtCentrado(b2,400,165,2,false);
  dibujarAvisoSustituye(225);
  // Boton ACTUALIZAR negro con texto blanco visible
  display.fillRect(40,300,350,100,NEGRO);
  display.drawRect(40,300,350,100,NEGRO); display.drawRect(41,301,348,98,NEGRO);
  // Boton CANCELAR blanco con borde negro
  display.drawRect(410,300,350,100,NEGRO); display.drawRect(411,301,348,98,NEGRO);
  txtCentrado(T_BTN_ACT[l],215,338,3,true,BLANCO);
  txtCentrado(T_BTN_CANC[l],585,338,3,true,NEGRO);
  txtCentrado(T_REBOOT[l],400,430,2,false);
  display.update();
}
// Boton "Buscar actualizaciones": consulta OTA ahora mismo (ignora el intervalo de 7 dias) - v13d FIX robusto
void accionBuscarOTA(){
  int l=idioma(); setCpuFrequencyMhz(240); WiFi.setSleep(false);
  pantallaAviso(T_CONECTANDO[l],"");
  Serial.printf("accionBuscarOTA WiFi status %d\n", WiFi.status());
  if(WiFi.status()!=WL_CONNECTED){
    bool ok = conectarWifi(cfg.ssid,cfg.pass);
    Serial.printf("conectarWifi result %d status %d\n", ok, WiFi.status());
    if(!ok){ pantallaAviso(T_ERROR[l],"WiFi"); delay(2000); setCpuFrequencyMhz(80); wifiOffSiProcede(); return; }
  }
  // NTP: hasta 10s
  configTzTime("CET-1CEST,M3.5.0,M10.5.0/3", "pool.ntp.org", "time.google.com", "time.nist.gov");
  struct tm ti; bool horaOk=false;
  for(int i=0;i<20;i++){
    if(getLocalTime(&ti, 500)){ horaOk=true; break; }
    Serial.printf("NTP intento %d...\n", i);
    delay(500);
  }
  Serial.printf("NTP horaOk=%d\n", horaOk);
  if(!horaOk){
    // sin hora no podemos comparar versiones, pero seguimos intentando mostrar error claro
    pantallaAviso("Sin hora / No time","NTP");
    delay(1000);
  }
  pantallaAviso(T_BUSCANDO[l],"");
  // Fuerza: borra version ignorada para probar
  prefs.begin("tiempo", false); prefs.remove("ota_ignore"); prefs.end();
  Serial.println("accionBuscarOTA: ota_ignore borrado para prueba");
  ultimoCheckOTA=0; 
  bool res = checkVersionOTA();
  bool consultaOk = (ultimoCheckOTA!=0);
  Serial.printf("accionBuscarOTA consultaOk=%d res=%d hayUpdate=%d ultimoCheck=%ld\n", consultaOk, res, hayActualizacion, (long)ultimoCheckOTA);
  if(!consultaOk){
    // fallo real de red o de hora
    if(!horaOk) pantallaAviso(T_ERROR[l],"NTP/Hora");
    else pantallaAviso(T_ERROR[l],"GitHub");
    delay(2500);
  }
  else if(!hayActualizacion){ pantallaAviso(T_ULTIMA[l],"v" VERSION_FIRMWARE); delay(2000); }
  else {
    pantallaUpdateBotones(versionRemotaDisponible);
    vaciarToques();
    unsigned long t0=millis(); int decision=0;
    while(millis()-t0<30000UL && !decision){
      int tx,ty; int g=leerGesto(tx,ty);
      if(g==1 && ty>=290 && ty<=410){ if(tx>=40 && tx<390) decision=1; else if(tx>=410 && tx<=760) decision=2; }
      delay(30); yield();
    }
    vaciarToques();
    if(decision==1){ String url=urlFirmwareRemoto; if(url.length()<10) url="https://estebangel-crypto.github.io/sticky-weather/firmware.bin"; realizarOTA(url); }
  }
  wifiOffSiProcede(); setCpuFrequencyMhz(80);
}

void setup(){
  pinMode(3, OUTPUT); pinMode(19, OUTPUT); digitalWrite(3, HIGH); digitalWrite(19, LOW);
  Serial.begin(115200);
  // CPU a 80MHz para ahorrar bateria, 240 solo para OTA
  setCpuFrequencyMhz(80);
  pinMode(PIN_BTN_DOWN, INPUT_PULLUP); pinMode(PIN_BTN_UP, INPUT_PULLUP); pinMode(PIN_BTN_AI, INPUT_PULLUP);
  stickyPowerLatch(); delay(200);
  pinMode(PIN_EPD_PWR, OUTPUT); digitalWrite(PIN_EPD_PWR, HIGH);
  pinMode(PIN_TOUCH_PWR, OUTPUT); digitalWrite(PIN_TOUCH_PWR, HIGH); delay(100);
  // I2C sensores en Wire1 (1/0)
  sensorWire.begin(PIN_SDA, PIN_SCL); sensorWire.setClock(100000);
  I2C_SENS = sensorWire;
  // Tactil en Wire (3/2)
  tactilInit();
  if(!display.begin()){ Serial.println(display.lastResult().message); dormir(5); }
  // --- "Actualizar desde SD" por fases (ver comentario en sdFaseLeer) ---
  int faseSD = sdFaseLeer();
  Serial.printf("SD fase al arrancar = %d\n", faseSD); Serial.flush();
  if(faseSD==1){ sdFaseGuardar(0); sdFase1(); }          // no vuelve (reinicia)
  if(faseSD==3){ sdFaseGuardar(0); sdFase3Instalar(); }  // no vuelve (reinicia)
  cargarConfig();
  sensorWire.begin(PIN_SDA, PIN_SCL); sensorWire.setClock(100000);
  delay(200);
  leerBateriaForzada();
  if(faseSD==2) explorarSDFase2();
  else if(faseSD==4) sdMostrarErrorFase4();
  bool abrirConfig=false, despBoton=false;
  if(esp_sleep_get_wakeup_cause()==ESP_SLEEP_WAKEUP_EXT1){
    uint64_t wakeMask = esp_sleep_get_ext1_wakeup_status();
    Serial.printf("Wake EXT1 mask=0x%llx UP=%d AI=%d DOWN=%d\n", wakeMask, digitalRead(PIN_BTN_UP), digitalRead(PIN_BTN_AI), digitalRead(PIN_BTN_DOWN));
    unsigned long t0=millis(); unsigned long dur=0;
    while(digitalRead(PIN_BTN_UP)==LOW && millis()-t0 < 2500){ delay(20); dur=millis()-t0; }
    Serial.printf("Duracion UP: %lu ms\n", dur);
    if(dur < 800){
      Serial.println("Corta -> dormir");
      time_t ahora=time(nullptr); uint64_t seg=INTERVALO_MIN*60ULL;
      if(ahora>1700000000 && proximaAct>ahora) seg=(uint64_t)(proximaAct-ahora);
      dormirSeg(seg);
    }
    despBoton=true;
    esperarSoltar(PIN_BTN_UP);
    rtc_gpio_hold_dis((gpio_num_t)PIN_BTN_UP);
  }
  if(faseSD==2 || faseSD==4) despBoton=true;   // volver al menu con los datos del RTC, sin descargar
  delay(200);
  if(!cfg.valida) abrirConfig=true;
  if(digitalRead(PIN_BTN_DOWN)==LOW) abrirConfig=true;
  if(abrirConfig){ modoConfiguracion(); if(!cfg.valida){ pantallaMensaje("Sin configurar",""); dormir(60);} }
  bool cicloTimer = (esp_sleep_get_wakeup_cause()==ESP_SLEEP_WAKEUP_TIMER);   // despertar automatico = silencioso
  // Si despierta por boton ARRIBA (manual), NO descargar datos, solo mostrar lo que hay en RTC
  if(despBoton){
    Serial.println("Despertar manual por ARRIBA: sin descarga, solo RTC");
    otaSinRed = true;
    if(rtc_astroValido) restaurarAstroRTC();
    if(datosValidosRTC){
      codeAhora = rtc_codeAhora; tempAhora = rtc_tempAhora; sensacion = rtc_sensacion; humedad = rtc_humedad; viento = rtc_viento;
      for(int i=0;i<4;i++) dias[i]=rtc_dias[i];
      currExt=rtc_currExt;
      Serial.println("Datos restaurados desde RTC");
    }
    // No WiFi, no descarga
    leerBateria(); leerSHT40(); iconoLunaVisible = false; pantalla = P_MENU; detalleActual = -1; redibujar();
  } else {
    otaSinRed = !cicloTimer;
    if(!cicloTimer) pantallaAviso(T_ACTUALIZANDO[idioma()], T_ITEMS[idioma()]);
    if(!conectarWifi(cfg.ssid,cfg.pass)){ pantallaAviso(T_SINWIFI[idioma()],""); dormir(5); }
    configTzTime("CET-1CEST,M3.5.0,M10.5.0/3", "pool.ntp.org", "time.google.com"); struct tm ti; for(int i=0;i<20 && !getLocalTime(&ti,500); i++){}
    if(rtc_astroValido) restaurarAstroRTC();
    bool tiempoOk = obtenerTiempo();
    bool astroOk = obtenerAstro();
    bool issOk = obtenerISS();
    if(tiempoOk) checkVersionOTA();
    otaSinRed = false;
    if(!hayActualizacion){
      Serial.println("WiFi OFF tras datos");
      WiFi.disconnect(true,true);
      WiFi.mode(WIFI_OFF);
      esp_wifi_stop();
      delay(200);
    }
    if(!tiempoOk && datosValidosRTC){ codeAhora = rtc_codeAhora; tempAhora = rtc_tempAhora; sensacion = rtc_sensacion; humedad = rtc_humedad; viento = rtc_viento; for(int i=0;i<4;i++) dias[i]=rtc_dias[i]; currExt=rtc_currExt; }
    else if(tiempoOk){ guardarTiempoRTC(); }
    if(astroOk || issOk) guardarAstroRTC();
    leerBateria(); leerSHT40(); iconoLunaVisible = false; pantalla = P_MENU; detalleActual = -1; redibujar();
  }
  if(hayActualizacion){ pantallaUpdateDisponible(versionRemotaDisponible); delay(10000); redibujar(); }
  vaciarToques();
  unsigned long tUltimaActividad = millis();
  const unsigned long TIEMPO_INACTIVIDAD_MS = 40000UL; // 40s sin tocar -> dormir

  if(despBoton){
    Serial.println("Despertar manual: 40s inactividad para dormir");
  }

  while(true){
    if(millis() - tUltimaActividad > TIEMPO_INACTIVIDAD_MS){
      Serial.println("40s sin actividad -> dormir");
      break;
    }
    // Botones V5 - mapeo corregido: ARRIBA=GPIO4 despierta, CENTRO=GPIO5 refresh, ABAJO=GPIO6 menu
    // ABAJO menu
    if(digitalRead(PIN_BTN_DOWN)==LOW){
      delay(40);
      if(digitalRead(PIN_BTN_DOWN)==LOW){
        unsigned long t0=millis();
        while(digitalRead(PIN_BTN_DOWN)==LOW && millis()-t0 < 1000){ delay(10); yield(); }
        while(digitalRead(PIN_BTN_DOWN)==LOW){ delay(10); yield(); }
        unsigned long dur=millis()-t0;
        if(dur>50 && dur<1000){
          Serial.printf("BTN ABAJO %lu ms -> MENU\n", dur);
          if(pantalla==P_MENU) pantalla=prevPantalla; else { prevPantalla=pantalla; pantalla=P_MENU; menuPagina=0; }
          if(pantalla==P_TIEMPO){ leerBateria(); leerSHT40(); }
          redibujar(); tUltimaActividad=millis();
        }
        delay(150);
      }
    }
    // ARRIBA (despertar) - en activo ignorado
    if(digitalRead(PIN_BTN_UP)==LOW){
      delay(40);
      if(digitalRead(PIN_BTN_UP)==LOW){
        unsigned long t0=millis();
        while(digitalRead(PIN_BTN_UP)==LOW && millis()-t0 < 1200){ delay(10); yield(); }
        while(digitalRead(PIN_BTN_UP)==LOW){ delay(10); yield(); }
        Serial.printf("BTN ARRIBA %lu ms (solo despierta)\n", millis()-t0);
        delay(150);
      }
    }
    // CENTRO - SOLO REFRESH, nunca actualizar datos
    if(digitalRead(PIN_BTN_AI)==LOW){
      delay(40);
      if(digitalRead(PIN_BTN_AI)==LOW){
        unsigned long t0=millis();
        while(digitalRead(PIN_BTN_AI)==LOW && millis()-t0 < 800){ delay(10); yield(); }
        unsigned long dur=millis()-t0;
        while(digitalRead(PIN_BTN_AI)==LOW){ delay(10); yield(); }
        if(dur>40){
          Serial.printf("BTN CENTRO %lu ms -> REFRESH\n", dur);
          display.fillScreen(BLANCO);
          redibujar();
          tUltimaActividad=millis();
        }
        delay(150);
      }
    }

    // Tactil
    int tx,ty; int g=leerGesto(tx,ty);
    if(g){
      Serial.printf("GESTO %d (%d,%d) pantalla=%d detalle=%d\n", g, tx, ty, (int)pantalla, detalleActual);
      Pant nueva = pantalla; int nuevoDet = detalleActual; bool forzar = false;
      bool zonaBoton = (tx>=660 && ty>=30 && ty<=90);   // recuadro Astro / Tiempo
      bool zonaMenu  = (tx<90 && ty<80);                // icono de menu
      switch(pantalla){
        case P_TIEMPO:
          if(g==1){
            if(zonaBoton) nueva=P_ASTRO;
            else if(zonaMenu) nueva=P_MENU;
            else if(ty>97 && ty<287 && tx<400){ nueva=P_DET_TIEMPO; nuevoDet=0; }
            else if(ty>305 && ty<475){ nueva=P_DET_TIEMPO; nuevoDet = (tx<273) ? 1 : (tx<526 ? 2 : 3); }
          }
          break;
        case P_DET_TIEMPO:
          if(g==1){ nueva=P_TIEMPO; nuevoDet=-1; }
          else if(g==2) nuevoDet=(detalleActual+1)%4;
          else if(g==3) nuevoDet=(detalleActual+3)%4;
          break;
        case P_MENU:
          if(g==2){ menuPagina = (menuPagina==0) ? 1 : 0; forzar=true; }                       // desliza a la izquierda: otra pagina
          else if(g==3){ if(menuPagina==1){ menuPagina=0; forzar=true; } else nueva=prevPantalla; }  // derecha: pagina 1 o salir
          else if(g==1){
            int b=menuBoton(tx,ty);
            if(b==0){ nueva=P_TIEMPO; nuevoDet=-1; }
            else if(b==1){ nueva=P_ASTRO; nuevoDet=-1; }
            else if(b==2){ accionActualizarDatos(); nueva=P_TIEMPO; nuevoDet=-1; forzar=true; }
            else if(b==3){ accionBuscarOTA(); forzar=true; }          // se queda en el menu
            else if(b==4){ accionActualizarDesdeSD(); forzar=true; }
            else if(b==5){ accionConfiguracion(); nueva=P_TIEMPO; nuevoDet=-1; forzar=true; }
            else if(b==6){ accionAbrirOtraApp(); forzar=true; }
          }
          break;
        case P_ASTRO:
          if(g==1){
            if(zonaBoton) nueva=P_TIEMPO;
            else if(zonaMenu) nueva=P_MENU;
            else if(ty>=96 && ty<242) nueva=(tx<400)?P_SOL:P_LUNA;
            else if(ty>=242 && ty<350) nueva=P_ESTRELLAS;
            else if(ty>=350) nueva=P_SOL;
          }
          else if(g==2) nueva=P_SOL;            // siguiente
          else if(g==3) nueva=P_ESTRELLAS;      // anterior
          break;
        case P_SOL: case P_LUNA: case P_ESTRELLAS: {
          int idx = (int)pantalla - (int)P_ASTRO;
          if(g==1) nueva=P_ASTRO;
          else if(g==2) nueva=(Pant)((int)P_ASTRO + (idx+1)%4);
          else if(g==3) nueva=(Pant)((int)P_ASTRO + (idx+3)%4);
          break; }
      }
      if(nueva==P_MENU && pantalla!=P_MENU){ prevPantalla=pantalla; menuPagina=0; }
      if(nueva==P_TIEMPO && pantalla==P_MENU){ leerBateria(); leerSHT40(); }
      if(nueva!=pantalla || nuevoDet!=detalleActual || forzar){ pantalla=nueva; detalleActual=nuevoDet; redibujar(); }
      vaciarToques();
      tUltimaActividad = millis();
    }
    delay(30); yield();
  }
  iconoLunaVisible = true; redibujar();   // se queda en la pantalla actual (con la lunita donde la cabecera la admite)
  delay(600); dormir(INTERVALO_MIN);
}
void loop(){}
