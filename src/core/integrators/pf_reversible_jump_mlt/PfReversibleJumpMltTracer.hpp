#ifndef PFREVERSIBLEJUMPMLTTRACER_HPP_
#define PFREVERSIBLEJUMPMLTTRACER_HPP_

#include "PfReversibleJumpMltSettings.hpp"
#include "PfWritableMetropolisSampler.hpp"

#include "integrators/bidirectional_path_tracer/ImagePyramid.hpp"
#include "integrators/bidirectional_path_tracer/LightPath.hpp"
#include "integrators/multiplexed_mlt/MultiplexedStats.hpp"
#include "integrators/multiplexed_mlt/LargeStepTracker.hpp"
#include "integrators/kelemen_mlt/SplatQueue.hpp"
#include "integrators/TraceBase.hpp"

#include "sampling/UniformPathSampler.hpp"

namespace Tungsten {

class AtomicFramebuffer;

class PfReversibleJumpMltTracer : public TraceBase
{
    struct ChainState
    {
        LightPath cameraPath, unprunedCameraPath;
        LightPath emitterPath, unprunedEmitterPath;
        SplatQueue splats;
        std::unique_ptr<float[]> ratios;

        ChainState(int length)
        : cameraPath(length + 1),
          unprunedCameraPath(length + 1),
          emitterPath(length),
          unprunedEmitterPath(length),
          splats(1),
          ratios(new float[length + 1])
        {
        }
    };
    struct MarkovChain
    {
        std::unique_ptr<PfWritableMetropolisSampler>  cameraSampler;
        std::unique_ptr<PfWritableMetropolisSampler> emitterSampler;
        std::unique_ptr<ChainState>  currentState;
        std::unique_ptr<ChainState> proposedState;
        int currentS;
    };

    PfReversibleJumpMltSettings _settings;
    UniformSampler _sampler;
    UniformPathSampler _cameraSampler;
    UniformPathSampler _emitterSampler;
    std::unique_ptr<MarkovChain[]> _chains;
    float _lightSplatScale;

    ImagePyramid *_pyramid;

    void tracePaths(LightPath & cameraPath, PathSampleGenerator & cameraSampler,
                    LightPath &emitterPath, PathSampleGenerator &emitterSampler,
                    int s = -1, int t = -1, bool prune = true);

    void evalSample(PfWritableMetropolisSampler &cameraSampler, PfWritableMetropolisSampler &emitterSampler,
            int length, int s, ChainState &state);

public:
    PfReversibleJumpMltTracer(TraceableScene *scene, const PfReversibleJumpMltSettings &settings, uint32 threadId,
            UniformSampler &sampler, ImagePyramid *pyramid);

    void traceCandidatePath(LightPath &cameraPath, LightPath &emitterPath,
            SplatQueue &queue, const std::function<void(Vec3f, int, int)> &addCandidate);
    void startSampleChain(int s, int t, float luminance, UniformSampler &cameraReplaySampler,
            UniformSampler &emitterReplaySampler);
    LargeStepTracker runSampleChain(int pathLength, int chainLength, MultiplexedStats &stats, float luminanceScale);

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

#endif /* PFREVERSIBLEJUMPMLTTRACER_HPP_ */
