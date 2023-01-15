//
// Created by nedonil on 07.01.23.
//

#ifndef WAVEPROCESSOR_WAVEPROCESSOR_H
#define WAVEPROCESSOR_WAVEPROCESSOR_H

#include <vector>
#include "Wave.h"

class WaveProcessor {
public:
    static std::vector<int> processWave(const Wave& w);
private:
    static bool compare(std::vector<int>& wave, int n);
    static std::vector<int> getMultipliers(int n);
};

#endif //WAVEPROCESSOR_WAVEPROCESSOR_H
