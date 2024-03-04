// UNO steuert 4x ROTOR für Antennen mit ADS1115 
// Letzte Änderung 2023_0728 - ROTOR 2 YAESU G-1000 für YAGI 10m - Poti Verarbeitung gleich wie Rotor 1 für Ultrabeam
//#include <Wire.h>
#include <Adafruit_ADS1X15.h>
 Adafruit_ADS1115 ads;//16 bit Version 
String STR_DIV_MSG = ""; // String diverse messages wird mit void SEND_SERIAL() gesendet

//Adafruit_ADS1115 ads1115;  // Construct an ads1115 

int LAUFT = 0;
String inputString = "h\n";         // Tastatureingabe zwischenspeichern
boolean stringComplete = true;      // Tastatureingabe fertig

float NUMBER = 0;

  unsigned long T;
  bool PULSE = true;

//// ****************** Variablen ROTOR Steuerug ***************************************************
//
// Potentiometer
float POTI1=0;
float POTI2=0;
float POTI3=0;
float POTI4=0;
//
// Relais Ausgang Definition

const int CW_R1 = 13; //CW drehen 
const int CCW_R1 = 12; //CCW drehen 

const int CW_R2 = 14; //CW drehen  
const int CCW_R2 = 27; //CCW drehen 

const int CW_R3 = 26; //CW drehen  
const int CCW_R3 = 25; //CCW drehen 

const int CW_R4 = 33; //CW drehen  
const int CCW_R4 = 32; //CCW drehen  


//
const int brakeDelay = 300; // nach dieser Zeit (Bremse gelöst) wird Ausgang CW oder CCW angesteuert.
//
long i = 0; //Wert gelesen von Serial Port
String RX1 = "0"; // Buttons an Windows übermitteln

////
float AZ_SOLL = 0;

//**************************************************
// Rotor 1 Parameter Constant
float lowerLimit_R1 = 0; // untere Poti Grenze
float lowerLimit_R2 = 0; 
float lowerLimit_R3 = 0; 
float lowerLimit_R4 = 0; 

float limitWidth_R1 = 15615; // Poti Bereich
float limitWidth_R2 = 15615; 
float limitWidth_R3 = 17760; 
float limitWidth_R4 = 17760; 

float AZ_Shift_R1 = 345; // Azimut Versatz
float AZ_Shift_R2 = 30; 
float AZ_Shift_R3 = 0; 
float AZ_Shift_R4 = 0; 

float DREH_GRAD_ROTOR_1 = 450; //Drehbereich des Rotors
float DREH_GRAD_ROTOR_2 = 450; 
float DREH_GRAD_ROTOR_3 = 450; 
float DREH_GRAD_ROTOR_4 = 450;

float  TOL_R1 = 3; // Toleranz für Halten in der STOP Position
float  TOL_R2 = 3; 
float  TOL_R3 = 3; 
float  TOL_R4 = 3; 

//**************************************************
// Rotor Variablen
float AZ_SOLL_R1 = 0;
float AZ_SOLL_R2 = 0;
float AZ_SOLL_R3 = 0;
float AZ_SOLL_R4 = 0;

float AZ_SOLL_R01 = 0;
float AZ_SOLL_R02 = 0;
float AZ_SOLL_R03 = 0;
float AZ_SOLL_R04 = 0;

float AZ_IST_R1 = 0;
float AZ_IST_R2 = 0;
float AZ_IST_R3 = 0;
float AZ_IST_R4 = 0;

float AZ_ANZ_R1 = 0;
float AZ_ANZ_R2 = 0;
float AZ_ANZ_R3 = 0;
float AZ_ANZ_R4 = 0;

float AZ_1 = 0;     // Azimut Wert für Anzeige
float AZ_2 = 0;
float AZ_3 = 0;
float AZ_4 = 0;

long POTI_IST_R1 = 0; // Poti Wiederstand Wert
long POTI_IST_R2 = 0; 
long POTI_IST_R3 = 0; 
long POTI_IST_R4 = 0; 

float WEG1 = 0; // löschen
float WEG2 = 0;

float RICHTUNG_R1 = 0; //bestimmt Drehrichtung in Abh. von AZ_SOLL und potDegrees
float RICHTUNG_R2 = 0; 
float RICHTUNG_R3 = 0;
float RICHTUNG_R4 = 0; 

bool R1_dreht = false; // Flag für Rotor am drehen oder angehalten
bool R2_dreht = false;
bool R3_dreht = false;
bool R4_dreht = false;

float WEG1_R1 = 0;
float WEG1_R2 = 0;
float WEG1_R3 = 0;
float WEG1_R4 = 0;

float WEG2_R1 = 0;
float WEG2_R2 = 0;
float WEG2_R3 = 0;
float WEG2_R4 = 0;

String S_R1 = "R1:0";
String S_R2 = "R2:0";
String S_R3 = "R3:0";
String S_R4 = "R4:0";
//****************************************************
  
//***************************** SETUP ***********************************************************   
void setup(void) 
    
