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
  Serial.println("Connected to WiFi");

  for (int i = 0; i < numCompanies; i++) {
    for (int j = 0; j < maxDataPoints; j++) {
      stockData[i][j] = 0.0;
      Serial.print(stockData[i][j]);
    }
  }
}

void loop() {
  for (int i = 0; i < numCompanies; i++) {
    if (fetchStockData(i)) {
      updateData(i);
      printToSerialPlotter(i);
      Serial.print("the function  updatedata (i) is colled");
    }
  }
  delay(5000);
}

bool fetchStockData(int companyIndex) {
  String requestUrl = "/query?function=TIME_SERIES_DAILY&symbol=" + companies[companyIndex] + "&outputsize=compact&apikey=4HWE4Y7D6EJW8M19";
  Serial.print(requestUrl);
  
  if (client.get(requestUrl) == 200) {
    String response = client.responseBody();
    
    DynamicJsonDocument doc(2048); // Adjust the size based on the expected JSON response

    DeserializationError error = deserializeJson(doc, response);

    if (!error) {
      JsonObject metaData = doc["Meta Data"];
      JsonObject timeSeries = doc["Time Series (Daily)"];
      
      String latestDate = "";
      float latestPrice = 0.0;

      // Retrieve the latest date and price
      for (JsonPair pair : timeSeries) {
        latestDate = pair.key().c_str();
        JsonObject dataForDate = timeSeries[latestDate];
        latestPrice = dataForDate["4. close"].as<float>();
        break; // Only need the most recent data
      }Serial.print(latestDate);
      Serial.print(latestPrice);

    stockData[companyIndex][0] = latestPrice;
      return true;
    }
  }
  return false;
}

void updateData(int companyIndex) {
  for (int i = maxDataPoints - 1; i > 0; i--) {
   (stockData[companyIndex][i] = stockData[companyIndex][i - 1]);
   Serial.print(stockData[companyIndex][i]);
  }
  
}

void printToSerialPlotter(int companyIndex) {
  Serial.print(stockData[companyIndex][0], 2);
  if (companyIndex < numCompanies - 1) {
    Serial.print(",");
  } else {
    Serial.println();
  }
}
