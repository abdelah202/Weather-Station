#include <DHT.h>
#include <ESP8266WiFi.h>

// Define the pin connected to the DHT sensor
#define DHT_PIN 13 // Equivalent to D7

// Initialize DHT sensor
DHT dht(DHT_PIN, DHT11);
WiFiClient client;

String apiKey = "2Z9ZA8TVO2HN847S";
const char *ssid =  "abou";
const char *pass =  "abc12345";
const char* server = "api.thingspeak.com";

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Initialize DHT sensor
  delay(10);
  dht.begin();
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  // Read temperature and humidity values from the sensor
  float temperature = dht.readTemperature(); // Read temperature in Celsius
  float humidity = dht.readHumidity();       // Read humidity
  
  // Check if any reads failed
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(temperature);
    postStr += "&field2=";
    postStr += String(humidity);
    postStr += "\r\n\r\n\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n\n\n");
    client.print(postStr);

    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Humidity: ");
    Serial.println(humidity);
  }
  client.stop();
  // Wait for a short delay before taking the next reading
  delay(10000); // Adjust delay as needed
}
