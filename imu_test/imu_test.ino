#include <WiFi.h>
#include <WebServer.h>
#include "esp_system.h"

// ======================
// WIFI CONFIG
// ======================

const char* ssid = " Galaxy S10+";
const char* password = "12345678";

// ======================
// WEB SERVER
// ======================

WebServer server(80);

// ======================
// BOOT TIME
// ======================

unsigned long bootTime;

// ======================
// RESET REASON HELPER
// ======================

String getResetReason() {

    esp_reset_reason_t reason = esp_reset_reason();

    switch(reason) {

        case ESP_RST_POWERON:
            return "Power On";

        case ESP_RST_EXT:
            return "External Reset";

        case ESP_RST_SW:
            return "Software Reset";

        case ESP_RST_PANIC:
            return "Kernel Panic";

        case ESP_RST_INT_WDT:
            return "Interrupt Watchdog";

        case ESP_RST_TASK_WDT:
            return "Task Watchdog";

        case ESP_RST_WDT:
            return "Other Watchdog";

        case ESP_RST_DEEPSLEEP:
            return "Wake From Deep Sleep";

        case ESP_RST_BROWNOUT:
            return "Brownout";

        default:
            return "Unknown";
    }
}

// ======================
// HTML PAGE
// ======================

void handleRoot() {

    String html = R"rawliteral(

    <!DOCTYPE html>
    <html>

    <head>

        <title>ESP32 Telemetry Portal</title>

        <meta http-equiv="refresh" content="2">

        <style>

            body {
                background: #111;
                color: white;
                font-family: Arial;
                padding: 20px;
            }

            .card {
                background: #1e1e1e;
                padding: 20px;
                border-radius: 12px;
                margin-bottom: 20px;
            }

            h1 {
                color: #00ffcc;
            }

            h2 {
                color: #66ccff;
            }

        </style>

    </head>

    <body>

        <h1>ESP32 Telemetry Portal</h1>

    )rawliteral";

    // ======================
    // SYSTEM UPTIME
    // ======================

    html += "<div class='card'>";
    html += "<h2>System Uptime</h2>";
    html += "<p>";

    html += String((millis() - bootTime) / 1000);

    html += " seconds</p>";
    html += "</div>";

    // ======================
    // WIFI SIGNAL
    // ======================

    html += "<div class='card'>";
    html += "<h2>WiFi Signal Strength</h2>";
    html += "<p>";

    html += String(WiFi.RSSI());

    html += " dBm</p>";
    html += "</div>";

    // ======================
    // LOCAL IP
    // ======================

    html += "<div class='card'>";
    html += "<h2>Local IP Address</h2>";
    html += "<p>";

    html += WiFi.localIP().toString();

    html += "</p>";
    html += "</div>";

    // ======================
    // MAC ADDRESS
    // ======================

    html += "<div class='card'>";
    html += "<h2>MAC Address</h2>";
    html += "<p>";

    html += WiFi.macAddress();

    html += "</p>";
    html += "</div>";

    // ======================
    // GATEWAY
    // ======================

    html += "<div class='card'>";
    html += "<h2>Gateway IP</h2>";
    html += "<p>";

    html += WiFi.gatewayIP().toString();

    html += "</p>";
    html += "</div>";

    // ======================
    // FREE HEAP
    // ======================

    html += "<div class='card'>";
    html += "<h2>Free Heap Memory</h2>";
    html += "<p>";

    html += String(ESP.getFreeHeap());

    html += " bytes</p>";
    html += "</div>";

    // ======================
    // CPU FREQUENCY
    // ======================

    html += "<div class='card'>";
    html += "<h2>CPU Frequency</h2>";
    html += "<p>";

    html += String(getCpuFrequencyMhz());

    html += " MHz</p>";
    html += "</div>";

    // ======================
    // FLASH SIZE
    // ======================

    html += "<div class='card'>";
    html += "<h2>Flash Size</h2>";
    html += "<p>";

    html += String(ESP.getFlashChipSize() / (1024 * 1024));

    html += " MB</p>";
    html += "</div>";

    // ======================
    // CHIP MODEL
    // ======================

    html += "<div class='card'>";
    html += "<h2>Chip Model</h2>";
    html += "<p>";

    html += ESP.getChipModel();

    html += "</p>";
    html += "</div>";

    // ======================
    // CHIP CORES
    // ======================

    html += "<div class='card'>";
    html += "<h2>CPU Cores</h2>";
    html += "<p>";

    html += String(ESP.getChipCores());

    html += "</p>";
    html += "</div>";

    // ======================
    // SDK VERSION
    // ======================

    html += "<div class='card'>";
    html += "<h2>ESP-IDF Version</h2>";
    html += "<p>";

    html += ESP.getSdkVersion();

    html += "</p>";
    html += "</div>";

    // ======================
    // RESET REASON
    // ======================

    html += "<div class='card'>";
    html += "<h2>Last Reset Reason</h2>";
    html += "<p>";

    html += getResetReason();

    html += "</p>";
    html += "</div>";

    // ======================
    // CLOSE HTML
    // ======================

    html += R"rawliteral(

    </body>
    </html>

    )rawliteral";

    server.send(200, "text/html", html);
}

// ======================
// WIFI CONNECTION
// ======================

void connectWiFi() {

    WiFi.mode(WIFI_STA);

    Serial.println();
    Serial.println("Connecting to WiFi...");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    int attempts = 0;

    while (WiFi.status() != WL_CONNECTED && attempts < 30) {

        delay(1000);

        Serial.print("WiFi Status: ");
        Serial.println(WiFi.status());

        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {

        Serial.println();
        Serial.println("=================================");
        Serial.println("WIFI CONNECTED");
        Serial.print("IP ADDRESS: ");
        Serial.println(WiFi.localIP());
        Serial.println("=================================");

    } else {

        Serial.println();
        Serial.println("=================================");
        Serial.println("WIFI CONNECTION FAILED");
        Serial.println("=================================");
    }
}

// ======================
// SETUP
// ======================

void setup() {

    Serial.begin(115200);

    delay(1000);

    bootTime = millis();

    connectWiFi();

    server.on("/", handleRoot);

    server.begin();

    Serial.println("Webserver started");
}

// ======================
// MAIN LOOP
// ======================

void loop() {

    server.handleClient();
}