
#include<Servo.h>
#include<LiquidCrystal.h>

LiquidCrystal lcd(7,6, 5, 4, 3, 2);


//Servo and their pins
Servo servobase;
int basePin = 10;
Servo servoelbow;
int elbowPin = 9;
Servo servowrist;
int wristPin = 8;


int sensorValue ;
//////////////////////////////////////////////////////////////////
//angles list
struct chess 
{int pos1,pos2,pos3;            // pos3 denotes the total height from top to bottom and we have to subtract height of piece from it (done later)

 void set_data(int base,int elbow , int wrist)
     {pos1 = base ; pos2 = elbow; pos3 = wrist;} 
};


//chess rows

chess A[9],B[9],C[9],D[9],E[9],F[9],G[9],H[9];


//Out structure 

struct OUT_structure{
  int pos1, pos2, pos3;
  //char piece;
  
  void set_data(int base, int elbow, int wrist)
        {pos1 = base ; pos2 = elbow; pos3 = wrist;}
  //void set_piece(char p) // all pieces are white (uppercase character)
    //    {piece = p;}  
};

//out columns
OUT_structure OUT[19]; // for 16 white pieces and 2 places for pawn promotion

///////////////////////////////////////////////////////////////////////////////////





//chess state 
//                               A   B   C   D   E   F   G  H
char chess_state[9][9] = { {'0','0','0','0','0','0','0','0','0'},//0
                           {'0','R','N','B','Q','K','B','N','R'},//1
                           {'0','P','P','P','P','P','P','P','P'},//2                     
                           {'0','0','0','0','0','0','0','0','0'},//3
                           {'0','0','0','0','0','0','0','0','0'},//4
                           {'0','0','0','0','0','0','0','0','0'},//5
                           {'0','0','0','0','0','0','0','0','0'},//6              
                           {'0','p','p','p','p','p','p','p','p'},//7 
                           {'0','r','n','b','q','k','b','n','r'} //8        
 
                          
};  // uppercase - white pieces , lowercase - black pieces , '0' - empty      

bool calibration = true;
bool condition =   true;
// name of piece whose height is to be subtracted from pos3
char dest = '0';
char inits = '0';
char real_time_char = '0';

//servo positions
int servobasePos ,servoelbowPos ,servowristPos;
int servobasePPos ,servoelbowPPos;

//// led pins /////
int red = 2;
int green = 3;
int blue = 4;


//magnet 
int magnet ;
int magnetpin=11; //relay

//outer position state
int out = 1;

//all delays  ( to be calculated experimentally )
int speedDelay = 50; 
int magnetdelay = 500;
int mechanicalDelay = 250; 

String givenCommand;

bool comp_move = false;

// servos' resting positions
const int servobaseRest = 0;
const int servoelbowRest = 90; // to be done later
const int servowristRest = 0;



//function declaration

void magnet_Func();
void Command_Func(String s);
void servobaseFunc();
void servoelbowFunc();
void servowristFunc();
String State_Func(String moves);
int index(String abc);

void Print_func();

