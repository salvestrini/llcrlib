#include <rib.h>
#include <rib_mgmt.h>
#include <rib_builtin_obj.h>

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
my_remote_obj_creation(rib_handle_t handle, const char* obj_name, 
					const char* class_name, 
					const uint32_t invoke_id,
					const rib_op_payload_t* op_payload){

		
	assert(strncmp(obj->class_name, "custom_type", 
				RIB_MAX_CLASS_NAME) == 0);

	//Create a custom object
	rib_obj_t* obj = rib_obj_init("custom_obj", "custom_type", &data);
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
//Create version&RIB
//
	rib_ver_t version;
	version.model = RIB_MODEL;
	version.major_version = RIB_MAJ_VERSION;
	version.minor_version = RIB_MIN_VERSION;
	version.encoding = RIB_ENCODING;
	version.reserved = 0x0;
		
	//Create a RIB
	rib_handle_t handle;
	if( rib_create(version, &handle) != RIB_SUCCESS )
		exit(-1);	

//
//Add existing objects of the 0x11 odel 0x11
//as well as register the necessary types for CREATE ops
//


/*
* Status of the RIB tree:
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
	* Status of the RIB tree:
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
	* Status of the RIB tree:
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
	* Status of the RIB tree:
	*
	* root 
	* ├── obj1
	* ├── obj2
	* └── obj3
	*/

	//
	//Add a completely custom type
	//The data type will be a struct. We define a class_name "custom_type"
	//that must be unique in the RIB
	//
	memset(&custom_data, 0, sizeof(custom_data));
	rib_obj_t* obj_c = rib_obj_init("custom_obj", "custom_type", &data);
	
	//Now set the object operations we want to support
	//Note that we could specify a common hook for all "custom_type"
	//objects, or specify a per-object hook. This is up to the application
	obj_c->ops.start = my_custom_start;
	obj_c->ops.delete = my_custom_delete;

	//Now add to the RIB	
	if( rib_add_obj(&rib_handle, &obj_c) != RIB_SUCCESS )
		exit(-1);
	assert(obj_c == NULL);

	/*
	* Status of the RIB tree:
	*
	* root 
	* ├── custom_obj 
	* ├── obj1
	* ├── obj2
	* └── obj3
	*/

	//
	//Register a class_name. With the registration of a class_name in a
	//certain part of the tree, as well as its depth, we can define where
	//objects of class_name X can be remotely crated by the peer and how
	//to capture such events
	//
	rib_obj_t* root = rib_get_root(&rib_handle); 
	if( rib_obj_reg_type(root,
				1 /* allow creation only in the root level */,
				"remote_obj_class",
				RIB_OBJ_COUNT_UNLIMITED, /* No limit */
				my_remote_obj_creation) != RIB_SUCCESS )
		exit(-1);
	
	/*
	* Status of the RIB tree:
	*
	* root 
	* ├-- (0..infinite 'remote_obj_class' objects can be created here)
	* ├── custom_obj 
	* ├── obj1
	* ├── obj2
	* └── obj3
	*/
	
	//
	//Containment relations. The CDAP spec defins that the class_name,
	//so the object CDAP type, must uniquely identify the type of the obj-
	//ect including its inner types
	//
	//Creation of inner objects, or registering types to simple objects
	// (built-in types) such int32_t for instance is STRICTLY forbidden
	//
	//rib_obj_t* failed_obj = rib_obj_int32p_init("custom_obj.failed_obj",
	//							 &var1, NULL);
	//rib_add_obj(&handle, "level1.obj3", &obj3, NULL) <= This will fail
	//
	// However, no one forbidds users to create other types (class_names)
	// That wrap simple types but allow containment, like 'custom_Type'
	//
	
	//But we can define that custom obj can contain other simple types
	rib_obj_t* inner = rib_obj_int32_init("custom_obj.inner");
	if( rib_add_obj(&handle, &inner) != RIB_SUCCESS )
		exit(-1);
	assert(inner == NULL);

	/*
	* Status of the RIB tree:
	*
	* root 
	* ├-- (0..infinite 'remote_obj_class' objects can be created here)
 	* ├── custom_obj 
	* │   └── inner
	* ├── obj1
	* ├── obj2
	* └── obj3
	*
	*/

	//TODO: evaluate if we want to use relative paths.
	//
	//This has implications of whether rib_obj_t initializers should 
	//define the name as a full path or relative to the parent and leave 
	//'the placement' in the tree to rib_add_obj().	

	//Registering types within another object works likewise 

//
//Associate RIB to one (or more) AEs
//In this example a new AE with default configuration is
//created and associate to the RIB
//
//
// Victor Alvarez: suggests to do this association the other way around;
// associate the RIB to an AE. This however should go in the CDAP part of the
// lib, so leaving it like this for the moment. 
//

	ae_id_t ae_id = AE_NEW;
	
	//Note that the second parameter is NULL => no ACLs to be
	//applied for incomming CDAP connections in this AE
	if( rib_associate_ae(&ae_id, NULL) != RIB_SUCCESS )
		exit(-1);


	//Specific AE example + ACLs
	/*	
	ae_id_t ae_id = 1;
	
	//ACLs
	ae_conn_acl_t* acl = XXX; //Initialize 
	//TODO: configure ACL

	if( rib_associate_ae(&ae_id, &ae_acl) != RIB_SUCCESS )
		exit(-1);
	*/

//the library will handle incoming messages and will call
//the object action hooks
//
//There is no need to explicitely create CDAP I/O threads or anything
//RIB library will create it/them during rib association time

//At this point incomming CDAP connections can already be stablished
//against this AE and this RIB in particular 

}

/*
* An example of an application publishing a RIB
*/
int main(int argc, char** argv){

	/*
	* Set up application entities
	*/ 

	/**
	* Clients
	*/
	
	return EXIT_SUCCESS;	
}
