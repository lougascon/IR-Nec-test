/*Read Nec IR Remote control like GLOBSAT ETC...                   */
/*    You can write module inside to control yours projects        */

/*                  VARIABLES                                      */

//Part IR
unsigned int OldTime;
int List[34];
int Index;
int Xrec;
boolean marque;
boolean marqueCodeRecu;
byte octet2a;//inverse commande reçue
byte octet2;//commande
byte cdeerr;//verifie la validité de la reception code erreur
byte poids[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,1,2,4,8,16,32,64,128,1,2,4,8,16,32,64,128};
//End part IR


/* Sub routines IR                                             */
/*===========================================================  */
/*Fonction interruption Rec                                    */
/*===========================================================  */

void Rec() //interrupt
{
marque=true;   //a falling signal is detected
}

/*==========================================================   */
/*                Reception pulses                     */
/*==========================================================   */

//store times in array List
    
void ReceptionPulses()
{
List[Xrec]=(micros()-OldTime);++Xrec;//calcule longueur reçue
OldTime=micros();//reprend l'heure de cette reception pour la suivante
if(Xrec-1==1){if(List[1]>13350&List[1]<14600){}else{--Xrec;}}
            //wait the good value  9000µs  and
                    //4500µs = 13500µs
                             
}
    
/*==========================================================   */
/*Fonction FinPulses                                           */
/*==========================================================   */

void FinPulses()
{  
cdeerr=0;
traduction();//transform times in  1 or 0
commande();Serial.println(octet2, HEX); //Here you read the remote button value
if(cdeerr!=0){Serial.print("erreur No");Serial.println(cdeerr);}else{marqueCodeRecu=true;}
}

/*=============================================================*/
/*           calculate and control the receve value            */
/*=============================================================*/




void commande()
{
octet2=0;octet2a=0;
for(int i=18;i<=25;i++){if(List[i]==1){octet2=octet2+poids[i];}}
for(int i=26;i<=33;i++){if(List[i]==1){octet2a=octet2a+poids[i];}}
        //control
if((octet2 ^ octet2a)!= 255){cdeerr=2;}
        //Xor between two value
}


/*================================================================*/
/* Store in List  0 et 1 with the length                          */
/*================================================================*/
void traduction()
{
  
  //traduction length to 0 or 1 in array
    
for (int ix=2;ix<34;ix++)
{
if ((List[ix]>1040)&(List[ix]<1220))   {List[ix]=1;}
if ((List[ix]>2150)&(List[ix]<2300))   {List[ix]=0;}
}
     
}

/*==============================================================*/
/*                   INIT SETUP()                               */
/*                                                              */
/*==============================================================*/

void setup()

{
interrupts();//allow interrupts      
Serial.begin(9600);
attachInterrupt(1,Rec,FALLING);
marqueCodeRecu=false;
  
}

/*=============================================================*/
/*                    MAIN LOOP()                              */
/*                                                             */
/*=============================================================*/

void loop()
{
if (Xrec==34){Xrec=0;FinPulses();}//all receved
if (marque==true){marque=false;ReceptionPulses();}
if (marqueCodeRecu==true){marqueCodeRecu=false;job();}
}
void job()
{
Serial.println("Your job start here");
}
