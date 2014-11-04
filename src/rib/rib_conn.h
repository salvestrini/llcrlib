//TODO: add license

#ifndef RIB_CONN_H
#define RIB_CONN_H 

#include <inttypes.h>
#include <stdbool.h>
#include "utils.h"

/**
* @file rib_conn.h
* @author Marc Sune<marc.sune (at) bisdn.de>
*
* @brief RIB connections
*/


//
// Data types
//

/**
* Remote connection flags
*/
typedef enum rib_rcon_flags{
	/**
	* Non blocking open()
	*/	
	RIB_RCON_NONBLOCK_OPEN,
	
	/**
	* Non blocking close()
	*/	
	RIB_RCON_NONBLOCK_CLOSE,
	
	/**
	* CDAP connection esablished flag
	*/
	RIB_RCON_ESTABLISHED,
}rib_rcon_flags_t;

typedef struct { int empty_for_the_moment; } cdap_rcon_handle_t;

/**
* @brief Remote RIB handle 
*/
typedef struct rib_rcon_handle{
	/**
	* RIB handle ID
	*/
	uint64_t __id;
	
	/**
	* Connection flags
	*/
	uint32_t flags;

	/**
	* CDAP lib remote connection handle 
	*/
	cdap_rcon_handle_t* __cdap;
}rib_rcon_handle_t;


//
// Functions
//

//extern C
BEGIN_DECLS

/**
* @brief Set callback non-blocking open callback completed 
*
* Can only be used with RIB_RCON_NONBLOCK_OPEN
* TODO
*/
rib_res_t rib_remote_set_open_cb(const rib_rcon_handle_t* handle /* TODO */);

/**
* @brief Set callback for connection close 
*
* TODO
*/
rib_res_t rib_remote_set_close_cb(const rib_rcon_handle_t* handle /* TODO */);

//extern C 
END_DECLS

#endif // RIB_CONN_H_ 
