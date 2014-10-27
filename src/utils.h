//TODO: add license

#ifndef UTILS_H
#define UTILS_H 

/**
* @file utils.h
* @author Marc Sune<marc.sune (at) bisdn.de>
*
* @brief Utilities
*/

/*
* Compilation assert
*/
#define COMPILATION_ASSERT(tag,cond) \
	enum { COMPILATION_ASSERT__ ## tag = 1/(cond) }

/*
* Extern C wrappings
*/
#ifdef __cplusplus
	# define BEGIN_DECLS extern "C" {
	# define END_DECLS   }
#else
	# define BEGIN_DECLS
	# define END_DECLS
#endif //__cplusplus

#endif // UTILS_H_ 
