#include <random>
#include <ratio>

namespace FailsCountingPolicies {
class NoCount {
protected:
    void count(bool failed) {};
};

template<typename CountersType = std::size_t>
class Count {
protected:
    Count() : nbFails(0), nbSuccesses(0) {}

    void count(bool failed) {
        if (failed) {
            ++nbFails;
        }
        else {
            ++nbSuccesses;
        }
    }

    CountersType nbFails, nbSuccesses;

public:
    CountersType getFails() { return nbFails; }
    CountersType getSuccesses() { return nbSuccesses; }
};

}


template<typename FailureRate, typename FailsCountingPolicy = FailsCountingPolicies::NoCount>
class FailureGenerator : public FailsCountingPolicy {
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<uint32_t> dis;
    volatile static constexpr uint32_t distLength = FailureRate::den/FailureRate::num;
    volatile static constexpr uint32_t medValue = distLength / 2;


public:
    FailureGenerator() : gen(rd()), dis(1, distLength) {}
    bool fail() {
        auto failed = (dis(gen) == medValue);
        FailsCountingPolicy::count(failed);
        return failed;
    }
};

