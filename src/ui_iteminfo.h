#pragma once
#ifndef CATA_SRC_UI_ITEMINFO_H
#define CATA_SRC_UI_ITEMINFO_H

#include <functional>
#include <string>

#include "cata_imgui.h"
#include "imgui/imgui.h"
#include "input_context.h"
#include "output.h"
#include "point.h"

class iteminfo_window : public cataimgui::window
{
    public:
        iteminfo_window( item_info_data &info, point pos, int width, int height,
                         ImGuiWindowFlags flags = ImGuiWindowFlags_NoNavInputs );
        iteminfo_window( item_info_data &info, const std::function<point()> &position,
                         const std::function<point()> &size,
                         const std::string &confirm_label = std::string(),
                         ImGuiWindowFlags flags = ImGuiWindowFlags_NoNavInputs );
        std::string execute();

    protected:
        void draw_controls() override;
        cataimgui::bounds get_bounds() override;

    private:
        item_info_data data;
        std::function<point()> position;
        std::function<point()> size;
        std::string confirm_label;
        input_context ctxt;

        cataimgui::scroll s = cataimgui::scroll::none;
};

#endif // CATA_SRC_UI_ITEMINFO_H
