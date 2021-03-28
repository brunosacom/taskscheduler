/* BMBS - Bruno Sá - www.bruno-sa.com */

#include <TimeLib.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
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

//task action
void pinVoltage(byte pin, byte voltage){
switch (voltage) {
  case 72:
    digitalWrite(pin, HIGH);
    break;
  case 76:
    digitalWrite(pin, LOW);
    break;
  case 104:
    digitalWrite(pin, HIGH);
    delay(500);
    digitalWrite(pin, LOW);
    break;
  case 108:
    digitalWrite(pin, LOW);
    delay(500);
    digitalWrite(pin, HIGH);
    break;
  default:
    // if nothing else matches, do the default
    // default is optional
    break;
  }
};

//BMBS IPconfig presence variables
String variavelCFG = "";
int variavelTSKpos;
String variavelTSK = "";

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
String HttpHeaderNTP;

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
byte HttpHeaderNTPbyte;

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
byte HttpHeaderPNbyte;
byte HttpHeaderPVbyte;
byte HttpHeaderSTSbyte;
byte HttpHeaderSUNbyte;
byte HttpHeaderMONbyte;
byte HttpHeaderTUEbyte;
byte HttpHeaderWEDbyte;
byte HttpHeaderTHUbyte;
byte HttpHeaderFRIbyte;
byte HttpHeaderSATbyte;

byte HttpHeaderNT1byte;
byte HttpHeaderNT2byte;
byte HttpHeaderNT3byte;
byte HttpHeaderNT4byte;

byte HttpHeaderTZ1byte;
byte HttpHeaderTZ2byte;

//BMBS declare NTP byte variables to write in EEPROM
const String n0 = "0";
const byte year0 = 20;
//const byte year0 = EEPROM.read(100);

//BMBS Task Schedule function return DateTime (YYYY-MM-DD HH:MM:SS)
String taskScheduleComplete (int nTask){
  return ((String) year0 + twoDigits(EEPROM.read(93 + nTask * 20)) + "-" + twoDigits(EEPROM.read(94 + nTask * 20)) + "-" + twoDigits(EEPROM.read(95 + nTask * 20)) + " " + twoDigits(EEPROM.read(96 + nTask * 20)) + ":" + twoDigits(EEPROM.read(97 + nTask * 20)) + ":" + twoDigits(EEPROM.read(98 + nTask * 20)));
};

//BMBS Task Schedule function return Time (HH:MM:SS)
String taskScheduleTime (int nTask){
  return ((String) twoDigits(EEPROM.read(96 + nTask * 20)) + ":" + twoDigits(EEPROM.read(97 + nTask * 20)) + ":" + twoDigits(EEPROM.read(98 + nTask * 20)));
};

//BMBS Task Schedule function return DateTime or Time according days of week filled
String taskScheduleDisplay (int nTask){
  String tskDate = ((String) year0 + twoDigits(EEPROM.read(93 + nTask * 20)) + "-" + twoDigits(EEPROM.read(94 + nTask * 20)) + "-" + twoDigits(EEPROM.read(95 + nTask * 20)));
  String tskTime = ((String) twoDigits(EEPROM.read(96 + nTask * 20)) + ":" + twoDigits(EEPROM.read(97 + nTask * 20)) + ":" + twoDigits(EEPROM.read(98 + nTask * 20)));
  String tskDateTime = tskDate + " " + tskTime;
  if (EEPROM.read(100 + nTask * 20) == 8){
    return tskTime;
  }

  return tskDateTime;
};

//BMBS Task Schedule function days of week (81234567), first char = 0 (no days filled) or 8 (any day filled)
String daysOfWeekComplete (int nTask){
  return ((String) EEPROM.read(100 + nTask * 20) + EEPROM.read(101 + nTask * 20) + EEPROM.read(102 + nTask * 20) + EEPROM.read(103 + nTask * 20) + EEPROM.read(104 + nTask * 20) + EEPROM.read(105 + nTask * 20) + EEPROM.read(106 + nTask * 20) + EEPROM.read(107 + nTask * 20));
};

String taskSchedule01Str = "";
String taskSchedule02Str = "";
String taskSchedule03Str = "";
String taskSchedule04Str = "";
String taskSchedule05Str = "";
String taskSchedule06Str = "";
String taskSchedule07Str = "";
String taskSchedule08Str = "";
String taskSchedule09Str = "";
String taskSchedule10Str = "";

String taskSchedule01Display = "";
String taskSchedule02Display = "";
String taskSchedule03Display = "";
String taskSchedule04Display = "";
String taskSchedule05Display = "";
String taskSchedule06Display = "";
String taskSchedule07Display = "";
String taskSchedule08Display = "";
String taskSchedule09Display = "";
String taskSchedule10Display = "";

String daysOfWeek01Str = "";
String daysOfWeek02Str = "";
String daysOfWeek03Str = "";
String daysOfWeek04Str = "";
String daysOfWeek05Str = "";
String daysOfWeek06Str = "";
String daysOfWeek07Str = "";
String daysOfWeek08Str = "";
String daysOfWeek09Str = "";
String daysOfWeek10Str = "";


byte taskNumberArray[11];
byte taskStatusArray[11];
byte taskWeekArray[11];
byte taskSunArray[11];
byte taskMonArray[11];
byte taskTueArray[11];
byte taskWedArray[11];
byte taskThuArray[11];
byte taskFriArray[11];
byte taskSatArray[11];
byte taskActionPinArray[11];
byte taskActionVoltArray[11];

String sts01;
String sun01;
String mon01;
String tue01;
String wed01;
String thu01;
String fri01;
String sat01;

String sts02;
String sun02;
String mon02;
String tue02;
String wed02;
String thu02;
String fri02;
String sat02;

String sts03;
String sun03;
String mon03;
String tue03;
String wed03;
String thu03;
String fri03;
String sat03;

String sts04;
String sun04;
String mon04;
String tue04;
String wed04;
String thu04;
String fri04;
String sat04;

String sts05;
String sun05;
String mon05;
String tue05;
String wed05;
String thu05;
String fri05;
String sat05;

String sts06;
String sun06;
String mon06;
String tue06;
String wed06;
String thu06;
String fri06;
String sat06;

String sts07;
String sun07;
String mon07;
String tue07;
String wed07;
String thu07;
String fri07;
String sat07;

