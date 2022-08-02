
#include <WiFi.h>
#include <WiFiClient.h>
#include "esp_wifi.h"
#include <WebServer.h>


// Set these to your desired credentials.
const char *ssid = "yourAP";
const char *password = "yourPassword";

String buf[2];
char temp_buf[50];
int i=0, j=0;                         
WebServer server(80);



void handleRoot() { 
  digitalWrite(LED_BUILTIN, 1);
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 400,
                    "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP32 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\      
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP32!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p>LED1 Status: ON</p><a href=\"/announcement\">Click to know the status</a>\
  </body>\
</html>",
           hr, min % 60, sec % 60
          );
  server.send(200, "text/html", temp);
  digitalWrite(LED_BUILTIN, 0);
}
    

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}


void handle_announcement() {
  char temp[400];
  char msg_buf[50]="Unknown User";
  String addy = server.client().remoteIP().toString();
  String ipAddress;
  String msg;
  for(int j=0; j<10;j++){
    if(buf[j]!=NULL){
      if(buf[j].indexOf('$')!=-1){
        Serial.println(buf[j]);
        //ipAddress.remove(0,40);
        //msg.remove(0,40);
        Serial.print("ipAddress :");
        ipAddress=buf[j].substring(0,buf[j].indexOf('$'));
        msg=buf[j].substring(buf[j].indexOf('$')+1);
        Serial.println(buf[j]); 
        Serial.println(ipAddress);
        Serial.print("MSG: ");
        Serial.println(msg);               
        if(addy.equals(ipAddress)){          
          memset(msg_buf,0,50);
          snprintf(msg_buf,50,"%s",msg);  
        }
      }      
    }
  }
  
  Serial.println(addy);
  Serial.println(msg_buf);
  snprintf(temp, 400,
                    "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP32 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\      
    </style>\
  </head>\
  <body>\
    <h1>%s</h1>\
    <a href=\"/\">Click here to go back</a>\
  </body>\
</html>",
           msg_buf
          );
  server.send(200, "text/html", temp);
}
  

void setup() {
    delay(1000);
    Serial.begin(115200);
    Serial.println();
    Serial.print("Configuring access point...");
    WiFi.softAP(ssid, password);
    Serial.println(WiFi.softAP(ssid,password) ? "soft-AP setup": "Failed to connect");
    Serial.println(WiFi.softAPIP());
    
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    
    server.on("/", handleRoot);
    server.on("/announcement", handle_announcement);
    server.onNotFound(handle_NotFound);
    server.begin();
    Serial.println("HTTP server started");  
}
 
void loop() {
    server.handleClient();    
    if(Serial.available()){
    char c=Serial.read();
    if(c=='\n'){
      buf[j++]=temp_buf;            
      Serial.print("buf: ");
      Serial.println(buf[j-1]);     
      Serial.print("j: ");
      Serial.println(j);
      if (j>9)
        j=0;
      i=0;
    }
    else{
      temp_buf[i++]=c;   
    }
  }
}