/////////////////////////////////////// ******SETUP***** ///////////////////////////////////////////////////////////
void setup() {
      Serial.begin(9600);
      lcd.begin(16,2);
      
      A[1].set_data(90,90,160);  A[2].set_data(90,90,160);  A[3].set_data(90,90,160);  A[4].set_data(90,90,160);
      A[5].set_data(90,90,160);  A[6].set_data(90,90,160);  A[7].set_data(90,90,160);  A[8].set_data(90,90,160);
      
      B[1].set_data(90,90,160);  B[2].set_data(90,90,160);  B[3].set_data(90,90,160);  B[4].set_data(90,90,160);
      B[5].set_data(90,90,160);  B[6].set_data(90,90,160);  B[7].set_data(90,90,160);  B[8].set_data(125,110,122);

      C[1].set_data(90,90,160);  C[2].set_data(90,90,160);  C[3].set_data(90,90,160);  C[4].set_data(90,90,160);
      C[5].set_data(90,90,160);  C[6].set_data(102,110,123);  C[7].set_data(90,90,160);  C[8].set_data(90,90,160);

      D[1].set_data(90,90,160);  D[2].set_data(130,130,130);  D[3].set_data(140,140,140);  D[4].set_data(150,150,150);
      D[5].set_data(86,111,135);  D[6].set_data(170,170,170);  D[7].set_data(96,131,137);  D[8].set_data(90,90,160);

      E[1].set_data(90,90,160);  E[2].set_data(90,90,160);  E[3].set_data(90,90,160);  E[4].set_data(70,110,135);
      E[5].set_data(60,122,160);  E[6].set_data(65,131,160);  E[7].set_data(74,141,160);  E[8].set_data(85,149,160);

      F[1].set_data(90,90,160);  F[2].set_data(90,90,160);  F[3].set_data(90,90,160);  F[4].set_data(90,90,160);
      F[5].set_data(48,128,160);  F[6].set_data(58,140,125);  F[7].set_data(58,152,160);  F[8].set_data(70,160,160);

      G[1].set_data(90,90,160);  G[2].set_data(90,90,160);  G[3].set_data(90,90,160);  G[4].set_data(90,90,160);
      G[5].set_data(39,134,160);  G[6].set_data(36,148,160);  G[7].set_data(45,160,160);  G[8].set_data(56,171,129);

      H[1].set_data(90,90,160);  H[2].set_data(90,90,160);  H[3].set_data(90,90,160);  H[4].set_data(90,90,160);
      H[5].set_data(20,138,160);  H[6].set_data(18,152,160);  H[7].set_data(19,165,160);  H[8].set_data(25,178,160);

      OUT[1].set_data(20,90,131);  OUT[2].set_data(90,90,160);  OUT[3].set_data(90,90,160);  OUT[4].set_data(90,90,160);
      OUT[5].set_data(90,90,160);  OUT[6].set_data(90,90,160);  OUT[7].set_data(90,90,160);  OUT[8].set_data(90,90,160);
      OUT[9].set_data(90,90,160);  OUT[10].set_data(90,90,160); OUT[11].set_data(90,90,160); OUT[12].set_data(90,90,160);
      OUT[13].set_data(90,90,160); OUT[14].set_data(90,90,160); OUT[15].set_data(90,90,160); OUT[16].set_data(90,90,160);
      OUT[17].set_data(90,90,160); OUT[18].set_data(90,90,160);  // 17,18 for pawn promotion (18 for queen) (17 for pawn)

      
      servobase.attach(basePin);
      servoelbow.attach(elbowPin);
      servowrist.attach(wristPin);
      
      pinMode(magnetpin,OUTPUT);
      digitalWrite(magnetpin,LOW);
      
      //////led//////
      pinMode(red,OUTPUT);
      digitalWrite(red,LOW);
      pinMode(green,OUTPUT);
      digitalWrite(green,LOW);
      pinMode(blue,OUTPUT);
      digitalWrite(blue,LOW);

      
      //arm initial-pos
      servobasePPos = servobaseRest;
      servobase.write(servobasePPos);

      servoelbowPPos = servoelbowRest;     
      servoelbow.write(servoelbowPPos);

      
      servowrist.write(servowristRest);
      
      magnet = 0;
      sensorValue = 0;

      
      lcd.print("Team Checmates");
      

 
}

//////////////////////////// ***********LOOP************  //////////////////////////////////////////////////

