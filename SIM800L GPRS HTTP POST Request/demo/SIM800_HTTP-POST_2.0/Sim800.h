#ifndef __SIM800_H__
#define __SIM800_H__

#include <Arduino.h>

void SimSetup();
void SimRun();
//*************************************//
void unrecognized();
void Answer();
//-------------------------------//
void SimTest();
void SendHost(const char *DATA);
void SimCeng();
void SimCLOCK();
void SimLBS();
void Sim_CSQ();
void Sim_TKC();
void AT_sim(const char *command);
void AT_wait(unsigned long wait);

#endif
