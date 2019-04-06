#include <Arduino.h>

#include <String.h>
#include <stdio.h>

String cmd = "";

#define DEBUG false

#define MQ9 5
#define MQ5 4
#define TEMP_SENS 3

#define RELAY_A 2
#define RELAY_B 6

#define deviceID 2

double getTemperature();
int getMQ5();
int getMQ9();
bool isSame(char *command, char *format, int length_f);

void setup() {
    Serial.begin(115200);
    analogReference(INTERNAL);

    pinMode(RELAY_A, OUTPUT);
    pinMode(RELAY_B, OUTPUT);

    digitalWrite(RELAY_A, LOW);
    digitalWrite(RELAY_B, LOW);

}

void loop() {
if (Serial.available() > 0) {
        char c = Serial.read();

        if (c != '\n') {
            cmd += c;
        } else {

            int buffer_l = cmd.length() + 1;
            char buffer[buffer_l];

            cmd.toCharArray(buffer, buffer_l);
            
            char commd[50] = {0};
            int device;

            sscanf(buffer, "NewGetRequest:deviceID=%i&command=%s\0", &device, commd);

            if (device == deviceID) {
                if (isSame(commd, "LAMP_ON", 7)) {
                    digitalWrite(RELAY_A, HIGH);
                } else if (isSame(commd, "LAMP_OFF", 8)) {
                    digitalWrite(RELAY_A, LOW);
                } else if (isSame(commd, "GET_MQ5", 7)) {
                    Serial.print("deviceID=0&cmd=");
                    Serial.println(getMQ5());
                } else if (isSame(commd, "GET_MQ9", 7)) {
                    Serial.print("deviceID=0&cmd=");
                    Serial.println(getMQ9());
                } else if (isSame(commd, "GET_TEMP", 8)) {
                    Serial.print("deviceID=0&cmd=");
                    Serial.println(getTemperature());
                }  else if (isSame(commd, "LAMP_ON_1", 9)) {
                    digitalWrite(RELAY_B, HIGH);
                } else if (isSame(commd, "LAMP_OFF_1", 10)) {
                    digitalWrite(RELAY_B, LOW);
                }
            }

            cmd = "";
        }
    }
}

int getMQ5() { // H2, LPG, CH4, CO, Alcohol
    return analogRead(MQ5);
}

int getMQ9() { // LPG, CO, CH4
    return analogRead(MQ9);
}

double getTemperature() {
    int valeur_brute = analogRead(TEMP_SENS);

    double temperature_celcius = valeur_brute * (5.0 / 1023.0 * 100.0);

    if (DEBUG) {
        Serial.println(temperature_celcius);
    }
    
    return temperature_celcius;
}

bool isSame(char *command, char *format, int length_f) {
    for(int i = 0; i < length_f; i++) {
        if (command[i] != format[i]) {
            return false;
        }
    }

    return true;
    
}