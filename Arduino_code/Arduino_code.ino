#include <Keypad.h>
#include <String.h>
#include <LiquidCrystal.h>
#include <Servo.h>
Servo myservo;
int pos=0; // LCD Connections

LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);
const byte rows=4;
const byte cols=3;

char key[rows][cols]= {  //Creating keypad
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[rows]= {1,2,3,4}; //Defining key row
byte colPins[cols]= {5,6,7};   //Defining key columns

Keypad keypad= Keypad(makeKeymap(key),rowPins,colPins,rows,cols);

String password="4567";        //Setting password
int currentposition=0;         //current posistion used for returning starting position
int redled=10;                 //Setting led to 10th pin
int greenled=11;               //Setting led to 11th pin
String epass = "";             //Password that we get from keypad keys
bool kapi=false;               //Status of gate

void setup() {

  displayscreen();
  Serial.begin(9600);
  pinMode(redled, OUTPUT);     
  pinMode(greenled, OUTPUT);
  myservo.attach(9);           //Servo attached to 9th pin
  myservo.write(180);
  lcd.begin(16,2);

}

void loop() {
  
  if(kapi==false){             //This used for starting the simulation as door closed
    pirsensor();                
    
  if(currentposition==0) {     
    displayscreen();
  }
  
  int l ;
  char code=keypad.getKey();   //Getting digits from keypad
  
  if(code!=NO_KEY) {
    epass = epass + code;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Password:");
    lcd.setCursor(10,0);
    for(l=0; l<=currentposition; ++l) {

      lcd.print("*");
    }
    ++currentposition;
    
  if(epass.length()==4){         //If the pressed number of digits equals to 4 it checks for the correctness
    
    if (epass[0]==password[0]&&epass[1]==password[1]&&epass[2]==password[2]&&epass[3]==password[3]) // Compares the password and pressed password digit by digit
    {
        kapi=true;
        unlockdoor();
        currentposition=0;
        epass="";
    }

    else {                       //If the pressed password is wrong
      incorrect();
      currentposition=0;
      epass="";
    }
}

  }
}

  else{                        //If the gate is open
    char code=keypad.getKey();
        if(code=='*'){
           kapi=false;
           lcd.clear();
           lcd.setCursor(3,0);
           lcd.print("RELOCKING...");    
           for(pos = 0; pos <= 180; pos +=5) { // goes from 0 degrees to 180 degrees
           myservo.write(pos);                 // tells servo to go to position in variable "pos"
           
  }
         delay(1000);
         lcd.clear();
         lcd.setCursor(6,0);
         lcd.print("LOCKED");
         delay(1000);
          
           displayscreen();
           currentposition=0;
           
        }
    }
}
//Loop Ends//

//***pirsensor function***//

void pirsensor(){
   int pirState=digitalRead(0);
  
  if(pirState==LOW){                   //Motion off 
        digitalWrite(greenled,LOW);
        digitalWrite(redled, HIGH);
  }
  else if(pirState==HIGH){             //Motion on
        digitalWrite(greenled,HIGH);
        digitalWrite(redled,LOW);
  }
}


//***Unlockdoor function***//

void unlockdoor() {                    
  delay(900);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Access Granted");
  lcd.setCursor(5,1);
  lcd.println("Welcome");

  for(pos = 180; pos>=0; pos-=5) {  // Goes from 180 degrees to 0 degrees
    myservo.write(pos);             // Tells servo to go to position in variable "pos"
    delay(5);                     
  }
  
  digitalWrite(greenled,LOW);
  digitalWrite(redled, LOW);
  delay(500);

}



//***Incorrect function***//

void incorrect() {              //If the pressed password is wrong this function will be executed
  delay(500);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Code");
  lcd.setCursor(6,0);
  lcd.print("Incorrect");
  lcd.setCursor(15,1);
  lcd.println(" ");

  delay(2000);
  lcd.clear();

  displayscreen();
}

//***displayscreen function***//

void displayscreen() {            //Starting screen
lcd.setCursor(0,1);
lcd.println("Write password");
lcd.setCursor(0,0);
lcd.println("To open the door");
}
