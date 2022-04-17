#include "DigiKeyboard.h"

#define KEY_BACKSPACE 42
#define MOD_CMD_LEFT 0x00000008

//SETINGS: ----------------------
bool IfMac = 1;
int ENorDEcryption = 1; //Encryption: -1; Decryption: 1

/*Alphabet:
 !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
*/
/*
Decryption will use addition.
So encryption should use subtraction.
Site for encryption: http://omerkel.github.io/caesar_cipher/html5/src/

*/
//-------------------------------

//Button pins:
const int buttonUP = 2;
const int buttonOK = 1;
const int buttonDOWN = 0;
//button vars:
const int debounceDelay = 200;    // the debounce time; increase if the output flickers



int lastMenuItem = -1;
int currMenuItem = 0;

int menu_mode = 0;
/*
 0 - Title -UP,DOWN >OK> >1
 1 - waiting >OK> (password print) >-1
-1 - Waiting >OK> 0
*/
const int NUMBER_OF_ELEMENTS = 10;
const int MAX_SIZE = 12;

char menu_title [NUMBER_OF_ELEMENTS] [MAX_SIZE] = { 
 { "Login#0" }, 
 { "Login#1" }, 
 { "Login#2" }, 
 { "Login#3" }, 
 { "Login#4" }, 
 { "Login#5" }, 
 { "Login#6" }, 
 { "Login#7" }, 
 { "Login#8" }, 
 { "Login#9" }, 
};

//if the password has " then \need to be added before it > \"
char menu_password [NUMBER_OF_ELEMENTS] [MAX_SIZE] = { 
 { " !\"#$%&'()*" },  //0
 { "+,-./012345" },   //1
 { "6789:;<=>?@" },   //2
 { "ABCDEFGHIJK" },   //3
 { "LMNOPQRSTUV" },   //4
 { "WXYZ[\]^_`a" },   //5
 { "bcdefghijkl" },   //6
 { "mnopqrstuvw" },   //7
 { "xyz{|}~ !\"#" },  //8
 { "Password#9" },    //9
//  

};


//char password_decoded [MAX_SIZE] = {"00000000000"};
  

int password[2] = {0,0}; //00-93

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonDOWN,INPUT);
  pinMode(buttonOK,INPUT);
  pinMode(buttonUP,INPUT);
  pinMode(5,INPUT);

  //print message:
  DigiKeyboard.print("Hello#0");
  DigiKeyboard.delay(500);

  //delete last message:
  SellectAllandDel();

  //print message:
  DigiKeyboard.print("Hello#1");
  DigiKeyboard.delay(500);



  
  //Getting password
  int printed_num = -1;
  int password_i = 0;
  SellectAllandDel();
  DigiKeyboard.print("Please enter 2 digit password");
  DigiKeyboard.delay(500);
  while(true){
    if(password[password_i] > 9){password[password_i]=0;}
    if(password[password_i] < 0){password[password_i]=9;}
    
    if(printed_num != password[password_i]){
      //delete last message:
      SellectAllandDel();
      //print new message:
      for(int i=0; i<=password_i; i++){
        //DigiKeyboard.print(char(password[i]));
        DigiKeyboard.print(password[i]);
      }
      printed_num = password[password_i];
    }
    
    
    //UP button
    if (digitalRead(buttonUP) ){
      //DigiKeyboard.print("UP pressed");
      password[password_i] = password[password_i] + 1;
      DigiKeyboard.delay(debounceDelay);
      
    }
  
    //OK button
    if (digitalRead(buttonOK) ) {
      //DigiKeyboard.print("OK pressed");
      if(password_i==0){
        printed_num = -1;
        password_i = 1;//0>1
      }
      else if(password_i==1){
        SellectAllandDel();
        DigiKeyboard.print("Password accepted: ");
        //printing password
        if(ENorDEcryption == 1){
          DigiKeyboard.print("+");
        }else{
          DigiKeyboard.print("-");
        }
        DigiKeyboard.print(password[0]);
        DigiKeyboard.print(password[1]);       
        DigiKeyboard.delay(1000);
        break;
      }
      DigiKeyboard.delay(debounceDelay);
    }

  
    //DOWN button
    if (digitalRead(buttonDOWN)){
      //DigiKeyboard.print("DOWN pressed");
      password[password_i] = password[password_i] - 1;
      DigiKeyboard.delay(debounceDelay);
    }
  }
}

