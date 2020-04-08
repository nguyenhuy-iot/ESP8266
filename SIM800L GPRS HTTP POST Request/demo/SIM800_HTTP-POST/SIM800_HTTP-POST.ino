//For more Information visit www.aeq-web.com?ref=arduinoide

#include <SoftwareSerial.h>
#include <SerialCommand.h>
#include "stdio.h"

SoftwareSerial mySerial(10, 16);             // RX, TX Pins
String apn = "m-wap";                        //APN
String url = "host123iot.000webhostapp.com"; //URL for HTTP-POST-REQUEST
char DATA[300] = "DATA=hello...";

SerialCommand SCmd;
void setup()
{
  Serial.begin(115200);
  mySerial.begin(9600);

  SCmd.addCommand("SEND", send_host);
  SCmd.addCommand("SIM", Sim_Ceng);
  SCmd.addDefaultHandler(unrecognized); // Handler for command that isn't matched  (says "What?")

  delay(3000);
  Serial.println("READY...");
}

void loop()
{
  SCmd.readSerial(); // We don't do much, just process serial commands
}
void runsl()
{
  while (Serial.findUntil("OK", "ERROR") == 0)
  {
    /* code */
  }
}

void Answer()
{
  if (mySerial.available())
  {
    while (mySerial.available())
    {
      Serial.write(mySerial.read());
    }
    delay(100);
  }
}
void send_host()
{
  mySerial.println("AT");
  delay(100);
  Answer();
  mySerial.println("AT+SAPBR=3,1,Contype,GPRS");
  delay(100);
  Answer();
  mySerial.println("AT+SAPBR=3,1,APN," + apn);
  delay(100);
  Answer();
  mySerial.println("AT+SAPBR =1,1");
  delay(100);
  Answer();
  mySerial.println("AT+SAPBR=2,1");
  delay(100);
  Answer();
  mySerial.println("AT+HTTPINIT");
  delay(100);
  Answer();
  mySerial.println("AT+HTTPPARA=CID,1");
  delay(100);
  Answer();
  mySerial.println("AT+HTTPPARA=URL," + url);
  delay(100);
  Answer();
  mySerial.println("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
  delay(500);
  Answer();
  mySerial.println("AT+HTTPDATA=300,3000");
  delay(1000);
  Answer();
  mySerial.print(DATA);
  delay(3000);
  Answer();
  mySerial.println("AT+HTTPACTION=1");
  delay(5000);
  Answer();
  //mySerial.println("AT+HTTPREAD");
  //runsl();
  mySerial.println("AT+HTTPTERM");
  delay(500);
  Answer();
}
// This gets set as the default handler, and gets called when no other command matches.
void unrecognized()
{
  Serial.println("What?");
}

void Sim_Ceng()
{
  char buffer[300] = "";
  mySerial.println("AT+CENG=3,0");
  delay(100);
  mySerial.println("AT+CENG?");
  mySerial.readBytes(buffer, 300);
  //Xuất dữ liệu
  Serial.println(buffer);
  sprintf(DATA, "DATA=%s", buffer);
  Serial.println(DATA);
}
