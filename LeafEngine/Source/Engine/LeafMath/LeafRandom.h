#ifndef LEAF_RANDOM_H
#define LEAF_RANDOM_H

namespace LeafMath
{

	/*
	Example Use:
		float randomFloat = RandomNumber<float>(0.0f, 1.0f);
		double randomDouble = RandomNumber<double>(0.0, 1.0);
		int randomInt = RandomNumber<int>(0, 10);
	*/
	template<typename T>
	T RandomNumber(T min, T max);

} // LeafMath

#endif // LEAF_RANDOM_H
