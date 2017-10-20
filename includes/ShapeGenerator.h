#pragma once
#include "RandomGenerator.h"
#include "Image.h"
#include "Ellipse.h"
#include <memory>

enum class ShapeType {
  TRIANGLE,
  ELLIPSE
};

class ShapeGenerator {
private:
  Image target_;
  RandomGenerator generator_;

  std::shared_ptr<Shape> GenerateTriangle() const {
    return std::shared_ptr<Shape>(new Triangle(generator_.RandomPoint(), generator_.RandomPoint(), generator_.RandomPoint()));
  }

  std::shared_ptr<Shape> GenerateEllipse() const {
    return std::shared_ptr<Shape>(new Ellipse(generator_.RandomPoint(), generator_.RandomX(), generator_.RandomY()));
  }

  std::shared_ptr<Shape> ShapeConstructor(const ShapeType type) const {
    switch (type) {
    case ShapeType::TRIANGLE:
      return GenerateTriangle();
    case ShapeType::ELLIPSE:
      return GenerateEllipse();
    default:
      return nullptr;
    }
  }
public:
  const int RANDOM_BEST = 15;

  ShapeGenerator(const Image& image)
    : target_(image), generator_(target_.GetWidth(), target_.GetHeight()) {

  }

  std::shared_ptr<Shape> Generate(const ShapeType type) const {
    std::shared_ptr<Shape> shape = ShapeConstructor(type);
    while (!shape->Valid()) {
      shape = ShapeConstructor(type);
    }
    shape->SetColor(target_);
    return shape;
  }

  std::shared_ptr<Shape> GenerateBest(const Image& current, const ShapeType type) const {
    float best_energy = std::numeric_limits<float>::max();
    std::shared_ptr<Shape> best_shape = nullptr;
    for (int i = 0; i < RANDOM_BEST; ++i) {
      std::shared_ptr<Shape> new_shape = Generate(type);
      const float new_energy = Utils::Energy(target_, current, new_shape);

      if (new_energy < best_energy) {
        best_shape = new_shape;
        best_energy = new_energy;
      }
    }
    return best_shape;
  }

};