void loop() {

        if (Serial.available()>0 and (calibration == true)){ 
            String s = Serial.readString();
            String g = Serial.readString();

          lcd.clear();
          lcd.print("Calibration");

          
          
          for (int i = 0; i <=90; i++) {
            servobase.write(i) ;
            delay(50);
          }
          for (int i = 90; i>=0; i--) {
            servobase.write(i) ;
            delay(50);
          }

          
          calibration = false;
          Serial.write("Calibration is done");
          lcd.clear();

          lcd.print(" calibration is");
          lcd.setCursor(0,1);
          lcd.print("     done");
 
        }
        
        else if(Serial.available()>0 and (comp_move == true) ){
        
                //Serial.println("loopenter");
                givenCommand = Serial.readString();
                // Serial.println(givenCommand);
                String part1 , part2 , part3 ;
            
            
                String type_of_move = State_Func(givenCommand);
                
                part1 = givenCommand.substring(0,2);
                part2 = givenCommand.substring(2,4);
                part3 = type_of_move;

                lcd.clear();
                lcd.print("My Move: ");
                lcd.print(givenCommand);
                
            
                if (part3 == "nor") {

                    
                    real_time_char = inits;
                    
                    
                    Command_Func(part1);
       ////////////////////////LED DISPLAY//////////////////////////
                     String quote = random_quotes();
                     String q1,q2;
                     q1 = quote.substring(0,16);
                     q2 = quote.substring(16);
                     lcd.clear();
                     lcd.print(q1);
                     lcd.setCursor(0,1);
                     lcd.print(q2);
        ///////////////////////////////////////////////////////////////       
                     
  
                    
                    Command_Func(part2);
                    
                    real_time_char = '0';
                    inits = '0';
                    // dest = '0' since destination is '0'
                }
                
                else if (part3 == "cut") {

                     
                    
                     real_time_char = dest;
                     Command_Func(part2);

                     lcd.clear();
                     lcd.print("Captured RIP ;(");
                     
                     String outer = "O";
                     outer.concat(out);
                
                    // Serial.println(outer);
                
                     Command_Func(outer);
                     out = out + 1;
                     
                     dest = '0';
                   
                     real_time_char = inits ;


          ////////////////////////LED DISPLAY//////////////////////////
                     String quote = random_quotes();
                     String q1,q2;
                     q1 = quote.substring(0,16);
                     q2 = quote.substring(16);
                     lcd.clear();
                     lcd.print(q1);
                     lcd.setCursor(0,1);
                     lcd.print(q2);
        ///////////////////////////////////////////////////////////////       
                     
                     Command_Func(part1);
                     Command_Func(part2);
                     
                     inits = '0';
                     real_time_char = '0';
                }

                // En Passant
                else if (part3 == "enPass") {    

                     real_time_char = 'p'; // or it can be equal to 'P'

                     // move pawn which is being cut to outer position
                     String pawn_cut = givenCommand.substring(0,1);             // pawn which is being cut is in same column as of initial pos of pawn which is moving
                     String pawn_cut_row = givenCommand.substring(3,4);         // pawn which is being cut is in same row as of final pos of pawn which is moving
                     pawn_cut.concat(pawn_cut_row.toInt());
                     
                     Command_Func(pawn_cut);

                     String outer = "O";
                     outer.concat(out);

            ////////////////////////LED DISPLAY//////////////////////////
                     String quote = random_quotes();
                     String q1,q2;
                     q1 = quote.substring(0,16);
                     q2 = quote.substring(16);
                     lcd.clear();
                     lcd.print(q1);
                     lcd.setCursor(0,1);
                     lcd.print(q2);
        ///////////////////////////////////////////////////////////////       
                     
                      
                     Command_Func(outer);
                     out = out + 1;

                     // move the pawn which has enpassed the other pawn
                     Command_Func(part1);
                     Command_Func(part2);
                     
                     inits = '0';
                     real_time_char = '0';
                     dest = '0';
                 
                }
    
                // Non-Queenside Castling
                else if (part3 == "cast1") {    // since comp_move == true so cast1 => E8G8 is the givenCommand
    
                     real_time_char = 'k';
                     
                     Command_Func(part1);      //move king
                     Command_Func(part2);
                     
          ////////////////////////LED DISPLAY//////////////////////////
                     String quote = random_quotes();
                     String q1,q2;
                     q1 = quote.substring(0,16);
                     q2 = quote.substring(16);
                     lcd.clear();
                     lcd.print(q1);
                     lcd.setCursor(0,1);
                     lcd.print(q2);
        ///////////////////////////////////////////////////////////////       
                     
                     real_time_char = 'r';
    
                     Command_Func("H8");         //move rook
                     Command_Func("F8");
    
                     real_time_char = '0';
                     inits = '0';
                     dest = '0';
                      
                }
    
                // Queenside Castling
                else if (part3 == "cast2") {    // since comp_move == true so cast2 => E8C8 is the givenCommand
    
                     real_time_char = 'k';
                     
                     Command_Func(part1);      //move king
                     Command_Func(part2);
    
                     real_time_char = 'r';


            ////////////////////////LED DISPLAY//////////////////////////
                     String quote = random_quotes();
                     String q1,q2;
                     q1 = quote.substring(0,16);
                     q2 = quote.substring(16);
                     lcd.clear();
                     lcd.print(q1);
                     lcd.setCursor(0,1);
                     lcd.print(q2);
        ///////////////////////////////////////////////////////////////       
                     
                     Command_Func("A8");         //move rook
                     Command_Func("D8");
    
                     real_time_char = '0';
                     inits = '0';
                     dest = '0';
                      
                }

                // Pawn Promotion without capturing
                else if (part3 == "promotion") {      // since comp_move == true so promotion => p will be promoted to q

                      real_time_char = 'p';
  
                      Command_Func(part1);       // pick up black pawn to be promoted   
                      
                      String outer = "O";        // move the pawn to outside the board
                      outer.concat(17);
                      Command_Func(outer);

                      real_time_char = 'q';
                                            
                      outer = "O";         // pick up black queen from outside
                      outer.concat(18);
                      Command_Func(outer);       

                      Command_Func(part2);

                      real_time_char = '0';
                      inits = '0';
                      dest = '0';                      
                    
                }

                // pawn promotion with capturing

              else if (part3 == "promotion&capture") {      // since comp_move == true so promotion => p will be promoted to q

                      real_time_char = dest;

                      Command_Func(part2);
                      
                      String outer = "O";         // move the piece captured to outside the board
                      outer.concat(out);
                      Command_Func(outer);
                      out = out +1;

                      real_time_char = inits;
  
                      Command_Func(part1);       // pick up black pawn to be promoted   
                      
                      outer = "O";        // move the pawn to outside the board
                      outer.concat(17);
                      Command_Func(outer);

                      real_time_char = 'q';
                                            
                      outer = "O";         // pick up black queen from outside
                      outer.concat(18);
                      Command_Func(outer);       

                      Command_Func(part2);

                      real_time_char = '0';
                      inits = '0';
                      dest = '0';                      
                    
                }
        
          comp_move = false;
         // Print_func();
          
          Serial.write("Robot Move Done");
          condition = true ;
          }
        
        else if (Serial.available()>0 and comp_move == false) {
            
            givenCommand = Serial.readString();

            lcd.clear();
            lcd.print("Your Move: ");
            lcd.print(givenCommand);
        
            String type_of_move = State_Func(givenCommand);
        
            comp_move = true;

           // Print_func();
        }
        
        sensorValue = analogRead(A5);
        if ((sensorValue>=500) and (condition == true)) {
          Serial.write("User has moved");
          lcd.clear();
          lcd.print("You have Moved");
          condition = false ;
        }
        else{}
        
        
}




