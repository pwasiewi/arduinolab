//https://github.com/zerokol/raj1

#include <FuzzyIO.h>
#include <Fuzzy.h>
#include <FuzzyInput.h>
#include <FuzzySet.h>
#include <FuzzyComposition.h>
#include <FuzzyRuleAntecedent.h>
#include <FuzzyOutput.h>
#include <FuzzyRule.h>
#include <FuzzyRuleConsequent.h>

const byte LEFT_MOTOR_INDEX = 0;
const byte RIGHT_MOTOR_INDEX = 1;

const byte MOTOR_ENABLE = 0;
const byte MOTOR_OUTPUT_1 = 1;
const byte MOTOR_OUTPUT_2 = 2;

const byte MOTOR_AMOUNT = 2;

const byte MOTOR_PINS[MOTOR_AMOUNT][3] = {{6, 7, 8}, {9, 10, 11}};

const byte SONAR_INIT_INDEX = 0;
const byte SONAR_ECHO_INDEX = 1;

const byte SONAR_LEFT_INDEX = 0;
const byte SONAR_RIGHT_INDEX = 1;

const byte SONAR_READ_AMOUNT = 3;  // Num de leituras para fazer uma media, evitar ruidos
const byte SONAR_PERIOD = 5;
const byte CONSTANT_TO_CONVERT_TO_CM = 58;

const byte SONAR_AMOUNT = 2;

const byte SONAR_PINS[SONAR_AMOUNT][2] = {{2, 3}, {4, 5}};
byte sonarDistances[SONAR_AMOUNT] = {0, 0};

// Instanciando um objeto da biblioteca
Fuzzy* fuzzy = new Fuzzy();

