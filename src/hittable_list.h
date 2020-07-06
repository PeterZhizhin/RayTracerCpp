#ifndef RAYTRACERPROJECT_HITTABLE_LIST_H
#define RAYTRACERPROJECT_HITTABLE_LIST_H

#include <memory>
#include <optional>
#include <vector>

#include "hittable.h"

namespace ray_tracer::geometry {
    class HittableList : public Hittable {
        using HitRecord = Hittable::HitRecord;

        void add(std::unique_ptr<Hittable> hittable) noexcept;

        [[nodiscard]] std::optional<HitRecord>
        hit(const ray::Ray &ray, float t_min, float t_max) const noexcept override;

        ~HittableList() override = default;

    private:
        std::vector<std::unique_ptr<Hittable>> hittable_list_;
    };
}


#endif //RAYTRACERPROJECT_HITTABLE_LIST_H
