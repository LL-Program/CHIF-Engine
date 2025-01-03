#ifndef CHIF_UNORDERED_SET_REPLACEMENT
#define CHIF_UNORDERED_SET_REPLACEMENT
// This file is used to allow replacement of std::unordered_set

#ifndef CHIF_UNORDERED_SET_TYPE
#define CHIF_UNORDERED_SET_TYPE 1
#endif // CHIF_UNORDERED_SET_TYPE

#if CHIF_UNORDERED_SET_TYPE == 1
#include "Utility/flat_hash_map.hpp"
#else
#include <unordered_set>
#endif // CHIF_UNORDERED_SET_TYPE

namespace chif
{
	template<typename T>
#if CHIF_UNORDERED_SET_TYPE == 1
	using unordered_set = ska::flat_hash_set<T>;
#else
	using unordered_set = std::unordered_set<T>;
#endif // CHIF_UNORDERED_SET_TYPE
}

#endif // CHIF_UNORDERED_SET_REPLACEMENT
