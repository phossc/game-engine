#ifndef ENGINE_CORE_PROFILER_HPP
#define ENGINE_CORE_PROFILER_HPP

#include <chrono>
#include <map>
#include <string_view>

namespace engine {

//! A profiler providing very basic functionality. It currently assumes that
//! functions being profiled are called on a single path and not recursively.
class Profiler {
public:
    struct Result {
        std::string_view name;
        std::string_view parent_name;
        std::chrono::nanoseconds duration;
        int sample_count;
    };

    void update();

    void add_sample(std::string_view name, std::string_view parent_name,
                    std::chrono::nanoseconds duration);

    Result result(std::string_view name) const noexcept;

private:
    std::map<std::string_view, Result> next_results_;
    std::map<std::string_view, Result> results_;
};

class Scope_profiler {
public:
    Scope_profiler(std::string_view name, std::string_view parent_name);
    ~Scope_profiler();

private:
    std::string_view name_;
    std::string_view parent_name_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};

} // namespace engine

#define PROFILE(name, parent_name)                                             \
    engine::Scope_profiler scope_profiler{name, parent_name};

#endif /* ENGINE_CORE_PROFILER_HPP */