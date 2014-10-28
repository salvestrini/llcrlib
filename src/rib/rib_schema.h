//TODO: add license

#ifndef RIB_SCHEMA_H
#define RIB_SCHEMA_H 

#include <inttypes.h>
#include <stdbool.h>
#include "rib.h" 

/**
* @file rib_schema.h
* @author Marc Sune<marc.sune (at) bisdn.de>
*
* @brief Local Resource Information Base(RIB) schema
*
* The RIB schema is the definition of the class_names (types) of objects and 
* their relation of contention in the tree.
*
* The RIB schema is an essential piece in the RIB library. It allows to:
*
* - Index and indentify the types of objects (class_name(s)) available in this
*   RIB version  
* - Define the relation of contention of these types.
* - Define the necessary allocation functions required by the library
*   to create objects.
* - Continuosly validate the RIB instance(s) to make sure it conforms to the 
*   schema
*
* It is important to note that a RIB schema does NOT define the actual objects
* that a paticular RIB (an instance) of this version(schema), NOR it populates
* objects during rib_create().
*/

/**
* Root class name
*/
#define RIB_ROOT_CN "root_cn"

/**
* Root name
*/
#define RIB_ROOT "" 

/**
* Unlimited number of objects
*/
#define RIB_OBJ_COUNT_UNLIMITED 0xFFFFFFFF


/**
* @brief RIB schema 
*/
typedef struct rib_schema{
	/* RIB version */
	rib_ver_t version;

	//TODO: hash table for obj names
	//TODO: hash table for obj class
}rib_schema_t;
//COMPILATION_ASSERT(RIB_SIZE, ( sizeof(rib_ver_t) == 8 ) );


//
// Functions
//

//extern C
BEGIN_DECLS


/**
* @brief Create a RIB schema 
*
* Create an empty RIB schema, with only the root type defined.
*
* @param version RIB version
*/
rib_res_t rib_schema_create(const rib_ver_t version);

/**
* @brief Import a RIB schema from a FILE
*
* @param file Full path to the schema file, including extension
* @param version On success the imported RIB version will be stored here
*/
rib_res_t rib_schema_import(const char* file_name, rib_ver_t* version);

/**
* @brief Destroy a RIB schema  
*
* @param version RIB version
*/
rib_res_t rib_schema_destroy(const rib_ver_t version);


//
// Type definition and contention 
//

/**
* @brief Define a class_name (object type) in the schema
*
* Defines a class_name (type) in the RIB. 
*
* @param version RIB schema version
* @param class_name Class name of the objects
*/
rib_res_t rib_schema_def_type(const rib_ver_t version, const char* class_name);

/**
* @brief Undefine a class_name (object type) from the schema
*
* @param version RIB schema version
* @param class_name Class name of the objects
*/
rib_res_t rib_schema_def_type(const rib_ver_t version, 
						const char* class_name);


/**
* @brief Defines a contention relation with its parent type
*
* Both types must be already defined 
*
* @param version RIB schema version
* @param container_cn Container class name (type)
* @param container_name Container name 
* @param class_name Inner class name (type)
* @param name Name of the contained 'class_name' object within the parent obj.
* @param mandatory When set, the parent object must always contain this obj. 
* @param max_objs Maximum number of objects of type 'class_name' that can be 
*  created in this position of the tree. If 'name' is defined, 'max_objs' must
*  be equal to 1 
*/
rib_res_t rib_schema_def_type_cont_rel(const rib_ver_t version, 
						const char* container_cn,
						const char* container_name,
						const char* class_name,
						const char* name,
						const bool mandatory, 
						const unsigned max_objs);

/**
* @brief Undefines a type contention relation between a type A (container)
* and B (contained) 
*
* @param version RIB schema version
* @param container_cn Container class name (type)
* @param class_name Inner class name (type)
* @param name Name of the contained 'class_name' object within the parent obj.
*/
rib_res_t rib_schema_undef_type_cont_rel(const rib_ver_t version, 
						const char* container_cn,
						const char* class_name,
						const char* name);


//extern C 
END_DECLS

#endif // RIB_SCHEMA_H_ 
