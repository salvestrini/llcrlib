#include <rib/rib.h>
#include <rib/rib_local.h>
#include <rib/rib_remote.h>
#include <rib/rib_builtin_obj.h>

//RIB version info
#define RIB_MODEL 0x11
#define RIB_MAJ_VERSION 0x1
#define RIB_MIN_VERSION 0x0
#define RIB_ENCODING 0xAA


//Custom application data structure 
typedef struct my_struct{
	uint32_t field1;
	uint16_t field2;
	uint16_t another_field;
}my_struct_t;

#define CUSTOM_TYPE_CN "custom_cn"

//Static variable for the sake of the example
static int32_t var1 = 1;
static my_struct_t custom_data;

//Custom WRITE for int32_t types
static rib_res_t my_fancy_write(/* TODO*/){

	assert(strncmp(obj->class_name, RIB_OBJ_INT32_CN, 
				RIB_MAX_CLASS_NAME) == 0);

	//Do my fancy stuff
	//...
	
	//Set the value and return
	rib_obj_int32_set(&obj, (int32_t*)data);

	return RIB_SUCCESS;
} 

//My custom type creation callback
struct rib_obj*
my_remote_obj_creation_cb(rib_handle_t handle, const char* obj_name, 
					const char* class_name, 
					const uint32_t invoke_id,
					const rib_op_payload_t* op_payload){

		
	assert(strncmp(obj->class_name, CUSTOM_TYPE_CN, 
				RIB_MAX_CLASS_NAME) == 0);

	//Create a custom object
	rib_obj_t* obj = rib_obj_init("custom_obj", CUSTOM_TYPE_CN, &data);
	obj->ops.start = my_custom_start;
	obj->ops.delete = my_custom_delete;

	//etc...

	return obj;	
}

/*
*
*
*/

