#pragma once

#include "HD_String.h"

#include <functional>
#include <string>

// For now just forwarding to std::hash.
	// * Future project: implement a custom hash function.

template<typename T>
size_t HD_Hash(const T& aDataToHash)
{
	return std::hash<T>{}(aDataToHash);
}

// Forwarding a hash of HD_String to std::string is not ideal
// * Every time a HD_String is hashed then a std::string
//		object is created which triggers a heap allocation.
//		However, at the moment only HD_ExeArgs is making use
//		of hashing HD_String:s, and that whole class is only
//		defined in debug so I'm not too worried about it. In
//		release/retail hashing strings should likely not happen.
template<>
struct std::hash<HD_String>
{
	size_t operator()(const HD_String& aString) const
	{
		return std::hash<std::string>()(aString.GetBuffer());
	}
};