void setup(){
    // FuzzyInput
    FuzzyInput* distanceL = new FuzzyInput(1);

    FuzzySet* closeL = new FuzzySet(0, 0.2, 0.2, 0.4);
    distanceL->addFuzzySet(closeL);
    FuzzySet* safeL = new FuzzySet(0.3, 0.6, 0.7, 0.9);
    distanceL->addFuzzySet(safeL);
    FuzzySet* farL = new FuzzySet(0.8, 0.9, 1.0, 1.0);
    distanceL->addFuzzySet(farL);

    fuzzy->addFuzzyInput(distanceL);

    // FuzzyInput
    FuzzyInput* distanceR = new FuzzyInput(2);

    FuzzySet* closeR = new FuzzySet(0, 0.2, 0.2, 0.4);
    distanceR->addFuzzySet(closeR);
    FuzzySet* safeR = new FuzzySet(0.3, 0.6, 0.7, 0.9);
    distanceR->addFuzzySet(safeR);
    FuzzySet* farR = new FuzzySet(0.8, 0.9, 1.0, 1.0);
    distanceR->addFuzzySet(farR);

    fuzzy->addFuzzyInput(distanceR);

    // FuzzyOutput
    FuzzyOutput* direction = new FuzzyOutput(1);

    FuzzySet* toLeft = new FuzzySet(-90, -90, -90, -10);
    direction->addFuzzySet(toLeft);
    FuzzySet* ahead = new FuzzySet(-20, -5, 5, 20);
    direction->addFuzzySet(ahead);
    FuzzySet* toRight = new FuzzySet(10, 90, 90, 90);
    direction->addFuzzySet(toRight);

    fuzzy->addFuzzyOutput(direction);

    // Building FuzzyRule
    FuzzyRuleAntecedent* ifDistanceLCloseLAndDistanceRCloseR = new FuzzyRuleAntecedent();
    ifDistanceLCloseLAndDistanceRCloseR->joinWithAND(closeL, closeR);

    FuzzyRuleConsequent* thenAhead = new FuzzyRuleConsequent();
    thenAhead->addOutput(ahead);

    FuzzyRule* fuzzyRule1 = new FuzzyRule(1, ifDistanceLCloseLAndDistanceRCloseR, thenAhead);
    fuzzy->addFuzzyRule(fuzzyRule1);

    // Building FuzzyRule
    FuzzyRuleAntecedent* ifDistanceSafeLAndDistanceRSafeR = new FuzzyRuleAntecedent();
    ifDistanceSafeLAndDistanceRSafeR->joinWithAND(safeL, safeR);

    FuzzyRule* fuzzyRule2 = new FuzzyRule(2, ifDistanceSafeLAndDistanceRSafeR, thenAhead);
    fuzzy->addFuzzyRule(fuzzyRule2);

    // Building FuzzyRule
    FuzzyRuleAntecedent* ifDistanceFarLAndDistanceRFarR = new FuzzyRuleAntecedent();
    ifDistanceFarLAndDistanceRFarR->joinWithAND(farL, farR);

    FuzzyRule* fuzzyRule3 = new FuzzyRule(3, ifDistanceFarLAndDistanceRFarR, thenAhead);
    fuzzy->addFuzzyRule(fuzzyRule3);

    // Building FuzzyRule
    FuzzyRuleAntecedent* ifDistanceLCloseLAndDistanceRFarR = new FuzzyRuleAntecedent();
    ifDistanceLCloseLAndDistanceRFarR->joinWithAND(closeL, farR);

    FuzzyRuleConsequent* thenDirectionToRight = new FuzzyRuleConsequent();
    thenDirectionToRight->addOutput(toRight);

    FuzzyRule* fuzzyRule4 = new FuzzyRule(4, ifDistanceLCloseLAndDistanceRFarR, thenDirectionToRight);
    fuzzy->addFuzzyRule(fuzzyRule4);

    // Building FuzzyRule
    FuzzyRuleAntecedent* ifDistanceLCloseLAndDistanceRSafeR = new FuzzyRuleAntecedent();
    ifDistanceLCloseLAndDistanceRSafeR->joinWithAND(closeL, safeR);

    FuzzyRule* fuzzyRule5 = new FuzzyRule(5, ifDistanceLCloseLAndDistanceRSafeR, thenDirectionToRight);
    fuzzy->addFuzzyRule(fuzzyRule5);

    // Building FuzzyRule
    FuzzyRuleAntecedent* ifDistanceLSafeLAndDistanceRFarR = new FuzzyRuleAntecedent();
    ifDistanceLSafeLAndDistanceRFarR->joinWithAND(safeL, farR);

    FuzzyRule* fuzzyRule6 = new FuzzyRule(6, ifDistanceLSafeLAndDistanceRFarR, thenDirectionToRight);
    fuzzy->addFuzzyRule(fuzzyRule6);

    // Building FuzzyRule
    FuzzyRuleAntecedent* ifDistanceLFarLAndDistanceRCloseR = new FuzzyRuleAntecedent();
    ifDistanceLFarLAndDistanceRCloseR->joinWithAND(farL, closeR);

    FuzzyRuleConsequent* thenDirectionToLeft = new FuzzyRuleConsequent();
    thenDirectionToLeft->addOutput(toLeft);

    FuzzyRule* fuzzyRule7 = new FuzzyRule(7, ifDistanceLFarLAndDistanceRCloseR, thenDirectionToLeft);
    fuzzy->addFuzzyRule(fuzzyRule7);

    // Building FuzzyRule
    FuzzyRuleAntecedent* ifDistanceLSafeLAndDistanceRCloseR = new FuzzyRuleAntecedent();
    ifDistanceLSafeLAndDistanceRCloseR->joinWithAND(safeL, closeR);

    FuzzyRule* fuzzyRule8 = new FuzzyRule(8, ifDistanceLSafeLAndDistanceRCloseR, thenDirectionToLeft);
    fuzzy->addFuzzyRule(fuzzyRule8);

    // Building FuzzyRule
    FuzzyRuleAntecedent* ifDistanceLFarLAndDistanceRSafeR = new FuzzyRuleAntecedent();
    ifDistanceLFarLAndDistanceRSafeR->joinWithAND(farL, safeR);

    FuzzyRule* fuzzyRule9 = new FuzzyRule(9, ifDistanceLFarLAndDistanceRSafeR, thenDirectionToLeft);
    fuzzy->addFuzzyRule(fuzzyRule9);

    // Configurando os motores
    for(byte i=0; i < MOTOR_AMOUNT; i++){
        pinMode(MOTOR_PINS[i][MOTOR_ENABLE], OUTPUT);
        pinMode(SONAR_PINS[i][MOTOR_OUTPUT_1], OUTPUT);
        pinMode(SONAR_PINS[i][MOTOR_OUTPUT_2], OUTPUT);
    }

    // Configurando o sonar
    for(byte i=0; i < SONAR_AMOUNT; i++){
        pinMode(SONAR_PINS[i][SONAR_INIT_INDEX], OUTPUT);  // Configurando o pino como saída
        pinMode(SONAR_PINS[i][SONAR_ECHO_INDEX], INPUT);  // Configurando o pino como entrada
    }
    // Ler as distancias iniciais
    readSonars();

    Serial.begin(9600);

    delay(1000);
}

