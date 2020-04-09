#include "Sim800.h"
#include <SoftwareSerial.h>
#include <SerialCommand.h>

String apn = "m-wap";                        //APN
String url = "host123iot.000webhostapp.com"; //URL for HTTP-POST-REQUEST
char DATA[300] = "DATA=hello...";

SoftwareSerial mySerial(10, 16); // RX, TX Pins
SerialCommand SCmd;

void SimSetup()
{
  mySerial.begin(9600);
  Serial.begin(115200);

  SCmd.addCommand("SEND", SendHost);
  SCmd.addCommand("SIM", SimCeng);
  SCmd.addCommand("test", SimTest);
  SCmd.addDefaultHandler(unrecognized); // Handler for command that isn't matched  (says "What?")

  delay(3000);
  Serial.println("READY...");

  mySerial.println("AT");
  delay(100);
  Answer();
  mySerial.println("ATE0");
  delay(100);
  Answer();
}
void SimRun()
{
  SCmd.readSerial(); // We don't do much, just process serial commands
  //Sim_AT("AT");
  //delay(3000);
}
///////********************************/////////////
void unrecognized()
{
  Serial.println("What?");
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
//----------------------------------------------//

void Sim_AT(const char *command, uint16_t wait = 1000)
{

  Serial.println(command);
  mySerial.println(command);
  if (wait < 100) //nhan du lieu ko can cho
  {
    delay(wait);
    Serial.println("wait<100");
  }
  else if (wait <= 1000) //cho Sim tra loi KO hoac ERROR
  {
    Serial.setTimeout(wait);
    if (mySerial.findUntil("OK", "ERROR"))
    {
      Serial.println("OK");
    }
    Serial.println("wait<=1000");
  }
  else
  {
    uint32_t time = millis();
    while (mySerial.available() == 0) //cho den khi nhan duoc du lieu
    {
      if (millis() - time > wait) //thoat khi qua thoi gian
      {
        break;
      }
    }
    while (mySerial.available()) //hien thi toan bo du lieu Sim phan hoi
    {
      Serial.write(mySerial.read());
      delay(1);
    }
  }
  Serial.setTimeout(1000);
}
void SimTest()
{
  char buffer[300] = "";
  Sim_AT("AT+CLBS=1,1", 3000);
  //mySerial.readBytes(buffer, 300);
  //Xuất dữ liệu
  //Serial.println(buffer);
  Serial.println("xong");
}
void SendHost()
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
void SimCeng()
{
  char buffer[300] = "";
  //mySerial.println("AT+CENG=3,0");
  Sim_AT("AT+CENG=3,0");
  //delay(100);
  //mySerial.println("AT+CENG?");
  Sim_AT("AT+CENG?", 0);
  mySerial.readBytes(buffer, 300);

  //Xuất dữ liệu
  Serial.println(buffer);
  //sprintf(DATA, "DATA=%s", buffer);
  //Serial.println(DATA);
}
