#pragma once

#if defined(_WIN64)||defined(_WIN32)
	#ifdef NOA_BUILD_DLL
		#define NOA_API __declspec(dllexport)
	#else
		#define NOA_API __declspec(dllimport)
	#endif // !NOA_BUILD_DLL
#endif

