#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32_Servo.h>
#include <Eeprom24Cxx.h>

#define BLYNK_TEMPLATE_ID "TMPL6yyR2ELgT"
#define BLYNK_TEMPLATE_NAME "Smart Lock"
#define BLYNK_AUTH_TOKEN "VO6J2nXY99TfF8oragz05t-ZsrrBlDcm"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <string.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "oni" ; //Ten Wifi
char pass[] = "198765432" ; // Mat Khau Wifi

#define SS_PIN 5
#define RST_PIN 4
#define SERVO_PIN 15
#define BUTTON_PIN 2
#define COI_PIN 16

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo servo;

#define ROW_NUM 4
#define COLUMN_NUM 4

char keys[ROW_NUM][COLUMN_NUM] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

byte pin_rows[ROW_NUM] = {32, 33, 25, 26};
byte pin_column[COLUMN_NUM] = {27, 14, 12, 13};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int cursorColumn = 0;
int Hbutton = LOW;
int Tbutton = LOW;
int angle = 0;
boolean swich = HIGH;

String D_password = "555555";
String C_password = D_password;
String input_password;
String new_password;
char read_password;
char key;
char keyC;
char keyD;
char keyMain;
unsigned long Millis;
int C_Millis;
const int Coi_time = 100;

unsigned long UID[4];
int a = 0;
int a0 = 0;
int b = 0;
int b0 = 0;
int c = 6;
int c1 = 1;
int d0 = 0;
int d1 = 0;
int d2 = 0;
int d3 = 0;
int passk = 0;
int themas = 0;
int bandau;
int id_moi[4];
int id_er[4];
int diachi_zero;
int diachi_pass = 2;
int m = 8;
int tam = 0;
int gt_zero;
int gt_er = 0;
int n = 0;
int o = 0;
int demco = 0;
int demchua = 0;
int demmas = 0;
int khoa = 0;