String sts08;
String sun08;
String mon08;
String tue08;
String wed08;
String thu08;
String fri08;
String sat08;

String sts09;
String sun09;
String mon09;
String tue09;
String wed09;
String thu09;
String fri09;
String sat09;

String sts10;
String sun10;
String mon10;
String tue10;
String wed10;
String thu10;
String fri10;
String sat10;

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

//BMBS function dayOfWeek HTMLcode
String dayOfWeekValue(byte dow){
  String dayOfWeekVAL;
  if (dow == 48){
    dayOfWeekVAL = " text-muted";
  }
  return dayOfWeekVAL;
};

//BMBS function Status HTMLcode
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
String divRow = "<div class='col-sm-6'><div class='input-group mb-2 input-group-sm col-2'><span class='col-3 input-group-text text-nowrap'>";
String divClassInput0 = "<input class='form-control form-control-sm' type='number' max='255' name='";
String divClassInput1 = "' value='";
String divClassInput2 = "'><span class='input-group-text input-group-sm'>.</span>";
String formSelected = " selected";

//BMBS NTP and timezone infos in EEPROM
byte ntpip[] = {EEPROM.read(21), EEPROM.read(22), EEPROM.read(23), EEPROM.read(24)}; // default NTP server address
byte tz[] = {EEPROM.read(26), EEPROM.read(27)};

//BMBS current NTP time(HH:MM:SS)
String timeNow(){
  return ((String)twoDigits(hour(now())) + ":" + twoDigits(minute(now())) + ":" + twoDigits(second(now())));
};

//BMBS current NTP time and weekday (HH:MM:SS W)
String timeWeekNow(){
  String nowTime =  ((String)twoDigits(hour(now())) + ":" + twoDigits(minute(now())) + ":" + twoDigits(second(now())));
  int nowWeekday = weekday();
  String tskTimeWeek = nowTime + " " + nowWeekday;

  return tskTimeWeek;
};

//BMBS current NTP DateTime(YYYY-MM-DD HH:MM:SS)
String dateTimeNow(){
  String nowDate =  ((String)year(now()) + "-" + twoDigits(month(now())) + "-" + twoDigits(day(now())));
  String nowTime =  ((String)twoDigits(hour(now())) + ":" + twoDigits(minute(now())) + ":" + twoDigits(second(now())));
  String tskDateTime = nowDate + " " + nowTime;

  return tskDateTime;
};

//BMBS current NTP DateTime and weekday (YYYY-MM-DD HH:MM:SS W)
String dateTimeWeekNow(){
  String nowDate =  ((String)year(now()) + "-" + twoDigits(month(now())) + "-" + twoDigits(day(now())));
  String nowTime =  ((String)twoDigits(hour(now())) + ":" + twoDigits(minute(now())) + ":" + twoDigits(second(now())));
  int nowWeekday = weekday();
  String tskDateTimeWeek = nowDate + " " + nowTime + " " + nowWeekday;

  return tskDateTimeWeek;
};

//BMBS time library configs
IPAddress timeServer(ntpip[0], ntpip[1], ntpip[2], ntpip[3]); // 201.49.148.135 = b.st1.ntp.br
int timeZone = 0;     // default UTC
EthernetUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

void setup()
{
  //BMBS piMode OUTPUT 1 to 53
  for (int i = 0; i < 54; i++){
    pinMode(i, OUTPUT);
  };
  
  //BMBS check previous BMBIPCONFIG
  variavelCFG = ((String)char(EEPROM.read(1)) + char(EEPROM.read(2)) + char(EEPROM.read(3)) + char(EEPROM.read(4)) + char(EEPROM.read(5)) + char(EEPROM.read(6)) + char(EEPROM.read(7)) + char(EEPROM.read(8)) + char(EEPROM.read(9)) + char(EEPROM.read(10))); 

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
  };

  //BMS populate task arrays
  for (int t = 1; t < 11; t++)
    {
      taskNumberArray[t] = EEPROM.read(t * 20 + 91);
      taskStatusArray[t] = EEPROM.read(t * 20 + 92);
      taskWeekArray[t] = EEPROM.read(t * 20 + 100);
      taskSunArray[t] = EEPROM.read(t * 20 + 101);
      taskMonArray[t] = EEPROM.read(t * 20 + 102);
      taskTueArray[t] = EEPROM.read(t * 20 + 103);
      taskWedArray[t] = EEPROM.read(t * 20 + 104);
      taskThuArray[t] = EEPROM.read(t * 20 + 105);
      taskFriArray[t] = EEPROM.read(t * 20 + 106);
      taskSatArray[t] = EEPROM.read(t * 20 + 107);
      taskActionPinArray[t] = EEPROM.read(t * 20 + 108);
      taskActionVoltArray[t] = EEPROM.read(t * 20 + 109);
    };

  //BMBS timezone info
  if (tz[0] == 45) {
    timeZone = -1 * tz[1];
    formSelected = "";
  } else {
    timeZone = 1 * tz[1];
  };

  //enable serial monitor
  Serial.begin(9600);

  //start Ethernet
  Ethernet.begin(mac, ip, dns, gateway, subnet);

  //BMBS time library configs
  Udp.begin(localPort);
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);

  //initialize variable
  HttpHeader = "";
}

//BMBS time library configs
time_t prevDisplay = 0; // when the digital clock was displayed


