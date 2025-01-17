#pragma once
#include "../../source engine/math/point.hpp"
#include "../../engine/interfaces/interfaces.hpp"
#include "../../engine/utilities/xorstring.hpp"
#include "../../engine/utilities/math.hpp"
#include "textures.hpp"
#include <windows.h>
#include <vector>
#include <string_view>
#include <type_traits>

enum textures
{
	alpha = 0,
	voltage,
};

enum fonts
{
	verdana10 = 0,
	csgo,
	tahoma11,
	small_font,
	velocity_indicator,
	other_indicators
};

enum font_flags
{
	font_centered_x = ( 1 << 0 ),
	font_dropshadow = ( 1 << 4 ),
	font_outline = ( 1 << 8 ),
	font_centered_y = ( 1 << 16 )
};

enum hfont_flags
{
	hfont_none,
	hfont_italic = 0x001,
	hfont_underline = 0x002,
	hfont_strikeout = 0x004,
	hfont_symbol = 0x008,
	hfont_antialias = 0x010,
	hfont_gaussianblur = 0x020,
	hfont_rotary = 0x040,
	hfont_dropshadow = 0x080,
	hfont_additive = 0x100,
	hfont_outline = 0x200,
	hfont_custom = 0x400,
	hfont_bitmap = 0x800,
};

namespace render
{
	class c_texture
	{
	public:
		std::vector<uint8_t> m_bytes;
		int m_texture_id = -1;
		int m_width;
		int m_height;

		bool is_valid( );

		void init( std::vector<uint8_t> _bytes, int _width, int _height );

		void destroy( );

		void draw( int x, int y, int w = 0, int h = 0 );
	};

	class c_font
	{
	public:
		int m_font_id = -1;

		bool is_valid( );

		void init( std::string_view font_name, int size, int weight, int flags );

		void draw_text( point_t pos, color_t c, const wchar_t* text );

		point_t get_text_size( const wchar_t* text );

		void destory( );
	};

	extern std::vector<c_texture*> textures;
	extern std::vector<c_font*> fonts;

	void set_clip( point_t pos, point_t size );

	void rect( point_t pos, point_t size, color_t c );
	void filled_rect( point_t pos, point_t size, color_t c );
	void line( point_t a, point_t b, color_t c );

	void texture( int texture_id, point_t pos );

	void gradient_h( point pos, point size, color a, color b, int alpha1, int alpha2 );

	void gradient_v( point pos, point size, color a, color b, int alpha1, int alpha2 );

	template <typename... Args>
	void text( point_t pos, int font_id, int flags, color_t c, const std::wstring& fmt, Args&&... args )
	{
		auto buf = fmt;

		if ( sizeof...( args ) > 0 )
		{
			const auto size = static_cast< std::size_t >( std::swprintf( nullptr, 0, fmt.data( ), std::forward<Args>( args )... ) + 1 );

			const auto buffer = std::make_unique<wchar_t[ ]>( size );
			std::swprintf( buffer.get( ), size, fmt.data( ), std::forward<Args>( args )... );

			buf = std::wstring{ buffer.get( ), buffer.get( ) + size - 1 };
		}

		auto font = fonts[ font_id ];
		if ( !font )
			return;

		if ( flags & font_centered_x || flags & font_centered_y )
		{
			auto size = font->get_text_size( buf.c_str( ) );

			if ( flags & font_centered_x )
				pos.x -= size.x / 2;

			if ( flags & font_centered_y )
				pos.y -= size.y / 2;
		}

		if ( flags & font_dropshadow )
		{
			font->draw_text( pos + 1, color( 0, 0, 0, std::max<int>( c.a - 40, 0 ) ), buf.c_str( ) );
		}

		if ( flags & font_outline )
		{
			for ( int x = -1; x <= 1; x++ ) for ( int y = -1; y <= 1; y++ )
				font->draw_text( point( pos.x + x, pos.y + y ), color( 0, 0, 0, std::max<int>( c.a - 40, 0 ) ), buf.c_str( ) );
		}

		font->draw_text( pos, c, buf.c_str( ) );
	}

	void circle( point_t center, float radius, float points, bool filled, color_t c );

	point_t text_size( const wchar_t* text, int font_id );

	void reset_clip( );

	void init( );

	void destroy( );
}