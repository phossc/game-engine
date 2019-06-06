#include "engine/core/profiler.hpp"

#include "engine/core/system.hpp"

namespace engine::core {

void Profiler::update() {
    std::swap(next_results_, results_);
    next_results_.clear();
}

void Profiler::add_sample(std::string_view name, std::string_view parent_name,
                          std::chrono::nanoseconds duration) {
    if (next_results_.count(name) == 0) {
        next_results_.emplace(name, Result{name, parent_name, {}, 0});
    }

    auto result_iter = next_results_.find(name);
    result_iter->second.duration += duration;
    result_iter->second.sample_count += 1;
}

Profiler::Result Profiler::result(std::string_view name) const noexcept {
    auto result_iter = results_.find(name);
    return result_iter == std::end(results_) ? Result{name, {}, {}, 0}
                                             : result_iter->second;
}

Scope_profiler::Scope_profiler(std::string_view name,
                               std::string_view parent_name)
    : name_(name), parent_name_(parent_name),
      start_(std::chrono::high_resolution_clock::now()) {}

Scope_profiler::~Scope_profiler() {
    auto duration = std::chrono::high_resolution_clock::now() - start_;
    sys->profiler().add_sample(name_, parent_name_, duration);
}

} // namespace engine::core