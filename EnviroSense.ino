#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

// WiFi credentials - CHANGE THESE
const char* ssid = "user_name"; // modify with your SSID
const char* password = "{assword"; //update your password

// DHT22 settings
#define DHTPIN D2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// MQ135 analog pin
#define MQ135_PIN A0

// Create web server on port 80
ESP8266WebServer server(80);

// Global variables for sensor readings
float temperature = 0;
float humidity = 0;
int airQuality = 0;
float voltage = 0;
String airQualityStatus = "";

void setup() {
  Serial.begin(115200);
  delay(10);
  
  pinMode(DHTPIN, INPUT_PULLUP);
  
  Serial.println();
  Serial.println("=================================");
  Serial.println("Air Quality Monitoring System");
  Serial.println("=================================");
  
  dht.begin();
  delay(2000);
  Serial.println("DHT22 sensor initialized!");
  
  // Connect to WiFi
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("✓ WiFi connected successfully!");
    Serial.print("✓ IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.println();
    Serial.println("========================================");
    Serial.println("Open this IP in your browser:");
    Serial.print("http://");
    Serial.println(WiFi.localIP());
    Serial.println("========================================");
    Serial.println();
    
    // Setup web server routes
    server.on("/", handleRoot);
    server.on("/data", handleData);
    
    // Start server
    server.begin();
    Serial.println("✓ Web server started!");
  } else {
    Serial.println("✗ WiFi connection failed!");
    Serial.println("Check credentials and try again.");
  }
  
  Serial.println("=================================");
  Serial.println();
}

void loop() {
  // Handle web server requests
  server.handleClient();
  
  // Read sensors every 3 seconds
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate >= 3000) {
    lastUpdate = millis();
    
    // Read sensors
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
    airQuality = analogRead(MQ135_PIN);
    voltage = airQuality * (3.3 / 1023.0);
    
    // Determine air quality status
    if (airQuality < 200) {
      airQualityStatus = "GOOD";
    } else if (airQuality < 400) {
      airQualityStatus = "MODERATE";
    } else if (airQuality < 600) {
      airQualityStatus = "POOR";
    } else {
      airQualityStatus = "HAZARDOUS";
    }
    
    // Print to Serial Monitor
    Serial.println("━━━ Sensor Update ━━━");
    if (!isnan(temperature)) {
      Serial.print("🌡️  Temperature: ");
      Serial.print(temperature);
      Serial.println(" °C");
    }
    if (!isnan(humidity)) {
      Serial.print("💧 Humidity: ");
      Serial.print(humidity);
      Serial.println(" %");
    }
    Serial.print("🌍 Air Quality: ");
    Serial.print(airQuality);
    Serial.print(" - ");
    Serial.println(airQualityStatus);
    Serial.println();
  }
}

