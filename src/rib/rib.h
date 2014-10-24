//TODO: add license

#ifndef RIB_H
#define RIB_H 

#include <inttypes.h>
#include <stdbool.h>
#include "utils.h"

/**
* @file rib.h
* @author Marc Sune<marc.sune (at) bisdn.de>
*
* @brief Resource Information Base common structures
*/


//
// Data types
//


/**
* Maximum object class name
*/
#define RIB_MAX_CLASS_NAME 32

/**
* RIB library result codes
*/
typedef enum rib_res{

	/* General code for success */
	RIB_SUCCESS  = 0,
	
	/* Operation in progress (async) */
	RIB_IN_PROGRESS = 1,

	/* General error */
	RIB_UNKOWN_ERROR = -1,
	
	//TODO: Other error codes
}rib_res_t;

/**
* @brief RIB version
*/
typedef struct rib_ver{
	/* RIB type */
	uint32_t model;

	/* Version */
	uint16_t major_version;
	uint8_t minor_version;

	/* Object value encoder */
	uint8_t encoding;
	
}rib_ver_t;
COMPILATION_ASSERT(RIB_VER_SIZE, ( sizeof(rib_ver_t) == 8 ) );


/**
* Supported RIB versions bitmap
*/
typedef struct rib_ver_bitmap{
	/* Number of versions */
	unsigned int num_of_supported_vers;
	
#define RIB_VER_BM_MAX_SUPPORTED 16
	/* Supported */
	rib_ver_t __supported[RIB_VER_BM_MAX_SUPPORTED];
}rib_ver_bitmap_t;
COMPILATION_ASSERT(RIB_VER_BM_SIZE, ( sizeof(rib_ver_bitmap_t) == 132 ) );


/**
* CDPA/RIB object operations
*/
typedef enum rib_op{
	RIB_OP_CREATE 	= 0,
	RIB_OP_DELETE	= 1,
	RIB_OP_READ 	= 2,
	RIB_OP_WRITE 	= 3,
	RIB_OP_START 	= 4,
	RIB_OP_STOP 	= 5,
}rib_op_t;

/**
* @brief RIB handle 
*/
typedef uint64_t rib_handle_t;

//
// Functions
//

//extern C
RIB_BEGIN_DECLS

//
// Version bitmap
//

/**
* Initialize supported RIB versions bitmap
*/
void rib_ver_bitmap_init(rib_ver_bitmap_t* bitmap);

/**
* Add version to bitmap 
*/
void rib_ver_bitmap_add(rib_ver_bitmap_t* bitmap, const rib_ver_t* version); 

/**
* Remove version from bitmap 
*/
void rib_ver_bitmap_remove(rib_ver_bitmap_t* bitmap, const rib_ver_t* version); 

/**
* Is the version supported
*/
bool rib_ver_bitmap_is_supported(const rib_ver_bitmap_t* bitmap, 
					const rib_ver_t* version); 


//extern C 
RIB_END_DECLS

#endif // RIB_H_ 