void loop() {
  if(currMenuItem > (NUMBER_OF_ELEMENTS-1) ){currMenuItem = 0;}
  if(currMenuItem < 0 ){currMenuItem = NUMBER_OF_ELEMENTS-1;}
  
  if(lastMenuItem != currMenuItem && menu_mode==0){
    //delete last message:
    SellectAllandDel();
    //print new message:
    DigiKeyboard.print(currMenuItem);
    DigiKeyboard.print(" : ");
    DigiKeyboard.print(menu_title[currMenuItem]);
    //update lastMenuItem:
    lastMenuItem = currMenuItem;

//    for (int i = 0; i < NUMBER_OF_ELEMENTS; i++){
//      DigiKeyboard.print(i);
//      DigiKeyboard.print(">");
//      DigiKeyboard.println (descriptions3 [i]);
//    }

  }
  
  //UP button
  if (digitalRead(buttonUP)) {
//    DigiKeyboard.print("UP pressed");
    currMenuItem = currMenuItem + 1;
    DigiKeyboard.delay(debounceDelay);
  }

  //OK button
  if (digitalRead(buttonOK)) {
//    SellectAllandDel();
//    DigiKeyboard.print("OK pressed");
//    DigiKeyboard.delay(debounceDelay);

    if(menu_mode==0){
      menu_mode = 1; 
      SellectAllandDel();
      DigiKeyboard.print(">Password>OK");
      DigiKeyboard.delay(500);
    }
    else if(menu_mode==1){
      SellectAllandDel();
      DigiKeyboard.print(menu_mode);
      menu_mode = -1; //to Waiting mode
      //Print password:
      SellectAllandDel();
      decryption(menu_password[currMenuItem]);
      DigiKeyboard.delay(debounceDelay);
    }
    else if(menu_mode == -1){
      menu_mode = 0; //from Waiting mode
      lastMenuItem = -1;
      DigiKeyboard.delay(debounceDelay);
    }
    
    /*
    if(menu_mode==HIGH){
      SellectAllandDel();
      DigiKeyboard.print(menu_mode);
      menu_mode = LOW; //to Waiting mode
      //Print password:
      SellectAllandDel();
      decoder(menu_password[currMenuItem]);
      DigiKeyboard.delay(debounceDelay);
    }
    else if(menu_mode==LOW){
      menu_mode = HIGH; //from Waiting mode
      lastMenuItem = -1;
      DigiKeyboard.delay(debounceDelay);
    }*/
    
    
  }

  //DOWN button
  if (digitalRead(buttonDOWN)){
//    DigiKeyboard.print("DOWN pressed");
    currMenuItem = currMenuItem - 1;
    DigiKeyboard.delay(debounceDelay);
  }

  

}

void decryption(char password_to_decode []){
  for (int i = 0; i < MAX_SIZE-1 ; i++){ //MAX_SIZE
    int new_char = password_to_decode[i]+ ENorDEcryption*((password[0]*10+password[1])) ; 
  
    if( new_char > 126){
      DigiKeyboard.print( char( new_char - 126 + 31 ) );
    } else if( new_char < 32){
      DigiKeyboard.print( char( new_char + 126 - 31 ) );
    }else{
      DigiKeyboard.print( char(new_char) );
    }
    
  }
}

void SellectAllandDel(){
  if(IfMac){
    DigiKeyboard.sendKeyStroke(KEY_A, MOD_CMD_LEFT);
  }else{
    DigiKeyboard.sendKeyStroke(KEY_A , MOD_CONTROL_LEFT);  
  }
  DigiKeyboard.sendKeyStroke(KEY_BACKSPACE);
  DigiKeyboard.delay(debounceDelay);
}
/*LED check:
Works  : 0,1,2,4
Doesn't: 3,5*/

/*buttonPin check:
Works  : 0, 2,
Doesn't: 1, 4, 5
Strangly works: 3*/

/*Encoder - Tick func
Works: 0,2
Doesn't: 1
isclick is reversed: 3
no start 4
restarts 5
*/

//Паролятор :)