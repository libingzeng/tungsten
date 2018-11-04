#ifndef PFMULTIPLEXEDMLTSETTINGS_HPP_
#define PFMULTIPLEXEDMLTSETTINGS_HPP_

#include "integrators/TraceSettings.hpp"

#include "io/JsonUtils.hpp"

namespace Tungsten {

struct PfMultiplexedMltSettings : public TraceSettings
{
    int initialSamplePool;
    bool imagePyramid;
    float largeStepProbability;

    PfMultiplexedMltSettings()
    : initialSamplePool(3000000),
      imagePyramid(false),
      largeStepProbability(0.1f)
    {
    }

    void fromJson(JsonPtr v)
    {
        TraceSettings::fromJson(v);
        v.getField("initial_sample_pool", initialSamplePool);
        v.getField("image_pyramid", imagePyramid);
        v.getField("large_step_probability", largeStepProbability);
    }

    rapidjson::Value toJson(rapidjson::Document::AllocatorType &allocator) const
    {
        rapidjson::Value v = TraceSettings::toJson(allocator);
        v.AddMember("type", "pf_multiplexed_mlt", allocator);
        v.AddMember("initial_sample_pool", initialSamplePool, allocator);
        v.AddMember("image_pyramid", imagePyramid, allocator);
        v.AddMember("large_step_probability", largeStepProbability, allocator);
        return std::move(v);
    }
};

}

#endif /* PFMULTIPLEXEDMLTSETTINGS_HPP_ */
