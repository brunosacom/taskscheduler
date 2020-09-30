/* BMBS - Bruno Sá - www.bruno-sa.com */


#include <ezTime.h>
#include <Ethernet.h>
#include <EEPROM.h>
#include <SPI.h>

#define MaxHeaderLength 255 //maximum length of http header required

// EEPROM size
#define espacoEEPROM 4096
/*
1024 bytes (1Kb) on the ATmega328P,
512 bytes on the ATmega168 and ATmega8,
4096 bytes (4 Kb) on the ATmega1280 and ATmega2560
*/

//reset function
void (*resetFunc)(void) = 0; //declare reset function at address 0

// function write string byte after byte in EEPROM
void eepromWriteStr(int eepromBlock, String text)
{
  int i;

  for (i = 0; i < text.length(); i++)
  {
    EEPROM.write(eepromBlock + i, text.charAt(i));

    if (i >= 1000)
    {
      break;
    }
  }

  if (i < 1000)
  {
    EEPROM.write(eepromBlock + i, (char)0);
  }
}

// function read string byte after byte from EEPROM
String eepromReadStr(int eepromBlock)
{
  String readStr = "";
  char readByte;
  int readEepromBlock = eepromBlock;

  do
  {
    readByte = EEPROM.read(readEepromBlock);
    readStr += readByte;
    readEepromBlock++;
  } while ((readByte != (char)0) && (readEepromBlock < (eepromBlock + 1000)));

  return readStr;
}

//BMBS IPconfig presence variables
int variavelCFGpos = 1;
String variavelCFG = "";
int variavelTSKpos;
String variavelTSK = "";

//BMBS IP Address from EEPROM
byte variavelIP1 = EEPROM.read(51);
byte variavelIP2 = EEPROM.read(52);
byte variavelIP3 = EEPROM.read(53);
byte variavelIP4 = EEPROM.read(54);

//BMBS Subnet Mask from EEPROM
byte variavelSB1 = EEPROM.read(56);
byte variavelSB2 = EEPROM.read(57);
byte variavelSB3 = EEPROM.read(58);
byte variavelSB4 = EEPROM.read(59);

//BMBS Gateway from EEPROM
byte variavelGW1 = EEPROM.read(61);
byte variavelGW2 = EEPROM.read(62);
byte variavelGW3 = EEPROM.read(63);
byte variavelGW4 = EEPROM.read(64);

//BMBS DNS from EEPROM
byte variavelDN1 = EEPROM.read(66);
byte variavelDN2 = EEPROM.read(67);
byte variavelDN3 = EEPROM.read(68);
byte variavelDN4 = EEPROM.read(69);

// IP config defaults
byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; //physical mac address
byte ip[] = {
    192, 168, 0, 95}; // static ip of Arduino
byte subnet[] = {
    255, 255, 255, 0}; //subnet mask
byte gateway[] = {
    192, 168, 0, 1}; // gateway address
byte dns[] = {
    192, 168, 0, 1}; // dns address

EthernetServer server(80); //web server port

String HttpHeader = String(MaxHeaderLength);

//BMBS declare HTTP string variables from form
String HttpHeaderCFG;
String HttpHeaderTSK;

String HttpHeaderIP1;
String HttpHeaderIP2;
String HttpHeaderIP3;
String HttpHeaderIP4;

String HttpHeaderSB1;
String HttpHeaderSB2;
String HttpHeaderSB3;
String HttpHeaderSB4;

String HttpHeaderGW1;
String HttpHeaderGW2;
String HttpHeaderGW3;
String HttpHeaderGW4;

String HttpHeaderDN1;
String HttpHeaderDN2;
String HttpHeaderDN3;
String HttpHeaderDN4;

String HttpHeaderY0;

//BMBS declare HTTP byte variables to write in EEPROM
byte HttpHeaderCFGbyte;
byte HttpHeaderTSKbyte;

byte HttpHeaderIP1byte;
byte HttpHeaderIP2byte;
byte HttpHeaderIP3byte;
byte HttpHeaderIP4byte;

byte HttpHeaderSB1byte;
byte HttpHeaderSB2byte;
byte HttpHeaderSB3byte;
byte HttpHeaderSB4byte;

byte HttpHeaderGW1byte;
byte HttpHeaderGW2byte;
byte HttpHeaderGW3byte;
byte HttpHeaderGW4byte;

byte HttpHeaderDN1byte;
byte HttpHeaderDN2byte;
byte HttpHeaderDN3byte;
byte HttpHeaderDN4byte;

