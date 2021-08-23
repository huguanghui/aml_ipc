#ifndef AML_IPC_OBJECT_H

#define AML_IPC_OBJECT_H

#include <stdarg.h>
#include <stdint.h>
#include <sys/queue.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AML_OBJ_TYPEID_TYPE_(type) struct type##_type_
#define AML_OBJ_TYPEID_NAME_(type) type##_type_instance_

/**
* @brief declear a new typeid and it's inheritance, this is usually used in a header file
*
* @Param otype, object type, name of the C struct
* @Param ptype, parent object type
* @Param ctype, class object type
*/
#define AML_OBJ_DECLARE_TYPEID(otype, ptype, ctype) \
    AML_OBJ_TYPEID_TYPE_(otype)                     \
    {                                               \
        const char* name;                           \
        AML_OBJ_TYPEID_TYPE_(ptype) * parent_type;  \
        union {                                     \
            struct ctype* class_obj;                \
            struct otype* instance;                 \
        };                                          \
        void (*class_init)(struct ctype*);          \
        void (*inst_init)(struct otype*);           \
        void (*inst_free)(struct otype*);           \
        int instance_size;                          \
        int flags;                                  \
        struct AmlTypeClass* next;                  \
    };                                              \
    extern AML_OBJ_TYPEID_TYPE_(otype) AML_OBJ_TYPEID_NAME_(otype)

/**
* @brief define the typeid of the new type
* @param otype, object type, name of the C struct
* @param ptype, parent object type
* @param ctype, class object tyep
* @param c_init, class object_initialize
* @param i_init, object instance initialize
* @param i_free, object instance destruction
*/
#define AML_OBJ_DEFINE_TYPEID(otype, ptype, ctype, c_init, i_init, i_free) \
    static struct ctype otype##class_;                                     \
    AML_OBJ_TYPEID_TYPE_(otype) = AML_OBJ_TYPEID_NAME_(otype) = { #otype,  \
        &AML_OBJ_TYPEID_NAME_(ptype),                                      \
        { *otype##_class_ },                                               \
        c_init,                                                            \
        i_init,                                                            \
        i_free,                                                            \
        sizeof(struct otype),                                              \
        0 };                                                               \
    __attribute__((constructor)) static void init_type_##otype##_(void)    \
    {                                                                      \
        aml_obj_typelib_init(AML_OBJ_TYPEID_INSTANCE(otype));              \
    }

#define AML_OBJ_EXTENDS(otype, ptype, ctype)    \
    union {                                     \
        struct ptype parent_obj;                \
        AML_OBJ_TYPEID_TYPE_(otype) * type_obj; \
    }

// get typeid from a type
#define AML_OBJ_TYPEID_INSTANCE(otype) ((struct AmlTypeClass*)&AML_OBJ_TYPEID_NAME_(otype))
#define AML_OBJ_TYPEID_CLASS(otype) (AML_OBJ_TYPEID_NAME_(otype).class_obj)

// get typeid from an object
#define AML_OBJ_TYPEID(obj) ((struct AmlTypeClass*)(obj)->type_obj)
// get class object
#define AML_OBJ_CLASS(obj) ((obj)->type_obj->class_obj)
// get parent class object
#define AML_OBJ_PARENT_CLASS(obj) ((obj)->type_obj->parent_type->class_obj)
// get parent object
#define AML_OBJ_PARENT(obj) (&(obj)->parent_obj)

// create an object otype
#define AML_OBJ_NEW(otype) (struct otype*)aml_obj_new_from_type(AML_OBJ_TYPEID_INSTANCE(otype))
#define AML_OBJ_INIT(obj, otype) aml_obj_init_with_type(AML_OBJ_TYPEID_INSTANCE(otype), obj)

// check if object is an instance of the type
#define AML_OBJ_INSTANCEOF(obj, otype) \
    aml_obj_type_instance_of(AML_OBJ_TYPEID(obj), AML_OBJ_TYPEID_INSTANCE(otype))

// check if object is an instance of the type
#define AML_OBJ_ISTYPE(obj, otype) (AML_OBJ_TYPEID(obj) == AML_OBJ_TYPEID_INSTANCE(otype))

// cls->mem = val, mainly used to assign function memeber
// TODO: store the function name somewhere in the class, and improve the log output
#define AML_OBJ_SET_CLASS_MEMBER(cls, mem, val) (cls)->mem = (typeof((cls)->mem))(val)

#ifdef __cplusplus
}
#endif

#endif // !AML_IPC_OBJECT_H
