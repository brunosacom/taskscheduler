/* BMBS - Bruno Sá - www.bruno-sa.com */

#include <ezTime.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>

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

//BMBS declare NTP byte variables to write in EEPROM
byte year0 = EEPROM.read(100);

byte task01 = EEPROM.read(111);
byte sts01 = EEPROM.read(112);
byte year01 = EEPROM.read(113);
byte month01 = EEPROM.read(114);
byte day01 = EEPROM.read(115);
byte hour01 = EEPROM.read(116);
byte min01 = EEPROM.read(117);
byte sec01 = EEPROM.read(118);

byte week01 = EEPROM.read(120);
byte sun01 = EEPROM.read(121);
byte mon01 = EEPROM.read(122);
byte tue01 = EEPROM.read(123);
byte wed01 = EEPROM.read(124);
byte thu01 = EEPROM.read(125);
byte fri01 = EEPROM.read(126);
byte sat01 = EEPROM.read(127);

String status01;
String sunday01;
String monday01;
String tuesday01;
String wednesday01;
String thursday01;
String friday01;
String saturday01;

byte task02 = EEPROM.read(131);
byte sts02 = EEPROM.read(132);
byte year02 = EEPROM.read(133);
byte month02 = EEPROM.read(134);
byte day02 = EEPROM.read(135);
byte hour02 = EEPROM.read(136);
byte min02 = EEPROM.read(137);
byte sec02 = EEPROM.read(138);
byte week02 = EEPROM.read(140);
byte sun02 = EEPROM.read(141);
byte mon02 = EEPROM.read(142);
byte tue02 = EEPROM.read(143);
byte wed02 = EEPROM.read(144);
byte thu02 = EEPROM.read(145);
byte fri02 = EEPROM.read(146);
byte sat02 = EEPROM.read(147);

String status02;
String sunday02;
String monday02;
String tuesday02;
String wednesday02;
String thursday02;
String friday02;
String saturday02;

//BMBS function HTTP GET byte from form
byte HttpHeaderValue(String fieldA, String fieldB)
{
  String HttpHeaderVAL;
  byte HttpHeaderVALbyte;
  HttpHeaderVAL = HttpHeader;
  HttpHeaderVAL.remove(HttpHeader.indexOf(fieldA) - 1, HttpHeader.length() - HttpHeader.indexOf(fieldA));
  HttpHeaderVAL.remove(0, HttpHeader.indexOf(fieldB) + 3);
  HttpHeaderVALbyte = HttpHeaderVAL.toInt();
  return HttpHeaderVALbyte;
};

