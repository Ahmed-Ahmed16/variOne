// SD smoke test for VariOne. Implements PRD §13.0 by isolating the microSD
// reader on the shared VSPI bus and printing wiring/card diagnostics over serial.

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#define PIN_SD_CS    5
#define PIN_VSPI_SCK 18
#define PIN_VSPI_MISO 19
#define PIN_VSPI_MOSI 23
#define PIN_CC_CS    15

static void listDir(File dir, int depth = 0) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) break;

    for (int i = 0; i < depth; i++) Serial.print("  ");
    Serial.print(entry.isDirectory() ? "[DIR]  " : "[FILE] ");
    Serial.print(entry.name());
    if (!entry.isDirectory()) {
      Serial.print("  ");
      Serial.print(entry.size());
      Serial.print(" bytes");
    }
    Serial.println();

    if (entry.isDirectory() && depth < 2) listDir(entry, depth + 1);
    entry.close();
  }
}

static const char* cardTypeName(uint8_t type) {
  switch (type) {
    case CARD_MMC: return "MMC";
    case CARD_SD: return "SDSC";
    case CARD_SDHC: return "SDHC/SDXC";
    case CARD_NONE: return "NONE";
    default: return "UNKNOWN";
  }
}

static void runSdProbe() {
  Serial.println();
  Serial.println("[SD-TEST] === probe ===");
  Serial.printf("[SD-TEST] wiring: CS=%d SCK=%d MISO=%d MOSI=%d\n",
                PIN_SD_CS, PIN_VSPI_SCK, PIN_VSPI_MISO, PIN_VSPI_MOSI);
  Serial.println("[SD-TEST] note: bare SPI reader cannot be proven present without a card response");

  pinMode(PIN_CC_CS, OUTPUT);
  digitalWrite(PIN_CC_CS, HIGH); // keep CC1101 off shared SPI
  pinMode(PIN_SD_CS, OUTPUT);
  digitalWrite(PIN_SD_CS, HIGH);

  SPI.end();
  SPI.begin(PIN_VSPI_SCK, PIN_VSPI_MISO, PIN_VSPI_MOSI, PIN_SD_CS);

  bool mounted = SD.begin(PIN_SD_CS, SPI, 4000000);
  Serial.printf("[SD-TEST] SD.begin=%s\n", mounted ? "OK" : "FAIL");
  if (!mounted) {
    Serial.println("[SD-TEST] meaning: no card, wrong VCC, wrong CS, bad wiring, or non-FAT card");
    Serial.println("[SD-TEST] try: FAT32 card, CS=5, SCK=18, MISO=19, MOSI=23, common GND");
    return;
  }

  uint8_t type = SD.cardType();
  Serial.printf("[SD-TEST] cardType=%s (%u)\n", cardTypeName(type), type);
  if (type == CARD_NONE) {
    Serial.println("[SD-TEST] mounted but no card type; check card seating/module power");
    return;
  }

  uint64_t cardMB = SD.cardSize() / (1024ULL * 1024ULL);
  uint64_t usedMB = SD.usedBytes() / (1024ULL * 1024ULL);
  uint64_t totalMB = SD.totalBytes() / (1024ULL * 1024ULL);
  Serial.printf("[SD-TEST] cardSize=%llu MB fsTotal=%llu MB used=%llu MB\n",
                cardMB, totalMB, usedMB);

  SD.mkdir("/captures");
  File f = SD.open("/sd_test.txt", FILE_WRITE);
  if (!f) {
    Serial.println("[SD-TEST] write=FAIL");
  } else {
    f.printf("VariOne SD test OK at %lu ms\n", millis());
    f.close();
    Serial.println("[SD-TEST] write=OK /sd_test.txt");
  }

  File root = SD.open("/");
  if (root) {
    Serial.println("[SD-TEST] root listing:");
    listDir(root);
    root.close();
  } else {
    Serial.println("[SD-TEST] root open=FAIL");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1200);
  Serial.println("VariOne SD smoke test boot");
  runSdProbe();
}

void loop() {
  static unsigned long lastProbe = 0;
  if (millis() - lastProbe > 2000) {
    lastProbe = millis();
    runSdProbe();
  }
}
