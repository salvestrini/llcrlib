//TODO: add license

#ifndef RIB_H
#define RIB_H 

#include <inttypes.h>
#include <stdbool.h>
#include "../utils.h"

/**
* @file rib.h
* @author Marc Sune<marc.sune (at) bisdn.de>
*
* @brief Resource Information Base common structures
*/


//
// Data types
//
//TODO: remove this when this type is defined
typedef uint64_t cdap_ae_handle_t;

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

	//
	// Negocation
	//

	/* There is no supported RIB version that can be understood */	
	RIB_VER_MISMATCH_ERR = -2,

	//
	// Schema
	//

	/* The RIB schema file extension is unknown */
	RIB_SCHEMA_EXT_ERR = -3,
	
	/* Error during RIB scheema file parsing */
	RIB_SCHEMA_FORMAT_ERR = -4,
	
	/* General validation error (unknown) */ 
	RIB_SCHEMA_VALIDATION_ERR = -5,
	
	/* Validation error, missing mandatory object */
	RIB_SCHEMA_VAL_MAN_ERR = -6,

	//
	// Invoke IDs
	//
	
	/* Unkown Invoke ID */ 
	RIB_INVALID_INVOKE_ID_ERR = -7,

	/* Operation associated with the invoke ID has concluded */ 
	RIB_EXPIRED_INVOKE_ID_ERR = -8,

	//
	// Misc
	//	
	//TODO: Other error codes
}rib_res_t;


/**
* @brief Get the error description
*
* TODO: 
*/
const char* rib_strerror(rib_res_t code); 

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

        uint8_t reserved; /* whatever, just le the thing compile */
}
rib_ver_t;
/* COMPILATION_ASSERT(RIB_VER_SIZE, ( sizeof(rib_ver_t) == 8 ) ); */


/**
* Supported RIB versions set 
*/
typedef struct rib_ver_set{
	/* Number of versions */
	unsigned int num_of_supported_vers;
	
#define RIB_VER_BM_MAX_SUPPORTED 16
	/* Supported */
	rib_ver_t __supported[RIB_VER_BM_MAX_SUPPORTED];
}rib_ver_set_t;
/* COMPILATION_ASSERT(RIB_VER_BM_SIZE, ( sizeof(rib_ver_set_t) == 132 ) ); */


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
* @brief Local RIB handle 
*/
typedef uint64_t rib_handle_t;

/**
* @brief RIB handle ANY
*/
extern rib_handle_t rib_handle_all;

//
// Functions
//

//extern C
BEGIN_DECLS

//
// Version set
//

/**
* Initialize supported RIB versions set
*/
void rib_ver_set_init(rib_ver_set_t* set);

/**
* Add version to set 
*/
void rib_ver_set_add(rib_ver_set_t* set, const rib_ver_t* version); 

/**
* Remove version from set 
*/
void rib_ver_set_remove(rib_ver_set_t* set, const rib_ver_t* version); 

/**
* Is the version supported
*/
bool rib_ver_set_is_supported(const rib_ver_set_t* set, 
					const rib_ver_t* version); 


//extern C 
END_DECLS

#endif // RIB_H_ 
