#include <LiquidCrystal.h>

int rs = 5;
int rw = 6;
int enable = 7;
int d4 = 10;
int d5 = 11;
int d6 = 12;
int d7 = 13;

int buttonUpMinutes = 2; //pulsante per aumentare i minuti
int buttonUpHours = 3; // pulsante per aumentare ore

int dotState = 0; // variabile per permettere il blinking dei due punti (utilizzatio per i secondi)

// variabili counter
int seconds = 0;
int minutes = 59;
int hours = 22;

LiquidCrystal lcd(rs, rw, enable, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 2);
  //utilizzo attachInterrupt per usare una funzione creata per l'aumento delle ore e dei minuti
  attachInterrupt(0, minutesButtonFunc, FALLING);
  attachInterrupt(1, hoursButtonFunc, FALLING); 
}

void loop() {
  // ogni while serve per dare "un limite" ai vari counter. Raggiunto il limite viene azzerata la variabile; vedi in basso.
  while(hours < 24) {
    while(minutes < 60) {
      while(seconds < 60) {
        if(minutes < 10) { // se i minuti sono inferiori a 10 viene messo uno zero davanti all'unità per non visualizzarla come 1, 2, 3... ma come 01, 02, 03...
        lcd.setCursor(3, 0);
        lcd.print("0");
        lcd.print(minutes);
        
      } else { // se invece è superiore a 10 vengono mostrati i minuti normalmente
        lcd.setCursor(3, 0);
        lcd.print(minutes);
      }
      if(hours < 10) { // se le ore sono inferiori a 10 viene messo uno zero davanti all'ora per non visualizzarla come 1, 2, 3... ma come 01, 02, 03...
      lcd.setCursor(0, 0);
      lcd.print("0");
      lcd.print(hours);
    } else { // se invece è superiore a 10 vengono mostrati i minuti normalmente
      lcd.setCursor(0, 0);
      lcd.print(hours);
    }
        if(dotState == 0) { // se la variabile è = a 0 vengono mostrati i due punti
          lcd.setCursor(2, 0);
          lcd.print(":");
        }
        delay(1000); //accelerazione del tempo
        dotState = !dotState; // ogni secondo viene invertito lo stato della variabile
        if(dotState == 1) { // se la variabile è = a 1 mostra uno spazio vuoto
          lcd.setCursor(2, 0);
          lcd.print(" ");
        }
        seconds++; // completato il cilco viene aumentata la variabile dei secondi.
      }
      seconds = 0; // quando passano 60 secondi viene pulito il display e aumentata la variabile dei minuti
      lcd.clear(); 
      minutes++;
    }
    minutes = 0; // passati 60 minuti si resetta la variabile della stessa e aumenta la variabile delle ore
    hours++;
  }
  hours = 0; // passate 24 ore si resetta la variabile della stessa 
}

void printHoursAndMinutes(int hours, int minutes, int modality) {
  switch(modality){
    case 0:
      lcd.clear();
      lcd.print(hours);
      lcd.print(":");
      lcd.print("0");
      lcd.print(minutes);
      break;
    case 1:
      lcd.clear();
      lcd.print("0");
      lcd.print(hours);
      lcd.print(":");
      lcd.print("0");
      lcd.print(minutes);
      break;
    case 3:
      lcd.clear();
      lcd.print(hours);
      lcd.print(":");
      lcd.print(minutes);
      break;
    case 4:
      lcd.clear();
      lcd.print("0");
      lcd.print(hours);
      lcd.print(":");
      lcd.print(minutes);
      break
    default:
      break;
  }
  
}

void minutesButtonFunc () {
    while(digitalRead(buttonUpMinutes) == 0) {} // quando il pulsante viene premuto si incrementa la variabile dei minuti
    delay(500);
    minutes++;
  if (minutes == 60) { minutes = 0; } // quando la variabile arriva a 60 viene resettata
  	if(minutes < 10 and hours > 10) { // se i minuti sono minori a 10 e le ore maggiori di dieci viene messo uno 0 davanti ai minuti per visualizzare 12:01 e non 12:1
      printHoursAndMinutes(hours, minutes, 0);
  }
  	else if(hours < 10 and minutes < 10) { // se entrambe le variabili sono minori di dieci viene messo uno zero davanti cosi da visualizzare 01:01 e non 1:1
      printHoursAndMinutes(hours, minutes, 1);
    }
  	else if(minutes > 10 and hours < 10) { // se i minuti sono maggiori di 10 e le ore minori di 10 viene messe uno 0 davanti alle ore così da visualizzare 09:21 invece di 9:21
      printHoursAndMinutes(hours, minutes, 4);
    }
    else { // se inevece sono maggiori o uguali a 10 vengono mostrati normalmente
      printHoursAndMinutes(hours, minutes, 3);
  }
}

void hoursButtonFunc () {
  while(digitalRead(buttonUpHours) == 0) {} // il processo in questa funzione è analogo, ripetuto per il pulsante delle ore.
  delay(500);
  hours++;
  if (hours == 24) { hours = 0; }
  	if(hours < 10 and minutes > 10) {
      printHoursAndMinutes(hours, minutes, 4);
    }
  	else if(hours < 10 and minutes < 10) {
      printHoursAndMinutes(hours, minutes, 1);
    }
    else if(hours > 10 and minutes <10) {
      printHoursAndMinutes(hours, minutes, 0);
    }
    else {
      printHoursAndMinutes(hours, minutes, 3);
  }
}
