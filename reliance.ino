#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
#include <WiFi.h> // Include the ESP32 WiFi library

const char* ssid = "BHAVIK";
const char* pass = "w6rd5kqn";

const int maxDataPoints = 50;
const int numCompanies = 1;
String companies[numCompanies] = {"RELIANCE"};
float stockData[numCompanies][maxDataPoints];

WiFiClient wifiClient;
HttpClient client = HttpClient(wifiClient, "www.alphavantage.co", 80);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass); // Connect to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
    Serial.println("connected to wifi");
  for (int i = 0; i < numCompanies; i++) {
    for (int j = 0; j < maxDataPoints; j++) {
      stockData[i][j] = 0.0;
    }
  }
}

void loop() {
  for (int i = 0; i < numCompanies; i++) {
    if (fetchStockData(i)) {
      updateData(i);
      printToSerialPlotter(i);
    }
  }
  delay(5000); 
}

bool fetchStockData(int companyIndex) {
  String requestUrl = "/query?function=TIME_SERIES_DAILY&symbol=" + companies[companyIndex] + "&outputsize=full&apikey=4HWE4Y7D6EJW8M19";
  if (client.get(requestUrl) == 200) {
    String response = client.responseBody();

    
    Serial.println(response);
    DynamicJsonDocument doc(1024); 

    DeserializationError error = deserializeJson(doc, response);

    if (!error) {
      JsonObject root = doc.as<JsonObject>();
      JsonObject timeSeries = root["Time Series (Daily)"];

      
      String latestDate;
      float price = 0;

      for (JsonPair pair : timeSeries) {
        latestDate = pair.key().c_str();
        price = pair.value()["4. close"].as<float>();
        break; // Only need the most recent data
      }

      stockData[companyIndex][0] = price;
      return true;
    }
  }
  return false;
}

void  updateData(int companyIndex) {
  for (int i = maxDataPoints - 1; i > 0; i--) {
    stockData[companyIndex][i] = stockData[companyIndex][i - 1];
  }Serial.print(stockData[companyIndex][0], 2);
}
void  printToSerialPlotter(int companyIndex) {
  Serial.print(stockData[companyIndex][0], 2);
  if (companyIndex < numCompanies - 1) {
    Serial.print(",");
  } else {
    Serial.println();
  }
}