byte HttpHeaderTAbyte;
byte HttpHeaderY0byte;
byte HttpHeaderYEbyte;
byte HttpHeaderMObyte;
byte HttpHeaderDAbyte;
byte HttpHeaderHObyte;
byte HttpHeaderMIbyte;
byte HttpHeaderSEbyte;
byte HttpHeaderSTSbyte;
byte HttpHeaderSUNbyte;
byte HttpHeaderMONbyte;
byte HttpHeaderTUEbyte;
byte HttpHeaderWEDbyte;
byte HttpHeaderTHUbyte;
byte HttpHeaderFRIbyte;
byte HttpHeaderSATbyte;

//BMBS declare NTP byte variables to write in EEPROM
const String n0 = "0";
const byte year0 = EEPROM.read(100);

String taskScheduleComplete (int nTask){
  return ((String) year0 + twoDigits(EEPROM.read(93 + nTask * 20)) + "-" + twoDigits(EEPROM.read(94 + nTask * 20)) + "-" + twoDigits(EEPROM.read(95 + nTask * 20)) + " " + twoDigits(EEPROM.read(96 + nTask * 20)) + ":" + twoDigits(EEPROM.read(97 + nTask * 20)) + ":" + twoDigits(EEPROM.read(98 + nTask * 20)));
};

String taskScheduleDisplay (int nTask){
  String tskDate = ((String) year0 + twoDigits(EEPROM.read(93 + nTask * 20)) + "-" + twoDigits(EEPROM.read(94 + nTask * 20)) + "-" + twoDigits(EEPROM.read(95 + nTask * 20)));
  String tskTime = ((String) twoDigits(EEPROM.read(96 + nTask * 20)) + ":" + twoDigits(EEPROM.read(97 + nTask * 20)) + ":" + twoDigits(EEPROM.read(98 + nTask * 20)));
  String tskDateTime = tskDate + " " + tskTime;
  if (EEPROM.read(100 + nTask * 20) == 8){
    return tskTime;
  }

  return tskDateTime;
};

String daysOfWeekComplete (int nTask){
  return ((String) EEPROM.read(100 + nTask * 20) + EEPROM.read(101 + nTask * 20) + EEPROM.read(102 + nTask * 20) + EEPROM.read(103 + nTask * 20) + EEPROM.read(104 + nTask * 20) + EEPROM.read(105 + nTask * 20) + EEPROM.read(106 + nTask * 20) + EEPROM.read(107 + nTask * 20));
};

String taskSchedule01Str = "";
String taskSchedule02Str = "";
String taskSchedule03Str = "";

String taskSchedule01Display = "";
String taskSchedule02Display = "";
String taskSchedule03Display = "";

String daysOfWeek01Str = "";
String daysOfWeek02Str = "";
String daysOfWeek03Str = "";

byte task01 = EEPROM.read(111);

String sts01;
String sun01;
String mon01;
String tue01;
String wed01;
String thu01;
String fri01;
String sat01;

byte task02 = EEPROM.read(131);

String sts02;
String sun02;
String mon02;
String tue02;
String wed02;
String thu02;
String fri02;
String sat02;

//BMBS function datetime pre n0
 String twoDigits (byte fieldA)
 {
    String twoDigitsStr = "";

    if (fieldA < 10){
      twoDigitsStr = n0 + fieldA;
      } else {
      twoDigitsStr = fieldA;
      }
    return twoDigitsStr;
 };

//BMBS function HTTP GET byte from form
byte HttpHeaderValue(String fieldA)
{
  String HttpHeaderVAL;
  byte HttpHeaderVALbyte;
  HttpHeaderVAL = HttpHeader;
  HttpHeaderVAL.remove(0, HttpHeaderVAL.indexOf(fieldA) + fieldA.length());
  HttpHeaderVAL.remove(HttpHeaderVAL.indexOf("&"), HttpHeaderVAL.length() - HttpHeaderVAL.indexOf("&") + 1);
  HttpHeaderVALbyte = HttpHeaderVAL.toInt();
  return HttpHeaderVALbyte;
};

//BMBS function dayOfWeek and Status HTMLcode
String dayOfWeekValue(byte dow){
  String dayOfWeekVAL;
  if (dow == 48){
    dayOfWeekVAL = " text-muted";
  }
  return dayOfWeekVAL;
};

String statusValue(byte sts){
  String statusVAL;
  if (sts == 0){
    statusVAL = " text-muted'>off";
  } else {
    statusVAL = "'>on";
  }
  return statusVAL;
};


