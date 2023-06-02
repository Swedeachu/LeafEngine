#include "LeafRandom.h"
#include <random>

namespace LeafMath
{

	template<typename T>
	T RandomNumber(T min, T max)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		// determine what type of randomness to do per each numeric data type
		if constexpr (std::is_integral<T>::value)
		{
			std::uniform_int_distribution<T> dis(min, max);
			return dis(gen);
		}
		else if constexpr (std::is_floating_point<T>::value)
		{
			std::uniform_real_distribution<T> dis(min, max);
			return dis(gen);
		}
	}

	// Explicit instantiation for float, double, and int
	template float LeafMath::RandomNumber<float>(float, float);
	template double LeafMath::RandomNumber<double>(double, double);
	template int LeafMath::RandomNumber<int>(int, int);

} // LeafMath
