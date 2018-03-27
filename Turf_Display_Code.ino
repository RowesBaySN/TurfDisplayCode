


/*
Drafted by Dylan VanDerWal (dylan.vanderwal@townsville.qld.gov.au)
MIT License
*/

//Pin that the DHT22 is connected to


/* ---WiFi Information--- */
//SSID of Network
const char* ssid = "TCC-Sensors";
//Password of Network
const char* pswd = "5Birds+Feathers";

/* ---Thingspeak Settings--- */
//Thingspeak API Key
String APIKey = "7X5IV0BT59LFE151";
//Update interval in seconds, must be greater than 15
const int interval = 15 ;

/* -----DO NOT EDIT PAST THIS POINT----- */
//Import libraries
#include <ESP8266WiFi.h>
#include <DHT.h>



//Definitions
#define DHTPIN D6
#define DHTPIN1 D7
#define DHTPIN2 D4
#define DHTPIN3 D3
#define DHTPIN4 D2

#define DHTTYPE DHT22
#define DHTTYPE1 DHT22
#define DHTTYPE2 DHT22
#define DHTTYPE3 DHT22
#define DHTTYPE4 DHT22

DHT dht(DHTPIN, DHTTYPE);
DHT dht1(DHTPIN1, DHTTYPE1);
DHT dht2(DHTPIN2, DHTTYPE2);
DHT dht3(DHTPIN3, DHTTYPE3);
DHT dht4(DHTPIN4, DHTTYPE4);
long lastConnectionTime = 0;

//define a function
void connectWiFi();


//Code that runs once at startup
void setup() {
  Serial.begin(115200);
  dht.begin();
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();

}


//Code that repeats itself
void loop() {
  if (WiFi.status() != WL_CONNECTED) { connectWiFi(); }

  if (millis() - lastConnectionTime > (interval*1000)) {

    String postData = (""); //define the initial post data

    //get the data from the DHT22
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    float h1 = dht1.readHumidity();
    float t1 = dht1.readTemperature();

    float h2 = dht2.readHumidity();
    float t2 = dht2.readTemperature();

    float h3 = dht3.readHumidity();
    float t3 = dht3.readTemperature();

    float h4 = dht4.readHumidity();
    float t4 = dht4.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    } else {
      String S0 = String(h);
      String S1 = String(t);

      String S2 = String(h1);
      String S3 = String(t1);

      String S4 = String(h2);
      String S5 = String(t2);

      String S6 = String(h3);
      String S7 = String(t3);

      String S8 = String(h4);
      String S9 = String(t4); 

      postData += ("field1=" + S1 + "&field2=" + S3 + "&field3=" + S5 + "&field4=" + S7 + "&field5=" + S9);

      

     
    }
    WiFiClient client;
    if (client.connect("api.thingspeak.com", 80)) {
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: " + APIKey + "\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postData.length());
      client.print("\n\n");
      client.print(postData);
      String line = client.readStringUntil('\r');   // read all the lines of the reply from server and print them to Serial
      Serial.println(line);
    }
    client.stop();
    lastConnectionTime = millis();
  }
  delay(5); // delay to let things reset
}
void connectWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pswd);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}
