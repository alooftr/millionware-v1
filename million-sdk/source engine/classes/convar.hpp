#pragma once
#include <vector>

class convar;
using fn_change_callback_t = void( * )( convar * var, const char* pOldValue, float flOldValue );

inline int UtlMemory_CalcNewAllocationCount( int nAllocationCount, int nGrowSize, int nNewSize, int nBytesItem )
{
	if ( nGrowSize )
		nAllocationCount = ( ( 1 + ( ( nNewSize - 1 ) / nGrowSize ) ) * nGrowSize );
	else
	{
		if ( !nAllocationCount )
			nAllocationCount = ( 31 + nBytesItem ) / nBytesItem;

		while ( nAllocationCount < nNewSize )
			nAllocationCount *= 2;
	}

	return nAllocationCount;
}

template< class T, class I = int >
class CUtlMemory
{
public:
	T& operator[]( I i )
	{
		return m_pMemory[ i ];
	}

	T* Base( )
	{
		return m_pMemory;
	}

	int NumAllocated( ) const
	{
		return m_nAllocationCount;
	}

	bool IsExternallyAllocated( ) const
	{
		return m_nGrowSize < 0;
	}

protected:
	T* m_pMemory;
	int m_nAllocationCount;
	int m_nGrowSize;
};

template <class T>
inline T* Construct( T* pMemory )
{
	return ::new( pMemory ) T;
}

template <class T>
inline void Destruct( T* pMemory )
{
	pMemory->~T( );
}

template< class T, class A = CUtlMemory<T> >
class CUtlVector
{
	typedef A CAllocator;
public:
	T& operator[]( int i )
	{
		return m_Memory[ i ];
	}

	T& Element( int i )
	{
		return m_Memory[ i ];
	}

	T* Base( )
	{
		return m_Memory.Base( );
	}

	int Count( ) const
	{
		return m_Size;
	}

	void RemoveAll( )
	{
		for ( int i = m_Size; --i >= 0; )
			Destruct( &Element( i ) );

		m_Size = 0;
	}

	int AddToTail( )
	{
		return InsertBefore( m_Size );
	}

	void SetSize( int size )
	{
		m_Size = size;
	}

	int InsertBefore( int elem )
	{
		GrowVector( );
		ShiftElementsRight( elem );
		Construct( &Element( elem ) );

		return elem;
	}

protected:
	void GrowVector( int num = 1 )
	{
		if ( m_Size + num > m_Memory.NumAllocated( ) )
			m_Memory.Grow( m_Size + num - m_Memory.NumAllocated( ) );

		m_Size += num;
		ResetDbgInfo( );
	}

	void ShiftElementsRight( int elem, int num = 1 )
	{
		int numToMove = m_Size - elem - num;
		if ( ( numToMove > 0 ) && ( num > 0 ) )
			memmove( &Element( elem + num ), &Element( elem ), numToMove * sizeof( T ) );
	}

	CAllocator m_Memory;
	int m_Size;

	T* m_pElements;

	inline void ResetDbgInfo( )
	{
		m_pElements = Base( );
	}
};

enum cvar_flags
{
	fcvar_none = 0,
	fcvar_unregistered = ( 1 << 0 ),
	fcvar_developmentonly = ( 1 << 1 ),
	fcvar_gamedll = ( 1 << 2 ),
	fcvar_clientdll = ( 1 << 3 ),
	fcvar_hidden = ( 1 << 4 ),
	fcvar_protected = ( 1 << 5 ),
	fcvar_sponly = ( 1 << 6 ),
	fcvar_archive = ( 1 << 7 ),
	fcvar_notify = ( 1 << 8 ),
	fcvar_userinfo = ( 1 << 9 ),
	fcvar_printableonly = ( 1 << 10 ),
	fcvar_unlogged = ( 1 << 11 ),
	fcvar_never_as_string = ( 1 << 12 ),
	fcvar_replicated = ( 1 << 13 ),
	fcvar_cheat = ( 1 << 14 ),
	fcvar_ss = ( 1 << 15 ),
	fcvar_demo = ( 1 << 16 ),
	fcvar_dontrecord = ( 1 << 17 ),
	fcvar_ss_added = ( 1 << 18 ),
	fcvar_release = ( 1 << 19 ),
	fcvar_reload_materials = ( 1 << 20 ),
	fcvar_reload_textures = ( 1 << 21 ),
	fcvar_not_connected = ( 1 << 22 ),
	fcvar_material_system_thread = ( 1 << 23 ),
	fcvar_archive_xbox = ( 1 << 24 ),
	fcvar_accessible_from_threads = ( 1 << 25 ),
	fcvar_server_can_execute = ( 1 << 28 ),
	fcvar_server_cannot_query = ( 1 << 29 ),
	fcvar_clientcmd_can_execute = ( 1 << 30 ),
	fcvar_meme_dll = ( 1 << 31 ),
	fcvar_material_thread_mask = ( fcvar_reload_materials | fcvar_reload_textures | fcvar_material_system_thread )
};

class convar
{
public:
	float get_float( )
	{
		using original_fn = float( __thiscall* )( convar* );
		return ( *( original_fn** )this )[ 12 ]( this );
	}
	void set_value( const char* value )
	{
		using original_fn = void( __thiscall* )( convar*, const char* );
		return ( *( original_fn** )this )[ 14 ]( this, value );
	}
	void set_value( float value )
	{
		using original_fn = void( __thiscall* )( convar*, float );
		return ( *( original_fn** )this )[ 15 ]( this, value );
	}
	void set_value( int value )
	{
		using original_fn = void( __thiscall* )( convar*, int );
		return ( *( original_fn** )this )[ 16 ]( this, value );
	}
	void set_value( bool value )
	{
		using original_fn = void( __thiscall* )( convar*, int );
		return ( *( original_fn** )this )[ 16 ]( this, static_cast< int >( value ) );
	}

private:
	char pad_0x0000[ 0x4 ];

public:
	convar* next;
	__int32 is_registered;
	char* name;
	char* help_string;
	__int32 flags;

private:
	char pad_0x0018[ 0x4 ];

public:
	convar* parent;
	char* default_value;
	char* string;
	__int32 string_length;
	float float_value;
	__int32 numerical_value;
	__int32 has_min;
	float min;
	__int32 has_max;
	float max;
	CUtlVector<fn_change_callback_t> callbacks;
};