//////////////////////////////************FUNCTIONS*************////////////////////////////////////////////////////////////


//magnet on off
void magnet_Func(){
    if (magnet == 0) {magnet = 1; digitalWrite(magnetpin,HIGH);}
    else if (magnet == 1) {magnet = 0; digitalWrite(magnetpin,LOW);}
    else {Serial.println("magnet error");}
  
    delay(magnetdelay);
}




// base rotation function
void servobaseFunc() {
        if(servobasePos>servobasePPos) {
            for(int j=servobasePPos; j<=servobasePos; j++)
            { servobase.write(j);
              delay(speedDelay); }
       }
          
       else if(servobasePos<=servobasePPos) {
            for(int j=servobasePPos; j>=servobasePos; j--)
            { servobase.write(j);
              delay(speedDelay); }
       }

       delay(mechanicalDelay);
}


void servoelbowFunc() {
  
        if(servoelbowPos>servoelbowPPos) {
            for(int j=servoelbowPPos; j<=servoelbowPos; j++)
            { servoelbow.write(j); 
              delay(speedDelay); }
        }

          
         else if(servoelbowPos<=servoelbowPPos) {
            for(int j=servoelbowPPos; j>=servoelbowPos; j--)
            { servoelbow.write(j);
              delay(speedDelay); }
         }

         delay(mechanicalDelay);
}


