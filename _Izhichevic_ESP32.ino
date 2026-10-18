#include <Arduino.h>

const int NUM_NEURONS = 4;
const int NEURON_PINS[NUM_NEURONS] = {18, 19, 21, 22};

float v[NUM_NEURONS];
float u[NUM_NEURONS];
float I[NUM_NEURONS];

void setup() {
  randomSeed(analogRead(0));

  for (int i = 0; i < NUM_NEURONS; i++) {
    pinMode(NEURON_PINS[i], OUTPUT);
    v[i] = -65.0;
    u[i] = -13.0;
    I[i] = 0.0;
  }
}

void loop() {
  float dt = 0.5;

  for (int i = 0; i < NUM_NEURONS; i++) {
    float noise = (random(0, 100) / 100.0) * 3.5; 
    
    v[i] = v[i] + dt * (0.04 * v[i] * v[i] + 5.0 * v[i] + 140.0 - u[i] + (I[i] + noise));
    u[i] = u[i] + dt * (0.02 * (0.2 * v[i] - u[i]));

    I[i] *= 0.85;

    if (v[i] >= 30.0) {
      analogWrite(NEURON_PINS[i], 255); 
      v[i] = -65.0;                   
      
      u[i] += 6.0 + (random(0, 400) / 100.0);

      for (int target = 0; target < NUM_NEURONS; target++) {
        if (target != i) {
          if (random(0, 100) < 40) { 
            float randomWeight = (random(10, 250) / 10.0); 
            I[target] += randomWeight;
          }
        }
      }
    } else {
      int brightness = map(constrain((int)v[i], -70, 30), -70, 30, 0, 150);
      analogWrite(NEURON_PINS[i], brightness);
    }
  }

  delay(20); 
}