//BMBS function dayOfWeek and Status HTMLcode
String dayOfWeekValue(byte dow){
  String dayOfWeekVAL;
  if (dow == 0){
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


//BMBS repeating HTML codes
String divRow = "<div class='form-row my-2'><div class='col-md-3 col-xs-12 text-nowrap'>";
String divClassInput0 = "<div class='col-2'><input class='form-control form-control-sm' type='number' size='3' max='255' name='";
String divClassInput1 = "' value='";
String divClassInput2 = "'></div>.";

void setup()
{
  variavelCFG = eepromReadStr(variavelCFGpos);

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
  }

  ;
  //start Ethernet
  Ethernet.begin(mac, ip, dns, gateway, subnet);

  //enable serial monitor
  Serial.begin(9600);

  //initialize variable
  HttpHeader = "";
}

void loop()
{
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

          HttpHeaderIP1byte = HttpHeaderValue("I2=", "I1=");
          HttpHeaderIP2byte = HttpHeaderValue("I3=", "I2=");
          HttpHeaderIP3byte = HttpHeaderValue("I4=", "I3=");
          HttpHeaderIP4byte = HttpHeaderValue("S1=", "I4=");

          HttpHeaderSB1byte = HttpHeaderValue("S2=", "S1=");
          HttpHeaderSB2byte = HttpHeaderValue("S3=", "S2=");
          HttpHeaderSB3byte = HttpHeaderValue("S4=", "S3=");
          HttpHeaderSB4byte = HttpHeaderValue("G1=", "S4=");

          HttpHeaderGW1byte = HttpHeaderValue("G2=", "G1=");
          HttpHeaderGW2byte = HttpHeaderValue("G3=", "G2=");
          HttpHeaderGW3byte = HttpHeaderValue("G4=", "G3=");
          HttpHeaderGW4byte = HttpHeaderValue("D1=", "G4=");

          HttpHeaderDN1byte = HttpHeaderValue("D2=", "D1=");
          HttpHeaderDN2byte = HttpHeaderValue("D3=", "D2=");
          HttpHeaderDN3byte = HttpHeaderValue("D4=", "D3=");
          HttpHeaderDN4byte = HttpHeaderValue("HTT", "D4=");

          status01 = statusValue(sts01);
          sunday01 = dayOfWeekValue(sun01);
          monday01 = dayOfWeekValue(mon01);
          tuesday01 = dayOfWeekValue(tue01);
          wednesday01 = dayOfWeekValue(wed01);
          thursday01 = dayOfWeekValue(thu01);
          friday01 = dayOfWeekValue(fri01);
          saturday01 = dayOfWeekValue(sat01);

          status02 = statusValue(sts02);
          sunday02 = dayOfWeekValue(sun02);
          monday02 = dayOfWeekValue(mon02);
          tuesday02 = dayOfWeekValue(tue02);
          wednesday02 = dayOfWeekValue(wed02);
          thursday02 = dayOfWeekValue(thu02);
          friday02 = dayOfWeekValue(fri02);
          saturday02 = dayOfWeekValue(sat02);

          Serial.print("Length: ");
          Serial.println(HttpHeader.length());
          Serial.print("HttpHeader: ");
          Serial.println(HttpHeader);
          Serial.print("HttpHeader[0]: ");
          Serial.println(HttpHeader[0]);
          Serial.print("HttpHeader[1]: ");
          Serial.println(HttpHeader[1]);
          Serial.print("HttpHeader[2]: ");
          Serial.println(HttpHeader[2]);
          Serial.print("HttpHeader[3]: ");
          Serial.println(HttpHeader[3]);
          Serial.print("HttpHeader[4]: ");
          Serial.println(HttpHeader[4]);
          Serial.print("HttpHeader[5]: ");
          Serial.println(HttpHeader[5]);

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
            client.println(F("<h5>Edit Task</h5>"));

            client.println(F("<div class='row my-2 bg-secondary'>"));
            client.println(F("<div class='col-1'><strong>task</strong></div>"));
            client.println(F("<div class='col-2'><strong>time</strong></div>"));
            client.println(F("<div class='col-2'><strong>date</strong></div>"));
            client.println(F("<div class='col-6'><strong>day of week</strong></div>"));
            client.println(F("<div class='col-1'><strong>status</strong></div>"));
            client.println(F("</div>"));
            client.println(F("<div class='row my-2'>"));
            client.println((String)"<div class='col-1'>" + task01 + "</div>");
            client.println((String)"<div class='col-2'>" + hour01 + ":" + min01 + ":" + sec01 + "</div>");
            client.println((String)"<div class='col-2'>" + year0 + year01 + "-" + month01 + "-" + day01 + "</div>");
            client.println("<div class='col-6'>");
            client.println("    <div class='row'>");
            client.println("        <div class='col" + sunday01 + "'> S </div>");
            client.println("        <div class='col" + monday01 + "'> M </div>");
            client.println("        <div class='col" + tuesday01 + "'>T</div>");
            client.println("        <div class='col" + wednesday01 + "'>W</div>");
            client.println("        <div class='col" + thursday01 + "'>T</div>");
            client.println("        <div class='col" + friday01 + "'>F</div>");
            client.println("        <div class='col" + saturday01 + "'>S</div>");
            client.println("    </div>");
            client.println("</div>");
            client.println("<div class='col-1" + status01 + "</div>");
            client.println("</div>");
             client.println(F("<div class='row my-2'>"));
            client.println((String)"<div class='col-1'>" + task02 + "</div>");
            client.println((String)"<div class='col-2'>" + hour02 + ":" + min02 + ":" + sec02 + "</div>");
            client.println((String)"<div class='col-2'>" + year0 + year02 + "-" + month02 + "-" + day02 + "</div>");
            client.println("<div class='col-6'>");
            client.println("    <div class='row'>");
            client.println("        <div class='col" + sunday02 + "'> S </div>");
            client.println("        <div class='col" + monday02 + "'> M </div>");
            client.println("        <div class='col" + tuesday02 + "'>T</div>");
            client.println("        <div class='col" + wednesday02 + "'>W</div>");
            client.println("        <div class='col" + thursday02 + "'>T</div>");
            client.println("        <div class='col" + friday02 + "'>F</div>");
            client.println("        <div class='col" + saturday02 + "'>S</div>");
            client.println("    </div>");
            client.println("</div>");
            client.println("<div class='col-1" + status02 + "</div>");
            client.println("</div><br><br>");

            
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
            client.println("              <input type='checkbox' class='custom-control-input' id='OO' name='OO' value='1'><label class='custom-control-label' for='OO'></label>");
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
            client.println("                  <input type='checkbox' class='custom-control-input' id='su' name='su' value='1'>");
            client.println("                  <label class='custom-control-label' for='su'>S</label>");
            client.println("              </div>");
            client.println("              <div class='col custom-control custom-checkbox'>");
            client.println("                  <input type='checkbox' class='custom-control-input' id='mo' name='mo' value='1'>");
            client.println("                  <label class='custom-control-label' for='mo'>M</label>");
            client.println("              </div>");
            client.println("              <div class='col custom-control custom-checkbox'>");
            client.println("                  <input type='checkbox' class='custom-control-input' id='tu' name='tu' value='1'>");
            client.println("                  <label class='custom-control-label' for='tu'>T</label>");
            client.println("              </div>");
            client.println("              <div class='col custom-control custom-checkbox'>");
            client.println("                  <input type='checkbox' class='custom-control-input' id='we' name='we' value='1'>");
            client.println("                  <label class='custom-control-label' for='we'>W</label>");
            client.println("              </div>");
            client.println("              <div class='col custom-control custom-checkbox'>");
            client.println("                  <input type='checkbox' class='custom-control-input' id='th' name='th' value='1'>");
            client.println("                  <label class='custom-control-label' for='th'>T</label>");
            client.println("              </div>");
            client.println("              <div class='col custom-control custom-checkbox'>");
            client.println("                  <input type='checkbox' class='custom-control-input' id='fr' name='fr' value='1'>");
            client.println("                  <label class='custom-control-label' for='fr'>F</label>");
            client.println("              </div>");
            client.println("              <div class='col custom-control custom-checkbox'>");
            client.println("                  <input type='checkbox' class='custom-control-input' id='sa' name='sa' value='1'>");
            client.println("                  <label class='custom-control-label' for='sa'>S</label>");
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

          //clearing string for next read
          HttpHeader = "";

          //stopping client
          client.stop();
        }
      }
    }
  }
}
