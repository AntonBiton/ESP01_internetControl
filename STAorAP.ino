#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
const char *ssid[3] = {"Сеть1", "Сеть2","Сеть3"};  
const char *password[3] = {"пароль1","пароль2","пароль3"};
const char *ssidap = "esp01";  
const char *passwordap = "12345678";
uint8_t LED1pin = 0;
uint8_t LED2pin = 2;
const char *sss = "0";
bool LED1status = LOW;
bool LED2status = LOW;

      IPAddress local_ip(192,168,1,20);
      IPAddress gateway(192,168,1,1);
      IPAddress subnet(255,255,255,0);
      
//const char *ssidc[3];
//const char *passwordc[3];
//const char *ssidapc;
//const char *passwordapc;

int t=0;
int i=0;
int ip=0;
ESP8266WebServer server(80);
void setup() { 
  //delay(500);
  /// Serial.begin(115200);
  pinMode(LED1pin, OUTPUT);
  digitalWrite(LED1pin, 0);
  pinMode(LED2pin, OUTPUT);
  digitalWrite(LED2pin, 0);
  connectedWiFi();
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.on("/connect", connectedWiFi);
  server.on("/lokac", handle_lokac);
  server.onNotFound(handle_NotFound);
  server.begin();
  /// Serial.println("HTTP server started");
  EEPROM.begin(1);
  LED1status = EEPROM.read(1);
  
}

void loop() {
  if(sss != "0"){
      if(WiFi.status() != WL_CONNECTED){
      connectedWiFi();
    }
  }
  
   server.handleClient();
  if(LED1status == 1)
  {digitalWrite(LED1pin, HIGH);}
  else
  {digitalWrite(LED1pin, LOW);}
  if(LED2status ==0 )
  {digitalWrite(LED2pin, HIGH);}
  else
  {digitalWrite(LED2pin, LOW);} 
  
}

void connectedWiFi(){
  WiFi.softAPdisconnect(false);
  for (i=0; i<3; i++) {
      WiFi.begin(ssid[i], password[i]);
      for(t=0; t<=20; t++){
        delay(1000);
      if(WiFi.status() == WL_CONNECTED){t=41; sss = ssid[i];}
      }
    }
    if(WiFi.status() != WL_CONNECTED){
      WiFi.persistent(false);
      delay(100);
      WiFi.disconnect(false);
      delay(200);

      WiFi.softAPConfig(local_ip, gateway, subnet);
      WiFi.softAP (ssidap, passwordap);
    }
else{
  ip=1;
  IPAddress ipAddress = WiFi.localIP();
String ipadres = String(ipAddress[0]) + '.' + String(ipAddress[1]) + '.' + String(ipAddress[2]) + '.' + String(ipAddress[3]);
char charVar[sizeof(ipadres)];
ipadres.toCharArray(charVar, sizeof(charVar)+3);
  WiFi.softAP (charVar, "12345678");
}
}

void handle_OnConnect() {
  if(ip){  WiFi.softAPdisconnect(false);ip=0;}///// Serial.println("Произошло подключение, скрываем ip");
  //LED1status = LOW;
  LED2status = LOW;
  server.send(200, "text/html", SendHTML(LED1status,LED2status,0)); 
}
void handle_led1on() {
  LED1status = HIGH;
  EEPROM.write(1, 1);
  /// Serial.println("ON");
  EEPROM.commit();
  server.send(200, "text/html", SendHTML(true,LED2status,0)); 
  
}
void handle_led1off() {
  LED1status = LOW;
  EEPROM.write(1, 0);
  /// Serial.println("OFF");
  EEPROM.commit();
  server.send(200, "text/html", SendHTML(false,LED2status,0)); 
}
void handle_led2on() {
  LED2status = HIGH;
//  /// Serial.println("GPIO5 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,true,0)); 
}
void handle_led2off() {
  LED2status = LOW;
//  /// Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false,0)); 
}
void handle_lokac() { 
  server.send(200, "text/html", SendHTML(LED1status,LED2status,1)); 
}
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}
String SendHTML(uint8_t led1stat,uint8_t led2stat, int loc){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Управление светодиодом</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #4AE83A;}\n";
  ptr +=".button-on:active {background-color: #14D100;}\n";
  ptr +=".button-off {background-color: #FD3F49;}\n";
  ptr +=".button-off:active {background-color: #FB000D;}\n";
  ptr +=".button-con {background-color: #363835;}\n";
  ptr +=".button-con:active {background-color: #ACA5A5;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n"; 
  //ptr +="<h1>Привет!</h1>\n";
  //ptr +="<h3>Режим станции (STA)</h3>\n";
   if(led1stat)
  {ptr +="<h1>Свет Включен</h1>";}
  else
  {ptr +="<h1>Свет вЫключен</h1>";}
   ptr +="<a class=\"button button-on\" href=\"/led1on\">ВКЛ.</a>\n\n\n";
   ptr +="<a class=\"button button-off\" href=\"/led1off\">ВЫКЛ.</a>\n\n\n\n\n\n\n\n";
   ptr +="<a class=\"button button-con\" href=\"/connect\">Подключиться к WiFi</a>\n";
   ptr +="<a class=\"button button-con\" href=\"/lokac\">сеть</a>\n";

  if(loc){
    if(sss == "0"){
      ptr +="<a>Сеть :(";
     // String esais = String(ssid[i]);
     // char sss[sizeof(esais)];
     // esais.toCharArray(sss, sizeof(sss)+3);
      ptr +=String(ssidap);
      ptr +=")</a>";    
    }
    else{
      ptr +="<a>Сеть :(";
      ptr +=String(sss);//ptr += String(*ssidap);
      ptr +=")</a>";  
    }
  }
 
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