void servowristFunc() {

        int down_length = servowristPos - height(real_time_char);
        
        if(servowristPos>servowristRest) {
            for(int j=servowristRest; j<=down_length;j++)
            { servowrist.write(j);
              delay(speedDelay); }

            delay(mechanicalDelay);
       
            magnet_Func();
            
            delay(mechanicalDelay);
            
            for(int j=down_length; j>=servowristRest;j--)
            { servowrist.write(j);
              delay(speedDelay); }

            delay(mechanicalDelay);
              
         }

          
         //else if(servowristPos<=servowristPPos) {
          //  for(int j=servowristPPos; j>=servowristPos;j--)
          //  { servowrist.write(j);
         //     delay(speedDelay); }
          //}                              

      
}



void Command_Func(String s) {
      String first = s.substring(0,1);
      //Serial.println(first);
      String temp = s.substring(1,2);
      int second = temp.toInt();
     // Serial.println(second);

      if (first == "A") { servobasePos = A[second].pos1 ;
                          servoelbowPos = A[second].pos2;
                          servowristPos = A[second].pos3;
                          
                          
                          servobaseFunc(); 
                          servoelbowFunc();  
                          
                          servowristFunc();
                          
                          servobasePPos = servobasePos;
                          servoelbowPPos = servoelbowPos;

                          //bring the servos to resting positions for better accuracy
                          servobasePos = servobaseRest;
                          servoelbowPos = servoelbowRest;
                           
                          servobaseFunc(); 
                          servoelbowFunc();  

                          servobasePPos=servobasePos;
                          servoelbowPPos = servoelbowPos;
                          
      }
                          
      
      else if (first == "B") {servobasePos = B[second].pos1 ;
                              servoelbowPos = B[second].pos2;
                              servowristPos = B[second].pos3;
                              
                              servobaseFunc();
                              servoelbowFunc();
                              
                              servowristFunc();
                              
                              servobasePPos = servobasePos;
                              servoelbowPPos = servoelbowPos;


                              //bring the servos to resting positions for better accuracy
                              servobasePos = servobaseRest;
                              servoelbowPos = servoelbowRest;
                               
                              servobaseFunc(); 
                              servoelbowFunc();

                              servobasePPos=servobasePos;
                              servoelbowPPos = servoelbowPos;

      }
                          
      else if (first == "C") {servobasePos = C[second].pos1 ;
                              servoelbowPos = C[second].pos2;
                              servowristPos = C[second].pos3;
                              
                              servobaseFunc();
                              servoelbowFunc();
                              
                              servowristFunc();
                              
                              servobasePPos = servobasePos;
                              servoelbowPPos = servoelbowPos;


                              //bring the servos to resting positions for better accuracy
                              servobasePos = servobaseRest;
                              servoelbowPos = servoelbowRest;
                               
                              servobaseFunc(); 
                              servoelbowFunc();  

                              servobasePPos=servobasePos;
                              servoelbowPPos = servoelbowPos;

      }
                          
      else if (first == "D") {servobasePos = D[second].pos1 ;
                              servoelbowPos = D[second].pos2;
                              servowristPos = D[second].pos3;
                              
                              servobaseFunc();
                              servoelbowFunc();
                              
                              servowristFunc();
                              
                              servobasePPos = servobasePos;
                              servoelbowPPos = servoelbowPos;


                              //bring the servos to resting positions for better accuracy
                              servobasePos = servobaseRest;
                              servoelbowPos = servoelbowRest;
                               
                              servobaseFunc(); 
                              servoelbowFunc();  

                              servobasePPos=servobasePos;
                              servoelbowPPos = servoelbowPos;

      }
                          
      else if (first == "E") {servobasePos = E[second].pos1 ;
                              servoelbowPos = E[second].pos2;
                              servowristPos = E[second].pos3;
                              
                              servobaseFunc();
                              servoelbowFunc();
                              
                              servowristFunc();
                              
                              servobasePPos=servobasePos;
                              servoelbowPPos = servoelbowPos;


                              //bring the servos to resting positions for better accuracy
                              servobasePos = servobaseRest;
                              servoelbowPos = servoelbowRest;
                               
                              servobaseFunc(); 
                              servoelbowFunc();

                              servobasePPos=servobasePos;
                              servoelbowPPos = servoelbowPos;

                              
      }
                          
      else if (first == "F") {servobasePos = F[second].pos1 ;
                              servoelbowPos = F[second].pos2;
                              servowristPos = F[second].pos3;

                              
                              servobaseFunc();
                              servoelbowFunc();
                              
                              servowristFunc();
                                                            
                              servobasePPos = servobasePos;
                              servoelbowPPos = servoelbowPos;


                              //bring the servos to resting positions for better accuracy
                              servobasePos = servobaseRest;
                              servoelbowPos = servoelbowRest;
                               
                              servobaseFunc(); 
                              servoelbowFunc(); 

                              servobasePPos=servobasePos;
                              servoelbowPPos = servoelbowPos;
      }
                          
      else if (first == "G") {servobasePos = G[second].pos1 ;
                              servoelbowPos = G[second].pos2;
                              servowristPos = G[second].pos3;
                              
                              servobaseFunc();
                              servoelbowFunc();
                              
                              servowristFunc();
                              
                              servobasePPos = servobasePos;
                              servoelbowPPos = servoelbowPos;


                              //bring the servos to resting positions for better accuracy
                              servobasePos = servobaseRest;
                              servoelbowPos = servoelbowRest;
                               
                              servobaseFunc(); 
                              servoelbowFunc();

                              servobasePPos=servobasePos;
                              servoelbowPPos = servoelbowPos;

      }
                          
      else if (first == "H") {servobasePos = H[second].pos1 ;
                              servoelbowPos = H[second].pos2;
                              servowristPos = H[second].pos3;
                              
                              servobaseFunc();
                              servoelbowFunc();
                              
                              servowristFunc();
                              
                              servobasePPos = servobasePos;
                              servoelbowPPos = servoelbowPos;


                              //bring the servos to resting positions for better accuracy
                              servobasePos = servobaseRest;
                              servoelbowPos = servoelbowRest;
                               
                              servobaseFunc(); 
                              servoelbowFunc();  

                              servobasePPos=servobasePos;
                              servoelbowPPos = servoelbowPos;

       }

      else if (first == "O") {servobasePos = OUT[second].pos1 ;
                              servoelbowPos = OUT[second].pos2;
                              servowristPos = OUT[second].pos3;
                              
                              servobaseFunc();
                              servoelbowFunc();
                              
                              servowristFunc();
                              
                              servobasePPos = servobasePos;
                              servoelbowPPos = servoelbowPos;


                              //bring the servos to resting positions for better accuracy
                              servobasePos = servobaseRest;
                              servoelbowPos = servoelbowRest;
                               
                              servobaseFunc(); 
                              servoelbowFunc();  
                              
                              servobasePPos=servobasePos;
                              servoelbowPPos = servoelbowPos;

        }

                          
      else {Serial.println ("wrong command");}
      
   
} 


