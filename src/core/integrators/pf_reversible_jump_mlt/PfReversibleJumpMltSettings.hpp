#ifndef PFREVERSIBLEJUMPMLTSETTINGS_HPP_
#define PFREVERSIBLEJUMPMLTSETTINGS_HPP_

#include "integrators/TraceSettings.hpp"

#include "io/JsonObject.hpp"

namespace Tungsten {

struct PfReversibleJumpMltSettings : public TraceSettings
{
    int initialSamplePool;
    int iterationsPerBatch;
    bool imagePyramid;
    bool gaussianMutation;
    float largeStepProbability;
    float strategyPerturbationProbability;

    PfReversibleJumpMltSettings()
    : initialSamplePool(3000000),
      iterationsPerBatch(-1),
      imagePyramid(false),
      gaussianMutation(false),
      largeStepProbability(0.1f),
      strategyPerturbationProbability(0.05f)
    {
    }

    void fromJson(JsonPtr v)
    {
        TraceSettings::fromJson(v);
        v.getField("initial_sample_pool", initialSamplePool);
        v.getField("iterations_per_batch", iterationsPerBatch);
        v.getField("image_pyramid", imagePyramid);
        v.getField("large_step_probability", largeStepProbability);
        v.getField("strategy_perturbation_probability", strategyPerturbationProbability);
        v.getField("gaussian_mutation", gaussianMutation);
    }

    rapidjson::Value toJson(rapidjson::Document::AllocatorType &allocator) const
    {
        rapidjson::Value v = TraceSettings::toJson(allocator);
        v.RemoveMember("type");

        return JsonObject{std::move(v), allocator,
            "type", "pf_reversible_jump_mlt",
            "initial_sample_pool", initialSamplePool,
            "iterations_per_batch", iterationsPerBatch,
            "image_pyramid", imagePyramid,
            "large_step_probability", largeStepProbability,
            "strategy_perturbation_probability", strategyPerturbationProbability,
            "gaussian_mutation", gaussianMutation
        };
    }
};

}

#endif /* PFREVERSIBLEJUMPMLTSETTINGS_HPP_ */
