
//
// Created by nedonil on 07.01.23.
//

#ifndef SPLINE_WAVE_H
#define SPLINE_WAVE_H

#include <vector>
#include <QString>

class Wave
{
public:
    Wave() = default;
    Wave(const std::vector<int>& values) { m_Values = values;}
    Wave(std::vector<int>& values) { m_Values = values;}

    void setPeriod(int period){ m_Period = period; };
    int getPeriod() {return m_Period;};

    int getAmpCount() { return m_Values.size();}
    int getAmp(size_t idx) { if(idx < m_Values.size()) return m_Values[idx];}

    std::vector<int>::const_iterator cbegin(){return m_Values.cbegin();}
    std::vector<int>::const_iterator cend(){return m_Values.cend();}

    bool fromFile(const QString& filename);
private:
    int m_Period = 1;
	std::vector<int> m_Values;

    friend class WaveProcessor;
};

#endif //SPLINE_WAVE_H