void loop()
{
  // refresh NTP DateTime
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      digitalClockDisplay();  
    }
  };

  taskSchedule01Str = taskScheduleComplete(taskNumberArray[1]);
  taskSchedule02Str = taskScheduleComplete(taskNumberArray[2]);
  taskSchedule03Str = taskScheduleComplete(taskNumberArray[3]);
  taskSchedule04Str = taskScheduleComplete(taskNumberArray[4]);
  taskSchedule05Str = taskScheduleComplete(taskNumberArray[5]);
  taskSchedule06Str = taskScheduleComplete(taskNumberArray[6]);
  taskSchedule07Str = taskScheduleComplete(taskNumberArray[7]);
  taskSchedule08Str = taskScheduleComplete(taskNumberArray[8]);
  taskSchedule09Str = taskScheduleComplete(taskNumberArray[9]);
  taskSchedule10Str = taskScheduleComplete(taskNumberArray[10]);

  taskSchedule01Display = taskScheduleDisplay(taskNumberArray[1]);
  taskSchedule02Display = taskScheduleDisplay(taskNumberArray[2]);
  taskSchedule03Display = taskScheduleDisplay(taskNumberArray[3]);
  taskSchedule04Display = taskScheduleDisplay(taskNumberArray[4]);
  taskSchedule05Display = taskScheduleDisplay(taskNumberArray[5]);
  taskSchedule06Display = taskScheduleDisplay(taskNumberArray[6]);
  taskSchedule07Display = taskScheduleDisplay(taskNumberArray[7]);
  taskSchedule08Display = taskScheduleDisplay(taskNumberArray[8]);
  taskSchedule09Display = taskScheduleDisplay(taskNumberArray[9]);
  taskSchedule10Display = taskScheduleDisplay(taskNumberArray[10]);

  daysOfWeek01Str = daysOfWeekComplete(taskNumberArray[1]);
  daysOfWeek02Str = daysOfWeekComplete(taskNumberArray[2]);
  daysOfWeek03Str = daysOfWeekComplete(taskNumberArray[3]);
  daysOfWeek04Str = daysOfWeekComplete(taskNumberArray[4]);
  daysOfWeek05Str = daysOfWeekComplete(taskNumberArray[5]);
  daysOfWeek06Str = daysOfWeekComplete(taskNumberArray[6]);
  daysOfWeek07Str = daysOfWeekComplete(taskNumberArray[7]);
  daysOfWeek08Str = daysOfWeekComplete(taskNumberArray[8]);
  daysOfWeek09Str = daysOfWeekComplete(taskNumberArray[9]);
  daysOfWeek10Str = daysOfWeekComplete(taskNumberArray[10]);

  //BMBS run Task Schedules
  taskScheduleExecute(1);
  taskScheduleExecute(2);
  taskScheduleExecute(3);
  taskScheduleExecute(4);
  taskScheduleExecute(5);
  taskScheduleExecute(6);
  taskScheduleExecute(7);
  taskScheduleExecute(8);
  taskScheduleExecute(9);
  taskScheduleExecute(10);
  
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

          HttpHeaderNTP = HttpHeader;
          HttpHeaderNTP.remove(HttpHeader.indexOf("N1=") - 1, HttpHeader.length() - HttpHeader.indexOf("N1="));
          HttpHeaderNTP.remove(0, HttpHeader.indexOf("TZ=") + 3);
          HttpHeaderNTPbyte = HttpHeaderNTP[0];

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
          HttpHeaderPNbyte = HttpHeaderValue("PN=");
          HttpHeaderPVbyte = HttpHeaderValue("PV=");
          HttpHeaderSTSbyte = HttpHeaderValue("sts=");
          HttpHeaderSUNbyte = HttpHeaderValue("sun=");
          HttpHeaderMONbyte = HttpHeaderValue("mon=");
          HttpHeaderTUEbyte = HttpHeaderValue("tue=");
          HttpHeaderWEDbyte = HttpHeaderValue("wed=");
          HttpHeaderTHUbyte = HttpHeaderValue("thu=");
          HttpHeaderFRIbyte = HttpHeaderValue("fri=");
          HttpHeaderSATbyte = HttpHeaderValue("sat=");

          HttpHeaderNT1byte = HttpHeaderValue("N1=");
          HttpHeaderNT2byte = HttpHeaderValue("N2=");
          HttpHeaderNT3byte = HttpHeaderValue("N3=");
          HttpHeaderNT4byte = HttpHeaderValue("N4=");

          HttpHeaderTZ1byte = HttpHeaderValue("T1=");
          HttpHeaderTZ2byte = HttpHeaderValue("T2=");

          sts01 = statusValue(taskStatusArray[1]);
          sun01 = dayOfWeekValue(daysOfWeek01Str[1]);
          mon01 = dayOfWeekValue(daysOfWeek01Str[2]);
          tue01 = dayOfWeekValue(daysOfWeek01Str[3]);
          wed01 = dayOfWeekValue(daysOfWeek01Str[4]);
          thu01 = dayOfWeekValue(daysOfWeek01Str[5]);
          fri01 = dayOfWeekValue(daysOfWeek01Str[6]);
          sat01 = dayOfWeekValue(daysOfWeek01Str[7]);

          sts02 = statusValue(taskStatusArray[2]);
          sun02 = dayOfWeekValue(daysOfWeek02Str[1]);
          mon02 = dayOfWeekValue(daysOfWeek02Str[2]);
          tue02 = dayOfWeekValue(daysOfWeek02Str[3]);
          wed02 = dayOfWeekValue(daysOfWeek02Str[4]);
          thu02 = dayOfWeekValue(daysOfWeek02Str[5]);
          fri02 = dayOfWeekValue(daysOfWeek02Str[6]);
          sat02 = dayOfWeekValue(daysOfWeek02Str[7]);

          sts03 = statusValue(taskStatusArray[3]);
          sun03 = dayOfWeekValue(daysOfWeek03Str[1]);
          mon03 = dayOfWeekValue(daysOfWeek03Str[2]);
          tue03 = dayOfWeekValue(daysOfWeek03Str[3]);
          wed03 = dayOfWeekValue(daysOfWeek03Str[4]);
          thu03 = dayOfWeekValue(daysOfWeek03Str[5]);
          fri03 = dayOfWeekValue(daysOfWeek03Str[6]);
          sat03 = dayOfWeekValue(daysOfWeek03Str[7]);

          sts04 = statusValue(taskStatusArray[4]);
          sun04 = dayOfWeekValue(daysOfWeek04Str[1]);
          mon04 = dayOfWeekValue(daysOfWeek04Str[2]);
          tue04 = dayOfWeekValue(daysOfWeek04Str[3]);
          wed04 = dayOfWeekValue(daysOfWeek04Str[4]);
          thu04 = dayOfWeekValue(daysOfWeek04Str[5]);
          fri04 = dayOfWeekValue(daysOfWeek04Str[6]);
          sat04 = dayOfWeekValue(daysOfWeek04Str[7]);

          sts05 = statusValue(taskStatusArray[5]);
          sun05 = dayOfWeekValue(daysOfWeek05Str[1]);
          mon05 = dayOfWeekValue(daysOfWeek05Str[2]);
          tue05 = dayOfWeekValue(daysOfWeek05Str[3]);
          wed05 = dayOfWeekValue(daysOfWeek05Str[4]);
          thu05 = dayOfWeekValue(daysOfWeek05Str[5]);
          fri05 = dayOfWeekValue(daysOfWeek05Str[6]);
          sat05 = dayOfWeekValue(daysOfWeek05Str[7]);

          sts06 = statusValue(taskStatusArray[6]);
          sun06 = dayOfWeekValue(daysOfWeek06Str[1]);
          mon06 = dayOfWeekValue(daysOfWeek06Str[2]);
          tue06 = dayOfWeekValue(daysOfWeek06Str[3]);
          wed06 = dayOfWeekValue(daysOfWeek06Str[4]);
          thu06 = dayOfWeekValue(daysOfWeek06Str[5]);
          fri06 = dayOfWeekValue(daysOfWeek06Str[6]);
          sat06 = dayOfWeekValue(daysOfWeek06Str[7]);

          sts07 = statusValue(taskStatusArray[7]);
          sun07 = dayOfWeekValue(daysOfWeek07Str[1]);
          mon07 = dayOfWeekValue(daysOfWeek07Str[2]);
          tue07 = dayOfWeekValue(daysOfWeek07Str[3]);
          wed07 = dayOfWeekValue(daysOfWeek07Str[4]);
          thu07 = dayOfWeekValue(daysOfWeek07Str[5]);
          fri07 = dayOfWeekValue(daysOfWeek07Str[6]);
          sat07 = dayOfWeekValue(daysOfWeek07Str[7]);

          sts08 = statusValue(taskStatusArray[8]);
          sun08 = dayOfWeekValue(daysOfWeek08Str[1]);
          mon08 = dayOfWeekValue(daysOfWeek08Str[2]);
          tue08 = dayOfWeekValue(daysOfWeek08Str[3]);
          wed08 = dayOfWeekValue(daysOfWeek08Str[4]);
          thu08 = dayOfWeekValue(daysOfWeek08Str[5]);
          fri08 = dayOfWeekValue(daysOfWeek08Str[6]);
          sat08 = dayOfWeekValue(daysOfWeek08Str[7]);

          sts09 = statusValue(taskStatusArray[9]);
          sun09 = dayOfWeekValue(daysOfWeek09Str[1]);
          mon09 = dayOfWeekValue(daysOfWeek09Str[2]);
          tue09 = dayOfWeekValue(daysOfWeek09Str[3]);
          wed09 = dayOfWeekValue(daysOfWeek09Str[4]);
          thu09 = dayOfWeekValue(daysOfWeek09Str[5]);
          fri09 = dayOfWeekValue(daysOfWeek09Str[6]);
          sat09 = dayOfWeekValue(daysOfWeek09Str[7]);

          sts10 = statusValue(taskStatusArray[10]);
          sun10 = dayOfWeekValue(daysOfWeek10Str[1]);
          mon10 = dayOfWeekValue(daysOfWeek10Str[2]);
          tue10 = dayOfWeekValue(daysOfWeek10Str[3]);
          wed10 = dayOfWeekValue(daysOfWeek10Str[4]);
          thu10 = dayOfWeekValue(daysOfWeek10Str[5]);
          fri10 = dayOfWeekValue(daysOfWeek10Str[6]);
          sat10 = dayOfWeekValue(daysOfWeek10Str[7]);

          Serial.println(Ethernet.localIP());
          Serial.println(Ethernet.dnsServerIP());
          Serial.print("variavelCFG: ");
          Serial.println(variavelCFG);
          Serial.print("Length: ");
          Serial.println(HttpHeader.length());
          Serial.print("HttpHeader: ");
          Serial.println(HttpHeader);

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
          client.println(F("<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta3/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-eOJMYsd53ii+scO/bJGFsiCZc+5NDVN2yr8+0RDqr0Ql0h+rP48ckxlpbzKgwra6' crossorigin='anonymous'>"));
          client.println(F("<!-- jQuery UI CSS -->"));
          client.println(F("<link rel='stylesheet' href='https://code.jquery.com/ui/1.12.1/themes/base/jquery-ui.css'>"));

          //BMBS internal page: /fsend
          //character conversion in https://www.arduino.cc/en/Reference/ASCIIchart
          if (HttpHeader[5] == 102 && HttpHeader[6] == 115 && HttpHeader[7] == 101 && HttpHeader[8] == 110 && HttpHeader[9] == 100)
          {
            client.println(F("<title>Form Sended</title>"));
            client.println(F("</head><body style='font-family:Didact Gothic; color:#FFF; background-color:#333;'><div class='container'><h2><strong>Form Sended</strong></h2>"));
            client.println(F("<h3><a href='/' class='row justify-content-center'><button class='btn btn-warning'>reload</button></a></h3><br><br>"));
          }


          //BMBS internal page: /ipcfg
          //character conversion in https://www.arduino.cc/en/Reference/ASCIIchart
          else if (HttpHeader[5] == 105 && HttpHeader[6] == 112 && HttpHeader[7] == 99 && HttpHeader[8] == 102 && HttpHeader[9] == 103)
          {
            client.println(F("<title>IP Config</title>"));
            client.println(F("</head>"));
            client.println(F("<body style='font-family:Didact Gothic; color:#FFF; background-color:#333;'><div class='container'><h2><strong>IP Config Page</strong></h2>"));
            client.println(F("<form action='/fsend'><input type='hidden' name='CF' value='BMB_ipconf'>"));
            client.print(divRow);
            client.print(F("IP Address: </span>"));
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
            client.print(F("Subnet Mask: </span>"));
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
            client.print(F("Gateway: </span>"));
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
            client.print(F("DNS: </span>"));
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
            client.println(F("<form action='/fsend'><input type='hidden' name='TS' value='BMB_tsksch'><input type='hidden' name='Y0' value='20'>"));
            client.println(F("  <div class='row my-2 bg-secondary'>"));
            client.println(F("      <div class='col-2'>task</div>"));
            client.println(F("      <div class='col-4'>time (H:M:S)</div>"));
            client.println(F("      <div class='col-4'>action (pin number / voltage)</div>"));
            client.println(F("      <div class='col-1'>status</div>"));
            client.println(F("  </div>"));
            client.println(F("  <div class='row my-2'>"));
            client.println(F("      <div class='col-2'>"));
            client.println(F("          <input class='form-control form-control-sm' type='number' size='2' min='1' max='10' name='TA'>"));
            client.println(F("      </div>"));
            client.println(F("      <div class='col-4'>"));
            client.println(F("          <div class='input-group mb-2 input-group-sm'><input class='form-control form-control-sm' type='number' size='2' max='23' name='HO'><span class='input-group-text input-group-sm'>:</span><input class='form-control form-control-sm' type='number' size='2' max='59' name='MI'><span class='input-group-text input-group-sm'>:</span> <input class='form-control form-control-sm' type='number' size='2' max='59' name='SE'></div>"));
            client.println(F("      </div>"));
            client.println(F("<div class='col-4'><div class='input-group mb-2 input-group-sm'><input class='form-control form-control-sm' type='number' size='2' min='1' max='53' name='PN'><span class='input-group-text input-group-sm'>/</span><select class='form-select form-select-sm'  name='PV'><option value='0' selected>select...</option><option value='72'>switch HIGH (H)</option><option value='76'>switch LOW (L)</option><option value='104'>pulse HIGHLOW (h)</option><option value='108'>pulse LOWHIGH (l)</option></select></div></div>"));
            client.println(F("      <div class='col-1'>"));
            client.println(F("          <div class='form-check mb-2 form-switch'>"));
            client.println(F("              <input type='checkbox' class='form-check-input' id='sts' name='sts' value='9'><label class='form-check-label' for='sts'></label>"));
            client.println(F("          </div>"));
            client.println(F("      </div>"));
            client.println(F("  </div>"));
            client.println(F("  <div class='row my-2 bg-secondary'>"));
            client.println(F("      <div class='col-4'>date (YY-MM-DD)</div>"));
            client.println(F("      <div class='col-6 text-nowrap'>day of week</div>"));
            client.println(F("  </div>"));
            client.println(F("  <div class='row'>"));
            client.println(F("      <div class='col-4'>"));
            client.println(F("          <div class='input-group mb-2 input-group-sm'><input class='form-control form-control-sm' type='number' size='2' max='99' name='YE'><span class='input-group-text input-group-sm'>-</span><input class='form-control form-control-sm' type='number' size='2' max='12' name='MO'><span class='input-group-text input-group-sm'>-</span><input class='form-control form-control-sm' type='number' size='2' max='31' name='DA'></div>"));
            client.println(F("      </div>"));
            client.println(F("      <div class='col-6 text-wrap'>"));
            client.println(F("              <div class='form-check form-check-inline'>"));
            client.println(F("                  <input type='checkbox' class='form-check-input' id='sun' name='sun' value='1'>"));
            client.println(F("                  <label class='form-check-label' for='sun'>S</label>"));
            client.println(F("              </div>"));
            client.println(F("              <div class='form-check form-check-inline'>"));
            client.println(F("                  <input type='checkbox' class='form-check-input' id='mon' name='mon' value='2'>"));
            client.println(F("                  <label class='form-check-label' for='mon'>M</label>"));
            client.println(F("              </div>"));
            client.println(F("              <div class='form-check form-check-inline'>"));
            client.println(F("                  <input type='checkbox' class='form-check-input' id='tue' name='tue' value='3'>"));
            client.println(F("                  <label class='form-check-label' for='tue'>T</label>"));
            client.println(F("              </div>"));
            client.println(F("              <div class='form-check form-check-inline'>"));
            client.println(F("                  <input type='checkbox' class='form-check-input' id='wed' name='wed' value='4'>"));
            client.println(F("                  <label class='form-check-label' for='wed'>W</label>"));
            client.println(F("              </div>"));
            client.println(F("              <div class='form-check form-check-inline'>"));
            client.println(F("                  <input type='checkbox' class='form-check-input' id='thu' name='thu' value='5'>"));
            client.println(F("                  <label class='form-check-label' for='thu'>T</label>"));
            client.println(F("              </div>"));
            client.println(F("              <div class='form-check form-check-inline'>"));
            client.println(F("                  <input type='checkbox' class='form-check-input' id='fri' name='fri' value='6'>"));
            client.println(F("                  <label class='form-check-label' for='fri'>F</label>"));
            client.println(F("              </div>"));
            client.println(F("              <div class='form-check form-check-inline'>"));
            client.println(F("                  <input type='checkbox' class='form-check-input' id='sat' name='sat' value='7'>"));
            client.println(F("                  <label class='form-check-label' for='sat'>S</label>"));
            client.println(F("              </div>"));
            client.println(F("      </div>"));
            client.println(F("      <div class='col-1 text-nowrap'>"));
            client.println(F("          <input class='btn btn-warning btn-sm' type='submit' value='submit'>"));
            client.println(F("      </div>"));
            client.println(F("    </div>"));
            client.println(F("  </div>"));
            client.println(F("</form><br>"));
          }

          //BMBS internal page: /ntpzn
          //character conversion in https://www.arduino.cc/en/Reference/ASCIIchart
          else if (HttpHeader[5] == 110 && HttpHeader[6] == 116 && HttpHeader[7] == 112 && HttpHeader[8] == 122 && HttpHeader[9] == 110)
          {
            client.println(F("<title>NTP & TimeZone</title>"));
            client.println(F("</head>"));
            client.println(F("<body style='font-family:Didact Gothic; color:#FFF; background-color:#333;'><div class='container'><h2><strong>NTP & TimeZone</strong></h2>"));
            client.println(F("<form action='/fsend'><input type='hidden' name='TZ' value='BMB_timezn'>"));
            client.print(divRow);
            client.print(F("NTP IP Address: </span>"));
            client.print(divClassInput0);
            client.print(F("N1"));
            client.print(divClassInput1);
            client.print(ntpip[0]);
            client.print(divClassInput2);
            client.print(divClassInput0);
            client.print(F("N2"));
            client.print(divClassInput1);
            client.print(ntpip[1]);
            client.print(divClassInput2);
            client.print(divClassInput0);
            client.print(F("N3"));
            client.print(divClassInput1);
            client.print(ntpip[2]);
            client.print(divClassInput2);
            client.print(divClassInput0);
            client.print(F("N4"));
            client.print(divClassInput1);
            client.print(ntpip[3]);
            client.println(F("'></div></div>"));
            client.print(divRow);
            client.print(F("TimeZone: </span>"));
            client.print(F("<select class='form-select form-select-sm' name='T1'><option value='45'>-</option><option value='43'"));
            client.print(formSelected);
            client.print(F(">+</option></select>"));
            client.print(divClassInput0);
            client.print(F("T2"));
            client.print(divClassInput1);
            client.print(tz[1]);
            client.println(F("'></div></div>"));
            client.println(F("<div class='form-row my-2'><input class='btn btn-warning btn-sm' type='submit' value='submit'></div></form>"));
          }

          //BMBS default internal page: /
          else
          {
            client.println(F("<title>Home</title>"));
            client.println(F("</head>"));
            client.println(F("<body style='font-family:Didact Gothic; color:#FFF; background-color:#333;'><div class='container'><h2><strong>Home Page</strong></h2>"));

            client.println(F("<div class='row my-2'>"));
            client.println(F("<div class='col-3 bg-secondary'>Network</div>"));
            client.println(F("<div class='col'></div>"));
            client.println(F("<div class='col-3 bg-secondary'>NTP / Timezone</div>"));
            client.println(F("</div>"));
            client.println(F("<div class='row my-2'>"));
            client.println((String)"<div class='col-3'>IP: " + ip[0] + "." + ip[1] + "." +  ip[2] + "." +  ip[3] + "</div>");
            client.println(F("<div class='col'></div>"));
            client.println((String)"<div class='col-3'>NTP Server IP: " + ntpip[0] + "." +  ntpip[1] + "." +  ntpip[2] + "." +  ntpip[3] + "</div>");
            client.println(F("</div>"));
            client.println(F("<div class='row my-2'>"));
            client.println((String)"<div class='col-3'>Subnet Mask: " + subnet[0] + "." +  subnet[1] + "." +  subnet[2] + "." +  subnet[3] + "</div>");
            client.println(F("<div class='col'></div>"));
            client.println((String)"<div class='col-3'>Timezone: " + char(tz[0]) + tz[1] + "</div>");
            client.println(F("</div>"));
            client.println(F("<div class='row my-2'>"));
            client.println((String)"<div class='col-3'>Gateway: " + gateway[0] + "." +  gateway[1] + "." +  gateway[2] + "." +  gateway[3] + "</div>");
            client.println(F("</div>"));
            client.println(F("<div class='row my-2'>"));
            client.println((String)"<div class='col-3'>DNS: " + dns[0] + "." +  dns[1] + "." +  dns[2] + "." +  dns[3] + "</div>");
            client.println(F("</div>"));
            client.println(F("<br>"));

            client.println(F("<div class='row my-2 bg-secondary'>"));
            client.println(F("<div class='col-1'>task</div>"));
            client.println(F("<div class='col-3'>timestamp</div>"));
            client.println(F("<div class='col-5'>day of week</div>"));
            client.println(F("<div class='col-2'>action pin | volt</div>"));
            client.println(F("<div class='col-1'>status</div>"));
            client.println(F("</div>"));
            client.println(F("<div class='row my-2'>"));
            client.println((String)"<div class='col-1'>" + taskNumberArray[1] + "</div>");
            client.println((String)"<div class='col-3'>" + taskSchedule01Display + "</div>");
            client.println(F("<div class='col-5'>"));
            client.println(F("    <div class='row'>"));
            client.println("        <div class='col" + sun01 + "'>S</div>");
            client.println("        <div class='col" + mon01 + "'>M</div>");
            client.println("        <div class='col" + tue01 + "'>T</div>");
            client.println("        <div class='col" + wed01 + "'>W</div>");
            client.println("        <div class='col" + thu01 + "'>T</div>");
            client.println("        <div class='col" + fri01 + "'>F</div>");
            client.println("        <div class='col" + sat01 + "'>S</div>");
            client.println(F("    </div>"));
            client.println(F("</div>"));
            client.println((String)"<div class='col-2'>" + taskActionPinArray[1] + " | " + char(taskActionVoltArray[1]) + "</div>");
            client.println("<div class='col-1" + sts01 + "</div>");
            client.println(F("</div>"));
            client.println(F("<div class='row my-2'>"));
            client.println((String)"<div class='col-1'>" + taskNumberArray[2] + "</div>");
            client.println((String)"<div class='col-3'>" + taskSchedule02Display + "</div>");
            client.println(F("<div class='col-5'>"));
            client.println(F("    <div class='row'>"));
            client.println("        <div class='col" + sun02 + "'>S</div>");
            client.println("        <div class='col" + mon02 + "'>M</div>");
            client.println("        <div class='col" + tue02 + "'>T</div>");
            client.println("        <div class='col" + wed02 + "'>W</div>");
            client.println("        <div class='col" + thu02 + "'>T</div>");
            client.println("        <div class='col" + fri02 + "'>F</div>");
            client.println("        <div class='col" + sat02 + "'>S</div>");
            client.println(F("    </div>"));
            client.println(F("</div>"));
            client.println((String)"<div class='col-2'>" + taskActionPinArray[2] + " | " + char(taskActionVoltArray[2]) + "</div>");
            client.println("<div class='col-1" + sts02 + "</div>");
            client.println(F("</div>"));
            client.println(F("<div class='row my-2'>"));
            client.println((String)"<div class='col-1'>" + taskNumberArray[3] + "</div>");
            client.println((String)"<div class='col-3'>" + taskSchedule03Display + "</div>");
            client.println(F("<div class='col-5'>"));
            client.println(F("    <div class='row'>"));
            client.println("        <div class='col" + sun03 + "'>S</div>");
            client.println("        <div class='col" + mon03 + "'>M</div>");
            client.println("        <div class='col" + tue03 + "'>T</div>");
            client.println("        <div class='col" + wed03 + "'>W</div>");
            client.println("        <div class='col" + thu03 + "'>T</div>");
            client.println("        <div class='col" + fri03 + "'>F</div>");
            client.println("        <div class='col" + sat03 + "'>S</div>");
            client.println(F("    </div>"));
            client.println(F("</div>"));
            client.println((String)"<div class='col-2'>" + taskActionPinArray[3] + " | " + char(taskActionVoltArray[3]) + "</div>");
            client.println("<div class='col-1" + sts03 + "</div>");
            client.println(F("</div>"));
            client.println(F("<div class='row my-2'>"));
            client.println((String)"<div class='col-1'>" + taskNumberArray[4] + "</div>");
            client.println((String)"<div class='col-3'>" + taskSchedule04Display + "</div>");
            client.println(F("<div class='col-5'>"));
            client.println(F("    <div class='row'>"));
            client.println("        <div class='col" + sun04 + "'>S</div>");
            client.println("        <div class='col" + mon04 + "'>M</div>");
            client.println("        <div class='col" + tue04 + "'>T</div>");
            client.println("        <div class='col" + wed04 + "'>W</div>");
            client.println("        <div class='col" + thu04 + "'>T</div>");
            client.println("        <div class='col" + fri04 + "'>F</div>");
            client.println("        <div class='col" + sat04 + "'>S</div>");
            client.println(F("    </div>"));
            client.println(F("</div>"));
            client.println((String)"<div class='col-2'>" + taskActionPinArray[4] + " | " + char(taskActionVoltArray[4]) + "</div>");
            client.println("<div class='col-1" + sts04 + "</div>");
            client.println(F("</div>"));
            client.println(F("<div class='row my-2'>"));
            client.println((String)"<div class='col-1'>" + taskNumberArray[5] + "</div>");
            client.println((String)"<div class='col-3'>" + taskSchedule05Display + "</div>");
            client.println(F("<div class='col-5'>"));
            client.println(F("    <div class='row'>"));
            client.println("        <div class='col" + sun05 + "'>S</div>");
            client.println("        <div class='col" + mon05 + "'>M</div>");
            client.println("        <div class='col" + tue05 + "'>T</div>");
            client.println("        <div class='col" + wed05 + "'>W</div>");
            client.println("        <div class='col" + thu05 + "'>T</div>");
            client.println("        <div class='col" + fri05 + "'>F</div>");
            client.println("        <div class='col" + sat05 + "'>S</div>");
            client.println(F("    </div>"));
            client.println(F("</div>"));
            client.println((String)"<div class='col-2'>" + taskActionPinArray[5] + " | " + char(taskActionVoltArray[5]) + "</div>");
            client.println("<div class='col-1" + sts05 + "</div>");
            client.println(F("</div>"));
            client.println(F("<div class='row my-2'>"));
            client.println((String)"<div class='col-1'>" + taskNumberArray[6] + "</div>");
            client.println((String)"<div class='col-3'>" + taskSchedule06Display + "</div>");
            client.println(F("<div class='col-5'>"));
            client.println(F("    <div class='row'>"));
            client.println("        <div class='col" + sun06 + "'>S</div>");
            client.println("        <div class='col" + mon06 + "'>M</div>");
            client.println("        <div class='col" + tue06 + "'>T</div>");
            client.println("        <div class='col" + wed06 + "'>W</div>");
            client.println("        <div class='col" + thu06 + "'>T</div>");
            client.println("        <div class='col" + fri06 + "'>F</div>");
            client.println("        <div class='col" + sat06 + "'>S</div>");
            client.println(F("    </div>"));
            client.println(F("</div>"));
            client.println((String)"<div class='col-2'>" + taskActionPinArray[6] + " | " + char(taskActionVoltArray[6]) + "</div>");
            client.println("<div class='col-1" + sts06 + "</div>");
            client.println(F("</div>"));
            client.println(F("<div class='row my-2'>"));
            client.println((String)"<div class='col-1'>" + taskNumberArray[7] + "</div>");
            client.println((String)"<div class='col-3'>" + taskSchedule07Display + "</div>");
            client.println(F("<div class='col-5'>"));
            client.println(F("    <div class='row'>"));
            client.println("        <div class='col" + sun07 + "'>S</div>");
            client.println("        <div class='col" + mon07 + "'>M</div>");
            client.println("        <div class='col" + tue07 + "'>T</div>");
            client.println("        <div class='col" + wed07 + "'>W</div>");
            client.println("        <div class='col" + thu07 + "'>T</div>");
            client.println("        <div class='col" + fri07 + "'>F</div>");
            client.println("        <div class='col" + sat07 + "'>S</div>");
            client.println(F("    </div>"));
            client.println(F("</div>"));
            client.println((String)"<div class='col-2'>" + taskActionPinArray[7] + " | " + char(taskActionVoltArray[7]) + "</div>");
            client.println("<div class='col-1" + sts07 + "</div>");
            client.println(F("</div>"));
            client.println(F("<div class='row my-2'>"));
            client.println((String)"<div class='col-1'>" + taskNumberArray[8] + "</div>");
            client.println((String)"<div class='col-3'>" + taskSchedule08Display + "</div>");
            client.println(F("<div class='col-5'>"));
            client.println(F("    <div class='row'>"));
            client.println("        <div class='col" + sun08 + "'>S</div>");
            client.println("        <div class='col" + mon08 + "'>M</div>");
            client.println("        <div class='col" + tue08 + "'>T</div>");
            client.println("        <div class='col" + wed08 + "'>W</div>");
            client.println("        <div class='col" + thu08 + "'>T</div>");
            client.println("        <div class='col" + fri08 + "'>F</div>");
            client.println("        <div class='col" + sat08 + "'>S</div>");
            client.println(F("    </div>"));
            client.println(F("</div>"));
            client.println((String)"<div class='col-2'>" + taskActionPinArray[8] + " | " + char(taskActionVoltArray[8]) + "</div>");
            client.println("<div class='col-1" + sts08 + "</div>");
            client.println(F("</div>"));
            client.println(F("<div class='row my-2'>"));
            client.println((String)"<div class='col-1'>" + taskNumberArray[9] + "</div>");
            client.println((String)"<div class='col-3'>" + taskSchedule09Display + "</div>");
            client.println(F("<div class='col-5'>"));
            client.println(F("    <div class='row'>"));
            client.println("        <div class='col" + sun09 + "'>S</div>");
            client.println("        <div class='col" + mon09 + "'>M</div>");
            client.println("        <div class='col" + tue09 + "'>T</div>");
            client.println("        <div class='col" + wed09 + "'>W</div>");
            client.println("        <div class='col" + thu09 + "'>T</div>");
            client.println("        <div class='col" + fri09 + "'>F</div>");
            client.println("        <div class='col" + sat09 + "'>S</div>");
            client.println(F("    </div>"));
            client.println(F("</div>"));
            client.println((String)"<div class='col-2'>" + taskActionPinArray[9] + " | " + char(taskActionVoltArray[9]) + "</div>");
            client.println("<div class='col-1" + sts09 + "</div>");
            client.println(F("</div>"));
            client.println(F("<div class='row my-2'>"));
            client.println((String)"<div class='col-1'>" + taskNumberArray[10] + "</div>");
            client.println((String)"<div class='col-3'>" + taskSchedule10Display + "</div>");
            client.println(F("<div class='col-5'>"));
            client.println(F("    <div class='row'>"));
            client.println("        <div class='col" + sun10 + "'>S</div>");
            client.println("        <div class='col" + mon10 + "'>M</div>");
            client.println("        <div class='col" + tue10 + "'>T</div>");
            client.println("        <div class='col" + wed10 + "'>W</div>");
            client.println("        <div class='col" + thu10 + "'>T</div>");
            client.println("        <div class='col" + fri10 + "'>F</div>");
            client.println("        <div class='col" + sat10 + "'>S</div>");
            client.println(F("    </div>"));
            client.println(F("</div>"));
            client.println((String)"<div class='col-2'>" + taskActionPinArray[10] + " | " + char(taskActionVoltArray[10]) + "</div>");
            client.println("<div class='col-1" + sts10 + "</div>");
            client.println(F("</div><br>"));
          }
          //BMBS web page's footer
          client.println(F("<div class='d-flex justify-content-center'><div><a href='/'>home</a> | <a href='/ipcfg'>IP config</a> | <a href='/ntpzn'>NTP & TimeZone</a> | <a href='/schdl'>task scheduler</a></div></div>"));
          client.println(F("<hr>"));
          client.println(F("<div class='d-flex justify-content-center'><div><small><small>Created by Bruno Sá - <a href='//www.bruno-sa.com' target='_blank'>www.bruno-sa.com</a></small></small></div></div>"));
          client.println(F("</div>"));
          client.println(F("<!-- Optional JavaScript -->"));
          client.println(F("<!-- jQuery first, then Popper.js, then Bootstrap JS -->"));
          client.println(F("<script src='https://code.jquery.com/jquery-3.6.0.js' integrity='sha256-H+K7U5CnXl1h5ywQfKtSj8PCmoN9aaq30gDh27Xc0jk=' crossorigin='anonymous'></script>"));
          client.println(F("<script src='https://code.jquery.com/ui/1.12.1/jquery-ui.js' integrity='sha256-T0Vest3yCU7pafRw9r+settMBX6JkKN06dqBnpQ8d30=' crossorigin='anonymous'></script>"));
          client.println(F("<script src='https://cdn.jsdelivr.net/npm/jquery-ui-touch-punch@0.2.3/jquery.ui.touch-punch.js' integrity='sha256-S9605h/+fTHx8kE89v4NQWuTGCEQJF0B9UGvMFYAiO8=' crossorigin='anonymous'></script>"));
          client.println(F("<script src='https://unpkg.com/@popperjs/core@2'></script>"));
          client.println(F("<script src='https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta3/dist/js/bootstrap.min.js' integrity='sha384-j0CNLUeiqtyaRmlzUHCPZ+Gy5fQu0dQ6eZ/xAww941Ai1SxSY+0EQqNXNE6DZiVc' crossorigin='anonymous'></script>"));
          client.println(F("</body></html>"));

          //BMBS checking previous IP config to overwrite
          if (HttpHeaderCFG[0] == 66 && HttpHeaderCFG[1] == 77 && HttpHeaderCFG[2] == 66 && HttpHeaderCFG[3] == 95 && HttpHeaderCFG[4] == 105 && HttpHeaderCFG[5] == 112 && HttpHeaderCFG[6] == 99 && HttpHeaderCFG[7] == 111 && HttpHeaderCFG[8] == 110 && HttpHeaderCFG[9] == 102)
          {
            EEPROM.write(1, 66);
            EEPROM.write(2, 77);
            EEPROM.write(3, 66);
            EEPROM.write(4, 95);
            EEPROM.write(5, 105);
            EEPROM.write(6, 112);
            EEPROM.write(7, 99);
            EEPROM.write(8, 111);
            EEPROM.write(9, 110);
            EEPROM.write(10, 102);

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
            EEPROM.write(108 + HttpHeaderTAint * 20, HttpHeaderPNbyte);
            EEPROM.write(109 + HttpHeaderTAint * 20, HttpHeaderPVbyte);

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
            resetFunc();
          };

          //BMBS checking previous NTP TimeZone to overwrite
          if (HttpHeaderNTP[0] == 66 && HttpHeaderNTP[1] == 77 && HttpHeaderNTP[2] == 66 && HttpHeaderNTP[3] == 95 && HttpHeaderNTP[4] == 116 && HttpHeaderNTP[5] == 105 && HttpHeaderNTP[6] == 109 && HttpHeaderNTP[7] == 101 && HttpHeaderNTP[8] == 122 && HttpHeaderNTP[9] == 110)
          {
            EEPROM.write(21, HttpHeaderNT1byte);
            EEPROM.write(22, HttpHeaderNT2byte);
            EEPROM.write(23, HttpHeaderNT3byte);
            EEPROM.write(24, HttpHeaderNT4byte);

            EEPROM.write(26, HttpHeaderTZ1byte);
            EEPROM.write(27, HttpHeaderTZ2byte);

            HttpHeaderNTP = "";
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

//BMBS time library configs
void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(year()); 
  Serial.print("-");
  printDigits(month());
  Serial.print("-");
  printDigits(day());
  Serial.print(" ");
  Serial.print(hour());
  Serial.print(":");
  printDigits(minute());
  Serial.print(":");
  printDigits(second());
  Serial.print(" ");
  Serial.print(weekday());
  Serial.println(); 
}

//BMBS time library configs
void printDigits(int digits){
  // utility for digital clock display: prints preceding colon and leading 0
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

//BMBS time library configs
/*-------- NTP code ----------*/
const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  sendNTPpacket(timeServer);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:                 
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

//BMBS Task Schedule execute function
void taskScheduleExecute (int ntask){
  if (timeNow() == taskScheduleTime(taskNumberArray[ntask]) && taskStatusArray[ntask] == 9){
    if (dateTimeNow() == taskScheduleComplete(taskNumberArray[ntask]) && daysOfWeekComplete(taskNumberArray[ntask])[0] == 48){
      pinVoltage(taskActionPinArray[ntask], taskActionVoltArray[ntask]);
    } else if (timeNow() == taskScheduleTime(taskNumberArray[ntask]) && daysOfWeekComplete(taskNumberArray[ntask]).indexOf(dateTimeWeekNow()[20]) > 0 ){
      pinVoltage(taskActionPinArray[ntask], taskActionVoltArray[ntask]);
    }
  }
}