void setup(){
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  EEPROM.begin(512);

  SPI.begin();
  mfrc522.PCD_Init();

  servo.attach(SERVO_PIN);
  servo.write(angle);

  lcd.init();
  lcd.backlight();
 
  pinMode(BUTTON_PIN, INPUT);
  pinMode(COI_PIN, OUTPUT);
  // for(int i = 0; i < EEPROM.length(); i++){  //xoa bo nho eeprom
  //   EEPROM.write(i, 0); 
  // }
}
BLYNK_WRITE(V0) {
  swich = param.asInt();
  if (swich == HIGH ){
    angle = 0;
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print("LOCKED");
    Blynk.virtualWrite(V0, HIGH);
    Blynk.virtualWrite(V1, String("LOCKED"));
    delay(500);
    lcd.clear();
    c1 = 0;
    d1 = 0;
    d3 = 0;
    d0 = 0;
    c = 0;
    input_password = "";
    new_password = "";
    manhinhchinh();
  } 
  else if (swich == LOW){
    angle = 180;
    lcd.clear();
    lcd.setCursor(6, 1);
    lcd.print("OPEN");
    Blynk.virtualWrite(V0, LOW);
    Blynk.virtualWrite(V1, String("OPEN"));
    delay(500);
    lcd.clear();
    c1 = 0;
    d1 = 0;
    d3 = 0;
    d0 = 0;
    c = 0;
    input_password = "";
    new_password = "";
    manhinhchinh();
  }
  servo.write(angle);
}
void loop(){
  Blynk.run();
  Millis = millis();
  passk = EEPROM.read(1);
  if (passk == 0){
    passk = 1;
    EEPROM.write(1, passk);
    for (int i = 0; i < 6; i++){
      EEPROM.write(diachi_pass, D_password[i]);
      diachi_pass = diachi_pass + 1;
    }
    EEPROM.commit();
    diachi_pass = 2;
    C_password = "";
    for (int i = 2; i < 8; i++){
      read_password = EEPROM.read(i);
      C_password += read_password;
    }
  } else if (passk != 0){
    C_password = "";
    for (int i = 2; i < 8; i++){
      read_password = EEPROM.read(i);
      C_password += read_password;
    }
  }
  if (a == 0){
    a = EEPROM.read(0);
  }
  else if (a != 0){
    EEPROM.write(0, a);
    EEPROM.commit();
    a = EEPROM.read(0);
  }
  if (Millis - C_Millis >= Coi_time){
    digitalWrite(COI_PIN, LOW);
  }
  key = keypad.getKey();
  if (key){
    C_Millis = Millis;
    digitalWrite(COI_PIN, HIGH);
    if (key == '*'){
      d0 = !d0;
    }
  }
  kiemtramaster();
  keypadchedo();
  chedo();
  Button();
}
void manhinhchinh(){
  lcd.setCursor(5, 0);
  lcd.print("^    ^");
  lcd.setCursor(6, 1);
  lcd.print("____");
  khoa = 2;
}
void nhap_pass(){
  lcd.setCursor(1, 0);
  lcd.print("NHAP MAT KHAU:");
  while (d0 = 1){
    Blynk.run();
    Millis = millis();
    if (Millis - C_Millis >= Coi_time){
      digitalWrite(COI_PIN, LOW);
    }
    keyMain = keypad.getKey();
    read_rfid();
    if (b == 1){
      if (demmas == 1 && demco == 0 && demchua == 0){
        b = 0;
        c = 0;
        reset();
        changeServo();
        break;
      }
      else if (demco == 1 && demchua == 0 && demmas == 0){
        b = 0;
        c = 0;
        reset();
        changeServo();
        break;
      }
        else if (demchua == 1 && demco == 0 && demmas == 0){
        b = 0;
        lcd.clear();
        lcd.setCursor(4, 1);
        lcd.print("SAI THE!");
        delay(1000);
        reset();
        d0 = 0;
        c = 0;
        lcd.clear();
        manhinhchinh();
        break;
      }
    }
    if (keyMain){
      C_Millis = Millis;
      digitalWrite(COI_PIN, HIGH);
      if (keyMain == 'A'){
        lcd.clear();
        c = 1;
        b = 0;
        a0 = 1;
        d0 = 1;
        chedoA();
        break;
      } else if (keyMain == 'B'){
        lcd.clear();
        c = 2;
        b = 0;
        b0 = 1;
        d0 = 1;
        chedoB();
        break;
      } else if (keyMain == 'C'){
        lcd.clear();
        c = 3;
        c1 = 1;
        b = 0;
        d0 = 1;
        chedoC();
        break;
      } else if (keyMain == 'D'){
        lcd.clear();
        d0 = 1;
        d1 = 1;
        d3 = 1;
        c = 4;
        chedoD();
        break;
      } else if (keyMain == '*'){
        cursorColumn = 0;
        input_password = "";
        d0 = 0;
        c = 0;
        lcd.clear();
        manhinhchinh();
        break;
      } else if (keyMain == '#') {
        if (input_password == C_password) {
          input_password = "";
          c = 0;
          d0 = 0;
          cursorColumn = 0;
          changeServo();
          break;
        } else {
          lcd.clear();
          lcd.setCursor(0, 1);
          lcd.print("MAT KHAU SAI");
          delay(500);
          lcd.clear();
          input_password = "";
          cursorColumn = 0;
          d0 = 0;
          c = 0;
          manhinhchinh();
          break;
        }
      } else {
        input_password += keyMain;
        lcd.setCursor(cursorColumn, 1);
        lcd.print("*");
        cursorColumn++;
        if (cursorColumn == 6) {
          if (input_password == C_password) {
            input_password = "";
            c = 0;
            d0 = 1;
            cursorColumn = 0;
            changeServo();
            break;
          } else {
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("MAT KHAU SAI");
            input_password = "";
            delay(500);
            lcd.clear();
            cursorColumn = 0;
            c = 0;
            d0 = 0;
            manhinhchinh();
            break;
          }
        }
      }
    }
  }
}
void nhap_master(){
  lcd.setCursor(0, 0);
  lcd.print("NHAP THE MASTER");
  lcd.setCursor(0, 1);
  lcd.print("*.BACK");
}
void them_the(){
  lcd.setCursor(0, 0);
  lcd.print("THEM THE PHU....");
  lcd.setCursor(0, 1);
  lcd.print("*.BACK");
}
void xoa_the(){
  lcd.setCursor(0, 0);
  lcd.print("XOA THE PHU.....");
  lcd.setCursor(0, 1);
  lcd.print("*.BACK");
}
void setupmaster(){
  lcd.setCursor(2, 0);
  lcd.print("SETUP MASTER");
  lcd.setCursor(0, 1);
  lcd.print("MOI NHAP THE....");

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    khoa = 1;
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("THE DA NHAP");
    a = 8;
    
    for (byte i = 0; i < 4; i++) {
      UID[i] = mfrc522.uid.uidByte[i];
      EEPROM.write(a, UID[i]);
      a++;
      Serial.println(UID[i]);
    }
    lcd.setCursor(1, 1);
    lcd.print("LUU THANH CONG");
    delay(1000);
    lcd.clear();
    kiemtramaster();
  }
  EEPROM.commit();

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
void kiemtramaster(){
  if (a == 0 && khoa == 0){
    setupmaster();
  }
  else if (a != 0 && khoa == 1){
    c = 0;
    EEPROM.write(0, a);
    EEPROM.commit();
    manhinhchinh();
  } else if ( a != 0 && khoa == 0 ){
    c = 0;
    EEPROM.write(0, a);
    EEPROM.commit();
    manhinhchinh();
  }
}
void read_rfid(){
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++){
      UID[i] = mfrc522.uid.uidByte[i];
      id_moi[i] = UID[i];
      Serial.println(UID[i]);
    }
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    ss_eeprom();
    b = 1;
  }
}
void ss_eeprom(){
  while (m <= a){
    tam = m + 4;
    for (m; m < tam; m++){
      gt_er = EEPROM.read(m);
      if (o <= 3){
        id_er[o] = gt_er;
      }
      o++;
    }
    if (id_moi[0] == EEPROM.read(8) && id_moi[1] == EEPROM.read(9) && id_moi[2] == EEPROM.read(10) && id_moi[3] == EEPROM.read(11)){
      demmas = 1;
      demco = 0;
      demchua = 0;
      break;
    }
    else if (id_moi[0] == id_er[0] && id_moi[1] == id_er[1] && id_moi[2] == id_er[2] && id_moi[3] == id_er[3]){
      demco = 1;
      demchua = 0;
      demmas = 0;
      break;
    }
    else if (id_moi[0] != id_er[0] || id_moi[1] != id_er[1] || id_moi[2] != id_er[2] || id_moi[3] != id_er[3]){
      demchua = 1;
      demco = 0;
      demmas = 0;
    }
    o = 0;
    m = tam;
  }
}
void tim_zero(){
  while (n <= a){
    gt_zero = EEPROM.read(n);
    if (gt_zero == 0){
      diachi_zero = n;
      break;
    }
    n++;
  }
  if (diachi_zero == 0){
    diachi_zero = a + 1;
    n = 0;
  }
}
void chedo(){
  if (c == 0){
    d0 = 0;
    manhinhchinh();
  }
  else if ( c == 1){
    chedoA();
  } 
  else if (c == 2) {
    chedoB();
  }
  else if (c == 3) {
    chedoC();
  }
  else if (c == 4) {
    chedoD();
  }
  if (c == 5){
    d0 = 1;
    nhap_pass();
  }
}
void keypadchedo(){
  if (key == 'A'){
    lcd.clear();
    c = 1;
    b = 0;
    a0 = 1;
    d0 = 1;
  } else if (key == 'B'){
    lcd.clear();
    c = 2;
    b = 0;
    b0 = 1;
    d0 = 1;
  } else if (key == 'C'){
    lcd.clear();
    c = 3;
    c1 = 1;
    b = 0;
  } else if (key == 'D'){
    lcd.clear();
    d1 = 1;
    d2 = 0;
    d3 = 1;
    c = 4;
  } else if (key == '*' && d0 == 0){
    lcd.clear();
    c = 0;
  } else if (key == '*' && d0 == 1){
    lcd.clear();
    c = 5;
  }
}
void chedoA(){
  if (a0 == 1){
    nhap_master();
    read_rfid();
    if (b == 1){
      if (demmas == 1 && demco == 0 && demchua == 0){
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("XAC NHAN MASTER");
        a0 = 2;
        b = 0;
        reset();
        delay(1000);
        lcd.clear();
        chedoA();
      }
      else {
        lcd.clear();
        lcd.setCursor(4, 1);
        lcd.print("SAI THE!");
        delay(1000);
        lcd.clear();
        b = 0;
        reset();
        chedoA();
      }
    }
  }
  else if (a0 == 2){
    them_the();
    read_rfid();
    if (b == 1){
      if (demmas == 1 && demco == 0 && demchua == 0){
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("DAY LA THE MASTER");
        delay(1000);
        lcd.clear();
        reset();
        chedoA();
      }
      else if (demco == 1 && demchua == 0 && demmas == 0){
        lcd.clear();
        lcd.setCursor(3, 1);
        lcd.print("THE DA CO!");
        delay(1000);
        lcd.clear();
        reset();
        chedoA();
      }
      else if (demchua == 1 && demco == 0 && demmas == 0){
        tim_zero();
        for (int i = 0; i < 4; i++){
          EEPROM.write(diachi_zero, id_moi[i]);
          diachi_zero = diachi_zero + 1;
        }
        EEPROM.commit();
        a = diachi_zero;
        lcd.clear();
        lcd.setCursor(2, 1);
        lcd.print("DA THEM THE!");
        delay(1000);
        lcd.clear();
        reset();
        chedoA();
      }
    }
  }
}
void chedoB(){
  if (b0 == 1){
    nhap_master();
    read_rfid();
    if (b == 1){
      if (demmas == 1 && demco == 0 && demchua == 0){
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("XAC NHAN MASTER");
        b0 = 2;
        b = 0;
        reset();
        delay(1000);
        lcd.clear();
      }
      else {
        lcd.clear();
        lcd.setCursor(4, 1);
        lcd.print("SAI THE!");
        delay(1000);
        lcd.clear();
        b = 0;
        reset();
        chedoB();
      }
    }
  }
  else if (b0 == 2){
    xoa_the();
    read_rfid();
    if (b ==1){
      if (demmas == 1 && demco == 0 && demchua == 0){
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("KHONG THE XOA");
        lcd.setCursor(3, 1);
        lcd.print("THE MASTER");
        delay(1000);
        lcd.clear();
        b = 0;
        reset();
        chedoB();
      }
      else if (demco == 1 && demchua == 0 && demmas == 0){
        m = m - 4;
        for (int i = 0; i < 4; i++){
          EEPROM.write(m, 0);
          m = m + 1;
        }
        EEPROM.commit();
        lcd.clear();
        lcd.setCursor(3, 1);
        lcd.print("DA XOA THE");
        delay(1000);
        lcd.clear();
        b = 0;
        reset();
        chedoB();
      }
      else if (demchua == 1 && demco == 0 && demmas == 0){
        lcd.clear();
        lcd.setCursor(2, 1);
        lcd.print("CHUA CO THE!");
        delay(1000);
        lcd.clear();
        b = 0;
        reset();
        chedoB();
      }
    }
  }
}
void chedoC(){
  lcd.setCursor(3, 0);
  lcd.print("RESET ALL?");
  lcd.setCursor(0, 1);
  lcd.print("*.BACK      #.OK");
  while (c1 == 1){
    Blynk.run();
    Millis = millis();
    if (Millis - C_Millis >= Coi_time){
      digitalWrite(COI_PIN, LOW);
    }
    keyC = keypad.getKey();
    read_rfid();
    if (keyC){
      C_Millis = Millis;
      digitalWrite(COI_PIN, HIGH);
      if (keyC == '*'){
        b = 0;
        c = 0;
        c1 = 0;
        lcd.clear();
        manhinhchinh();
      } else if (keyC == '#'){
        lcd.clear();
        nhap_master();
      }
    }
    if (b == 1){
      if (demmas == 1 && demco == 0 && demchua == 0){
        for(int i = 0; i < EEPROM.length(); i++){  //xoa bo nho eeprom
        EEPROM.write(i, 0); 
      }
      lcd.clear();
      passk = 0;
      b = 0;
      c = 6;
      c1 = 0;
      a = 0;
      khoa = 0;
      break;
      } else {
      lcd.clear();
      lcd.setCursor(4, 1);
      lcd.print("SAI THE!");
      delay(1000);
      lcd.clear();
      c = 0;
      c1 = 0;
      manhinhchinh();
      }
    }
  }
}
void chedoD(){
  while (d1 == 1){
    Blynk.run();
    Millis = millis();
    if (Millis - C_Millis >= Coi_time){
      digitalWrite(COI_PIN, LOW);
    }
    if (cursorColumn <= 6){
      keyD = keypad.getKey();
    } else {
      keyD = '#';
    }
    if (cursorColumn == 0){
      d2 = 1;
    } else if (cursorColumn != 0){
      d2 = 0;
    }
    if (d3 == 1){
      lcd.setCursor(0, 0);
      lcd.print("NHAP MAT KHAU CU");
      if (keyD){
        C_Millis = Millis;
        digitalWrite(COI_PIN, HIGH);
        if (keyD == '*'){
          if (d2 == 0){
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("NHAP MAT KHAU CU");
            cursorColumn = 0;
            input_password = "";
          } else if (d2 == 1){
            lcd.clear();
            cursorColumn = 0;
            c = 0;
            d1 = 0;
          }
        }
        else if (keyD == '#') {
          if (input_password == C_password) {
            input_password = "";
            d3 = 2;
            cursorColumn = 0;
            lcd.clear();
          }
          else {
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("MAT KHAU SAI");
            delay(1000);
            lcd.clear();
            input_password = "";
            cursorColumn = 0;
          }
        }
        else {
          input_password += keyD;
          lcd.setCursor(cursorColumn, 1);
          lcd.print("*");
          cursorColumn++;
          if (cursorColumn == 6) {
            cursorColumn = 7;
            lcd.clear();
          }
        }
      }
    } else if (d3 == 2){
      lcd.setCursor(2, 0);
      lcd.print("MAT KHAU MOI");
      if (keyD){
        C_Millis = Millis;
        digitalWrite(COI_PIN, HIGH);
        if (keyD == '*'){
          if (d2 == 0){
            lcd.clear();
            lcd.setCursor(2, 0);
            lcd.print("MAT KHAU MOI");
            cursorColumn = 0;
            new_password = "";
          } else if (d2 == 1){
            cursorColumn = 0;
            c = 0;
            d1 = 0;
            lcd.clear();
          }
        }
        else if (keyD == '#') {
          if (cursorColumn < 6) {
            new_password = "";
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("PHAI DU 6 KY TU");
            delay(1000);
            lcd.clear();
            cursorColumn = 0;
          }
          else if(cursorColumn >= 6){
            lcd.clear();
            input_password = "";
            d3 = 3;
            cursorColumn = 0;
          }
        }
        else {
          new_password += keyD;
          lcd.setCursor(cursorColumn, 1);
          lcd.print("*");
          cursorColumn++;
          if (cursorColumn == 6) {
            cursorColumn = 7;
            lcd.clear();
          }
        }
      }
    } else if (d3 == 3){
      lcd.setCursor(0, 0);
      lcd.print("XAC NHAN LAI");
      if (keyD){
        C_Millis = Millis;
        digitalWrite(COI_PIN, HIGH);
        if (keyD == '*'){
          if (d2 == 0){
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("XAC NHAN LAI");
            cursorColumn = 0;
            input_password = "";
          } else if (d2 == 1){
            cursorColumn = 0;
            c = 0;
            d1 = 0;
            lcd.clear();
          }
        }
        else if (keyD == '#') {
          if (input_password == new_password) {
            C_password = input_password;
            diachi_pass = 2;
            for (int i = 0; i < 6; i++){
              EEPROM.write(diachi_pass, C_password[i]);
              diachi_pass = diachi_pass + 1;
            }
            EEPROM.commit();
            diachi_pass = 2;
            C_password = "";
            for (int i = 2; i < 8; i++){
              read_password = EEPROM.read(i);
              C_password += read_password;
            }
            input_password = "";
            c = 0;
            d1 = 0;
            d3 = 1;
            cursorColumn = 0;
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("DA DOI MAT KHAU");
            delay(1000);
            lcd.clear();
            manhinhchinh();
            break;
          }
          else {
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("MAT KHAU SAI");
            delay(1000);
            input_password = "";
            lcd.clear();
            cursorColumn = 0;
          }
        }
        else {
          input_password += keyD;
          lcd.setCursor(cursorColumn, 1);
          lcd.print("*");
          cursorColumn++;
          if (cursorColumn == 6) {
            cursorColumn = 7;
            lcd.clear();
          }
        }
      }
    }
  }
}
void changeServo(){
  delay(100);
  digitalWrite(COI_PIN, LOW);
  if (angle == 180){
    angle = 0;
    Blynk.virtualWrite(V0, HIGH);
    Blynk.virtualWrite(V1, String("LOCKED"));
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print("LOCKED");
    delay(500);
    reset();
    lcd.clear();
    d0 = 0;
    manhinhchinh();
  }
  else {
    angle = 180;
    Blynk.virtualWrite(V0, LOW);
    Blynk.virtualWrite(V1, String("OPEN"));
    lcd.clear();
    lcd.setCursor(6, 1);
    lcd.print("OPEN");
    delay(500);
    reset();
    lcd.clear();
    d0 = 0;
    manhinhchinh();
  }
  servo.write(angle);
}
void Button(){
  Tbutton = Hbutton;
  Hbutton = digitalRead(BUTTON_PIN);
  if (Tbutton == LOW && Hbutton == HIGH){
    changeServo();
  }
}
void reset(){
  demco = 0;
  demchua = 0;
  demmas = 0;
  m = 8;
  o = 0;
  tam = 0;
}