// Handle root page request
void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Air Quality Monitor</title>";
  html += "<style>";
  html += "* { margin: 0; padding: 0; box-sizing: border-box; }";
  html += "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); min-height: 100vh; padding: 20px; }";
  html += ".container { max-width: 1000px; margin: 0 auto; }";
  html += ".header { text-align: center; color: white; margin-bottom: 40px; animation: fadeIn 1s; }";
  html += ".header h1 { font-size: 3em; margin: 20px 0 10px; text-shadow: 2px 2px 4px rgba(0,0,0,0.2); }";
  html += ".header p { font-size: 1.2em; opacity: 0.9; }";
  html += ".card { background: white; border-radius: 20px; padding: 35px; margin-bottom: 25px; box-shadow: 0 15px 40px rgba(0,0,0,0.25); animation: slideUp 0.8s; }";
  html += ".sensor-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(250px, 1fr)); gap: 25px; margin-bottom: 30px; }";
  html += ".sensor-box { text-align: center; padding: 30px 20px; background: linear-gradient(135deg, #f5f7fa 0%, #c3cfe2 100%); border-radius: 15px; transition: transform 0.3s, box-shadow 0.3s; position: relative; overflow: hidden; }";
  html += ".sensor-box:hover { transform: translateY(-5px); box-shadow: 0 10px 25px rgba(0,0,0,0.15); }";
  html += ".sensor-box::before { content: ''; position: absolute; top: 0; left: 0; right: 0; height: 4px; background: linear-gradient(90deg, #667eea, #764ba2); }";
  html += ".sensor-icon { font-size: 3em; margin-bottom: 15px; }";
  html += ".sensor-value { font-size: 3em; font-weight: bold; color: #667eea; margin: 15px 0; text-shadow: 1px 1px 2px rgba(0,0,0,0.1); }";
  html += ".sensor-label { color: #555; font-size: 1.1em; font-weight: 600; text-transform: uppercase; letter-spacing: 1px; }";
  html += ".sensor-unit { color: #888; font-size: 1em; margin-top: 5px; }";
  html += ".status { padding: 25px; border-radius: 15px; text-align: center; font-size: 1.5em; font-weight: bold; margin-top: 25px; transition: all 0.5s; }";
  html += ".status-good { background: linear-gradient(135deg, #d4edda 0%, #a8e6cf 100%); color: #155724; }";
  html += ".status-moderate { background: linear-gradient(135deg, #fff3cd 0%, #ffd89b 100%); color: #856404; }";
  html += ".status-poor { background: linear-gradient(135deg, #f8d7da 0%, #ffb3ba 100%); color: #721c24; }";
  html += ".status-hazardous { background: linear-gradient(135deg, #f5c6cb 0%, #ff6b6b 100%); color: #721c24; }";
  html += ".footer { text-align: center; color: white; margin-top: 30px; opacity: 0.8; }";
  html += ".update-time { text-align: center; color: #666; font-size: 1em; margin-top: 20px; padding: 10px; background: #f8f9fa; border-radius: 10px; }";
  html += ".pulse { animation: pulse 2s infinite; }";
  html += "@keyframes fadeIn { from { opacity: 0; transform: translateY(-20px); } to { opacity: 1; transform: translateY(0); } }";
  html += "@keyframes slideUp { from { opacity: 0; transform: translateY(30px); } to { opacity: 1; transform: translateY(0); } }";
  html += "@keyframes pulse { 0%, 100% { opacity: 1; } 50% { opacity: 0.7; } }";
  html += "@media (max-width: 768px) { .header h1 { font-size: 2em; } .sensor-value { font-size: 2.5em; } }";
  html += "</style>";
  html += "</head><body>";
  html += "<div class='container'>";
  html += "<div class='header'>";
  html += "<h1>🌍 Air Quality Monitor</h1>";
  html += "<p>Real-time Environmental Monitoring Dashboard</p>";
  html += "</div>";
  html += "<div class='card'>";
  html += "<div class='sensor-grid' id='sensorData'>";
  html += "<div class='sensor-box'><div class='sensor-icon'>🌡️</div><div class='sensor-label'>Temperature</div><div class='sensor-value pulse'>--</div><div class='sensor-unit'>°C</div></div>";
  html += "<div class='sensor-box'><div class='sensor-icon'>💧</div><div class='sensor-label'>Humidity</div><div class='sensor-value pulse'>--</div><div class='sensor-unit'>%</div></div>";
  html += "<div class='sensor-box'><div class='sensor-icon'>🌫️</div><div class='sensor-label'>Air Quality</div><div class='sensor-value pulse'>--</div><div class='sensor-unit'>PPM</div></div>";
  html += "</div>";
  html += "<div class='status' id='statusBox'>🔄 Loading sensor data...</div>";
  html += "<div class='update-time' id='updateTime'>⏱️ Initializing...</div>";
  html += "</div>";
  html += "<div class='footer'>";
  html += "<p>Powered by ESP8266 NodeMCU | DHT22 | MQ135</p>";
  html += "</div>";
  html += "</div>";
  html += "<script>";
  html += "function updateData() {";
  html += "  fetch('/data').then(response => response.json()).then(data => {";
  html += "    const boxes = document.querySelectorAll('.sensor-value');";
  html += "    boxes[0].textContent = data.temperature.toFixed(1);";
  html += "    boxes[1].textContent = data.humidity.toFixed(1);";
  html += "    boxes[2].textContent = data.airQuality;";
  html += "    const statusBox = document.getElementById('statusBox');";
  html += "    let icon = '';";
  html += "    if (data.status === 'GOOD') icon = '✅';";
  html += "    else if (data.status === 'MODERATE') icon = '⚠️';";
  html += "    else if (data.status === 'POOR') icon = '🔶';";
  html += "    else icon = '🚨';";
  html += "    statusBox.textContent = icon + ' Air Quality: ' + data.status;";
  html += "    statusBox.className = 'status status-' + data.status.toLowerCase();";
  html += "    const now = new Date();";
  html += "    document.getElementById('updateTime').textContent = '🕒 Last updated: ' + now.toLocaleTimeString();";
  html += "  }).catch(err => {";
  html += "    console.error('Error fetching data:', err);";
  html += "    document.getElementById('statusBox').textContent = '⚠️ Connection Error';";
  html += "  });";
  html += "}";
  html += "updateData();";
  html += "setInterval(updateData, 3000);";
  html += "</script>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

// Handle data API request
void handleData() {
  String json = "{";
  json += "\"temperature\":" + String(isnan(temperature) ? 0 : temperature) + ",";
  json += "\"humidity\":" + String(isnan(humidity) ? 0 : humidity) + ",";
  json += "\"airQuality\":" + String(airQuality) + ",";
  json += "\"voltage\":" + String(voltage) + ",";
  json += "\"status\":\"" + airQualityStatus + "\"";
  json += "}";
  
  server.send(200, "application/json", json);
}

