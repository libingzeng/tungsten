#ifndef PFCHAINTRACKER_HPP_
#define PFCHAINTRACKER_HPP_

#include "io/FileUtils.hpp"

#include <cstring>
#include <atomic>

namespace Tungsten {

typedef Vec<std::atomic<int>, 2> Vec2ia;

class AtomicPfChainTracker
{
    int _numBounces;
    std::unique_ptr<Vec2ia[]> _chainData;

public:
    AtomicPfChainTracker(int numBounces)
    : _numBounces(numBounces + 4),
      _chainData(new Vec2ia[_numBounces])
    {
        std::memset(_chainData.get(), 0, _numBounces*sizeof(Vec2ia));
    }

    int numBounces() const
    {
        return _numBounces;
    }

    Vec2ia *chainData()
    {
        return _chainData.get();
    }

    float acceptanceRatio(int length) const
    {
        return _chainData[length].x()/float(_chainData[length].x() + _chainData[length].y());
    }

    int numMutations(int length) const
    {
        return _chainData[length].x() + _chainData[length].y();
    }
};

class PfChainTracker
{
    AtomicPfChainTracker &_parent;
    int _numBounces;
    std::unique_ptr<Vec2i[]> _chainData;

public:
    PfChainTracker(AtomicPfChainTracker &parent)
    : _parent(parent),
      _numBounces(parent.numBounces()),
      _chainData(new Vec2i[_numBounces])
    {
        std::memset(_chainData.get(), 0, _numBounces*sizeof(Vec2i));
    }
    PfChainTracker(PfChainTracker &&o)
    : _parent(o._parent),
      _numBounces(o._numBounces),
      _chainData(std::move(o._chainData))
    {
    }

    ~PfChainTracker()
    {
        if (_chainData) {
            for (int i = 0; i < _numBounces; ++i) {
                _parent.chainData()[i].x() += _chainData[i].x();
                _parent.chainData()[i].y() += _chainData[i].y();
            }
        }
    }

    void accept(int length)
    {
        _chainData[length].x()++;
    }

    void reject(int length)
    {
        _chainData[length].y()++;
    }
};

}

#endif /* PFCHAINTRACKER_HPP_ */