void loop(){
    readSonars();

    fuzzy->setInput(1, sonarDistances[SONAR_LEFT_INDEX] / 100.0);
    fuzzy->setInput(2, sonarDistances[SONAR_RIGHT_INDEX] / 100.0);

    fuzzy->fuzzify();

    moveRobot(fuzzy->defuzzify(1));

    delay(100);
}

void readSonars(){
    for(byte i=0; i < SONAR_AMOUNT; i++){
        sonarDistances[i] = readSonarAt(i);
        if(sonarDistances[i] > 100){
            sonarDistances[i] = 100;
        }
    }
}

unsigned long readSonarAt(byte sonarIndex){
    unsigned long distancesSum = 0;
    // Fazer uma iteração do tamanho desejado para tirar a média
    for(byte i = 0; i < SONAR_READ_AMOUNT; i++) {
        digitalWrite(SONAR_PINS[sonarIndex][SONAR_INIT_INDEX], HIGH);  // Enviando um pulso deve durar 10us
        delayMicroseconds(10);  // Esperando os 10us
        digitalWrite(SONAR_PINS[sonarIndex][SONAR_INIT_INDEX], LOW);  // Encerrando o envio do pulso

        // Procurando por um retorno do pulso
        unsigned long pulseTime = pulseIn(SONAR_PINS[sonarIndex][SONAR_ECHO_INDEX], HIGH);
        // distancia = tempoDoPulso / CONSTANTE_CONVERTER_PARA_CM para converter para cm.
        distancesSum += pulseTime / CONSTANT_TO_CONVERT_TO_CM;

        delay(SONAR_PERIOD);  // Aguardar um periodo para a próxima iteração
    }

    return distancesSum / SONAR_READ_AMOUNT;
}

void moveRobot(float direction){
    float leftSpeed = (direction / 90.0) + 1;
    if(leftSpeed > 0){
        wheelCounterclockwise(LEFT_MOTOR_INDEX, map(leftSpeed, -1.0, 1.0, 0, 255));
    }else{
        wheelClockwise(LEFT_MOTOR_INDEX, map(leftSpeed, -1.0, 1.0, 0, 255));
    }

    float rightSpeed = (direction / -90.0) + 1;          
    if(rightSpeed > 0){
        wheelCounterclockwise(RIGHT_MOTOR_INDEX, map(rightSpeed, -1.0, 1.0, 0, 255));
    }else{
        wheelClockwise(RIGHT_MOTOR_INDEX, map(rightSpeed, -1.0, 1.0, 0, 255));
    }
    
    Serial.print("leftSpeed: ");
    Serial.print(leftSpeed);
    Serial.print(", rightSpeed: ");
    Serial.println(rightSpeed);
}

void wheelClockwise(byte motorIndex, word speed){
    digitalWrite(MOTOR_PINS[motorIndex][MOTOR_OUTPUT_1], HIGH);
    digitalWrite(MOTOR_PINS[motorIndex][MOTOR_OUTPUT_2], LOW);
    analogWrite(MOTOR_PINS[motorIndex][MOTOR_ENABLE], speed);
}

void wheelCounterclockwise(byte motorIndex, word speed){
    digitalWrite(MOTOR_PINS[motorIndex][MOTOR_OUTPUT_1], LOW);
    digitalWrite(MOTOR_PINS[motorIndex][MOTOR_OUTPUT_2], HIGH);
    analogWrite(MOTOR_PINS[motorIndex][MOTOR_ENABLE], speed);
}

