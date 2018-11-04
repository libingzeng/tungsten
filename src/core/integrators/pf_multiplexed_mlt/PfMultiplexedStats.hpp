#ifndef PFMULTIPLEXEDSTATS_HPP_
#define PFMULTIPLEXEDSTATS_HPP_

#include "PfChainTracker.hpp"

namespace Tungsten {

class AtomicPfMultiplexedStats
{
    int _numBounces;
    AtomicPfChainTracker _techniqueChange;
    AtomicPfChainTracker _largeStep;
    AtomicPfChainTracker _smallStep;
    AtomicPfChainTracker _inversion;

public:
    AtomicPfMultiplexedStats(int numBounces)
    : _numBounces(numBounces),
      _techniqueChange(numBounces),
      _largeStep(numBounces),
      _smallStep(numBounces),
      _inversion(numBounces)
    {
    }

    AtomicPfChainTracker &techniqueChange()
    {
        return _techniqueChange;
    }

    AtomicPfChainTracker &largeStep()
    {
        return _largeStep;
    }

    AtomicPfChainTracker &smallStep()
    {
        return _smallStep;
    }

    AtomicPfChainTracker &inversion()
    {
        return _inversion;
    }

    int numBounces() const
    {
        return _numBounces;
    }
};

class PfMultiplexedStats
{
    PfChainTracker _techniqueChange;
    PfChainTracker _largeStep;
    PfChainTracker _smallStep;
    PfChainTracker _inversion;

public:
    PfMultiplexedStats(AtomicPfMultiplexedStats &parent)
    : _techniqueChange(parent.techniqueChange()),
      _largeStep(parent.largeStep()),
      _smallStep(parent.smallStep()),
      _inversion(parent.inversion())
    {
    }

    PfChainTracker &techniqueChange()
    {
        return _techniqueChange;
    }

    PfChainTracker &largeStep()
    {
        return _largeStep;
    }

    PfChainTracker &smallStep()
    {
        return _smallStep;
    }

    PfChainTracker &inversion()
    {
        return _inversion;
    }
};

}

#endif /* PFMULTIPLEXEDSTATS_HPP_ */
