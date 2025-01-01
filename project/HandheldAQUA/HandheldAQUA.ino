#include <LiquidCrystal_I2C.h>
float _voltage = 0.0;
// TMP
const float Ko = 1.02119e-3;
const float K1 = 2.22468e-4;
const float K2 = 1.33342e-7;
uint16_t resistance = 17;
float _slope_temp=1.0915;
float _intercept_temp=-3.3994;
// sal
float slope1 = -7.78;
float intercept1 = 16.34;
// PH
float slope = 16.3;
float intercept = 0.0;
//NH4
float _slope_NH4=1;
float _intercept_NH4=0;
// do
float _slope_DO=4.444;
float _intercept_DO=-0.4444;
// begin
LiquidCrystal_I2C lcd(0x27, 20, 4);
int input=A2;
int ID=A1;
int id=0;
//tran analog to voltage
float voltage(int analog){
  int numberAveraged = 10; 
  int _count;
  int _sum = 0;
  float _voltage=0;
  for (int _i = 0 ; _i < numberAveraged; _i++)
  {
    _count = analogRead(analog); 
    _sum = _sum + _count;
  }
  _voltage = _sum / numberAveraged / 1024.0 * 5.0;
  return _voltage;
}
// get sensor id
int getid(){
  if(voltage(ID)>=3.60 && voltage(ID)<=3.65 ){
    return 1;
  }
  else if(voltage(ID)>=1.57 &&voltage(ID)<=1.65 ){
    return 2;
  }
  else if(voltage(ID)>=3.62  ){
     return 3;
  }
  // else if(voltage(ID)>=3.58&& voltage(input)!=0&& voltage(ID)<=3.62 ){
  //   return 4;
  // }
   else if(voltage(ID)>=3.53 &&voltage(ID)<=3.564 ){
    return 5;
  }
  else{
    return 0;
  }
}
void setup(){
  pinMode(input,INPUT);
  pinMode(ID,INPUT);
  Serial.begin(9600);
  lcd.init(); 
  lcd.backlight(); 
}
void loop(){
  id=getid();
  Serial.println(voltage(ID));
  Serial.println(id);

  lcd.setCursor(2, 1);
  lcd.print("HANDHELD DEVICES");
  delay(500);
  if(id!=0){
    int dem=0;
    lcd.clear();
    while(id==1)//salinity
    {    
  Serial.println(voltage(input));

      delay(200);
      if(dem==0){
      lcd.setCursor(0, 0);
      lcd.print("ID: ");
      lcd.setCursor(3, 0);
      lcd.print(id);
      lcd.setCursor(9, 0);
      lcd.print("salnility");
      lcd.setCursor(7, 2);
      lcd.print("ptt");
  Serial.println(id);
      dem++;}
      lcd.setCursor(7, 1);
      float Sal = voltage(input) * slope + intercept;
      Sal = Sal * 0.8063 + 0.1614;
      Sal = Sal * 1.0604 - 1.3692;
      if (voltage == 0)
      {
        Sal = 0;
      }
      if (Sal < 0)
      {
        Sal = 0;
      }
      lcd.print(Sal);
      if(getid()==0){
        dem=0;
        break;
      }
    }
    while(id==2)// TMP
    {
      delay(200);
      if(dem==0){
      lcd.setCursor(0, 0);
      lcd.print("ID: ");
      lcd.setCursor(3, 0);
      lcd.print(id);
      lcd.setCursor(9, 0);
      lcd.print("TMP");
      lcd.setCursor(8, 2);  
      lcd.print("oC");
  Serial.println(id);
      dem++;}
      lcd.setCursor(7, 1);  
      float measured_resistance = (resistance * voltage(input)) / (5 - voltage(input));
      float temp = 1.0 / (Ko + K1 * logf(1000 * measured_resistance) + K2 * powf(log(1000.0 * measured_resistance), 3)) - 273.15;
      lcd.print( temp * _slope_temp + _intercept_temp);
      if(getid()==0){
        dem=0;
        break;
      }
    }
    while(id==3)//PH  
    {
      delay(200);
      if(dem==0){
      lcd.setCursor(0, 0);
      lcd.print("ID: ");
      lcd.setCursor(3, 0);
      lcd.print(id);
      lcd.setCursor(9, 0);
      lcd.print("PH");
      lcd.setCursor(8, 2);  
      lcd.print("H+");
  Serial.println(id);

      dem++;}
      lcd.setCursor(7, 1);    
      lcd.print( voltage(input) * slope1 + intercept1);
      if(getid()==0){
        dem=0;
        break;
      }
    }
    while(id==4)//NH4
    {
      delay(200);
      if(dem==0){
      lcd.setCursor(0, 0);
      lcd.print("ID: ");
      lcd.setCursor(3, 0);
      lcd.print(id);
      lcd.setCursor(9, 0);
      lcd.print("NH4");
  Serial.println(id);

      dem++;}
      lcd.setCursor(7, 1);    
      lcd.print( "                   ");
      lcd.setCursor(7, 1);    
      float NH4 = exp(((((voltage(input) - 1.223) / 0.00727) + 168.23) / 23.889) - 7.487);
      NH4 = NH4 * _slope_NH4 + _intercept_NH4;
      NH4 = NH4 * 0.3446 + 0.0766;
      lcd.print( NH4);
      if(getid()==0){
        dem=0;
        break;
      }
    }
     while(id==5)//DO
    {
      delay(200);
      if(dem==0){
      lcd.setCursor(0, 0);
      lcd.print("ID: ");
      lcd.setCursor(3, 0);
      lcd.print(id);
      lcd.setCursor(9, 0);
      lcd.print("DO");
      dem++;}
      lcd.setCursor(7, 1);    
      float DO = voltage(input) * _slope_DO + _intercept_DO;
      DO = DO * 0.582 + 3.2405; // calibrate DO 3/1/2024
      if (DO < 0)
      {
        DO = 0;
      }
      lcd.print(DO);
      if(getid()==0){
        dem=0;
        break;
      }
    }
  }
  else{
    lcd.clear();
  }
}