static void setup_aes(){

	/**
	* TODO CDAP
	*/
	
	//TODO..
	
	/**
	* RIB code
	*/

//
// Create RIB schema (version)
//
	rib_ver_t version;
	version.model = RIB_MODEL;
	version.major_version = RIB_MAJ_VERSION;
	version.minor_version = RIB_MIN_VERSION;
	version.encoding = RIB_ENCODING;
	version.reserved = 0x0;

	// Now create an empty schema	
	if( rib_schema_create(version) != RIB_SUCCESS )
		exit(-1);	
	//
	// Now register the different types to the schema
	//
	
	// int32_t in the root and with a fixed name (obj1)
	assert(rib_schema_def_type(version, RIB_INT32_T) == RIB_SUCCESS);
	assert(rib_schema_def_type_cont_rel(version, RIB_ROOT_CN, RIB_ROOT
						RIB_INT32_T,	
						"obj1",
						false,
						1) == RIB_SUCCESS);

	// int32_t pointer type (more details beloew)
	// in the root and with a fixed name (obj2)	
	assert(rib_schema_def_type(version, RIB_INT32P_T) == RIB_SUCCESS);
	assert(rib_schema_def_type_cont_rel(version, RIB_ROOT_CN, RIB_ROOT,
						RIB_INT32P_T,	
						"obj2",
						false,
						1) == RIB_SUCCESS);
	
	//int32p_t in the root and with a fixed name (obj3)
	//Note, the type has been already register once, there is no need to
	//re-register
	assert(rib_schema_def_type_cont_rel(version, RIB_ROOT_CN, RIB_ROOT, 
							RIB_INT32P_T,	
							"obj3",
							false,
							1) == RIB_SUCCESS);
	
	// Create a custom type in the root of the tree
	// The type will be called "custom_cn" 
	// in the root and with a fixed name (custom_obj)	
	assert(rib_schema_def_type(version, RIB_INT32P_T) == RIB_SUCCESS);
	assert(rib_schema_def_type_cont_rel(version, RIB_ROOT_CN, RIB_ROOT,
						CUSTOM_TYPE_CN,	
						"custom_obj",
						false,
						1) == RIB_SUCCESS);

	// Register a relation of contention
	// custom_obj must have an inner object called inner that is an int32_t
	assert(rib_schema_def_type_cont_rel(version, CUSTOM_TYPE_CN,
						"custom_obj",
						RIB_INT32_T,	
						"inner",
						true,
						1) == RIB_SUCCESS);

	// Finally register an undefined number of objects that can be created
	// in the root of the tree with whatever name (except the ones used by
	// existing defined types).
	//
	// Note that usually one may want to define this type of "unlimited"
	// number of objects within a container type.
	assert(rib_schema_def_type(version, RIB_INT8_T) == RIB_SUCCESS);
	assert(rib_schema_def_type_cont_rel(version, RIB_ROOT_CN, RIB_ROOT,
						RIB_INT8_T,	
						NULL, //No name
						false,
						RIB_OBJ_COUNT_UNLIMITED) 
							== RIB_SUCCESS);



	// All this could be eventually substituted by simply doing something 
	// like:
	//rib_schema_import(my_app_rib_schema.json, version);

//
// Create RIB based on this schema
//	
	//Create a RIB
	rib_handle_t handle;
	if( rib_create(version, &handle) != RIB_SUCCESS )
		exit(-1);	

//
// Populate initial objects in the RIB instance and register CREATE 
// callbacks
//


/*
* Status of the RIB instance tree:
*
* root 
*
*/
	//Add a simple int32 object
	//The actual value resides in the RIB library
	rib_obj_t* obj1 = rib_obj_int32_init("obj1");
	
	//Note: we can recover instance id via rib_obj_get_inst_id() 
	if( rib_add_obj(&handle, &obj1) != RIB_SUCCESS )
		exit(-1);	
	assert(obj1 == NULL);

	/*
	* Status of the RIB instance tree:
	*
	* root 
	* └── obj1
	*/
	
	//Add a simple int32* object
	//The value is outside of the RIB library, and can be shared
	//across RIB objects of different RIBs
	//
	rib_obj_t* obj2 = rib_obj_int32p_init("obj2", &var1);
	if( rib_add_obj(&handle, &obj2) != RIB_SUCCESS )
		exit(-1);
	assert(obj2 == NULL);
	
	/*
	* Status of the RIB instance tree:
	*
	* root 
	* ├── obj1
	* └── obj2
	*/


	//TODO: locked types are necessary, specially for structs and any type
	//larger than the CPU register length, for instace for atomic WRITEs or
	//to acquire a lock before calling the operation HOOK

	//Add a simple type but intercept the WRITE
	rib_obj_t* obj3 = rib_obj_int32p_init("obj3", &var1);
	obj3->ops.write = my_fancy_write;
	if( rib_add_obj(&handle, &obj3) != RIB_SUCCESS )
		exit(-1);
	assert(obj3 == NULL);

	/*
	* Status of the RIB instance tree:
	*
	* root 
	* ├── obj1
	* ├── obj2
	* └── obj3
	*/

	//
	// Add a completely custom type(using raw rib_obj_init())
	// The data type will be a struct. 
	//
	memset(&custom_data, 0, sizeof(custom_data));
	rib_obj_t* obj_c = rib_obj_init("custom_obj", CUSTOM_TYPE_CN, &data);
	
	// Set the object operations we want to support
	// Note that we could specify a common hook for all "custom_type"
	// objects, or specify a per-object hook. This is up to the application
	obj_c->ops.start = my_custom_start;
	obj_c->ops.delete = my_custom_delete;

	//Now add to the RIB	
	if( rib_add_obj(&rib_handle, &obj_c) != RIB_SUCCESS )
		exit(-1);
	assert(obj_c == NULL);

	/*
	* Status of the RIB instance tree:
	*
	* root 
	* ├── custom_obj 
	* ├── obj1
	* ├── obj2
	* └── obj3
	*/

	//
	// Containment. Creating the mandatory inner object, 
	// within the custom_obj. Note that not creating this object but 
	// creating the parent, would result in a validation error.
	//
	rib_obj_t* inner = rib_obj_int32_init("custom_obj.inner");
	if( rib_add_obj(&handle, &inner) != RIB_SUCCESS )
		exit(-1);
	assert(inner == NULL);

	/*
	* Status of the RIB instance tree:
	*
	* root 
 	* ├── custom_obj 
	* │   └── inner
	* ├── obj1
	* ├── obj2
	* └── obj3
	*
	*/
	
	//
	// Finally, we have defined in the schema that we can have 0..N-1 
	// objects of class int8_t in the root of the tree. 
	//
	// We want to allow the remote peer to create (and destroy) 
	//
	// We currenly don't have any of this objects to populate, so there
	// is no need to make any call rib_add_obj().
	//
	// So we only register to capture CREATE events in the root folder for
	// this type.
	//
	if(rib_reg_type_creation_cb(handle, RIB_ROOT, 
				1 /* depth */,
				RIB_INT8_T,	
				my_remote_obj_creation_cb) != RIB_SUCCESS )
		exit(-1);
	

	/*
	* Status of the RIB instance tree:
	*
	* root 
	* ├-- (0..infinite 'int8_t' objects can be created here)
 	* ├── custom_obj 
	* │   └── inner
	* ├── obj1
	* ├── obj2
	* └── obj3
	*
	*/

	//TODO: evaluate if we want to use relative paths.
	//
	// This has implications of whether rib_obj_t initializers should 
	// define the name as a full path or relative to the parent and leave 
	// 'the placement' in the tree to rib_add_obj().	


//
// Associate RIB to one (or more) AEs
// In this example a new AE with default configuration is
// created and associate to the RIB
//
//
// Victor Alvarez: suggests to do this association the other way around;
// associate the RIB to an AE. This however should go in the CDAP part of the
// lib, so leaving it like this for the moment. 
//

	// Associate the RIB to an AE. This API may also create
	// an AE with a certain name if required or could require the AE
	// to already exist, which would mean that the AEs need to be 
	// explicitely be created beforehand
	// TBD 
	if( rib_associate_ae(/* TODO */) != RIB_SUCCESS )
		exit(-1);


	// ACLs would be used using the interceptor 
	// TODO

// the library will handle incoming messages and will call
// the object action hooks
//
// There is no need to explicitely create CDAP I/O threads or anything
// RIB library will create it/them during rib association time

// At this point incomming CDAP connections can already be stablished
// against this AE and this RIB in particular 

}

