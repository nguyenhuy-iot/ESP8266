#include "Sim800.h"
#include <SoftwareSerial.h>
#include <SerialCommand.h>

String apn = "m-wap";                        //APN
String url = "host123iot.000webhostapp.com"; //URL for HTTP-POST-REQUEST
char DATA[300] = "DATA=hello...";

char BTS_GSM[60] = "";           // AT+CENG?
SoftwareSerial mySerial(10, 16); // RX, TX Pins
SerialCommand SCmd;

void SimSetup()
{
  mySerial.begin(9600);
  Serial.begin(115200);

  SCmd.addCommand("SEND", SendHost);
  SCmd.addCommand("CENG", SimCeng);
  SCmd.addCommand("CSQ", Sim_CSQ);
  SCmd.addCommand("TKC", Sim_TKC);
  SCmd.addCommand("test", SimTest);
  SCmd.addDefaultHandler(unrecognized); // Handler for command that isn't matched  (says "What?")

  delay(3000);
  Serial.println("READY...");

  AT_sim("AT");
  AT_wait(1000);
  AT_sim("ATE0");
  AT_wait(1000);
  AT_sim("");
}
void SimRun()
{
  SCmd.readSerial(); // We don't do much, just process serial commands
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
  mySerial.setTimeout(3000);
  AT_sim("AT+CSQ");
  AT_wait(1000);
  AT_sim("AT+SAPBR=1,1");
  AT_wait(3000);
  AT_sim("AT+SAPBR=2,1");
  AT_wait(3000);
  AT_sim("AT+CLBS=1,1");
  AT_wait(60000);
  AT_sim("");

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
  mySerial.setTimeout(1000);
  AT_sim("AT+CENG=3,0");
  AT_wait(1000);
  AT_sim("AT+CENG?");
  AT_wait(1000);
  mySerial.find("CENG:");
  mySerial.find("+CENG: 0,");
  mySerial.readBytes(BTS_GSM, 24);
  mySerial.find("+CENG: 1,");
  mySerial.readBytes(&BTS_GSM[24], 24);
  Serial.print("BTS_GSM=");
  Serial.println(BTS_GSM);
  //AT_sim("");
  Serial.println("***");
}
void SimLBS()
{
}
void Sim_CSQ()
{
  long CSQ;
  mySerial.setTimeout(1000);
  AT_sim("AT+CSQ");
  AT_wait(1000);
  CSQ = mySerial.parseInt();
  Serial.print("CSQ=");
  Serial.println(CSQ);
  AT_sim("");
  Serial.println("***");
}
void Sim_TKC()
{
  long TKC;
  mySerial.setTimeout(10000);
  AT_sim("AT+CUSD=1,\"*101#\"");
  AT_wait(1000);
  mySerial.find("TKC");
  TKC = mySerial.parseInt();
  Serial.print("TKC=");
  Serial.println(TKC);
  //AT_sim("");
  Serial.println("***");
}
void AT_sim(const char *command)
{
  if (mySerial.available())
  {
    while (mySerial.available()) //xoa bo dem
    {
      Serial.write(mySerial.read()); // in
      delay(1);
    }
    Serial.println("");
  }
  Serial.print("Command:  ");
  Serial.println(command);
  mySerial.println(command); //gui lenh AT
}
void AT_find(const char *key, unsigned long wait)
{
  char keyword[] = "";
  strcpy(keyword, key);

  if (mySerial.available())
  {
    unsigned long time = millis();
    while (mySerial.available() == 0) //cho den khi nhan duoc du lieu
    {
      if (millis() - time > wait) //thoat khi qua thoi gian
      {
        break;
      }
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
void AT_wait(unsigned long wait)
{
  unsigned long time = millis();
  while (mySerial.available() == 0) //cho den khi nhan duoc du lieu
  {
    if (millis() - time > wait) //thoat khi qua thoi gian
    {
      break;
    }
  }
}
