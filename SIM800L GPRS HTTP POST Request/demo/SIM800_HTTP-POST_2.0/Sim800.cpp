#include "Sim800.h"
#include <SoftwareSerial.h>
#include <SerialCommand.h>
#include <string.h>

String apn = "m-wap";                        //APN
String url = "host123iot.000webhostapp.com"; //URL for HTTP-POST-REQUEST
//char DATA[300] = "DATA=hello...hello...!";

char BTS_GSM[60] = "";           // AT+CENG?
char CLOCK[20] = "";             //AT+CIPGSMLOC=2,1
SoftwareSerial mySerial(10, 16); // RX, TX Pins
SerialCommand SCmd;

void SimSetup()
{
  mySerial.begin(9600);
  Serial.begin(115200);

  SCmd.addCommand("SEND", SendHost);
  SCmd.addCommand("CENG", SimCeng);
  SCmd.addCommand("LBS", SimLBS);
  SCmd.addCommand("CLOCK", SimCLOCK);
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
  SimTest();
  delay(30000);
}
///////********************************/////////////
void unrecognized()
{
  Serial.println("What?");
}

//----------------------------------------------//

void SimTest()
{
  char send_data[100] = "DATA=";
  int n = 5;
  SimCLOCK();
  SimCeng();

  strcpy(&send_data[n], CLOCK);        // length=20
  strcpy(&send_data[n + 20], BTS_GSM); // length=48

  Serial.println(send_data);
  SendHost(send_data);
  Serial.println("XONG");
}
void SendHost(const char *DATA)
{
  int len;
  len = strlen(DATA);
  mySerial.setTimeout(30000);
  AT_sim("AT+SAPBR=1,1");
  AT_wait(3000);
  AT_sim("AT+SAPBR=2,1");
  AT_wait(3000);
  AT_sim("AT+HTTPINIT");
  AT_wait(3000);
  AT_sim("AT+HTTPPARA=CID,1");
  AT_wait(3000);
  AT_sim("");
  Serial.println("AT+HTTPPARA=URL," + url);
  mySerial.println("AT+HTTPPARA=URL," + url);
  AT_wait(3000);
  AT_sim("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
  AT_wait(3000);
  //AT_sim("AT+HTTPDATA=300,3000");
  AT_sim("");
  Serial.println(len);
  mySerial.print("AT+HTTPDATA=");
  mySerial.print(len);
  mySerial.println(",3000");

  mySerial.find("DOWNLOAD"); //DOWNLOAD
  mySerial.print(DATA);      //nhap du lieu
  Serial.println(DATA);
  mySerial.find("OK");
  AT_sim("AT+HTTPACTION=1");
  if (mySerial.find(",200")) //+HTTPACTION: 0,200,4
  {
    Serial.println("---SEND HOST OK---");
  }
  AT_sim("AT+HTTPTERM");
  AT_wait(3000);
  //AT_sim("");
  Serial.println("***");
}
void SimCeng()
{
  mySerial.setTimeout(1000);
  AT_sim("AT+CENG=3,0");
  AT_wait(1000);
  AT_sim("AT+CENG?"); //+CENG: 0,"452,01,7c9e,c032,21,42"
  AT_wait(1000);
  mySerial.find("CENG:");
  mySerial.find("+CENG: 0,");
  mySerial.readBytes(BTS_GSM, 24);
  mySerial.find("+CENG: 1,");
  mySerial.readBytes(&BTS_GSM[24], 24);
  //BTS_GSM[48] = '\0';
  Serial.print("BTS_GSM=");
  Serial.println(BTS_GSM);
  //AT_sim("");
  Serial.println("***");
}
void SimCLOCK()
{
  mySerial.setTimeout(1000);
  AT_sim("AT+SAPBR=1,1");
  AT_wait(3000);
  AT_sim("AT+SAPBR=2,1");
  AT_wait(3000);
  AT_sim("AT+CIPGSMLOC=2,1"); //+CIPGSMLOC: 0,2020/04/12,05:57:48
  AT_wait(10000);
  mySerial.find(": 0,");
  mySerial.readBytes(CLOCK, 20);
  //CLOCK[20] = '\0';
  Serial.print("CLOCK=");
  Serial.println(CLOCK);
  //AT_sim("");

  Serial.println("***");
}
void SimLBS()
{
  mySerial.setTimeout(3000);
  AT_sim("AT+SAPBR=1,1");
  AT_wait(3000);
  AT_sim("AT+SAPBR=2,1");
  AT_wait(3000);
  AT_sim("AT+CLBS=1,1");
  AT_wait(60000);
  AT_sim("");

  Serial.println("*...*");
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
