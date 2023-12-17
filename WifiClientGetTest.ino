 #include <ESP8266WiFi.h>
 #include <ESP8266HTTPClient.h>

//#include <WiFi.h>
//#include <HTTPClient.h>

const char* ssid     = "Multipass Friends";
const char* password = "Willkommen";

// Your Domain name with URL path or IP address with path
String openWeatherMapApiKey = "0ca6e13112e998823ce775237c5fb829";

// Replace with your country code and city
String city = "Porto";
String countryCode = "PT";

// THE DEFAULT TIMER IS SET TO 10 SECONDS FOR TESTING PURPOSES
// For a final application, check the API call limits per hour/minute to avoid getting blocked/banned
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 10 seconds (10000)
unsigned long timerDelay = 10000;

unsigned int availableRemainingCounter = 0;

String jsonBuffer;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");
}

void loop() {
  // Send an HTTP GET request
  if ((millis() - lastTime) > timerDelay) {
    // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey + "&units=metric&lang=de";
      //String serverPath = "http://api.openweathermap.org/data/3.0/onecall?lat=48.8085568&lon=9.3774813&appid=0ca6e13112e998823ce775237c5fb829&units=metric&lang=de";
      
      jsonBuffer = httpGETRequest(serverPath.c_str());
      //Serial.println(jsonBuffer);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload; 
  WiFiClient* pClientStream;

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    //payload = http.getString();
    pClientStream = http.getStreamPtr();

    Serial.printf("length: %d\n", http.getSize());

    while (pClientStream && pClientStream->available())
    {
      char c = pClientStream->read();
      payload += c;
      Serial.print(c);

      yield();//give the stream a chance to read the next character
    }
    unsigned int availableRemaining = pClientStream->available();
    if (availableRemaining > 0)
    {
      availableRemainingCounter++;
    }
    Serial.printf("\nStream is available: %d, is null %d times\n", availableRemaining, availableRemainingCounter);

    Serial.flush();
    Serial.println("done read");
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  Serial.println("waiting for next call\n-------------");

  return payload;
}