String State_Func(String moves){
      String p1 = moves.substring(0,1);
      String p2 = moves.substring(1,2);
      String p3 = moves.substring(2,3);
      String p4 = moves.substring(3,4);

      int j_init = index(p1);
      int i_init = p2.toInt();
      
      int j_fin = index(p3);
      int i_fin = p4.toInt();

     char initial = chess_state[i_init][j_init];
     char destination = chess_state[i_fin][j_fin];
     
      if (destination != '0') {
               

           ////////////// pawn promotion with capturing ///////////////////
        
            if ( ((initial == 'p')  or (initial == 'P')) and ( ( i_fin == 8) or (i_fin == 1)) ) {

                if (initial == 'p') { chess_state[i_fin][j_fin] = 'q';} 
                else { chess_state[i_fin][j_fin] = 'Q'; }
                
                chess_state[i_init][j_init] = '0';     

                dest = destination;
                inits = initial;

                  
                return "promotion&capture";
              
            }

            else {
                chess_state[i_fin][j_fin]= initial;
                chess_state[i_init][j_init]= '0';
        
                dest = destination;
                inits = initial;
                
                 return "cut";
                 }
      }
        
      else if (destination == '0') {


            //////// Castling ///////
            if((initial == 'k') and (moves == "E8G8")) {      
                
                chess_state[8][7] = 'k';      // move king
                chess_state[8][5] = '0';
    
                chess_state[8][8] = '0';      // move rook
                chess_state[8][6] = 'r';
                
                return "cast1";       // non-queenside castling
                     
            }
            
            else if((initial == 'k') and (moves == "E8C8")) {      
                
                chess_state[8][3] = 'k';      // move king
                chess_state[8][5] = '0';
    
                chess_state[8][1] = '0';      // move rook
                chess_state[8][4] = 'r';    
                
                return "cast2";       // queenside castling
                      
            }
    
            else if((initial == 'K') and (moves == "E1G1")) {      
                
                chess_state[1][7] = 'K';      // move king
                chess_state[1][5] = '0';
    
                chess_state[1][8] = '0';      // move rook
                chess_state[1][6] = 'R';
                
                return "cast1";        // non-queenside castling
                     
            }
    
            else if((initial == 'K') and (moves == "E1C1")) {      
                
                chess_state[1][3] = 'K';      // move king
                chess_state[1][5] = '0';
    
                chess_state[1][1] = '0';      // move rook
                chess_state[1][4] = 'R';
                
                return "cast2";          // queenside castling
                     
            }
    
    
            ////////// En Passant /////////
            else if ( ((initial == 'p') or (initial == 'P')) and (abs(j_fin - j_init) == 1 ) ){
                
                chess_state[i_fin][j_fin] = initial;
                chess_state[i_init][j_init] = '0';
    
                chess_state[i_fin][j_init] = '0';
    
                return "enPass";
              
            }


            ///////// Pawn Promotion without capturing any piece ////// 
            else if( ((initial == 'p')  or (initial == 'P')) and ( ( i_fin == 8) or (i_fin == 1)) ){
                
                if (initial == 'p') { chess_state[i_fin][j_fin] = 'q';} 
                else { chess_state[i_fin][j_fin] = 'Q'; }
                
                chess_state[i_init][j_init] = '0';     

                return "promotion";
            }

    
            /////// Normal Move //////
            else {
                chess_state[i_fin][j_fin] = initial;
                chess_state[i_init][j_init] = '0';
        
                dest = destination;
                inits = initial;
                return "nor";  // nor - normal move 
            }
      }
    
  
}


