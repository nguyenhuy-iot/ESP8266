#ifndef __SIM800_H__
#define __SIM800_H__

#include <Arduino.h>

void SimSetup();
void SimRun();
//*************************************//
void unrecognized();
void Answer();
//-------------------------------//
void Sim_AT(const char *command, uint16_t wait = 1000);
void SimTest();
void SendHost();
void SimCeng();

#endif
