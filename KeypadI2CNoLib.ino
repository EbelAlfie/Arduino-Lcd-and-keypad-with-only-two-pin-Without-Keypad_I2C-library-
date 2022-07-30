#include<Wire.h>
#include<LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x26, 16, 2); //0x26 is LCD's address. It is required in I2C communication. It is like and address house when someone wants to send a package to your house
//Be sure to check that every device has a distinct address, such as LCD with 0x26 and keypad 0x27 

char one ; 
void setup() {
  Serial.begin(9600);
  lcd.begin();
  Wire.begin() ; //begin I2C communication
}

void loop() {
  Wire.beginTransmission(0x27); //Use this if you want to transmit data into I2C IC. 0x27 is 3x4 keypad's address
  Wire.write(B00000111); //kolom == high
  Wire.endTransmission(); //Use this to end it. Arduino wont be able to communicate to other devices via I2C if the communication to 0x27 hasn't been end yet
  if(availableChar()){
    one = getChar();
    lcd.print(one) ;
    delay(200);
  }
}

bool availableChar(){//Check if column is diffrent
  Wire.requestFrom(0x27, 1); //Use this to begin reading data from I2C device from address number 0x27
  if(Wire.available()){
    byte col = Wire.read(); //Use this to get the data
    if(col != B00000111){
      return true; //if column is diffrent, then there is a pressed key
    }else{
      return false;
    }
  }  
}

char getChar(){
      Wire.beginTransmission(0x27); //Use this if you want to transmit data into I2C IC
      Wire.write(B00000000);
      Wire.endTransmission(); //Use this to end it. Arduino wont be able to communicate to other devices via I2C if the communication to 0x27 hasn't been end yet
      byte rowShift = B0001 ;
      
      while(rowShift != B1000){
        Wire.beginTransmission(0x27); 
        Wire.write(rowShift);
        Wire.endTransmission(); 
        
        Wire.requestFrom(0x27,1);
        byte rowStats = rowShift | Wire.read();         
        if(rowStats != B00000000){ 
           switch(rowStats){
            case B00010100 ://Serial.print('1');
              return '1';
            case B00010010 ://Serial.print('2');
              return '2';
            case B00010001 : //Serial.print('3'); 
              return '3';
              
            case B00100100 ://Serial.print('4'); 
              return '4';
            case B00100010 ://Serial.print('5');  
              return '5';    
            case B00100001 ://Serial.print('6'); 
              return '6'; 

            case B01000100 : //Serial.print('7'); 
              return '7';  
            case B01000010 : //Serial.print('8');
              return '8';
            case B01000001 : //Serial.print('9'); 
              return '9';

            case B10000100 : //Serial.print('*'); 
              return '*';
            case B10000010 : //Serial.print('0'); 
              return '0';
            case B10000001 : //Serial.print('#'); 
              return '#';
              
            default: Serial.println("Error");
           }
        }
        rowShift <<= 1 ;
      }
}
