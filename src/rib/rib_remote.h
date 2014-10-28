//TODO: add license

#ifndef RIB_REMOTE_H
#define RIB_REMOTE_H 

#include <inttypes.h>
#include <stdbool.h>
#include "rib.h"
#include "rib_conn.h"

/**
* @file rib_remote.h
* @author Marc Sune<marc.sune (at) bisdn.de>
*
* @brief Remote Resource Information Base operations
*/



//
// Functions
//

//extern C
BEGIN_DECLS

//
// Connection handler 
//


/**
* TODO authentication; 
*/

//
// Connection management
//

/**
* @brief Open a RIB/CDAP connection to a remote peer AP 
*
* ill use defaults values, attempt to guess
* the AE and attempt to negociate the CDAP stablishment of the connection.
*
* For more refined API use rib_remote_open(); 
*
* @param handle A pointer to an already initialized handle 
* @param apname AP name
* @param apinst AP instance ID. Pass NULL if not specified. 
* @param ae_name An AE name to be targeted or NULL.
* @param ae_inst An AE inst be targeted or NULL.
* @param versions A mandatory set of supported versions
* @param flags A bitmap with enum rib_rcon_flags values 
*
* @ret If the flags set the opening to be non-blocking, RIB_IN_PROGRESS will be
*  returned, otherwise the RIB_SUCCESS or error code
*/
rib_rcon_handle_t* rib_remote_open(uint64_t flow_id,
					const char* ap_name,
					const char* ap_inst,
					const char* ae_name,
					const char* ae_inst,
					const rib_ver_set_t* versions, 
					/* TODO cdap auth?*/
					const uint32_t flags);

/**
* @brief Simplified open a RIB/CDAP connection to a remote peer AP
*
* This is a synchronous call. It will:
* 1) Allocate a flow that supports the communication with ap_name
* 2) Call rib_remote_open with default parameters. If no AE name is defined
*    it will be guessed.
* 3) Return the result
*
* For more powerful API use rib_remote_open(); 
*
* @param apname AP name
* @param aename AE name or NULL
* @param supported Bitmap with the supported RIB versions
* @param versions A mandatory set of supported versions
*
* @ret On success a valid, ready to use rib_rcon_handle_t is return or NULL
*  otherwise
*/
rib_rcon_handle_t* rib_remote_open_simple(const char* ap_name,
					const char* ae_name,
					const rib_ver_set_t* versions); 

/**
* @brief Simplified open connection from a prexisting flow
*
* TODO
*
*/
rib_rcon_handle_t* rib_remote_open_from_flow(/* TODO*/);


/**
* @brief Close an existing RIB remote connection 
*
* @param handle Remote connection handle  
*
* @ret If the connection is marked to be non-blocking on close() 
* RIB_IN_PROGRESS will be returned, otherwise the appropriate RIB_SUCCESSS 
* or error code.
*/
rib_res_t rib_remote_close(rib_rcon_handle_t** handle);

//
// Remote RIB operations
//

//TODO: scope

/**
* @brief Perform a READ operation over an object of the RIB 
*
* @param handle Remote connection handle  
* @param invoke_id On success, the invoke ID will be stored here
* @param full_name Full name (path) Instance ID used when 'full_name' is NULL.
* @param class_name Class name of the object 
* @param inst_id Instance ID used when 'full_name' is NULL.
* @param obj An empty object of class 'class_name'. On success, the value
*  will be filled in this object
*
* @ret RIB_SUCCESSS or error code.
*/
rib_res_t rib_remote_read(const rib_rcon_handle_t* handle, 
					const char* full_name,
					uint64_t* inst_id,
					const char* class_name,
					/*const uint32_t scope,*/
					rib_obj_t* obj,	
					uint64_t* invoke_id);

/**
* @brief Perform a WRITE operation over an object of the RIB 
*
* TODO
*/
rib_res_t rib_remote_write(/*TODO*/);


/**
* @brief Perform a START operation over an object of the RIB 
*
* TODO
*/
rib_res_t rib_remote_start(/*TODO*/);

/**
* @brief Perform a STOP operation over an object of the RIB 
*
* TODO
*/
rib_res_t rib_remote_stop(/*TODO*/);

/**
* @brief Perform a CREATE operation over an object of the RIB 
*
* TODO
*/
rib_res_t rib_remote_create(/*TODO*/);

/**
* @brief Perform a DELETE operation over an object of the RIB 
*
* TODO
*/
rib_res_t rib_remote_delete(/*TODO*/);

//
// Pending operations mgmt
//

/**
* @brief Retrieves the current status of an operation 
*
* This call is strictly NON blocking. 
*
* @param handle Remote connection handle  
* @param invoke_id Invoke ID 
*
* @ret RIB_IN_PROGRESS if still not completed, RIB_SUCCESS or error code 
*   otherwise.
*/
rib_res_t rib_remote_op_status(const rib_rcon_handle_t* handle, 
						const uint64_t invoke_id);


/**
* @brief Waits until an operation has conclued
*
* This call is blocking. 
*
* @param handle Remote connection handle  
* @param invoke_id Invoke ID 
*
* @ret RIB_SUCCESSS or error code.
*/
rib_res_t rib_remote_op_wait(const rib_rcon_handle_t* handle, 
						const uint64_t invoke_id);

//TODO: set callback

/**
* @brief Cancels a previously issued operation 
*
* @param handle Remote connection handle  
* @param invoke_id Invoke ID 
*
* @ret RIB_SUCCESSS or error code.
*/
rib_res_t rib_remote_op_cancel(const rib_rcon_handle_t* handle, 
						const uint64_t invoke_id);



//extern C 
END_DECLS

#endif // RIB_REMOTE_H_ 
