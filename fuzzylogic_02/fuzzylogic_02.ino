#include <FuzzyRule.h>
#include <FuzzyComposition.h>
#include <Fuzzy.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyOutput.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzySet.h>
#include <FuzzyRuleAntecedent.h>

#define VOLTAGEINPUTVALUE 5.0 // I'm using 5 Volts

int temperatureSensorPin = 0; // Analog pin 0
int buzzerPin = 9; // Digital pin 9
float temperature; // Hold temperature value
float pertinence;
unsigned long timeNow;
unsigned long timeLast;

Fuzzy* fuzzy = new Fuzzy();// Instantiating an object library

// Creating the FuzzySets to compond FuzzyInput temperature  
FuzzySet* cold = new FuzzySet(0, 0, 20, 40); // cold temperature
FuzzySet* fresh = new FuzzySet(30, 50, 50, 70); // fresh temperature
FuzzySet* hot = new FuzzySet(60, 80, 80, 80); // hot temperature

// Creating FuzzySets to compond FuzzyOutput danger
FuzzySet* freezingDanger = new FuzzySet(0, 20, 20, 40); // freezingDanger danger
FuzzySet* small = new FuzzySet(30, 50, 50, 70); // good small
FuzzySet* meltingDanger = new FuzzySet(60, 80, 80, 100); // meltingDanger danger

//FuzzyRule* fuzzyRule01;
//FuzzyRule* fuzzyRule02;
//FuzzyRule* fuzzyRule03;

void setup(){
   pinMode(buzzerPin, OUTPUT);
   digitalWrite(buzzerPin, LOW);
   
   FuzzyInput* temperature = new FuzzyInput(1);// Creating a FuzzyInput temperature, (as param, its ID)
 
  temperature->addFuzzySet(cold); // Add FuzzySet cold to FuzzyInput temperature
  temperature->addFuzzySet(fresh); // Add FuzzySet fresh to FuzzyInput temperature
  temperature->addFuzzySet(hot); // Add FuzzySet hot to FuzzyInput temperature

  fuzzy->addFuzzyInput(temperature); // Add FuzzyInput to Fuzzy object
  
  FuzzyOutput* danger = new FuzzyOutput(1);// Creating FuzzyOutput danger, With its ID in param

  danger->addFuzzySet(freezingDanger); // Add FuzzySet freezingDanger to danger
  danger->addFuzzySet(small); // Add FuzzySet small to danger
  danger->addFuzzySet(meltingDanger); // Add FuzzySet meltingDanger to danger
  
  fuzzy->addFuzzyOutput(danger); // Add FuzzyOutput to Fuzzy object
  
  //Assembly the Fuzzy rules
  // FuzzyRule "IF temperature = hot THEN danger = meltingDanger"
  FuzzyRuleAntecedent* ifTemperatureHot = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
  ifTemperatureHot->joinSingle(hot); // Adding corresponding FuzzySet to Antecedent object
  FuzzyRuleConsequent* thenDangerMeltingDanger = new FuzzyRuleConsequent(); // Instantiating a Consequent to expression
  thenDangerMeltingDanger->addOutput(meltingDanger);// Adding corresponding FuzzySet to Consequent object
  // Instantiating a FuzzyRule object
  FuzzyRule* fuzzyRule01 = new FuzzyRule(1, ifTemperatureHot, thenDangerMeltingDanger); // Passing the Antecedent and the Consequent of expression
 
  fuzzy->addFuzzyRule(fuzzyRule01); // Adding FuzzyRule to Fuzzy object

  // FuzzyRule "IF temperature = fresh THEN danger = small"
  FuzzyRuleAntecedent* ifTemperatureFresh = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
  ifTemperatureFresh->joinSingle(fresh); // Adding corresponding FuzzySet to Antecedent object
  FuzzyRuleConsequent* thenDangerSmall = new FuzzyRuleConsequent(); // Instantiating a Consequent to expression
  thenDangerSmall->addOutput(small); // Adding corresponding FuzzySet to Consequent object
  // Instantiating a FuzzyRule object
  FuzzyRule* fuzzyRule02 = new FuzzyRule(2, ifTemperatureFresh, thenDangerSmall); // Passing the Antecedent and the Consequent of expression

  fuzzy->addFuzzyRule(fuzzyRule02); // Adding FuzzyRule to Fuzzy object

  // FuzzyRule "IF temperature = cold THEN danger = freezingDanger"
  FuzzyRuleAntecedent* ifTemperatureCold = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
  ifTemperatureCold->joinSingle(cold); // Adding corresponding FuzzySet to Antecedent object
  FuzzyRuleConsequent* thenDangerFreezingDanger = new FuzzyRuleConsequent(); // Instantiating a Consequent to expression
  thenDangerFreezingDanger->addOutput(freezingDanger);// Adding corresponding FuzzySet to Consequent object
  // Instantiating a FuzzyRule object
  FuzzyRule* fuzzyRule03 = new FuzzyRule(3, ifTemperatureCold, thenDangerFreezingDanger); // Passing the Antecedent and the Consequent of expression

  fuzzy->addFuzzyRule(fuzzyRule03); // Adding FuzzyRule to Fuzzy object
}

void loop(){
  timeNow = millis();// Get time since the program started in miliseconds
  if( (timeNow - timeLast) >= 60000 ){// Every 1 minute check the temperature
    timeLast = timeNow;
    // Read temperature
    temperature = getTemperature(temperatureSensorPin);
      
    //Report teh temperature value, passing its ID and value
    fuzzy->setInput(1, temperature);
     
    // Executing the fuzzification
    fuzzy->fuzzify();
     
    /*
      Now, in this example, id dont want to know the crisp output value,
      I just want to know if fuzzyRule01 or fuzzyRule03 was fired, tu buz
    */
    if( (fuzzy->isFiredRule(1) == true) or (fuzzy->isFiredRule(3) == true) ){
      pertinence = freezingDanger->getPertinence();
      if(pertinence == 0.0){
        pertinence = meltingDanger->getPertinence();
      }
      // buzzer will delay in a percentage of the pertinence, max 10 seconds
      int buzzerDelay = (int) (pertinence * 10000);
      buzzerAlert(buzzerDelay);
    }
  }
}

float getCurrent(float input){
  return ((input * VOLTAGEINPUTVALUE) / 1023.0);
}

float getTemperature(int pin){
  float lecture = analogRead(pin);
  float current = getCurrent(lecture);
  return current * 100;
}

void buzzerAlert(int buzzerDelay){
  digitalWrite(buzzerPin, HIGH);
  delay(buzzerDelay);
  digitalWrite(buzzerPin, LOW);
}
