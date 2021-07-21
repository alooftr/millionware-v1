#pragma once
#include <cstdint>

class i_client_entity_list
{
public:
	void* get_client_entity( int index )
	{
		using original_fn = void* ( __thiscall* )( i_client_entity_list*, int );
		return ( *( original_fn** )this )[ 3 ]( this, index );
	}

	void* get_client_entity( uintptr_t handle )
	{
		using original_fn = void* ( __thiscall* )( i_client_entity_list*, uintptr_t );
		return ( *( original_fn** )this )[ 4 ]( this, handle );
	}

	void* get_client_entity_handle( unsigned long handle )
	{
		using original_fn = void* ( __thiscall* )( i_client_entity_list*, unsigned long );
		return ( *( original_fn** )this )[ 4 ]( this, handle );
	}

	int get_highest_entity_index( )
	{
		using original_fn = int( __thiscall* )( i_client_entity_list* );
		return ( *( original_fn** )this )[ 6 ]( this );
	}

	template< class T >
	T* get( int index )
	{
		return reinterpret_cast< T* >( get_client_entity( index ) );
	}
};