int index(String abc){
    if (abc == "A") {return 1;}
    else if (abc == "B") {return 2;}
    else if (abc == "C") {return 3;}
    else if (abc == "D") {return 4;}
    else if (abc == "E") {return 5;}
    else if (abc == "F") {return 6;}
    else if (abc == "G") {return 7;}
    else if (abc == "H") {return 8;}
    
  
}


int height(char pieces)
    {
      if ( (pieces == 'p') or (pieces == 'P')) {
        return 0; 
      }
      else if ( (pieces == 'k') or (pieces == 'K')) {
        return 0; 
      }
      else if ( (pieces == 'r') or (pieces == 'R')) {
        return 0; 
      }
      else if ( (pieces == 'q') or (pieces == 'Q')) {
        return 0; 
      }
      else if ( (pieces == 'n') or (pieces == 'N')) {
        return 0; 
      }
      else if ( (pieces == 'b') or (pieces == 'B')) {
        return 0; 
      }
      
    }


void Print_func() {

     for (int i = 8 ; i >=1 ; i--) {
            for (int j = 1 ; j<= 8 ; j++) {
              Serial.print(chess_state[i][j]);
              Serial.print(" ");
            }
            Serial.println();
          
     }

     Serial.println();
     Serial.println();
  
}


String random_quotes() {

  int a = random(7);
  if ( a == 0) {return "Pawns are the soul of the game";}
  else if (a == 1) {return "A player surprised is half beaten";}
  else if (a == 2) {return "Every chess master was beginner";}
  else if (a == 3) {return "Be the player, not the piece";}
  else if (a == 4) {return "Don't play if you are kind-hearted";}
  else if (a == 5) {return "When in doubt... play Chess!";}
  else if (a == 6)  {return "Help pawn so it can help you"; }
  
}






      
