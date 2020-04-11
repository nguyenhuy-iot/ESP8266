#include "Sim800.h"
#include <SoftwareSerial.h>
#include <SerialCommand.h>

String apn = "m-wap";                        //APN
String url = "host123iot.000webhostapp.com"; //URL for HTTP-POST-REQUEST
char DATA[300] = "DATA=hello...";

char BTS_GSM[200] = "";          // AT+CENG?
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
  while (mySerial.available()) //xoa bo dem
  {
    mySerial.read();
  }
  Serial.println(command);
  mySerial.println(command);
  if (wait < 100) //nhan du lieu ko can cho
  {
    delay(wait);
  }
  else
  {
    unsigned long time = millis();
    while (mySerial.available() == 0) //cho den khi nhan duoc du lieu
    {
      if (millis() - time > wait) //thoat khi qua thoi gian
      {
        break;
      }
    }
    if (wait <= 1000) //100<wait<=1000 cho Sim tra loi KO hoac ERROR
    {
      //mySerial.setTimeout(wait);
      if (mySerial.findUntil("OK\r\n", "ERROR"))
      {
        Serial.println("OK");
      }
    }
    else
    {
      Answer();
    }
  }
  //mySerial.setTimeout(1000);
}
void SimTest()
{
  char buffer[60] = ""; //27+27=54
  AT_sim("AT+CENG?");
  AT_find("CENG:", 10000); //bo qua  +CENG: 3,0
  mySerial.find("CENG:");   //+CENG: 0,"452,01,7c9e,c1fd,13,68"
  mySerial.readBytes(buffer, 27);
  mySerial.find("CENG:"); //+CENG: 1,
  mySerial.readBytes(&buffer[27], 27);
  Serial.println(buffer);
  Serial.println("...");
  AT_sim("AT");
  Serial.println("XONG");
}
void SendHost()
{
  mySerial.println("AT");
  delay(100);
  Answer();
  // mySerial.println("ATE1");
  // delay(100);
  // Answer();
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
  delay(10000);
  Answer();
  //mySerial.println("AT+HTTPREAD");
  //runsl();
  mySerial.println("AT+HTTPTERM");
  delay(500);
  Answer();
  //Sim_AT("ATE0");
}
void SimCeng()
{
  char buffer[7][27] = {"", "", "", "", "", "", ""};
  Sim_AT("AT+CENG=3,0", 500);
  Sim_AT("AT+CENG?", 0);   // +CENG: 0,"452,01,7c9e,c1fd,13,68"
  mySerial.find("+CENG:"); // bo qua  +CENG: 3,0
  for (uint8_t i = 0; i < 7; i++)
  {
    mySerial.find("+CENG:");
    mySerial.readBytes(buffer[i], 27);
    Serial.println(buffer[i]);
    //BTS_GSM = buffer;
  }
  Serial.println("BTS_GSM");
  sprintf(DATA, "DATA=%s", buffer);
  Serial.println(DATA);
}
void SimLBS()
{
}
void AT_sim(const char *command)
{
  while (mySerial.available()) //xoa bo dem
  {
    Serial.write(mySerial.read()); // in
    delay(1);
  }
  Serial.print("Command:  ");
  Serial.println(command);
  mySerial.println(command); //gui lenh AT
}
void AT_find(const char *key, uint16_t wait)
{
  char keyword[] = "";
  unsigned long time = millis();

  strcpy(keyword, key);
  while (mySerial.available() == 0) //cho den khi nhan duoc du lieu
  {
    if (millis() - time > wait) //thoat khi qua thoi gian
    {
      break;
    }
  }
  if (mySerial.find(keyword))
  {
    Serial.println(keyword);
  }
  else
  {
    Serial.println("Unknown");
  }
}
