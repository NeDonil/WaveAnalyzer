//
// Created by nedonil on 07.01.23.
//

#include "WaveProcessor.h"
#include <math.h>
#include <algorithm>

std::vector<int> WaveProcessor::processWave(const Wave& w){

    auto wave = w.m_Values;

    auto multipliers = getMultipliers(wave.size());
    std::vector<int> result;

    for(auto i : multipliers){
        if(compare(wave, i)){
            result.insert(result.begin(), wave.begin(), wave.begin() + i);
            break;
        }
    }

    return result;
}

bool WaveProcessor::compare(std::vector<int>& wave, int n){
    int count = wave.size() / n;

    for(int i = 0; i < n; i++){
        int last_elem = wave[i];
        for(int j = 1; j < count; j++){
            if(last_elem != wave[j * n + i])
                return false;
            last_elem = wave[j * n + i];
        }
    }

    return true;
}

std::vector<int> WaveProcessor::getMultipliers(int n){
    std::vector<int> multipliers;

    for(int i = 2; i <= sqrt(n); i++){
        if(n % i == 0) {
            multipliers.push_back(i);
            multipliers.push_back(n / i);
        }
    }

    std::sort(multipliers.begin(), multipliers.end());
    return multipliers;
}
