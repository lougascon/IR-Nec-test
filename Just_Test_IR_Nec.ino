
/*Read Nec IR Remote control like GLOBSAT ETC...                   */
/*    You can write module inside to control yours projects        */

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
/*Fonction interrupt Rec                                       */
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
/*Fonction FinPulses                                           */
/*==========================================================   */

void FinPulses()
{  
cdeerr=0;
tradcom();

Serial.println(octet2,HEX);//octet2 contains button code on a byte
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
if((octet2 ^ octet2a)!= 255){cdeerr=2;}
}
/* Sub routines IR  END                                         */
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


/*==============================================================*/
/*                   INIT SETUP()                               */
/*                                                              */
/*==============================================================*/

void setup()

{
  //PART IR****
interrupts();//allow interrupts      
Serial.begin(9600);
attachInterrupt(1,Rec,FALLING);//every falling signal launches rec
marqueCodeRecu=false;
OldTime=micros();
  //PART IR END****
  
}

/*=============================================================*/
/*                    MAIN LOOP()                              */
/*                                                             */
/*=============================================================*/

void loop()
{
  //PART IR****
if (Xrec==34){Xrec=0;FinPulses();}//all receved
if (marqueCodeRecu==true){marqueCodeRecu=false;job();}
  //PART IR END****
}

void job()
{
Serial.println("Your job start here");
}
