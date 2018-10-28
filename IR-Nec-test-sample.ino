
/*Read Nec IR Remote control like GLOBSAT ETC...                   */
/*    You can write module inside to control yours projects        */
/*     This version give an exemple how to command Led 13          */
/*                  VARIABLES                                      */

//Part IR ****

unsigned long OldTime;
int List[34];
byte Xrec;
boolean marqueCodeRecu;
byte octet2a;//control word = command logical inverse
byte octet2;//command
byte cdeerr;//error

//End part IR ****

byte T1;
boolean L13;
boolean debug;



#include <MsTimer2.h>

/*=============================================================*/
/*              Bit operation                                  */
/*=============================================================*/

byte rotateRight(byte value, int amount) {
    return (value >> amount) | (value << (8 - amount));
}
//this function comes from web forums

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/* Sub routines IR     early                                   */
/*===========================================================  */
/*Function interrupt Rec                                       */
/*===========================================================  */

void Rec() //interrupt
{
List[Xrec]=(micros()-OldTime);++Xrec;//calculate lenth receved
OldTime=micros();// ini time to next interrupt
if(Xrec-1==1){if(List[1]>13300&List[1]<14800){}else{--Xrec;}}
            //wait the good value  9000µs  and
                    //4500µs = 13500µs

}



    
/*==========================================================   */
/*Function FinPulses                                           */
/*==========================================================   */

void FinPulses()
{  
cdeerr=0;
tradcom();

if(debug==true){Serial.println(octet2,HEX);}//octet2 contains button code on a byte
if(cdeerr!=0){Serial.print("erreur No");Serial.println(cdeerr);}else{marqueCodeRecu=true;}
}



/*=============================================================*/
/*                     TRADCOM                                 */
/*=============================================================*/

void tradcom()
{
for (int ix=2;ix<34;ix++)
{
if ((List[ix]>1040)&(List[ix]<1220))   {List[ix]=1;}
if ((List[ix]>2150)&(List[ix]<2300))   {List[ix]=0;}
}//read 0 and 1 transmitted
octet2=0;octet2a=0;
for(int i=18;i<=25;i++){if(List[i]==1){octet2=octet2+0x01;}octet2=(rotateRight(octet2,1));}
for(int i=26;i<=33;i++){if(List[i]==1){octet2a=octet2a+0x01;}octet2a=(rotateRight(octet2a,1));}
// build the command byte
if((octet2 ^ octet2a)!= 255){cdeerr=2;}//control transmission
}
/* Sub routines IR  END                                         */
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


/*==============================================================*/
/*                   INIT SETUP()                               */
/*                                                              */
/*==============================================================*/

void setup()

{
  debug=false;//place true to read command byte on serial monitor
  
  //PART IR****
interrupts();//allow interrupts      
Serial.begin(9600);
attachInterrupt(1,Rec,FALLING);//every falling signal launches rec
marqueCodeRecu=false;//when true a command byte is received
OldTime=micros();
  //PART IR END****

pinMode(13,OUTPUT);

MsTimer2::set(100, itsTime); // parameter in ms
MsTimer2::start(); // start Timer2 
}

/*=============================================================*/
/*                    MAIN LOOP()                              */
/*                                                             */
/*=============================================================*/

void loop()
{
  //PART IR****
if (Xrec==34){Xrec=0;FinPulses();}//all receved
if (marqueCodeRecu==true){marqueCodeRecu=false;job(octet2);}
  //PART IR END****
}

byte job(byte Com)
{

switch(Com)
{
case 0xEB:
L13=false;
digitalWrite(13,HIGH);//peekton IR green  on
break;

case 0xE1:
L13=true;
digitalWrite(13,LOW);//yellow blink
break;

case 0xF8:
L13=false;
digitalWrite(13,LOW);//blue Off
break;

}

}

void itsTime()//executed on timer 2 parameter
{
  if (L13==true){++T1;T1=outonoff(T1,11,10,13);}//parameter T1 incremented each itstime 
                                                //11 time = Fall reinit T1
                                                //10 time = On
                                                //13 Led 
                                                //here the blinking is dissymmetric
}

byte outonoff(byte T,byte arf,byte aro,byte pin)
{
 if(T==aro){digitalWrite(pin, !digitalRead(pin));}
 if(T==arf){digitalWrite(pin, !digitalRead(pin));T=0;}
 return T;

}

