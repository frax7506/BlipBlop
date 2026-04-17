#pragma once

template<typename T>
struct HD_IsFundamental { enum { Result = false, }; };

template<>
struct HD_IsFundamental<char> { enum { Result = true, }; };

template<>
struct HD_IsFundamental<unsigned char> { enum { Result = true, }; };

template<>
struct HD_IsFundamental<short> { enum { Result = true, }; };

template<>
struct HD_IsFundamental<unsigned short> { enum { Result = true, }; };

template<>
struct HD_IsFundamental<int> { enum { Result = true, }; };

template<>
struct HD_IsFundamental<unsigned int> { enum { Result = true, }; };

template<>
struct HD_IsFundamental<long long> { enum { Result = true, }; };

template<>
struct HD_IsFundamental<unsigned long long> { enum { Result = true, }; };

template<>
struct HD_IsFundamental<float> { enum { Result = true, }; };

template<>
struct HD_IsFundamental<double> { enum { Result = true, }; };