//BMBS /ipcfg repeating HTML codes
String divRow = "<div class='form-row my-2'><div class='col-md-3 col-xs-12 text-nowrap'>";
String divClassInput0 = "<div class='col-2'><input class='form-control form-control-sm' type='number' size='3' max='255' name='";
String divClassInput1 = "' value='";
String divClassInput2 = "'></div>.";

void setup()
{
  variavelCFG = eepromReadStr(variavelCFGpos);
  //variavelCFG = ((String)char(EEPROM.read(1)) + char(EEPROM.read(2)) + char(EEPROM.read(3)) + char(EEPROM.read(4)) + char(EEPROM.read(5)) + char(EEPROM.read(6)) + char(EEPROM.read(7)) + char(EEPROM.read(8)) + char(EEPROM.read(9)) + char(EEPROM.read(10))); 

  //BMBS check previous BMBIPCONFIG
  if (variavelCFG == "BMB_ipconf")
  {
    for (int i = 0; i < 4; i++)
    {
      ip[i] = EEPROM.read(i + 51);
    }
    for (int i = 0; i < 4; i++)
    {
      subnet[i] = EEPROM.read(i + 56);
    }
    for (int i = 0; i < 4; i++)
    {
      gateway[i] = EEPROM.read(i + 61);
    }
    for (int i = 0; i < 4; i++)
    {
      dns[i] = EEPROM.read(i + 66);
    }
  }
  else
  {
    eepromWriteStr(variavelCFGpos, "BMB_ipconf");
    /*EEPROM.write(1, 66);
    EEPROM.write(2, 77);
    EEPROM.write(3, 66);
    EEPROM.write(4, 95);
    EEPROM.write(5, 105);
    EEPROM.write(6, 112);
    EEPROM.write(7, 99);
    EEPROM.write(8, 111);
    EEPROM.write(9, 110);
    EEPROM.write(10, 102);*/
  };

  //enable serial monitor
  Serial.begin(9600);
  //while (!Serial) { ; }

  //start Ethernet
  Ethernet.begin(mac, ip, dns, gateway, subnet);

  Serial.println(Ethernet.localIP());
          Serial.println(Ethernet.dnsServerIP());

  //initialize variable
  HttpHeader = "";
}

