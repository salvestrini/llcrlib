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

//TODO static initializer


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




//extern C 
END_DECLS

#endif // RIB_REMOTE_H_ 