void perform_remote_ops(void){

	//
	// It is important to note that at this point, this application
	// already has the schema loaded (by setup_aes). In a pure client,
	// the schema should be loaded/constructed prior to issue a connect
	// (version needs to be known by the library)
	// 

	rib_ver_set_t supported; 
	rib_rcon_handle_t* con_handle;
	rib_ver_t version;
	rib_obj_t obj1 = RIB_OBJ_INT32_INIT;
	uint64_t invoke_id;

	/* Set supported versions */
	version.model = RIB_MODEL;
	version.major_version = RIB_MAJ_VERSION;
	version.minor_version = RIB_MIN_VERSION;
	version.encoding = RIB_ENCODING;
	version.reserved = 0x0;


	rib_ver_set_init(&supported);
	rib_ver_set_add(&supported, &version); 

	/**
	* Stablish a simple CDAP connection 
	*/
	con_handle = rib_simple_connect(REMOTE_AP_NAME, NULL, supported);
	
	if(!con_handle)
		exit(-1);

	//Perform a simple read over an object
	rib_res_t res = rib_remote_read(con_handle, 
					"obj1"	
					NULL,
					RIB_INT32_T,	
					/*const uint32_t scope,*/
					&obj,	
					&invoke_id);

	if(res < 0)
		exit(-1);

	if(res == RIB_IN_PROGRESS){
		//Operation is still not completed block
		res = rib_remote_op_wait(con_handle, invoke_id);
	
		if(res != RIB_SUCCESS) 
			//Unknown error
			exit(-1);
	}

	//Do stuff with obj1...
	
}

void wait_remote_perform_ops(){
	//TODO
}

/*
* An example of an application publishing a RIB
*/
int main(int argc, char** argv){

	/*
	* Set up application entities RIBs
	*/ 
	setup_aes();

	/**
	* Client (starts a CDAP dialogue)
	*/
	perform_remote_ops();
	
	/**
	* Client (waits for CDAP peer to start the dialogue)
	*/
	wait_remote_perform_ops();
	
	return EXIT_SUCCESS;	
}
