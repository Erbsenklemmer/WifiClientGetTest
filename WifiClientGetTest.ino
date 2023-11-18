#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//#include <ESPHTTPClient.h>
#include <ArduinoHttpClient.h>

const char* ssid = "Multipass Friends";
const char* password = "Willkommen";

// const char* ssid = "Multipass";
// const char* password = "PlauderDaTSCH";
char servername[] = "api.openweathermap.org";

void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  IPAddress myIP = WiFi.localIP();
  Serial.println(myIP);
}

void printState(int state) {

  Serial.print("State: ");
  switch (state) {
    case CLOSE_WAIT:
      Serial.println("Close wait");
      break;
    case CLOSING:
      Serial.println("Closing");
      break;
    case CLOSED:
      Serial.println("Closed");
      break;
    case ESTABLISHED:
      Serial.println("Established");
      break;
    default:
      Serial.println(String(state));
      break;
  }
}

//String url = "http://api.openweathermap.org/data/2.5/forecast?lat=48.8085568&lon=9.3774813&appid=0ca6e13112e998823ce775237c5fb829&units=metric&lang=de";
String url = "http://api.openweathermap.org/data/3.0/onecall?lat=48.8085568&lon=9.3774813&appid=0ca6e13112e998823ce775237c5fb829&units=metric&lang=de";

void loop() {

  Serial.printf("Getting url: %s\n", url.c_str());

  WiFiClient client;
  HttpClient http(client, "217.70.184.38");

  bool isBody = false;
  char c;
  unsigned long lostTest = 10000UL;
  unsigned long lost_do = millis();

  int httpCode = http.get("http://api.openweathermap.org/data/3.0/onecall?lat=48.8085568&lon=9.3774813&appid=0ca6e13112e998823ce775237c5fb829&units=metric&lang=de");
  Serial.printf("[HTTP] GET... code: %d\n", httpCode);

  int statusCode = http.responseStatusCode();
  String response = http.responseBody();
  Serial.printf("responseStatusCode: %d\n", statusCode);
  Serial.printf("responseBody: %s\n", response.c_str());

  if (httpCode == 0) {

    //    WiFiClient * client = http.getStreamPtr();
    Serial.printf("connected: %d\n", http.connected());
    Serial.printf("available: %d\n", http.available());

    while (http.available() == 0) {
      if ((millis() - lost_do) > lostTest) {
        Serial.println("lost in client with a timeout");
        client.stop();
        ESP.restart();
      }
      Serial.print(".");
    }
    Serial.println("");
    while (http.available() > 0) {
      c = http.read();
      if (c == '{' || c == '[') {

        isBody = true;
      }
      if (isBody) {
        //          parser.parse(c);
        Serial.print(c);
      }
      // give WiFi and TCP/IP libraries a chance to handle pending events
      yield();
    }
  }

  // WiFiClient client;

  //   if (client.connect(servername, 80)) {  //starts client connection, checks for connection
  //     client.println("GET /data/2.5/weather?lat=48.8085568&lon=9.3774813&appid=0ca6e13112e998823ce775237c5fb829&units=metric&lang=de");
  //     //client.println("GET /data/2.5/forecast?lat=48.8085568&lon=9.3774813&appid=0ca6e13112e998823ce775237c5fb829&units=metric&lang=de");
  //     client.println("Host: api.openweathermap.org");
  //     client.println("Connection: close");
  //     client.println("User-Agent: ArduinoWiFi/1.1");
  //     client.println("Pragma: no-cache");
  //     client.println("Cache-Control: no-cache");
  //     client.println("Accept: text/html,application/json");
  //     client.println();

  //     Serial.println("printed to client");
  //   } else {
  //     Serial.println("connection failed"); //error message if no client connect
  //     Serial.println();
  //   }

  //   Serial.println("get abgesetzt");

  //   printState(client.status());

  //   while(client.connected() && !client.available()) {
  //       delay(250); //waits for data
  //       Serial.print(".");
  //   }

  //   Serial.println("printing state");
  //   printState(client.status());

  //   if (client.available())
  //     Serial.println("data aveilible");
  //   else
  //     Serial.println("no data");

  //   int i = 0;
  //   String result;
  //   while (client.connected() || client.available()) { //connected or data available
  //     char c = client.read(); //gets byte from ethernet buffer
  //     i++;
  //     result = result+c;
  //     if (i > 1024)
  //     {
  //       Serial.println(result);
  //       result = "";
  //       i = 0;
  //     }
  //   }
  Serial.println("Ende und Aus");

  delay(60 * 1000);
}
