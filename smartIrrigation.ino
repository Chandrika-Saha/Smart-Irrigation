#include <SoftwareSerial.h>
int readv;  //var for storing the value read from water indicator
int in = A0; //input pin for water indicator
char incoming_char=0; //stor text message char by char
int led = 13; //led high pin
int flag=0;  
String text = "";
int f=0;
int ff=0;
SoftwareSerial SIM900(7, 8);  //configuring serial input

void setup() {
  SIM900.begin(19200); // sim900 communicates at 19200 speed
  delay(20000);   // setup time for sim900
  pinMode(in,INPUT);
  Serial.begin(19200);
  pinMode(led,OUTPUT);
  SIM900.print("AT+CMGF=1\r");   // AT command to set SIM900 to SMS mode
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r"); // Set module to send SMS data to serial out upon receipt
  delay(100);
}

void loop() { 
   readv = analogRead(in);
   if(readv > 230 && f==0) //if water level cross the limit 
   {
      f=1;
      sendSMSHigh(); //motor turn OFF sms sent to mobile
      Serial.println("KODU");
     // delay(20000); 
   }
   
   if(readv < 80 && ff==0) //if the water level lowered to a critical level
   {
      ff = 1;
      sendSMSLow(); //motor turn ON sms sent to mobile
      Serial.println("MODU");
      //delay(20000);
   }
   
    Serial.print("Level: ");
    Serial.println(readv);
    
    if(SIM900.available() > 0)
    {
      incoming_char = SIM900.read(); //Get the character from the cellular serial port
      Serial.print(incoming_char); //Print the incoming character to the terminal
      text = String(incoming_char+text); //storing the characters to form a string
    }

  int i;
  for(i=0;i<text.length();i=i+1)
  {
    if(text[i]=='#')//if the string contains "#"
    {
      
      flag = 1; //indicator that sms for turing on motor is received
      break;
    }
  }
  
  if(flag == 1)
  {  
     digitalWrite(led,HIGH); //turn on motor
     Serial.println(text);
     Serial.println("Kodu Ajad");
     flag = 0; //re-initialize motor
     f = 0;
     text=""; //re-initialize string to store 
     SIM900.flush(); //clearing sim900's buffer
  }

  for(i=0;i<text.length();i=i+1)
  {
    if(text[i]=='*') //if the msg contains "*"
    {
      
      flag = 2; //indicator for turing off motor
      break;
    }
  }

  if(flag == 2)
  {  
     digitalWrite(led,LOW);
     Serial.println(text);
     Serial.println("Modhu Ajad");
     flag = 0;
     ff = 0;
     text="";
     SIM900.flush();
  }
  
}

void sendSMSHigh() {
  SIM900.print("AT+CMGF=1\r"); // AT command to set SIM900 to SMS mode
  delay(100);
  SIM900.println("AT + CMGS = \"+8801521229179\""); //SMS sending number
  delay(100);
  SIM900.println("Water level CRITICAL..!!!"); //SMS content
  delay(100);
  SIM900.println((char)26);  // End AT command with a ^Z, ASCII code 26
  delay(100);
  SIM900.println();
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r");   // Set module to send SMS data to serial out upon receipt 
  delay(100);
  
}

void sendSMSLow() {
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);
  SIM900.println("AT + CMGS = \"+8801521229179\""); 
  delay(100);
  SIM900.println("Turn on the Motor NOWWW!!!"); 
  delay(100);
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();  
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
  
}