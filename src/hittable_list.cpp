#include "hittable_list.h"

#include <memory>
#include <optional>
#include <vector>

namespace ray_tracer::geometry {
    void HittableList::add(std::unique_ptr<Hittable> hittable) noexcept {
        hittable_list_.push_back(std::move(hittable));
    }

    [[nodiscard]] std::optional<HittableList::HitRecord>
    HittableList::hit(const ray::Ray &ray, float t_min, float t_max) const noexcept {
        for (const auto &object : hittable_list_) {
            const auto maybe_hit_record = object->hit(ray, t_min, t_max);
            if (maybe_hit_record) {
                return maybe_hit_record.value();
            }
        }
        return std::nullopt;
    }
}
