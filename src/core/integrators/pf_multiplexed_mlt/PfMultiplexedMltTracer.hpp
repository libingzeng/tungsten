#ifndef PFMULTIPLEXEDMLTTRACER_HPP_
#define PFMULTIPLEXEDMLTTRACER_HPP_

#include "PfMultiplexedMltSettings.hpp"
#include "PfMultiplexedStats.hpp"
#include "PfLargeStepTracker.hpp"

#include "integrators/bidirectional_path_tracer/ImagePyramid.hpp"
#include "integrators/bidirectional_path_tracer/LightPath.hpp"

#include "integrators/kelemen_mlt/MetropolisSampler.hpp"
#include "integrators/kelemen_mlt/SplatQueue.hpp"

#include "integrators/TraceBase.hpp"

#include "sampling/UniformPathSampler.hpp"

namespace Tungsten {

class AtomicFramebuffer;

class PfMultiplexedMltTracer : public TraceBase
{
private:
    struct MarkovChain
    {
        std::unique_ptr<MetropolisSampler>  cameraSampler;
        std::unique_ptr<MetropolisSampler> emitterSampler;
        std::unique_ptr<LightPath>  cameraPath;
        std::unique_ptr<LightPath> emitterPath;
        std::unique_ptr<SplatQueue>  currentSplats;
        std::unique_ptr<SplatQueue> proposedSplats;
        int currentS;
    };

    PfMultiplexedMltSettings _settings;
    UniformSampler _sampler;
    UniformPathSampler _cameraSampler;
    UniformPathSampler _emitterSampler;
    std::unique_ptr<MarkovChain[]> _chains;
    float _lightSplatScale;

    ImagePyramid *_pyramid;

    void tracePaths(LightPath & cameraPath, PathSampleGenerator & cameraSampler,
                    LightPath &emitterPath, PathSampleGenerator &emitterSampler,
                    int s = -1, int t = -1);

    int evalSample(LightPath & cameraPath, PathSampleGenerator & cameraSampler,
                   LightPath &emitterPath, PathSampleGenerator &emitterSampler,
                   int length, SplatQueue &queue);

public:
    PfMultiplexedMltTracer(TraceableScene *scene, const PfMultiplexedMltSettings &settings, uint32 threadId,
            UniformSampler &sampler, ImagePyramid *pyramid);

    void traceCandidatePath(LightPath &cameraPath, LightPath &emitterPath,
            SplatQueue &queue, const std::function<void(Vec3f, int, int)> &addCandidate);
    void startSampleChain(int s, int t, float luminance, UniformSampler &cameraReplaySampler,
            UniformSampler &emitterReplaySampler);
    PfLargeStepTracker runSampleChain(int pathLength, int chainLength, PfMultiplexedStats &stats, float luminanceScale);

    UniformPathSampler &cameraSampler()
    {
        return _cameraSampler;
    }

    UniformPathSampler &emitterSampler()
    {
        return _emitterSampler;
    }
};

}

#endif /* PFMULTIPLEXEDMLTTRACER_HPP_ */
