// Author: nhan.pro41
// Build date: 8/3013
#include <time.h>
#include <Ticker.h>
#include <ESP8266WiFi.h>

Ticker flip;
#define CLKPIN 12
#define DIPIN 13
const char *ssid = " nnn";  // SSID
const char *password = "nnn";

String dayOfWeek, thang, ngay, gio, phut, giay, nam;
int hour, minute, sec;

#define NUMBERS 6
//
unsigned char Num1_Ram;  //
unsigned char Num2_Ram;  //
unsigned char Num3_Ram;  //
unsigned char Num4_Ram;
unsigned char Num5_Ram;  //
unsigned char Num6_Ram;  //
unsigned char buf;
// 1-7 seg : EDCHBAFG  , need to change if your LCD is diferrents
//unsigned char NUMCODETAB[] = { 0xEE, 0x28, 0xCD, 0x6D, 0x2B, 0x67, 0xE7, 0x2C, 0xEF, 0x6F }; // noninverted

unsigned char NUMCODETAB[] = { 0x11, 0xD7, 0x32, 0x92, 0xD4, 0x98, 0x18, 0xD3, 0x10, 0x90 };  // 0,1,2,..9 inverted

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(CLKPIN, OUTPUT);
  pinMode(DIPIN, OUTPUT);

  Serial.println("Connecting Wifi.....");
  WiFi.begin(ssid, password);

  String IPaddress = WiFi.localIP().toString();
  Serial.println(IPaddress);
  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  updateTime();
  while (nam.toInt() < 2000) {

    updateTime();
    Serial.println("years < 2000");
    delay(1000);
  }
  flip.attach(1, interrupTimer);
  clear_LCD();
}

void loop() {
}
void interrupTimer() {

  sec++;
  if (sec == 60) {
    minute++;
    sec = 0;
  }

  if (minute == 60) {
    hour++;
    minute = 0;
  }
  if (hour == 24) {
    hour = 0;
    updateTime();
    Serial.println("update time after 24h");
  }
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(sec);
  numfill();
  //clear_LCD();
  display_LCD();
}
void updateTime() {
  time_t now = time(nullptr);
  String data = ctime(&now);
  // Serial.println("updated time");

  String ngayTrongTuan = data.substring(0, 3);
  String month = data.substring(4, 7);
  ngay = data.substring(8, 10);
  gio = data.substring(11, 13);
  phut = data.substring(14, 16);
  giay = data.substring(17, 19);
  nam = data.substring(20, 24);
  //Biến đổi
  ngayTrongTuan.toUpperCase();
  ngay.trim();
  if (ngayTrongTuan == "MON")
    dayOfWeek = "MONDAY";
  else if (ngayTrongTuan == "TUE")
    dayOfWeek = "TUESDAY";
  else if (ngayTrongTuan == "WED")
    dayOfWeek = "WEDNESDAY";
  else if (ngayTrongTuan == "THU")
    dayOfWeek = "THURSDAY";
  else if (ngayTrongTuan == "FRI")
    dayOfWeek = "FRIDAY";
  else if (ngayTrongTuan == "SAT")
    dayOfWeek = "SATURDAY";
  else if (ngayTrongTuan == "SUN")
    dayOfWeek = "SUNDAY";
  if (ngay.toInt() < 10) {
    ngay = "0" + ngay;
  }
  if (month == "Jan")
    thang = "01";
  else if (month == "Feb")
    thang = "02";
  else if (month == "Mar")
    thang = "03";
  else if (month == "Apr")
    thang = "04";
  else if (month == "May")
    thang = "05";
  else if (month == "Jun")
    thang = "06";
  else if (month == "Jul")
    thang = "07";
  else if (month == "Aug")
    thang = "08";
  else if (month == "Sep")
    thang = "09";
  else if (month == "Oct")
    thang = "10";
  else if (month == "Nov")
    thang = "11";
  else if (month == "Dec")
    thang = "12";
  hour = gio.toInt();
  minute = phut.toInt();
  sec = giay.toInt();
}
void clear_LCD() {
  //Clear LCD
  for (int i = 0; i < 7; i++) {
    transbyte(0xff);
  }
}
void transbyte(unsigned char d) {
  Serial.println(d, HEX);
  unsigned char i;
  for (i = 0; i < 8; i++) {
    if ((d & 0x1) == 0x1)
      transbit(1);
    else
      transbit(0);
    d >>= 1;
  }
}
void transbit(int d) {

  digitalWrite(DIPIN, d);
  digitalWrite(DIPIN, d); // nothing but a bit of delay

  digitalWrite(CLKPIN, 1);
  digitalWrite(CLKPIN, 1); // nothing but a bit of delay

  digitalWrite(CLKPIN, 0);
  digitalWrite(CLKPIN, 0);// nothing but a bit of delay

  digitalWrite(CLKPIN, 1);
  digitalWrite(CLKPIN, 1);// nothing but a bit of delay
}

void numfill() {
  int conv1 = hour / 10;  //
  int conv2 = hour % 10;  //
  Num1_Ram = conv1;       //num6
  Num2_Ram = conv2;       //num5

  conv1 = minute / 10;  //
  conv2 = minute % 10;
  Num3_Ram = conv1;  //num4
  Num4_Ram = conv2;  //num3

  conv1 = sec / 10;  //
  conv2 = sec % 10;
  Num5_Ram = conv1;  //num2
  Num6_Ram = conv2;  //num1
}
void display_LCD() {

  transbyte(0xFC); // display 2 dot colmum
  //LCDBUF+0
  buf = 0x00;
  buf |= NUMCODETAB[Num1_Ram];
  transbyte(buf);

  //LCDBUF+1
  buf = 0x00;
  buf |= NUMCODETAB[Num2_Ram];
  transbyte(buf);


  //LCDBUF+2
  buf = 0x00;
  buf |= NUMCODETAB[Num3_Ram];
  transbyte(buf);

  //LCDBUF+3
  buf = 0x00;
  buf |= NUMCODETAB[Num4_Ram];
  transbyte(buf);

  buf = 0x00;
  buf |= NUMCODETAB[Num5_Ram];
  transbyte(buf);

  buf = 0x00;
  buf |= NUMCODETAB[Num6_Ram];
  // if ((Z5_Ram & Z5_BMASK) == 0)
  //  buf |= 0x01;
  transbyte(buf);
}
