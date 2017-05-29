#define input_voltage A3
#define usb A7
#define battery A1
#define current A0
bool charge = false;
int avg = 0;
/////////////////// we have 3 voltages to show

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

float ipVoltage = 0.0;
float amps = 0.0;
int v = 0;
float w = 0.0;
float per = 0.0;
void setup() {
 Serial.begin(115200);
 lcd.begin(20,4);  
 lcd.backlight();
 pinMode(3,OUTPUT);
 pinMode(4,OUTPUT);   //  charge or not
 pinMode(5,OUTPUT);   //  full charge 
 pinMode(6,OUTPUT);   //   load or not
 digitalWrite(4,HIGH);
 digitalWrite(5,HIGH);
 digitalWrite(6,HIGH);

 drawTemplate();
}

void loop() {
  
  float tmp,vread;
  lcd.setCursor(0,0);
  lcd.print(" ");
  
  tmp = analogRead(input_voltage);
  vread = tmp/1023 * 5;
  ipVoltage = (vread/18)*100;
  if(ipVoltage>12){
    ipVoltage = ipVoltage - 1.0;
  }
  avg = ipVoltage;
   v  = ipVoltage;
  lcd.setCursor(2,1);
  lcd.print(ipVoltage);

  tmp = analogRead(usb);
  vread = tmp/1023 * 5;
  ipVoltage = vread * 2 - 0.5 ;

  lcd.setCursor(16,3);
  lcd.print(ipVoltage);

  float average = 0;
 for(int i = 0; i < 1000; i++) {
    average = average + (.0264 * analogRead(A0) -13.51);//for the 5A mode,  
// average = average + (.049 * analogRead(A0) -25);// for 20A mode
 //average = average + (.742 * analogRead(A0) -37.8);// for 30A mode
   delay(1);
 }
 amps = average/1000;
 lcd.setCursor(2,2);
 if(amps<0){
  amps = amps *-1;
  lcd.print(amps);
 }
 else{
  lcd.print(amps);
 }

 lcd.setCursor(2,3);
 w = v*amps;
 lcd.print(w);
 Serial.println(w*2);
 
  tmp = analogRead(battery);
  vread = tmp/1023 * 5;
  ipVoltage = (vread/33)*115;
  if(ipVoltage>12){
    ipVoltage = ipVoltage - 1.0;
  }
   if(w>3.5 ){               // offset for charging or not
    digitalWrite(6,LOW);
    digitalWrite(5,HIGH);
    lcd.setCursor(11,1);
  lcd.print(" CHG ");
  lcd.setCursor(8,3);
      lcd.print(per );
   }
    else{
      digitalWrite(6,HIGH);
      digitalWrite(5,LOW);
      lcd.setCursor(11,1);
      lcd.print(ipVoltage);
      lcd.setCursor(8,3);
      per = ipVoltage/13.00 * 100;
      lcd.print(per );
    }

 
 //delay(50);

}


void drawTemplate(){
  
  lcd.setCursor(0,0);
  lcd.print(" SOLAR ");
  lcd.setCursor(7,0);
  lcd.print("   BATT.");
  lcd.setCursor(14,0);
  lcd.print("   PWM");

  lcd.setCursor(0,1);
  lcd.print("V=");

  lcd.setCursor(9,1);
  lcd.print("V=");

  lcd.setCursor(0,2);
  lcd.print("I=");

  lcd.setCursor(8,2);
  lcd.print("Batt %");




  lcd.setCursor(0,3);
  lcd.print("W=");
  lcd.setCursor(6,3);

//  lcd.setCursor(8,3);
//  lcd.print("%");

  lcd.setCursor(14,2);
  lcd.print("   USB");

  lcd.setCursor(15,1);
  lcd.print("  62%");

}
