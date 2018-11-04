#ifndef PFLARGESTEPTRACKER_HPP_
#define PFLARGESTEPTRACKER_HPP_

#include "math/MathUtil.hpp"

#include "IntTypes.hpp"

namespace Tungsten {

class PfLargeStepTracker
{
    double _cumulativeLuminance;
    uint64 _numLargeSteps;

public:
    PfLargeStepTracker() : _cumulativeLuminance(0.0), _numLargeSteps(0) {}

    void add(double luminance)
    {
        _cumulativeLuminance += luminance;
        _numLargeSteps++;
    }
    PfLargeStepTracker &operator+=(const PfLargeStepTracker &o)
    {
        _cumulativeLuminance += o._cumulativeLuminance;
        _numLargeSteps += o._numLargeSteps;
        return *this;
    }

    double getAverage() const
    {
        return _cumulativeLuminance/max(_numLargeSteps, uint64(1));
    }

    double getSum() const
    {
        return _cumulativeLuminance;
    }

    uint64 getSampleCount() const
    {
        return _numLargeSteps;
    }

    void setSampleCount(uint64 numSamples)
    {
        _numLargeSteps = numSamples;
    }

    void clear()
    {
        _cumulativeLuminance = 0.0f;
        _numLargeSteps = 0;
    }
};

}

#endif /* PFLARGESTEPTRACKER_HPP_ */