{
  //  ads1115.begin(0x49);  // Initialize ads1115 at address 0x49
  //  delay(500);
  Serial.begin(115200);
  delay(500);
  
  //STR_DIV_MSG = " 2023_0728_4x_ROTOR_Steuerung_UNO_ADS115.ino "; SEND_SERIAL();

 // ads1115.begin();
   if (!ads.begin())
  {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
// ****************************************************************************************
//   //  ROTOR pins an Ausgang zuweisen
//
  pinMode(CCW_R1, OUTPUT);
  pinMode(CW_R1, OUTPUT);

  pinMode(CCW_R2, OUTPUT);
  pinMode(CW_R2, OUTPUT);

  pinMode(CCW_R3, OUTPUT);
  pinMode(CW_R3, OUTPUT);

  pinMode(CCW_R4, OUTPUT);
  pinMode(CW_R4, OUTPUT);
  
 // set Start Position

  digitalWrite(CCW_R1,HIGH);
  digitalWrite(CW_R1, HIGH);

  digitalWrite(CCW_R2, HIGH);
  digitalWrite(CW_R2, HIGH);
  
  digitalWrite(CCW_R3, HIGH);
  digitalWrite(CW_R3, HIGH);
   
  digitalWrite(CCW_R4, HIGH);
  digitalWrite(CW_R4, HIGH);

  //i=10; TASTEN_SCHALTEN();
  
  }

// ******************* ENDE SETUP ***********************************************************

// ******************** VOIDs ***************************************************************
void SEND_SERIAL() // Senden an COM PORT
{
 Serial.println(STR_DIV_MSG); 
}



void Help() // ************ Hilfe Anleitung **************************
{ 
  STR_DIV_MSG = "2023_0728_4x_ROTOR_Steuerung_UNO_ADS115.ino "; SEND_SERIAL();
  inputString="";
}

//******************************************************************************************
void FehlerMelden() // Fehler bei der Eingabe melden
{
  STR_DIV_MSG = "! Eingabefehler! "; SEND_SERIAL();
}

 /*************************************************************/
/*                   VOID  ROTOR                    */
/*************************************************************/
void READ_ALL_POTI()
  {
  int16_t adc0, adc1, adc2, adc3;
 
  POTI_IST_R1 = ads.readADC_SingleEnded(0);
  POTI_IST_R2 = ads.readADC_SingleEnded(1);
  POTI_IST_R3 = ads.readADC_SingleEnded(2);
  POTI_IST_R4 = ads.readADC_SingleEnded(3);
    

    AZ_IST_R1 =  map(POTI_IST_R1, lowerLimit_R1, limitWidth_R1, (float)0, DREH_GRAD_ROTOR_1);
 
    AZ_IST_R2 =  map(POTI_IST_R2, lowerLimit_R2, limitWidth_R2, (float)0, DREH_GRAD_ROTOR_2);

    AZ_IST_R3 =  map(POTI_IST_R3, lowerLimit_R3, limitWidth_R3, (float)0, DREH_GRAD_ROTOR_3);
 
    AZ_IST_R4 =  map(POTI_IST_R4, lowerLimit_R4, limitWidth_R4, (float)0, DREH_GRAD_ROTOR_4);
  }
//******************************************************************************************
void ALLE_OUT_HIGH()
  {
  digitalWrite(CCW_R1,LOW);
  digitalWrite(CW_R1, LOW);

  digitalWrite(CCW_R2, LOW);
  digitalWrite(CW_R2, LOW);
  
  digitalWrite(CCW_R3, LOW);
  digitalWrite(CW_R3, LOW);
  
  digitalWrite(CCW_R4, LOW);
  digitalWrite(CW_R4, LOW);
}
//******************************************************************************************
void WRITE_DEBUG()
  {
   READ_ALL_POTI();
    
   STR_DIV_MSG = "#"+String(int(AZ_SOLL_R01+1000))+":"+String(int(AZ_SOLL_R02+1000))+":"+String(int(AZ_SOLL_R03+1000))+":"+String(int(AZ_SOLL_R04+1000))+
                 ":"+String(int(AZ_1+1000))+":"+String(int(AZ_2+1000))+":"+String(int(AZ_3+1000))+":"+String(int(AZ_4+1000))+":"+String(int(RICHTUNG_R1))
                 +":"+String(int(RICHTUNG_R2))+":"+String(int(RICHTUNG_R3))+":"+String(int(RICHTUNG_R4));
   SEND_SERIAL(); 

   STR_DIV_MSG = "/"+String(int(lowerLimit_R1+1000))+":"+String(int(lowerLimit_R2+1000))+":"+String(int(lowerLimit_R3+1000))+":"+String(int(lowerLimit_R4+1000))
                +":"+String(long(limitWidth_R1+100000))+":"+String(long(limitWidth_R2+100000))+":"+String(long(limitWidth_R3+100000))+":"+String(long(limitWidth_R4+100000))
                +":"+String(long(POTI_IST_R1+100000))+":"+String(long(POTI_IST_R2+100000))+":"+String(long(POTI_IST_R3+100000))+":"+String(long(POTI_IST_R4+100000));
   SEND_SERIAL();
   
   STR_DIV_MSG = "?"+String(int(AZ_Shift_R1+1000))+":"+String(int(AZ_Shift_R2+1000))+":"+String(int(AZ_Shift_R3+1000))+":"+String(int(AZ_Shift_R4+1000))
                +":"+String(int(DREH_GRAD_ROTOR_1))+":"+String(int(DREH_GRAD_ROTOR_2))+":"+String(int(DREH_GRAD_ROTOR_3))+":"+String(int(DREH_GRAD_ROTOR_4));
   SEND_SERIAL();
   STR_DIV_MSG = "!ROTOREN: "+S_R1+" - "+S_R2+" - "+S_R3+" - "+S_R4 ; SEND_SERIAL();
  }
//******************************************************************************************
void DREH_CW_R1()
  {
  digitalWrite(CCW_R1, HIGH); 
   delay(10);
  digitalWrite(CW_R1, LOW);
  S_R1 = "R1:R";WRITE_DEBUG();
  delay(1);
  }
//******************************************************************************************
void DREH_CCW_R1()
 {  
  digitalWrite(CW_R1, HIGH);
  delay(10);
  digitalWrite(CCW_R1, LOW);
  S_R1 = "R1:L";WRITE_DEBUG();
  delay(1);
  }
//******************************************************************************************    
void DREH_STOP_R1()
 {
  digitalWrite(CCW_R1,HIGH);
  digitalWrite(CW_R1, HIGH);
  R1_dreht = false;
  S_R1 = "R1:O";WRITE_DEBUG();
  delay(1);
  }  
//******************************************************************************************
void DREH_CW_R2()
  {
  digitalWrite(CCW_R2, HIGH);
  delay(10); 
  digitalWrite(CW_R2, LOW);
  S_R2 = "R2:R";WRITE_DEBUG();
  delay(1);
  }
//******************************************************************************************  
void DREH_CCW_R2()
 {
  digitalWrite(CW_R2, HIGH);
  delay(10);
  digitalWrite(CCW_R2, LOW);
  S_R2 = "R2:L";WRITE_DEBUG();
  delay(1);
  }
//******************************************************************************************    
void DREH_STOP_R2()
 {
  digitalWrite(CCW_R2,HIGH);
  digitalWrite(CW_R2, HIGH);
  R2_dreht = false;
  S_R2 = "R2:O";WRITE_DEBUG();
  delay(1);
  } 
//******************************************************************************************
void DREH_CW_R3()
  {
  digitalWrite(CCW_R3, HIGH); 
  delay(10); 
  digitalWrite(CW_R3, LOW);
  S_R3 = "R3:R";WRITE_DEBUG();
  delay(1);
  }
//******************************************************************************************  
void DREH_CCW_R3()
 {
  digitalWrite(CW_R3, HIGH);
  delay(10);
  digitalWrite(CCW_R3, LOW);
  S_R3 = "R3:L";WRITE_DEBUG();
  delay(1);
  }
//******************************************************************************************    
void DREH_STOP_R3()
 {
  digitalWrite(CCW_R3,HIGH);
  digitalWrite(CW_R3, HIGH);
  R3_dreht = false;
  S_R3 = "R3:O";WRITE_DEBUG();
  delay(1);
  } 
//******************************************************************************************
void DREH_CW_R4()
  {
  digitalWrite(CCW_R4, HIGH); 
  delay(10);
  digitalWrite(CW_R4, LOW);
  S_R4 = "R4:R";WRITE_DEBUG();
  delay(1);
  }
//******************************************************************************************  
void DREH_CCW_R4()
 {
  digitalWrite(CW_R4, HIGH);
  delay(10);
  digitalWrite(CCW_R4, LOW);
  S_R4 = "R4:L";WRITE_DEBUG();
  delay(1);
  }
//******************************************************************************************    
void DREH_STOP_R4()
 {
  digitalWrite(CCW_R4,HIGH);
  digitalWrite(CW_R4, HIGH);
  R4_dreht = false;
  S_R4 = "R4:O";WRITE_DEBUG();  
  delay(1);
  } 
//****************************************************************************************** 


//****************************************************************************************** 
 void TASTEN_SCHALTEN(){
  if (i==10)       {                  
                    READ_ALL_POTI();
                    AZ_ANZ_R1 = AZ_IST_R1 + AZ_Shift_R1;
                    if (AZ_ANZ_R1 > 360){AZ_1=AZ_ANZ_R1-360;} else{ AZ_1=AZ_ANZ_R1;}
                    if (int(AZ_1) % 30 >= 15) {AZ_SOLL_R01 = AZ_1 - (int(AZ_1) % 30) + 30;} else {AZ_SOLL_R01 = AZ_1 - (int(AZ_1) % 30);}

                    AZ_ANZ_R2 = AZ_IST_R2 + AZ_Shift_R2;
                    if (AZ_ANZ_R2 > 360){AZ_2=AZ_ANZ_R2-360;} else{ AZ_2=AZ_ANZ_R2;}
                    if (int(AZ_2) % 30 >= 15) {AZ_SOLL_R02 = AZ_2 - (int(AZ_2) % 30) + 30;} else {AZ_SOLL_R02 = AZ_2 - (int(AZ_2) % 30);}

                    AZ_ANZ_R3 = AZ_IST_R3 + AZ_Shift_R3;
                    if (AZ_ANZ_R3 > 360){AZ_3=AZ_ANZ_R3-360;} else{ AZ_3=AZ_ANZ_R3;}
                    if (int(AZ_3) % 30 >= 15) {AZ_SOLL_R03 = AZ_3 - (int(AZ_3) % 30) + 30;} else {AZ_SOLL_R03 = AZ_3 - (int(AZ_3) % 30);}

                    AZ_ANZ_R4 = AZ_IST_R4 + AZ_Shift_R4;
                    if (AZ_ANZ_R4 > 360){AZ_4=AZ_ANZ_R4-360;} else{ AZ_4=AZ_ANZ_R4;}
                    if (int(AZ_4) % 30 >= 15) {AZ_SOLL_R04 = AZ_4 - (int(AZ_4) % 30) + 30;} else {AZ_SOLL_R04 = AZ_4 - (int(AZ_4) % 30);}
                    
                    WRITE_DEBUG();
                   }
//**********************AZ_SOLL_R1 ********************************
  if (i==1000)      {R1_dreht = true;AZ_SOLL_R01 = 0; Dreh_R1_ber();}    // >> ROTOR 1 - 0°
  if (i==1030)      {R1_dreht = true;AZ_SOLL_R01 = 30; Dreh_R1_ber();}   // >> ROTOR 1 - 30°
  if (i==1060)      {R1_dreht = true;AZ_SOLL_R01 = 60; Dreh_R1_ber();}   // >> ROTOR 1 - 60°
  if (i==1090)      {R1_dreht = true;AZ_SOLL_R01 = 90; Dreh_R1_ber();}   // >> ROTOR 1 - 90°
  if (i==1120)      {R1_dreht = true;AZ_SOLL_R01 = 120; Dreh_R1_ber();}  // >> ROTOR 1 - 120°
  if (i==1150)      {R1_dreht = true;AZ_SOLL_R01 = 150; Dreh_R1_ber();}  // >> ROTOR 1 - 150°
  if (i==1180)      {R1_dreht = true;AZ_SOLL_R01 = 180; Dreh_R1_ber();}  // >> ROTOR 1 - 180°
  if (i==1210)      {R1_dreht = true;AZ_SOLL_R01 = 210; Dreh_R1_ber();}  // >> ROTOR 1 - 210°
  if (i==1240)      {R1_dreht = true;AZ_SOLL_R01 = 240; Dreh_R1_ber();}  // >> ROTOR 1 - 240°
  if (i==1270)      {R1_dreht = true;AZ_SOLL_R01 = 270; Dreh_R1_ber();}  // >> ROTOR 1 - 270°
  if (i==1300)      {R1_dreht = true;AZ_SOLL_R01 = 300; Dreh_R1_ber();}  // >> ROTOR 1 - 300°
  if (i==1330)      {R1_dreht = true;AZ_SOLL_R01 = 330; Dreh_R1_ber();}  // >> ROTOR 1 - 330° 

  if (i==1501)      { DREH_CCW_R1(); AZ_SOLL_R01 = AZ_SOLL_R1 - 15;
                      if (AZ_SOLL_R01 < 0) {AZ_SOLL_R1 = 345;}
                      if (AZ_SOLL_R01 > 360) {AZ_SOLL_R1 = 15;}
                    }  // >> ROTOR 1 - 15°
  if (i==1502)      {R1_dreht = false; DREH_STOP_R1(); STR_DIV_MSG = "! ROTOR 1 - GESTOPPT"; SEND_SERIAL();}  // >> ROTOR 1 STOP
  if (i==1503)      { DREH_CW_R1(); AZ_SOLL_R1 = AZ_SOLL_R1 + 15;
                      if (AZ_SOLL_R01 < 0) {AZ_SOLL_R1 = 345;}
                      if (AZ_SOLL_R01 > 360) {AZ_SOLL_R1 = 15;}
                    }  // >> ROTOR 1 + 15°
  
//**********************AZ_SOLL_R2 ********************************
  if (i==2000)      {R2_dreht = true;AZ_SOLL_R02 = 0;Dreh_R2_ber();}    // >> ROTOR 2 - 0°
  if (i==2030)      {R2_dreht = true;AZ_SOLL_R02 = 30;Dreh_R2_ber();}   // >> ROTOR 2 - 30°
  if (i==2060)      {R2_dreht = true;AZ_SOLL_R02 = 60;Dreh_R2_ber();}   // >> ROTOR 2 - 60°
  if (i==2090)      {R2_dreht = true;AZ_SOLL_R02 = 90;Dreh_R2_ber();}   // >> ROTOR 2 - 90°
  if (i==2120)      {R2_dreht = true;AZ_SOLL_R02 = 120;Dreh_R2_ber();}  // >> ROTOR 2 - 120°
  if (i==2150)      {R2_dreht = true;AZ_SOLL_R02 = 150;Dreh_R2_ber();}  // >> ROTOR 2 - 150°
  if (i==2180)      {R2_dreht = true;AZ_SOLL_R02 = 180;Dreh_R2_ber();}  // >> ROTOR 2 - 180°
  if (i==2210)      {R2_dreht = true;AZ_SOLL_R02 = 210;Dreh_R2_ber();}  // >> ROTOR 2 - 210°
  if (i==2240)      {R2_dreht = true;AZ_SOLL_R02 = 240;Dreh_R2_ber();}  // >> ROTOR 2 - 240°
  if (i==2270)      {R2_dreht = true;AZ_SOLL_R02 = 270;Dreh_R2_ber();}  // >> ROTOR 2 - 270°
  if (i==2300)      {R2_dreht = true;AZ_SOLL_R02 = 300;Dreh_R2_ber();}  // >> ROTOR 2 - 300°
  if (i==2330)      {R2_dreht = true;AZ_SOLL_R02 = 330;Dreh_R2_ber();}  // >> ROTOR 2 - 330° 

  if (i==2501)      { DREH_CCW_R2(); AZ_SOLL_R2 = AZ_SOLL_R2 - 15;
                      if (AZ_SOLL_R2 < 0) {AZ_SOLL_R2 = 345;}
                      if (AZ_SOLL_R2 > 360) {AZ_SOLL_R2 = 15;}
                      }  // >> ROTOR 2 - 15°
  if (i==2502)      {DREH_STOP_R2(); STR_DIV_MSG = "! ROTOR 2 - GESTOPPT"; SEND_SERIAL();}  // >> ROTOR 2 STOP
  if (i==2503)      { DREH_CW_R2(); AZ_SOLL_R2 = AZ_SOLL_R2 + 15;
                      if (AZ_SOLL_R2 < 0) {AZ_SOLL_R2 = 345;}
                      if (AZ_SOLL_R2 > 360) {AZ_SOLL_R2 = 15;}
                      }  // >> ROTOR 2 + 15°
  
//**********************AZ_SOLL_R3 ********************************
  if (i==3000)      {R3_dreht = true;AZ_SOLL_R03 = 0;Dreh_R3_ber();}    // >> ROTOR 3 - 0°
  if (i==3030)      {R3_dreht = true;AZ_SOLL_R03 = 30;Dreh_R3_ber();}   // >> ROTOR 3 - 30°
  if (i==3060)      {R3_dreht = true;AZ_SOLL_R03 = 60;Dreh_R3_ber();}   // >> ROTOR 3 - 60°
  if (i==3090)      {R3_dreht = true;AZ_SOLL_R03 = 90;Dreh_R3_ber();}   // >> ROTOR 3 - 90°
  if (i==3120)      {R3_dreht = true;AZ_SOLL_R03 = 120;Dreh_R3_ber();}  // >> ROTOR 3 - 120°
  if (i==3150)      {R3_dreht = true;AZ_SOLL_R03 = 150;Dreh_R3_ber();}  // >> ROTOR 3 - 150°
  if (i==3180)      {R3_dreht = true;AZ_SOLL_R03 = 180;Dreh_R3_ber();}  // >> ROTOR 3 - 180°
  if (i==3210)      {R3_dreht = true;AZ_SOLL_R03 = 210;Dreh_R3_ber();}  // >> ROTOR 3 - 210°
  if (i==3240)      {R3_dreht = true;AZ_SOLL_R03 = 240;Dreh_R3_ber();}  // >> ROTOR 3 - 240°
  if (i==3270)      {R3_dreht = true;AZ_SOLL_R03 = 270;Dreh_R3_ber();}  // >> ROTOR 3 - 270°
  if (i==3300)      {R3_dreht = true;AZ_SOLL_R03 = 300;Dreh_R3_ber();}  // >> ROTOR 3 - 300°
  if (i==3330)      {R3_dreht = true;AZ_SOLL_R03 = 330;Dreh_R3_ber();}  // >> ROTOR 3 - 330° 

  if (i==3501)      { DREH_CCW_R3(); AZ_SOLL_R3 = AZ_SOLL_R3 - 15;
                      if (AZ_SOLL_R3 < 0) {AZ_SOLL_R3 = 345;}
                      if (AZ_SOLL_R3 > 360) {AZ_SOLL_R3 = 15;}
                      }  // >> ROTOR 3 - 15°
  if (i==3502)      {DREH_STOP_R3(); STR_DIV_MSG = "! ROTOR 3 - GESTOPPT"; SEND_SERIAL();}  // >> ROTOR 3 STOP
  if (i==3503)      { DREH_CW_R3(); AZ_SOLL_R3 = AZ_SOLL_R3 + 15;
                      if (AZ_SOLL_R3 < 0) {AZ_SOLL_R3 = 345;}
                      if (AZ_SOLL_R3 > 360) {AZ_SOLL_R3 = 15;}
                      }  // >> ROTOR 3 + 15°
  
//**********************AZ_SOLL_R4 ********************************
  if (i==4000)      {R4_dreht = true;AZ_SOLL_R04 = 0;Dreh_R4_ber();}    // >> ROTOR 4 - 0°
  if (i==4030)      {R4_dreht = true;AZ_SOLL_R04 = 30;Dreh_R4_ber();}   // >> ROTOR 4 - 30°
  if (i==4060)      {R4_dreht = true;AZ_SOLL_R04 = 60;Dreh_R4_ber();}   // >> ROTOR 4 - 60°
  if (i==4090)      {R4_dreht = true;AZ_SOLL_R04 = 90;Dreh_R4_ber();}   // >> ROTOR 4 - 90°
  if (i==4120)      {R4_dreht = true;AZ_SOLL_R04 = 120;Dreh_R4_ber();}  // >> ROTOR 4 - 120°
  if (i==4150)      {R4_dreht = true;AZ_SOLL_R04 = 150;Dreh_R4_ber();}  // >> ROTOR 4 - 150°
  if (i==4180)      {R4_dreht = true;AZ_SOLL_R04 = 180;Dreh_R4_ber();}  // >> ROTOR 4 - 180°
  if (i==4210)      {R4_dreht = true;AZ_SOLL_R04 = 210;Dreh_R4_ber();}  // >> ROTOR 4 - 210°
  if (i==4240)      {R4_dreht = true;AZ_SOLL_R04 = 240;Dreh_R4_ber();}  // >> ROTOR 4 - 240°
  if (i==4270)      {R4_dreht = true;AZ_SOLL_R04 = 270;Dreh_R4_ber();}  // >> ROTOR 4 - 270°
  if (i==4300)      {R4_dreht = true;AZ_SOLL_R04 = 300;Dreh_R4_ber();}  // >> ROTOR 4 - 300°
  if (i==4330)      {R4_dreht = true;AZ_SOLL_R04 = 330;Dreh_R4_ber();}  // >> ROTOR 4 - 330° 

  if (i==4501)      { DREH_CCW_R4(); AZ_SOLL_R4 = AZ_SOLL_R4 - 15;
                      if (AZ_SOLL_R4 < 0) {AZ_SOLL_R4 = 345;}
                      if (AZ_SOLL_R4 > 360) {AZ_SOLL_R4 = 15;}
                      }  // >> ROTOR 4 - 15°
  if (i==4502)      {DREH_STOP_R4(); STR_DIV_MSG = "! ROTOR 4 - GESTOPPT"; SEND_SERIAL();}  // >> ROTOR 4 STOP
  if (i==4503)      { DREH_CW_R4();AZ_SOLL_R4 = AZ_SOLL_R4 + 15;
                      if (AZ_SOLL_R4 < 0) {AZ_SOLL_R4 = 345;}
                      if (AZ_SOLL_R4 > 360) {AZ_SOLL_R4 = 15;}
                     }  // >> ROTOR 4 + 15°
  
 //********************** PARAMETER ******************************** 
 
  if (i>=5000 && i<=5999)
  { NUMBER=i-5000;
      if (NUMBER == 450) {DREH_GRAD_ROTOR_1 = 450;NUMBER=0;}
      if (NUMBER == 360) {DREH_GRAD_ROTOR_1 = 360;NUMBER=0;}
      if (NUMBER >=0 && NUMBER <=359) {AZ_Shift_R1 = NUMBER;NUMBER=0;}
      NUMBER=0;
  }
  if (i>=6000 && i<=6999)
  { NUMBER=i-6000;
      if (NUMBER == 450) {DREH_GRAD_ROTOR_2 = 450;NUMBER=0;}
      if (NUMBER == 360) {DREH_GRAD_ROTOR_2 = 360;NUMBER=0;}
      if (NUMBER >=0 && NUMBER <=359) {AZ_Shift_R2 = NUMBER;NUMBER=0;}
      NUMBER=0;
  }
  if (i>=7000 && i<=7999)
  { NUMBER=i-7000;
      if (NUMBER == 450) {DREH_GRAD_ROTOR_3 = 450;NUMBER=0;}
      if (NUMBER == 360) {DREH_GRAD_ROTOR_3 = 360;NUMBER=0;}
      if (NUMBER >=0 && NUMBER <=359) {AZ_Shift_R3 = NUMBER;NUMBER=0;}
      NUMBER=0;
  }
  if (i>=8000 && i<=8999)
  { NUMBER=i-8000;
      if (NUMBER == 450) {DREH_GRAD_ROTOR_4 = 450;NUMBER=0;}
      if (NUMBER == 360) {DREH_GRAD_ROTOR_4 = 360;NUMBER=0;}
      if (NUMBER >=0 && NUMBER <=359) {AZ_Shift_R4 = NUMBER;NUMBER=0;}
      NUMBER=0;
  }
    if (i>=11000 && i<=11999) { NUMBER=i-11000; lowerLimit_R1 = NUMBER;NUMBER=0;}
    if (i>=12000 && i<=12999) { NUMBER=i-12000; lowerLimit_R2 = NUMBER;NUMBER=0;}
    if (i>=13000 && i<=13999) { NUMBER=i-13000; lowerLimit_R3 = NUMBER;NUMBER=0;}
    if (i>=14000 && i<=14999) { NUMBER=i-14000; lowerLimit_R4 = NUMBER;NUMBER=0;}

    if (i>=110000 && i<=119999) { NUMBER=i-110000; limitWidth_R1 = NUMBER;NUMBER=0;}
    if (i>=120000 && i<=129999) { NUMBER=i-120000; limitWidth_R2 = NUMBER;NUMBER=0;}
    if (i>=130000 && i<=139999) { NUMBER=i-130000; limitWidth_R3 = NUMBER;NUMBER=0;}
    if (i>=140000 && i<=149999) { NUMBER=i-140000; limitWidth_R4 = NUMBER;NUMBER=0;}
    
WRITE_DEBUG();
}
//******************************************************************************************
void Dreh_R1_ber()  // Drehbewegung vorbereiten
    {
            AZ_SOLL_R1= AZ_SOLL_R01 - AZ_Shift_R1; if (AZ_SOLL_R1 < 0) { AZ_SOLL_R1= 360 + AZ_SOLL_R1; }   
     
            WEG1_R1 = AZ_IST_R1 - AZ_SOLL_R1 - 360; // ******** AZ_IST und AZ_SOLL auswerten und den kuerzeren Weg besstimmen 
            WEG2_R1 = AZ_IST_R1 - AZ_SOLL_R1 - 0;
     
            if (abs(WEG1_R1) > abs(WEG2_R1)) {RICHTUNG_R1 =  AZ_SOLL_R1 - AZ_IST_R1;} else { RICHTUNG_R1 =  AZ_SOLL_R1 + AZ_IST_R1;}

            if (AZ_SOLL_R1 < 0) {AZ_SOLL_R1 = 0;}
            if (AZ_SOLL_R1 > DREH_GRAD_ROTOR_1) {AZ_SOLL_R1 = DREH_GRAD_ROTOR_1;}
            
            READ_ALL_POTI(); //AZ_IST_R1 bis R4 ermitteln
            WRITE_DEBUG();
            PosRotor();
    }
void Dreh_R2_ber()  // Drehbewegung vorbereiten
    {
            AZ_SOLL_R2= AZ_SOLL_R02 - AZ_Shift_R2; if (AZ_SOLL_R2 < 0) { AZ_SOLL_R2= 360 + AZ_SOLL_R2; }   
     
            WEG1_R2 = AZ_IST_R2 - AZ_SOLL_R2 - 360; // ******** AZ_IST und AZ_SOLL auswerten und den kuerzeren Weg besstimmen 
            WEG2_R2 = AZ_IST_R2 - AZ_SOLL_R2 - 0;
     
            if (abs(WEG1_R2) > abs(WEG2_R2)) {RICHTUNG_R2 =  AZ_SOLL_R2 - AZ_IST_R2;} else { RICHTUNG_R2 =  AZ_SOLL_R2 + AZ_IST_R2;}

            if (AZ_SOLL_R2 < 0) {AZ_SOLL_R2 = 0;}
            if (AZ_SOLL_R2 > DREH_GRAD_ROTOR_2) {AZ_SOLL_R2 = DREH_GRAD_ROTOR_2;}
            
            READ_ALL_POTI(); //AZ_IST_R1 bis R4 ermitteln
            WRITE_DEBUG();
            PosRotor();
    }
    void Dreh_R3_ber()  // Drehbewegung vorbereiten
    {
            AZ_SOLL_R3= AZ_SOLL_R03 - AZ_Shift_R3; if (AZ_SOLL_R3 < 0) { AZ_SOLL_R3= 360 + AZ_SOLL_R3; }   
     
            WEG1_R3 = AZ_IST_R3 - AZ_SOLL_R3 - 360; // ******** AZ_IST und AZ_SOLL auswerten und den kuerzeren Weg besstimmen 
            WEG2_R3 = AZ_IST_R3 - AZ_SOLL_R3 - 0;
     
            if (abs(WEG1_R3) > abs(WEG2_R3)) {RICHTUNG_R3 =  AZ_SOLL_R3 - AZ_IST_R3;} else { RICHTUNG_R3 =  AZ_SOLL_R3 + AZ_IST_R3;}

            if (AZ_SOLL_R3 < 0) {AZ_SOLL_R3 = 0;}
            if (AZ_SOLL_R3 > DREH_GRAD_ROTOR_3) {AZ_SOLL_R3 = DREH_GRAD_ROTOR_3;}
            
            READ_ALL_POTI(); //AZ_IST_R1 bis R4 ermitteln
            WRITE_DEBUG();
            PosRotor();
    }
    void Dreh_R4_ber()  // Drehbewegung vorbereiten
    {
            AZ_SOLL_R4= AZ_SOLL_R04 - AZ_Shift_R4; if (AZ_SOLL_R4 < 0) { AZ_SOLL_R4= 360 + AZ_SOLL_R4; }   
     
            WEG1_R4 = AZ_IST_R4 - AZ_SOLL_R4 - 360; // ******** AZ_IST und AZ_SOLL auswerten und den kuerzeren Weg besstimmen 
            WEG2_R4 = AZ_IST_R4 - AZ_SOLL_R4 - 0;
     
            if (abs(WEG1_R4) > abs(WEG2_R4)) {RICHTUNG_R4 =  AZ_SOLL_R4 - AZ_IST_R4;} else { RICHTUNG_R4 =  AZ_SOLL_R4 + AZ_IST_R4;}

            if (AZ_SOLL_R4 < 0) {AZ_SOLL_R4 = 0;}
            if (AZ_SOLL_R4 > DREH_GRAD_ROTOR_4) {AZ_SOLL_R4 = DREH_GRAD_ROTOR_4;}
            
            READ_ALL_POTI(); //AZ_IST_R1 bis R4 ermitteln
            WRITE_DEBUG();
            PosRotor();
    }
  void PosRotor()
  {                
     if (R1_dreht == true)
     {     
               READ_ALL_POTI();
               AZ_ANZ_R1 = AZ_IST_R1 + AZ_Shift_R1;
               if (AZ_ANZ_R1 > 360){AZ_1=AZ_ANZ_R1-360;} else{ AZ_1=AZ_ANZ_R1;}
               
                T = millis();
  if (T >= T - 1000) {PULSE = true;}
  if (PULSE == true){WRITE_DEBUG(); PULSE = false;} 
               
                RICHTUNG_R1 =  AZ_SOLL_R1 - AZ_IST_R1;
                               
              // *** CW ***         
                if (RICHTUNG_R1 > TOL_R1){DREH_CW_R1();}              
            
              // *** CCW *** 
                if (RICHTUNG_R1 < -TOL_R1){DREH_CCW_R1();}             
                                 
              // *** STOP ***
                if (RICHTUNG_R1 < TOL_R1 && RICHTUNG_R1 > -TOL_R1) { DREH_STOP_R1();}          
       }
      if (R2_dreht == true)
     {     
               READ_ALL_POTI();
               AZ_ANZ_R2 = AZ_IST_R2 + AZ_Shift_R2;
               if (AZ_ANZ_R2 > 360){AZ_2=AZ_ANZ_R2-360;} else{ AZ_2=AZ_ANZ_R2;}
               
      T = millis();
  if (T >= T - 1000) {PULSE = true;}
  if (PULSE == true){WRITE_DEBUG(); PULSE = false;}
               
                RICHTUNG_R2 =  AZ_SOLL_R2 - AZ_IST_R2;
                               
              // *** CW ***         
                if (RICHTUNG_R2 > TOL_R2){DREH_CW_R2();}              
            
              // *** CCW *** 
                if (RICHTUNG_R2 < -TOL_R2){DREH_CCW_R2();}             
                                 
              // *** STOP ***
                if (RICHTUNG_R2 < TOL_R2 && RICHTUNG_R2 > -TOL_R2) { DREH_STOP_R2();}          
       }
            if (R3_dreht == true)
     {     
               READ_ALL_POTI();
               AZ_ANZ_R3 = AZ_IST_R3 + AZ_Shift_R3;
               if (AZ_ANZ_R3 > 360){AZ_3=AZ_ANZ_R3-360;} else{ AZ_1=AZ_ANZ_R3;}
               WRITE_DEBUG(); 
               
  T = millis();
  if (T >= T - 1000) {PULSE = true;}
  if (PULSE == true){WRITE_DEBUG(); PULSE = false;}
                               
              // *** CW ***         
                if (RICHTUNG_R3 > TOL_R3){DREH_CW_R3();}              
            
              // *** CCW *** 
                if (RICHTUNG_R3 < -TOL_R3){DREH_CCW_R3();}             
                                 
              // *** STOP ***
                if (RICHTUNG_R3 < TOL_R3 && RICHTUNG_R3 > -TOL_R3) { DREH_STOP_R3();}          
       }
            if (R4_dreht == true)
     {     
               READ_ALL_POTI();
               AZ_ANZ_R4 = AZ_IST_R4 + AZ_Shift_R4;
               if (AZ_ANZ_R4 > 360){AZ_4=AZ_ANZ_R4-360;} else{ AZ_4=AZ_ANZ_R4;}
               
          T = millis();
  if (T >= T - 1000) {PULSE = true;}
  if (PULSE == true){WRITE_DEBUG(); PULSE = false;}
               
                RICHTUNG_R4 =  AZ_SOLL_R4 - AZ_IST_R4;
                               
              // *** CW ***         
                if (RICHTUNG_R4 > TOL_R4){DREH_CW_R4();}              
            
              // *** CCW *** 
                if (RICHTUNG_R4 < -TOL_R4){DREH_CCW_R4();}             
                                 
              // *** STOP ***
                if (RICHTUNG_R4 < TOL_R4 && RICHTUNG_R4 > -TOL_R4) { DREH_STOP_R4();}          
       }     

   }
   
//****************************************************************************************** 
//****************************************************************************************** 
// ******************************* LOOP *********************************************
void loop() 
{ 
    if (Serial.available() > 0)
    {
        // ****  lese letzten byte vom serial buffer:
         i = ( Serial.parseInt());
         TASTEN_SCHALTEN();
    }

//    int16_t adc0, adc1, adc2, adc3;
//  POTI_IST_R1 = ads1115.readADC_SingleEnded(0);
//  POTI_IST_R2 = ads1115.readADC_SingleEnded(1);
//  POTI_IST_R3 = ads1115.readADC_SingleEnded(2);
//  POTI_IST_R4 = ads1115.readADC_SingleEnded(3); 
      
          //  READ_ALL_POTI(); //AZ_IST_R1 bis R4 ermitteln
            //Serial.print(POTI_IST_R1);Serial.print("   ");Serial.print(AZ_IST_R1);Serial.print("   ");Serial.print(POTI_IST_R2);Serial.print("   ");Serial.println(AZ_IST_R2);
      //Serial.print(POTI_IST_R1);Serial.print("   ");Serial.println(POTI_IST_R2);
         //   WRITE_DEBUG();
     
  PosRotor(); delay(100);

  
 }
//// *********************************** ENDE LOOP **********************************************