void loop()
{
  
  Timezone Bembos;
  Bembos.setPosix("3:00");

  taskSchedule01Str = taskScheduleComplete(task01);
  taskSchedule02Str = taskScheduleComplete(task02);
  taskSchedule03Str = taskScheduleComplete(3);

  taskSchedule01Display = taskScheduleDisplay(task01);
  taskSchedule02Display = taskScheduleDisplay(task02);
  taskSchedule03Display = taskScheduleDisplay(3);

  daysOfWeek01Str = daysOfWeekComplete(task01);
  daysOfWeek02Str = daysOfWeekComplete(task02);
  daysOfWeek03Str = daysOfWeekComplete(3);
  
  // Create a client connection
  EthernetClient client = server.available();
  if (client)
  {
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        //read MaxHeaderLength number of characters in the HTTP header
        //discard the rest until \n
        if (HttpHeader.length() < MaxHeaderLength)
        {
          //store characters to string
          HttpHeader = HttpHeader + c;
        }

        //if HTTP request has ended
        if (c == '\n')
        {

          HttpHeaderCFG = HttpHeader;
          HttpHeaderCFG.remove(HttpHeader.indexOf("I1=") - 1, HttpHeader.length() - HttpHeader.indexOf("I1="));
          HttpHeaderCFG.remove(0, HttpHeader.indexOf("CF=") + 3);
          HttpHeaderCFGbyte = HttpHeaderCFG[0];

          HttpHeaderTSK = HttpHeader;
          HttpHeaderTSK.remove(HttpHeader.indexOf("TA=") - 1, HttpHeader.length() - HttpHeader.indexOf("TA="));
          HttpHeaderTSK.remove(0, HttpHeader.indexOf("TS=") + 3);
          HttpHeaderTSKbyte = HttpHeaderTSK[0];

          HttpHeaderIP1byte = HttpHeaderValue("I1=");
          HttpHeaderIP2byte = HttpHeaderValue("I2=");
          HttpHeaderIP3byte = HttpHeaderValue("I3=");
          HttpHeaderIP4byte = HttpHeaderValue("I4=");

          HttpHeaderSB1byte = HttpHeaderValue("S1=");
          HttpHeaderSB2byte = HttpHeaderValue("S2=");
          HttpHeaderSB3byte = HttpHeaderValue("S3=");
          HttpHeaderSB4byte = HttpHeaderValue("S4=");

          HttpHeaderGW1byte = HttpHeaderValue("G1=");
          HttpHeaderGW2byte = HttpHeaderValue("G2=");
          HttpHeaderGW3byte = HttpHeaderValue("G3=");
          HttpHeaderGW4byte = HttpHeaderValue("G4=");

          HttpHeaderDN1byte = HttpHeaderValue("D1=");
          HttpHeaderDN2byte = HttpHeaderValue("D2=");
          HttpHeaderDN3byte = HttpHeaderValue("D3=");
          HttpHeaderDN4byte = HttpHeaderValue("D4=");

          HttpHeaderTAbyte = HttpHeaderValue("TA=");
          HttpHeaderY0byte = HttpHeaderValue("Y0=");
          HttpHeaderYEbyte = HttpHeaderValue("YE=");
          HttpHeaderMObyte = HttpHeaderValue("MO=");
          HttpHeaderDAbyte = HttpHeaderValue("DA=");
          HttpHeaderHObyte = HttpHeaderValue("HO=");
          HttpHeaderMIbyte = HttpHeaderValue("MI=");
          HttpHeaderSEbyte = HttpHeaderValue("SE=");
          HttpHeaderSTSbyte = HttpHeaderValue("sts=");
          HttpHeaderSUNbyte = HttpHeaderValue("sun=");
          HttpHeaderMONbyte = HttpHeaderValue("mon=");
          HttpHeaderTUEbyte = HttpHeaderValue("tue=");
          HttpHeaderWEDbyte = HttpHeaderValue("wed=");
          HttpHeaderTHUbyte = HttpHeaderValue("thu=");
          HttpHeaderFRIbyte = HttpHeaderValue("fri=");
          HttpHeaderSATbyte = HttpHeaderValue("sat=");

          sts01 = statusValue(EEPROM.read(112));
          sun01 = dayOfWeekValue(daysOfWeek01Str[1]);
          mon01 = dayOfWeekValue(daysOfWeek01Str[2]);
          tue01 = dayOfWeekValue(daysOfWeek01Str[3]);
          wed01 = dayOfWeekValue(daysOfWeek01Str[4]);
          thu01 = dayOfWeekValue(daysOfWeek01Str[5]);
          fri01 = dayOfWeekValue(daysOfWeek01Str[6]);
          sat01 = dayOfWeekValue(daysOfWeek01Str[7]);

          sts02 = statusValue(EEPROM.read(132));
          sun02 = dayOfWeekValue(daysOfWeek02Str[1]);
          mon02 = dayOfWeekValue(daysOfWeek02Str[2]);
          tue02 = dayOfWeekValue(daysOfWeek02Str[3]);
          wed02 = dayOfWeekValue(daysOfWeek02Str[4]);
          thu02 = dayOfWeekValue(daysOfWeek02Str[5]);
          fri02 = dayOfWeekValue(daysOfWeek02Str[6]);
          sat02 = dayOfWeekValue(daysOfWeek02Str[7]);

          Serial.println(Ethernet.localIP());
          Serial.println(Ethernet.dnsServerIP());
          Serial.println("UTC: " + UTC.dateTime());
          Serial.println("BEMBOS: " + Bembos.dateTime("Y-m-d H:i:s.v"));
          Serial.print("variavelCFG: ");
          Serial.println(variavelCFG);
          Serial.print("Length: ");
          Serial.println(HttpHeader.length());
          Serial.print("HttpHeader: ");
          Serial.println(HttpHeader);
          Serial.print("HttpHeaderTSK: ");
          Serial.println(HttpHeaderTSK);
          Serial.print("taskSchedule01Str: ");
          Serial.println(taskSchedule01Str);
          Serial.print("taskSchedule02Str: ");
          Serial.println(taskSchedule02Str);
          Serial.print("taskSchedule03Str: ");
          Serial.println(taskSchedule03Str);
          Serial.print("taskSchedule01Display: ");
          Serial.println(taskSchedule01Display);
          Serial.print("taskSchedule02Display: ");
          Serial.println(taskSchedule02Display);
          Serial.print("taskSchedule03Display: ");
          Serial.println(taskSchedule03Display);
          Serial.print("daysOfWeek01Str: ");
          Serial.println(daysOfWeek01Str);
          Serial.print("daysOfWeek02Str: ");
          Serial.println(daysOfWeek02Str);
          Serial.print("daysOfWeek03Str: ");
          Serial.println(daysOfWeek03Str);
          Serial.print("daysOfWeek01Str[5]: ");
          Serial.println(daysOfWeek01Str[5]);
          Serial.print("daysOfWeek02Str[1]: ");
          Serial.println(daysOfWeek02Str[1]);
          Serial.print("daysOfWeek03Str[4]: ");
          Serial.println(daysOfWeek03Str[4]);

          //BMBS web page's header
          client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: text/html"));
          client.println(F(""));
          client.println(F("<!DOCTYPE HTML>"));
          client.println(F("<html lang='en'>"));
          client.println(F("<head>"));
          client.println(F("<meta charset='utf-8'>"));
          client.println(F("<meta name='author' content='Bruno Sá - www.bruno-sa.com'>"));
          client.println(F("<meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'>"));
          client.println(F("<link href='https://fonts.googleapis.com/css?family=Didact Gothic' rel='stylesheet'>"));
          client.println(F("<!-- Bootstrap CSS -->"));
          client.println(F("<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css' integrity='sha384-JcKb8q3iqJ61gNV9KGb8thSsNjpSL0n8PARn9HuZOnIxN0hoP+VmmDGMN5t9UJ0Z' crossorigin='anonymous'>"));
          client.println(F("<!-- jQuery UI CSS -->"));
          client.println(F("<link rel='stylesheet' href='https://code.jquery.com/ui/1.12.1/themes/base/jquery-ui.css'>"));

          //BMBS internal page: /ipcfg
          //character conversion in https://www.arduino.cc/en/Reference/ASCIIchart
          if (HttpHeader[5] == 105 && HttpHeader[6] == 112 && HttpHeader[7] == 99 && HttpHeader[8] == 102 && HttpHeader[9] == 103)
          {
            client.println(F("<title>IP Config</title>"));
            client.println(F("</head>"));
            client.println(F("<body style='font-family:Didact Gothic; color:#FFF; background-color:#333;'><div class='container'><h2><strong>IP Config Page</strong></h2>"));
            client.println(F("<form><input type='hidden' name='CF' value='BMB_ipconf'>"));
            client.print(divRow);
            client.print(F("IP Address: </div>"));
            client.print(divClassInput0);
            client.print(F("I1"));
            client.print(divClassInput1);
            client.print(ip[0]);
            client.print(divClassInput2);
            client.print(divClassInput0);
            client.print(F("I2"));
            client.print(divClassInput1);
            client.print(ip[1]);
            client.print(divClassInput2);
            client.print(divClassInput0);
            client.print(F("I3"));
            client.print(divClassInput1);
            client.print(ip[2]);
            client.print(divClassInput2);
            client.print(divClassInput0);
            client.print(F("I4"));
            client.print(divClassInput1);
            client.print(ip[3]);
            client.println(F("'></div></div>"));
            client.print(divRow);
            client.print(F("Subnet Mask: </div>"));
            client.print(divClassInput0);
            client.print(F("S1"));
            client.print(divClassInput1);
            client.print(subnet[0]);
            client.print(divClassInput2);
            client.print(divClassInput0);
            client.print(F("S2"));
            client.print(divClassInput1);
            client.print(subnet[1]);
            client.print(divClassInput2);
            client.print(divClassInput0);
            client.print(F("S3"));
            client.print(divClassInput1);
            client.print(subnet[2]);
            client.print(divClassInput2);
            client.print(divClassInput0);
            client.print(F("S4"));
            client.print(divClassInput1);
            client.print(subnet[3]);
            client.println(F("'></div></div>"));
            client.print(divRow);
            client.print(F("Gateway: </div>"));
            client.print(divClassInput0);
            client.print(F("G1"));
            client.print(divClassInput1);
            client.print(gateway[0]);
            client.print(divClassInput2);
            client.print(divClassInput0);
            client.print(F("G2"));
            client.print(divClassInput1);
            client.print(gateway[1]);
            client.print(divClassInput2);
            client.print(divClassInput0);
            client.print(F("G3"));
            client.print(divClassInput1);
            client.print(gateway[2]);
            client.print(divClassInput2);
            client.print(divClassInput0);
            client.print(F("G4"));
            client.print(divClassInput1);
            client.print(gateway[3]);
            client.println(F("'></div></div>"));
            client.print(divRow);
            client.print(F("DNS: </div>"));
            client.print(divClassInput0);
            client.print(F("D1"));
            client.print(divClassInput1);
            client.print(dns[0]);
            client.print(divClassInput2);
            client.print(divClassInput0);
            client.print(F("D2"));
            client.print(divClassInput1);
            client.print(dns[1]);
            client.print(divClassInput2);
            client.print(divClassInput0);
            client.print(F("D3"));
            client.print(divClassInput1);
            client.print(dns[2]);
            client.print(divClassInput2);
            client.print(divClassInput0);
            client.print(F("D4"));
            client.print(divClassInput1);
            client.print(dns[3]);
            client.println(F("'></div></div>"));
            client.println(F("<div class='form-row my-2'><input class='btn btn-warning btn-sm' type='submit' value='submit'></div></form>"));
          }

          //BMBS internal page: /schdl
          //character conversion in https://www.arduino.cc/en/Reference/ASCIIchart
          else if (HttpHeader[5] == 115 && HttpHeader[6] == 99 && HttpHeader[7] == 104 && HttpHeader[8] == 100 && HttpHeader[9] == 108)
          {
            client.println(F("<title>Task Scheduler</title>"));
            client.println(F("</head>"));
            client.println(F("<body style='font-family:Didact Gothic; color:#FFF; background-color:#333;'><div class='container'><h2><strong>Task Scheduler</strong></h2>"));
            client.println(F("<div class='row my-2 bg-secondary'>"));
            client.println(F("<div class='col-1'><strong>task</strong></div>"));
            client.println(F("<div class='col-4'><strong>timestamp</strong></div>"));
            client.println(F("<div class='col-6'><strong>day of week</strong></div>"));
            client.println(F("<div class='col-1'><strong>status</strong></div>"));
            client.println(F("</div>"));
            client.println(F("<div class='row my-2'>"));
            client.println((String)"<div class='col-1'>" + task01 + "</div>");
            client.println((String)"<div class='col-4'>" + taskSchedule01Display + "</div>");
            client.println("<div class='col-6'>");
            client.println("    <div class='row'>");
            client.println("        <div class='col" + sun01 + "'>S</div>");
            client.println("        <div class='col" + mon01 + "'>M</div>");
            client.println("        <div class='col" + tue01 + "'>T</div>");
            client.println("        <div class='col" + wed01 + "'>W</div>");
            client.println("        <div class='col" + thu01 + "'>T</div>");
            client.println("        <div class='col" + fri01 + "'>F</div>");
            client.println("        <div class='col" + sat01 + "'>S</div>");
            client.println("    </div>");
            client.println("</div>");
            client.println("<div class='col-1" + sts01 + "</div>");
            client.println("</div>");
             client.println(F("<div class='row my-2'>"));
            client.println((String)"<div class='col-1'>" + task02 + "</div>");
            client.println((String)"<div class='col-4'>" + taskSchedule02Display + "</div>");
            client.println("<div class='col-6'>");
            client.println("    <div class='row'>");
            client.println("        <div class='col" + sun02 + "'>S</div>");
            client.println("        <div class='col" + mon02 + "'>M</div>");
            client.println("        <div class='col" + tue02 + "'>T</div>");
            client.println("        <div class='col" + wed02 + "'>W</div>");
            client.println("        <div class='col" + thu02 + "'>T</div>");
            client.println("        <div class='col" + fri02 + "'>F</div>");
            client.println("        <div class='col" + sat02 + "'>S</div>");
            client.println("    </div>");
            client.println("</div>");
            client.println("<div class='col-1" + sts02 + "</div>");
            client.println("</div><br><br>");
            client.println(F("<h5>Edit Task</h5>"));           
            client.println("<form><input type='hidden' name='TS' value='BMB_tsksch'><input type='hidden' name='Y0' value='20'>");
            client.println("  <div class='row my-2 bg-secondary'>");
            client.println("      <div class='col-4'>task</div>");
            client.println("      <div class='col-4'>time (H:M:S)</div>");
            client.println("      <div class='col'>status</div>");
            client.println("  </div>");
            client.println("  <div class='row my-2'>");
            client.println("      <div class='col-4'>");
            client.println("          <div class='col-8'><input class='form-control form-control-sm' type='number' size='2' min='1' max='10' name='TA'></div>");
            client.println("      </div>");
            client.println("      <div class='col-4'>");
            client.println("          <div class='row text-nowrap'><input class='form-control form-control-sm col-3' type='number' size='2' max='23' name='HO'> : <input class='form-control form-control-sm col-3' type='number' size='2' max='59' name='MI'> : <input class='form-control form-control-sm col-3' type='number' size='2' max='59' name='SE'></div>");
            client.println("      </div>");
            client.println("      <div class='col'>");
            client.println("          <div class='custom-control custom-switch'>");
            client.println("              <input type='checkbox' class='custom-control-input' id='sts' name='sts' value='9'><label class='custom-control-label' for='sts'></label>");
            client.println("          </div>");
            client.println("      </div>");
            client.println("  </div>");
            client.println("  <div class='row my-2 bg-secondary'>");
            client.println("      <div class='col-4'>date (YY-MM-DD)</div>");
            client.println("      <div class='col-6 text-nowrap'>day of week</div>");
            client.println("  </div>");
            client.println("  <div class='row'>");
            client.println("      <div class='col-4'>");
            client.println("          <div class='row'><input class='form-control form-control-sm col-3' type='number' size='2' max='99' name='YE'> - <input class='form-control form-control-sm col-3' type='number' size='2' max='12' name='MO'> - <input class='form-control form-control-sm col-3' type='number' size='2' max='31' name='DA'></div>");
            client.println("      </div>");
            client.println("      <div class='col-6 text-wrap'>");
            client.println("          <div class='row'>");
            client.println("              <div class='col custom-control custom-checkbox'>");
            client.println("                  <input type='checkbox' class='custom-control-input' id='sun' name='sun' value='1'>");
            client.println("                  <label class='custom-control-label' for='sun'>S</label>");
            client.println("              </div>");
            client.println("              <div class='col custom-control custom-checkbox'>");
            client.println("                  <input type='checkbox' class='custom-control-input' id='mon' name='mon' value='2'>");
            client.println("                  <label class='custom-control-label' for='mon'>M</label>");
            client.println("              </div>");
            client.println("              <div class='col custom-control custom-checkbox'>");
            client.println("                  <input type='checkbox' class='custom-control-input' id='tue' name='tue' value='3'>");
            client.println("                  <label class='custom-control-label' for='tue'>T</label>");
            client.println("              </div>");
            client.println("              <div class='col custom-control custom-checkbox'>");
            client.println("                  <input type='checkbox' class='custom-control-input' id='wed' name='wed' value='4'>");
            client.println("                  <label class='custom-control-label' for='wed'>W</label>");
            client.println("              </div>");
            client.println("              <div class='col custom-control custom-checkbox'>");
            client.println("                  <input type='checkbox' class='custom-control-input' id='thu' name='thu' value='5'>");
            client.println("                  <label class='custom-control-label' for='thu'>T</label>");
            client.println("              </div>");
            client.println("              <div class='col custom-control custom-checkbox'>");
            client.println("                  <input type='checkbox' class='custom-control-input' id='fri' name='fri' value='6'>");
            client.println("                  <label class='custom-control-label' for='fri'>F</label>");
            client.println("              </div>");
            client.println("              <div class='col custom-control custom-checkbox'>");
            client.println("                  <input type='checkbox' class='custom-control-input' id='sat' name='sat' value='7'>");
            client.println("                  <label class='custom-control-label' for='sat'>S</label>");
            client.println("              </div>");
            client.println("          </div>");
            client.println("      </div>");
            client.println("      <div class='col-1 text-nowrap'>");
            client.println("          <input class='btn btn-warning btn-sm' type='submit' value='submit'>");
            client.println("      </div>");
            client.println("    </div>");
            client.println("  </div>");
            client.println("</form><br>");
          }

          //BMBS default internal page: /
          else
          {
            client.println(F("<title>Home</title>"));
            client.println(F("</head>"));
            client.println(F("<body style='font-family:Didact Gothic; color:#FFF; background-color:#333;'><div class='container'><h2><strong>Home Page</strong></h2>"));
          }
          //BMBS web page's footer
          client.println(F("<div class='row justify-content-center'><div><a href='/'>home</a> | <a href='/ipcfg'>IP config</a> | <a href='/schdl'>task scheduler</a></div></div>"));
          client.println(F("</div>"));
          client.println(F("<!-- Optional JavaScript -->"));
          client.println(F("<!-- jQuery first, then Popper.js, then Bootstrap JS -->"));
          client.println(F("<script src='https://code.jquery.com/jquery-3.5.1.js' integrity='sha256-QWo7LDvxbWT2tbbQ97B53yJnYU3WhH/C8ycbRAkjPDc=' crossorigin='anonymous'></script>"));
          client.println(F("<script src='https://code.jquery.com/ui/1.12.1/jquery-ui.js' integrity='sha256-T0Vest3yCU7pafRw9r+settMBX6JkKN06dqBnpQ8d30=' crossorigin='anonymous'></script>"));
          client.println(F("<script src='https://cdn.jsdelivr.net/npm/jquery-ui-touch-punch@0.2.3/jquery.ui.touch-punch.js' integrity='sha256-S9605h/+fTHx8kE89v4NQWuTGCEQJF0B9UGvMFYAiO8=' crossorigin='anonymous'></script>"));
          client.println(F("<script src='https://unpkg.com/@popperjs/core@2'></script>"));
          client.println(F("<script src='https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js' integrity='sha384-B4gt1jrGC7Jh4AgTPSdUtOBvfO8shuf57BaghqFfPlYxofvL8/KUEfYiJOMMV+rV' crossorigin='anonymous'></script>"));
          client.println(F("</body></html>"));

          //BMBS checking previous IP config to overwrite
          if (HttpHeaderCFG[0] == 66 && HttpHeaderCFG[1] == 77 && HttpHeaderCFG[2] == 66 && HttpHeaderCFG[3] == 95 && HttpHeaderCFG[4] == 105 && HttpHeaderCFG[5] == 112 && HttpHeaderCFG[6] == 99 && HttpHeaderCFG[7] == 111 && HttpHeaderCFG[8] == 110 && HttpHeaderCFG[9] == 102)
          {
            EEPROM.write(51, HttpHeaderIP1byte);
            EEPROM.write(52, HttpHeaderIP2byte);
            EEPROM.write(53, HttpHeaderIP3byte);
            EEPROM.write(54, HttpHeaderIP4byte);

            EEPROM.write(56, HttpHeaderSB1byte);
            EEPROM.write(57, HttpHeaderSB2byte);
            EEPROM.write(58, HttpHeaderSB3byte);
            EEPROM.write(59, HttpHeaderSB4byte);

            EEPROM.write(61, HttpHeaderGW1byte);
            EEPROM.write(62, HttpHeaderGW2byte);
            EEPROM.write(63, HttpHeaderGW3byte);
            EEPROM.write(64, HttpHeaderGW4byte);

            EEPROM.write(66, HttpHeaderDN1byte);
            EEPROM.write(67, HttpHeaderDN2byte);
            EEPROM.write(68, HttpHeaderDN3byte);
            EEPROM.write(69, HttpHeaderDN4byte);
            HttpHeaderCFG = "";
            resetFunc();
          };


          //BMBS checking previous Task Schedule to overwrite
          if (HttpHeaderTSK[0] == 66 && HttpHeaderTSK[1] == 77 && HttpHeaderTSK[2] == 66 && HttpHeaderTSK[3] == 95 && HttpHeaderTSK[4] == 116 && HttpHeaderTSK[5] == 115 && HttpHeaderTSK[6] == 107 && HttpHeaderTSK[7] == 115 && HttpHeaderTSK[8] == 99 && HttpHeaderTSK[9] == 104)
          {
            int HttpHeaderTAint = HttpHeaderTAbyte;
            
            EEPROM.write(91 + HttpHeaderTAint * 20, HttpHeaderTAbyte);
            EEPROM.write(92 + HttpHeaderTAint * 20, HttpHeaderSTSbyte);
            EEPROM.write(93 + HttpHeaderTAint * 20, HttpHeaderYEbyte);
            EEPROM.write(94 + HttpHeaderTAint * 20, HttpHeaderMObyte);
            EEPROM.write(95 + HttpHeaderTAint * 20, HttpHeaderDAbyte);
            EEPROM.write(96 + HttpHeaderTAint * 20, HttpHeaderHObyte);
            EEPROM.write(97 + HttpHeaderTAint * 20, HttpHeaderMIbyte);
            EEPROM.write(98 + HttpHeaderTAint * 20, HttpHeaderSEbyte);

            if (HttpHeaderSUNbyte == 0 && HttpHeaderMONbyte == 0 && HttpHeaderTUEbyte == 0 && HttpHeaderWEDbyte == 0 && HttpHeaderTHUbyte == 0 && HttpHeaderFRIbyte == 0 && HttpHeaderSATbyte == 0){
              EEPROM.write(100 + HttpHeaderTAint * 20, 0);
            } else {
              EEPROM.write(100 + HttpHeaderTAint * 20, 8);
            }

            EEPROM.write(101 + HttpHeaderTAint * 20, HttpHeaderSUNbyte);
            EEPROM.write(102 + HttpHeaderTAint * 20, HttpHeaderMONbyte);
            EEPROM.write(103 + HttpHeaderTAint * 20, HttpHeaderTUEbyte);
            EEPROM.write(104 + HttpHeaderTAint * 20, HttpHeaderWEDbyte);
            EEPROM.write(105 + HttpHeaderTAint * 20, HttpHeaderTHUbyte);
            EEPROM.write(106 + HttpHeaderTAint * 20, HttpHeaderFRIbyte);
            EEPROM.write(107 + HttpHeaderTAint * 20, HttpHeaderSATbyte);

            HttpHeaderTSK = "";
            //resetFunc();
          };

          //clearing string for next read
          HttpHeader = "";

          //stopping client
          client.stop();
        }
      }
    }
  }
}
