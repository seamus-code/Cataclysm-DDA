#include "ui_iteminfo.h"

#include <imgui/imgui.h>
#include <string>
#include <vector>

#include "color.h"
#include "input_enums.h"
#include "translations.h"
#include "ui_manager.h"


iteminfo_window::iteminfo_window( item_info_data &info, point pos, int width, int height,
                                  ImGuiWindowFlags flags ) :
    iteminfo_window( info, [pos]()
{
    return pos;
},
[width, height]()
{
    return point( width, height );
}, std::string(), flags )
{
}

iteminfo_window::iteminfo_window( item_info_data &info, const std::function<point()> &position,
                                  const std::function<point()> &size, const std::string &confirm_label,
                                  ImGuiWindowFlags flags ) :
    cataimgui::window( _( "Item info" ), flags ),
    data( info ), position( position ), size( size ), confirm_label( confirm_label )
{
    ctxt = input_context( "default" );
    if( info.handle_scrolling ) {
        ctxt.register_action( "PAGE_UP" );
        ctxt.register_action( "PAGE_DOWN" );
        ctxt.register_action( "HOME" );
        ctxt.register_action( "END" );
        if( info.arrow_scrolling ) {
            ctxt.register_action( "UP" );
            ctxt.register_action( "DOWN" );
        }
    }
    ctxt.register_action( "HELP_KEYBINDINGS" );
    ctxt.register_action( "CONFIRM" );
    ctxt.register_action( "QUIT" );
    if( info.any_input ) {
        ctxt.register_action( "ANY_INPUT" );
    }

    ctxt.set_timeout( 10 );
}

cataimgui::bounds iteminfo_window::get_bounds()
{
    const point pos = position();
    const point dimensions = size();
    return { pos.x < 0 ? -1.f : static_cast<float>( str_width_to_pixels( pos.x ) ),
             pos.y < 0 ? -1.f : static_cast<float>( str_height_to_pixels( pos.y ) ),
             static_cast<float>( str_width_to_pixels( dimensions.x ) ),
             static_cast<float>( str_height_to_pixels( dimensions.y ) ) };
}

void iteminfo_window::draw_controls()
{
    const bool show_actions = !confirm_label.empty();
    if( show_actions ) {
        action_button( "CONFIRM", ctxt.get_button_text( "CONFIRM", confirm_label ) );
        ImGui::SameLine();
        action_button( "QUIT", ctxt.get_button_text( "QUIT" ) );
        ImGui::Separator();
    }
    if( !show_actions || ImGui::BeginChild( "##item_details" ) ) {
        cataimgui::set_scroll( s );

        nc_color base_color = c_light_gray;
        if( !data.get_item_name().empty() ) {
            cataimgui::draw_colored_text( data.get_item_name(), base_color );
            ImGui::Spacing();
        }
        if( !data.get_type_name().empty() &&
            data.get_item_name().find( data.get_type_name() ) == std::string::npos ) {
            cataimgui::draw_colored_text( data.get_type_name(), base_color );
        }

        display_item_info( data.get_item_display(), data.get_item_compare() );
    }
    if( show_actions ) {
        ImGui::EndChild();
    }
}

std::string iteminfo_window::execute()
{
    if( data.without_getch ) {
        ui_manager::redraw();
        return std::string();
    }

    while( true ) {
        ui_manager::redraw();
        const std::string action = has_button_action() ? get_button_action() : ctxt.handle_input();

        if( data.handle_scrolling && data.arrow_scrolling && action == "UP" ) {
            s = cataimgui::scroll::line_up;
        } else if( data.handle_scrolling && data.arrow_scrolling && action == "DOWN" ) {
            s = cataimgui::scroll::line_down;
        } else if( data.handle_scrolling && action == "PAGE_UP" ) {
            s = cataimgui::scroll::page_up;
        } else if( data.handle_scrolling && action == "PAGE_DOWN" ) {
            s = cataimgui::scroll::page_down;
        } else if( data.handle_scrolling && action == "HOME" ) {
            s = cataimgui::scroll::begin;
        } else if( data.handle_scrolling && action == "END" ) {
            s = cataimgui::scroll::end;
        } else if( action == "CONFIRM" || action == "QUIT" ||
                   ( data.any_input && action == "ANY_INPUT" && !ctxt.get_raw_input().sequence.empty() ) ) {
            return action;
        }

        // mouse click on x to close leads here
        if( !get_is_open() ) {
            return "QUIT";
        }
    }
}
