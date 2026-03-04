#ifndef MACHINE_LOGIC_H
#define MACHINE_LOGIC_H

#include <stdint.h>

class MachineLogic {
public:
    // --- Sensor Logic ---
    // Checks if coin is detected (< 25mm)
    static bool isCoinDetected(int distMM);
    
    // Checks if cup is placed correctly to START (20mm < d <= 60mm)
    static bool isCupCorrectlyPlaced(int distMM);
    
    // Checks if cup is present during FILLING (<= 62mm)
    static bool isCupPresentDuringFill(int distMM);
    
    // Checks if cup is still present after finishing (<= 50mm)
    static bool isCupStillPresent(int distMM);

    // --- Math & Hardware Logic ---
    // Converts sensor duration to distance: (duration * 10) / 58
    static int calculateDistanceMM(long durationPulse);

    // Calculates if the relay pin should be HIGH or LOW based on active_low flag
    static int getRelayLevel(bool turnOn, bool activeLow);
};

#endif
