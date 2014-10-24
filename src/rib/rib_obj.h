//TODO: add license

#ifndef RIB_OBJ_H
#define RIB_OBJ_H 

#include <inttypes.h>
#include <stdbool.h>
#include <sys/uio.h>
#include "rib.h" 

/**
* @file rib_obj.h
* @author Marc Sune<marc.sune (at) bisdn.de>
*
* @brief Resource Information Base(RIB) object abstraction  
*/


//
// Data types
//

//fwd decl
struct rib_obj;

/**
* RIB(CDAP) operation payload 
*
* This encapsulates the raw buffer(buf_ptr and len) of a CDAP operation. This
* payload is rawly obtained from the CDAP message, and needs to be decoded
* properly by the encoder defined by the concrete syntax.
*/
typedef struct iovec rib_op_payload_t;

/**
* @brief RIB object type create callback
*
* @ret An object of type rib_obj_t or NULL if the object shall not be created
*/
typedef struct rib_obj* 
	(*rib_obj_type_create_callback_t)(rib_handle_t handle, 
					const char* obj_name, 
					const char* class_name, 
					const uint32_t invoke_id,
					const rib_op_payload_t* op_payload);
				      
/**
* @brief RIB object operation callbacks 
*/
typedef struct rib_obj_ops{
	/* 
	* @brief Delete callback 
	*
	* Will be called during CDAP RIB_OP_DELETE operation over the object.
	* The op_payload contains the raw (buffer,len) payload of the 
	* operation. This payload is strictly specific to the object type as
	* well as the concrete syntax used
	*
	* The callback must NOT be blocking. If the operation cannot be 
	* immediately completed, RIB_IN_PROGRESS should be returned and 
	* rib_operation_completed() with the appropriate invoke_id. 
	*
	* @param op_payload operation payload 
	* @ret Shall return RIB_SUCCESS/RIB_FAILURE or RIB_IN_PROGRESS when
	*  the operation cannot be immediately fulfilled. 
	*/
	rib_res_t (*del)(rib_handle_t handle, struct rib_obj* obj, 
					uint32_t invoke_id,
					const rib_op_payload_t* op_payload);
	/* 
	* @brief Read callback 
	*
	* Will be called during CDAP RIB_OP_READ operation over the object.
	* On success, the op_payload raw (buffer,len) operation payload should
	* be filled in with the serialized
	*
	* The callback must NOT be blocking. If the operation cannot be 
	* immediately completed, RIB_IN_PROGRESS should be returned and 
	* rib_operation_completed() with the appropriate invoke_id. 
	*
	* @param op_payload pre-allocated serialization buffer
	* @ret Shall return RIB_SUCCESS/RIB_FAILURE or RIB_IN_PROGRESS when
	*  the operation cannot be immediately fulfilled. 
	*/
	rib_res_t (*read)(rib_handle_t handle, struct rib_obj* obj,
					uint32_t invoke_id,
					const rib_op_payload_t* op_payload);
	/* 
	* @brief write callback 
	*
	* Will be called during CDAP RIB_OP_WRITE operation over the object.
	* The op_payload contains the raw (buffer,len) payload of the 
	* operation. This payload is strictly specific to the object type as
	* well as the concrete syntax used
	*
	* The callback must NOT be blocking. If the operation cannot be 
	* immediately completed, RIB_IN_PROGRESS should be returned and 
	* rib_operation_completed() with the appropriate invoke_id. 
	*
	* @param op_payload operation payload
 	* @ret Shall return RIB_SUCCESS/RIB_FAILURE or RIB_IN_PROGRESS when
	*  the operation cannot be immediately fulfilled. 
	*/
	rib_res_t (*write)(rib_handle_t handle, struct rib_obj* obj,
					uint32_t invoke_id,
					const rib_op_payload_t* op_payload);

	/* 
	* @brief start callback 
	*
	* Will be called during CDAP RIB_OP_START operation over the object.
	* The op_payload contains the raw (buffer,len) payload of the 
	* operation. This payload is strictly specific to the object type as
	* well as the concrete syntax used.
	*
	* The callback must NOT be blocking. If the operation cannot be 
	* immediately completed, RIB_IN_PROGRESS should be returned and 
	* rib_operation_completed() with the appropriate invoke_id. 
	*
	* @param op_payload operation payload 
	* @ret Shall return RIB_SUCCESS/RIB_FAILURE or RIB_IN_PROGRESS when
	*  the operation cannot be immediately fulfilled. 
	*/
	rib_res_t (*start)(rib_handle_t handle, struct rib_obj* obj,
					uint32_t invoke_id,
					const rib_op_payload_t* op_payload);

	/* 
	* @brief stop callback 
	*
	* Will be called during CDAP RIB_OP_STOP operation over the object.
	* The op_payload contains the raw (buffer,len) payload of the 
	* operation. This payload is strictly specific to the object type as
	* well as the concrete syntax used.
	*
	* The callback must NOT be blocking. If the operation cannot be 
	* immediately completed, RIB_IN_PROGRESS should be returned and 
	* rib_operation_completed() with the appropriate invoke_id. 
	*
	* @param op_payload operation payload 
	* @ret Shall return RIB_SUCCESS/RIB_FAILURE or RIB_IN_PROGRESS when
	*  the operation cannot be immediately fulfilled. 
	*/
	rib_res_t (*stop)(rib_handle_t handle, struct rib_obj* obj,
					uint32_t invoke_id,
					const rib_op_payload_t* op_payload);
}rib_obj_ops_t;

/**
* @brief RIB object handlers 
*/
typedef struct rib_obj{

	// CDAP object properties

	/* Instance id */
	uint64_t inst_id;

	/* Name */
	char* name;
	
	/* Class type */
	char class_name[RIB_MAX_CLASS_NAME];	

	/* Operations */
	rib_obj_ops_t ops;

	// Application specific data

	/* 
	* Opaque user data 
	* Useid to store the value
	*/
	void* data;
}rib_obj_t;

//
// Functions
//

//extern C
RIB_BEGIN_DECLS

//
// RIB objects
//

/**
* @brief Get object's instance ID
*
* @param handle RIB handle
* @param full_name RIB path where objects of type 'class_name' can be created
* @param class_name Class name of the objects
*
*/
uint64_t rib_obj_get_inst_id(rib_handle_t handle,
						const char* full_name,
						const char* class_name);


//extern C 
RIB_END_DECLS

#endif // RIB_OBJ_H_ 
