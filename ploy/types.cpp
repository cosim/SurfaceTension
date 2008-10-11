#include "types.h"
#include "symbol.h"
#include <assert.h>


//TODO: allow compile time code transformation by appending source data (strings, and line info to the type data.

//TODO: Dynamic build this too
// It is essential that this is orderd in the same way the enums are numbered.
static const dynamic_type TypeList[DT_Invalid] =
{
	{DT_Pair, sizeof(pair)},
	{DT_Symbol, sizeof(symbol)},
	{DT_Int, sizeof(int)},
	{DT_Real, sizeof(float)},
	{DT_String, invalid_size}
};

const dynamic_type* get_type(dynamic_types typeId)
{
	assert(typeId < DT_Invalid);
	return &TypeList[typeId];
}

pointer ploy_alloc(const dynamic_type* type, size_t size)
{
	assert(size != invalid_size || type->AllocSize != invalid_size);

	size_t alloc_size = size;
	if(alloc_size == invalid_size)
		alloc_size = type->AllocSize;

	alloc_size += sizeof(dynamic_types);

	void* alloc = malloc(alloc_size);
	memset(alloc, 0, alloc_size); // Remove if we implement type initializers
	*((dynamic_types*)alloc) = type->Id;

	return (pointer)alloc;
	
}

pointer ploy_alloc(const dynamic_type* type)
{
	return ploy_alloc(type, invalid_size);
}

dynamic_types get_type_id(pointer P)
{
	return *(dynamic_types*)P;
}

bool is_type(pointer P, dynamic_types type)
{
	return get_type_id(P) == type;
}

void* get_value(pointer P)
{
	return (char*)P+sizeof(dynamic_types);
}

pair* get_pair(pointer P)
{
	assert(is_type(P, DT_Pair));
	return (pair*)get_value(P);
}

pointer create_pair(pointer car, pointer cdr)
{
	pointer ret = ploy_alloc(get_type(DT_Pair));
	
	pair* value = get_pair(ret);
	
	value->_car = car;
	value->_cdr = cdr;

	return ret;
}

symbol* get_symbol(pointer P)
{
	assert(is_type(P, DT_Symbol));
	return (symbol*)get_value(P);
}

pointer create_symbol(symbol_table* tbl, const char* sym)
{
	pointer ret = ploy_alloc(get_type(DT_Symbol));

	*get_symbol(ret) = symbol_from_string(tbl, sym);

	return ret;
}

int* get_int(pointer P)
{
	assert(is_type(P, DT_Int));
	return (int*)get_value(P);
}

pointer create_int(int i)
{
	pointer ret = ploy_alloc(get_type(DT_Int));
	
	*get_int(ret) = i;
	
	return ret;
}

float* get_real(pointer P)
{
	assert(is_type(P, DT_Real));
	return (float*)get_value(P);
}

pointer create_real(float f)
{
	pointer ret = ploy_alloc(get_type(DT_Real));
	
	*get_real(ret) = f;

	return ret;
}

char** get_string(pointer P)
{
	assert(is_type(P, DT_String));
	return (char**)get_value(P);
}

pointer create_string(const char* str)
{
	return create_string(str, strlen(str));
}

pointer create_string(const char* str, size_t len)
{
	pointer ret = ploy_alloc(get_type(DT_String), len+1);
	
	strncpy(*get_string(ret), str, len);
	
	(*get_string(ret))[len+1] = '\0'; // not strictly nessacary while we are zeroing the mem, but for if/when we remove it.
	
	return ret;
}
