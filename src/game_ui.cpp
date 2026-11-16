#include "game_ui.h"

#include <string>

#include "options.h"
#include "point.h"
#include "terminal_layout.h"

#if defined(TILES)
#include "sdltiles.h"
#endif

bool game_ui::is_portrait_layout()
{
#if defined(TILES)
    const point pixels = get_interface_layout_size();
#else
    // Text terminals have no portable usable-pixel measurement; Auto stays landscape.
    const point pixels = point::zero;
#endif
    return terminal_layout::use_portrait( get_option<std::string>( "INTERFACE_LAYOUT" ), pixels );
}

#if !defined(TILES)

void reinitialize_framebuffer( const bool /*force_invalidate*/ )
{
}

void to_map_font_dim_width( int & )
{
}

void to_map_font_dim_height( int & )
{
}

void to_map_font_dimension( int &, int & )
{
}
void from_map_font_dimension( int &, int & )
{
}
void to_overmap_font_dimension( int &, int & )
{
}

#endif
