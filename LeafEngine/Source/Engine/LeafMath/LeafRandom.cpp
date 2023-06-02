#include "LeafRandom.h"
#include <random>

namespace LeafMath
{

  template<typename T>
  T RandomNumber(T min, T max)
  {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<T> dis(min, max);
    return dis(gen);
  }

} // LeafMath
