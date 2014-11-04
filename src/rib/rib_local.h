//TODO: add license

#ifndef RIB_LOCAL_H
#define RIB_LOCAL_H 

#include <inttypes.h>
#include <stdbool.h>

#include <rib/rib.h>
#include <rib/rib_schema.h>
#include <rib/rib_obj.h>

/**
* @file rib_local.h
* @author Marc Sune<marc.sune (at) bisdn.de>
*
* @brief Local Resource Information Base(RIB) management
*/

/**
* @brief RIB 
*/
typedef struct rib{
	/* Handle for this RIB */
	rib_handle_t handle;

#define RIB_MAX_NAME 32
	/* RIB name (human readable) */
	char name[RIB_MAX_NAME];

#define RIB_MAX_AES 128
	/* Application entities exposing this RIB */ 
	cdap_ae_handle_t aes[RIB_MAX_AES];

	/* RIB version and schema */
	rib_schema_t schema;

	//TODO: hash table for obj names
	//TODO: hash table for obj class
}rib_t;
//COMPILATION_ASSERT(RIB_SIZE, ( sizeof(rib_ver_t) == 8 ) );


//
// Functions
//

//extern C
BEGIN_DECLS

//
// RIB mgmt 
//

/**
* @brief Create a RIB 
*
* Create an empty RIB.
*
* @param version RIB version
* @param handle On success the handle will be stored here
*/
rib_res_t rib_create(const rib_ver_t version, rib_handle_t* handle);


/**
* @brief Destroy a RIB 
*
* @param version RIB version
* @param handle On success the handle will be stored here
*/
rib_res_t rib_destroy(const rib_handle_t handle);


//
// RIB object mgmt 
//

/**
* @brief Register a callback to capture remote object creation
*
* Registers a callback associated with a class_name (type) in the RIB, to 
* capture object CREATE events.
*
* @param handle RIB handle
* @param full_name RIB path where objects of type 'class_name' can be created
* @param class_name Class name of the objects
* @param max_objs Maximum number of objects of type 'class_name' that can be 
*  created per tree level 
* @param cb Callback for remote CREATE operations of type 'type' in the path 
*           and depth specified
*/
rib_res_t rib_reg_type_creation_cb(const rib_handle_t handle, 
				const char* path, 
				const unsigned int depth,
				const char* class_name,
				const rib_obj_type_create_callback_t cb);

/**
* @brief Add object to a RIB
*
* @param handle RIB handle
* @param full_name RIB full_name where the object shall be created
* @param obj Ref to a reference of the object to be added. On success will be
*  set to NULL
* @param obj_id If non-NULL, on success the object Id will be stored here
*/
rib_res_t rib_add_obj(const rib_handle_t handle, const char* full_name, 
					rib_obj_t** obj,
					uint64_t* inst_id);

/**
* @brief Remove object from a RIB
*
* @param handle RIB handle
* @param full_name RIB full_name where object to be removed is 
* @param class_name Object's class name (type)
*/
rib_res_t rib_rem_obj_by_name(const rib_handle_t handle, 
						const char* full_name,
						const char* class_name); 

/**
* @brief Remove object from a RIB
*
* @param handle RIB handle
* @param inst_id RIB object instance Id to be removed
*/
rib_res_t rib_rem_obj_by_inst(const rib_handle_t handle, 
						const uint64_t  inst_id); 

/**
* Internal API: shall not be used
* //TODO move to an inner header
*/
rib_res_t __rib_rem_obj_by_inst(const rib_handle_t handle, 
						const rib_obj_t** obj); 

//
// Asynchronous operations
//


/**
* @brief Complete a previously derefered operation 
*
* @param handle RIB handle
* @param inst_id Invoke Id which was passed during operation callback
* @param result Result of the operation. Can only be SUCCESS or FAILURE
* @param op_payload On SUCCESS the op_payload should contain the encoded
*  operation payload
*/
void rib_complete_operation(const rib_handle_t handle, uint32_t invoke_id,
					rib_res_t result, 
					const rib_op_payload_t* op_payload);

//
// Other
//

/**
* @brief Set interceptor module for the RIB.
*
* Set a software module that will intercept the RIB remote operations before
* any callback is sent. The interceptor module must conform the API.
*
* @param handle RIB handle or rib_handle_all to intercept ALL RIBs. 
* @param interceptor Set or NULL to  
*/
void rib_set_interceptor(const rib_handle_t handle,
				/*TODO*/const rib_interceptor_t* interceptor);

/**
* Inner API: shall not be used
* //TODO move to an inner header
*/
rib_obj_t* __rib_get_obj(const rib_handle_t handle, 
						const char* full_name,
						const char* class_name); 



//extern C 
END_DECLS

#endif // RIB_LOCAL_H_ 
