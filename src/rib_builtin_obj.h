//TODO: add license

#ifndef RIB_BUILTIN_T_H
#define RIB_BUILTIN_T_H 

#include <inttypes.h>
#include <stdbool.h>
#include "rib.h" 
#include "rib_obj.h" 

/**
* @file rib_builtin_obj.h
* @author Marc Sune<marc.sune (at) bisdn.de>
*
* @brief Resource Information Base(RIB) built-in types
*/

//TODO XXX this is very drafty

/**
* Available built-in types class_names.
*
* This enumeration is to be used internally in the library.
* These class names are reserved and cannot be used or redefined by the users
* of the library
*/
typedef enum __rib_obj_builtin_types{
	//This object type shall NEVER be used
	RIB_OBJ_INVALID_T 	= -1,

	//Unsigned integers
	RIB_OBJ_UINT8_T 	= 0,
	RIB_OBJ_UINT16_T 	= 1,
	RIB_OBJ_UINT32_T 	= 2,
	RIB_OBJ_UINT64_T 	= 3,
	
	//Signed integers
	RIB_OBJ_INT8_T	 	= 4,
	RIB_OBJ_INT16_T 	= 5,
	RIB_OBJ_INT32_T 	= 6,
	RIB_OBJ_INT64_T 	= 7,
	
	//Float
	RIB_OBJ_FLOAT_T 	= 8,
	
	//Double
	RIB_OBJ_DOUBLE_T 	= 9,
	
	//String
	RIB_OBJ_STRING_T 	= 10,

	//Buffer
	RIB_OBJ_BUF_T		= 11	
}__rib_obj_builtin_types_t;

//extern C 
RIB_END_DECLS

#endif // RIB_BUILTIN_T_H_ 
