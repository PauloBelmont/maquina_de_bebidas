#include "MachineLogic.h"

// Logic from: "if (distMM < 25)"
bool MachineLogic::isCoinDetected(int distMM) {
    return (distMM < 25 && distMM > 0); 
}

// Logic from: "if (distMM > 20 && distMM <= 60)"
bool MachineLogic::isCupCorrectlyPlaced(int distMM) {
    return (distMM > 20 && distMM <= 60);
}

// Logic from: "if (distMM <= 62)" inside processarEnchimento
bool MachineLogic::isCupPresentDuringFill(int distMM) {
    return (distMM <= 62 && distMM > 0);
}

// Logic from: "while(lerDistanciaMM(...) <= 50)"
bool MachineLogic::isCupStillPresent(int distMM) {
    return (distMM <= 50 && distMM > 0);
}

// Logic from: "(duracao * 10) / 58"
int MachineLogic::calculateDistanceMM(long durationPulse) {
    return (int)((durationPulse * 10) / 58);
}

// Logic from: "controlarRele"
int MachineLogic::getRelayLevel(bool turnOn, bool activeLow) {
    // If Active Low (true) AND Turn On (true) -> LOW (0)
    // If Active Low (true) AND Turn Off (false) -> HIGH (1)
    return (activeLow ^ turnOn) ? 1 : 0; // 1=HIGH, 0=LOW
}
