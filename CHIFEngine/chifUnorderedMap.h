#ifndef CHIF_UNORDERED_MAP_REPLACEMENT
#define CHIF_UNORDERED_MAP_REPLACEMENT
// This file is used to allow replacement of std::unordered_map

#ifndef CHIF_UNORDERED_MAP_TYPE
#define CHIF_UNORDERED_MAP_TYPE 1
#endif // CHIF_UNORDERED_MAP_TYPE

#if CHIF_UNORDERED_MAP_TYPE == 1
#include "Utility/flat_hash_map.hpp"
#elif CHIF_UNORDERED_MAP_TYPE == 2
#include "Utility/robin_hood.h"
#else
#include <unordered_map>
#endif // CHIF_UNORDERED_MAP_TYPE

namespace chif
{
	template<typename K, typename V, typename H = std::hash<K>, typename E = std::equal_to<K>, typename A = std::allocator<std::pair<const K, V> > >
#if CHIF_UNORDERED_MAP_TYPE == 1
	using unordered_map = ska::flat_hash_map<K, V, H, E, A>;
#elif CHIF_UNORDERED_MAP_TYPE == 2
	using unordered_map = robin_hood::unordered_flat_map<K, V, H, E>;
#else
	using unordered_map = std::unordered_map<K, V, H, E, A>;
#endif // CHIF_UNORDERED_MAP_TYPE
}

#endif // CHIF_UNORDERED_MAP_REPLACEMENT
