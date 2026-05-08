#include "TitelbarMy.h"

static bool PointInRect(const ImVec2& p, const ImRect& r) {
    return p.x >= r.Min.x && p.x <= r.Max.x && p.y >= r.Min.y && p.y <= r.Max.y;
}

float TitleBar::AnimTo(float v, float target, float speed, float dt) {
    float k = 1.0f - ImPow(0.001f, speed * dt);
    return v + (target - v) * k;
}

void TitleBar::AddOrUpdateButton(const TBButtonDesc& desc) {
    auto& b = btn_[desc.id];
    b.visible = desc.visible;
    b.enabled = desc.enabled;
    b.draw = desc.draw;
    b.on_click = desc.on_click;
}

void TitleBar::SetVisible(uint32_t id, bool v) {
    if (auto it = btn_.find(id); it != btn_.end()) {
        it->second.visible = v;
    }
}

void TitleBar::SetEnabled(uint32_t id, bool e) {
    if (auto it = btn_.find(id); it != btn_.end()) {
        it->second.enabled = e;
    }
}

void TitleBar::SetAction(uint32_t id, std::function<void()> fn) {
    if (auto it = btn_.find(id); it != btn_.end()) {
        it->second.on_click = std::move(fn);
    }
}

void TitleBar::Draw() {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (!window) return;

    ImGuiIO& io = ImGui::GetIO();
    const float dt = io.DeltaTime;

    const ImVec2 win_pos  = ImGui::GetWindowPos();
    const ImVec2 win_size = ImGui::GetWindowSize();

    const float title_h = ImGui::GetFrameHeight() * 1.5 * theme_.height_mul;

    ImRect title_r(win_pos, ImVec2(win_pos.x + win_size.x, win_pos.y + title_h));

    float x = title_r.Max.x - theme_.pad_x;
    const float btn_sz = title_h * theme_.btn_size_mul;
    const float y0 = title_r.Min.y + (title_h) * 0.5f;

    std::vector<ImRect> exclude;

    for (uint32_t id : order_) {
        auto it = btn_.find(id);
        if (it == btn_.end()) {
            continue;
        }
        auto& b = it->second;
        if (!b.visible) {
            continue;
        }

        b.rect = ImRect(ImVec2(x, y0), ImVec2(x + btn_sz, y0 + btn_sz));
        exclude.push_back(b.rect);

        x -= btn_sz + theme_.gap;
    }

    //TODO
    // ImRect icon_r(ImVec2(x + theme_.gap, title_r.Min.y), ImVec2(x + theme_.gap + title_h, title_r.Max.y));
    // ImRect text_r(ImVec2(icon_r.Max.x + theme_.gap, title_r.Min.y), ImVec2(title_r.Max.x - theme_.pad_x, title_r.Max.y));
    //TODO


    for (uint32_t id : order_) {
        auto it = btn_.find(id);
        if (it == btn_.end()) {
            continue;
        }
        auto& b = it->second;
        if (!b.visible) {
            continue;
        }

        ImGui::SetCursorScreenPos(b.rect.Min);
        ImGui::PushID(static_cast<int>(id));

        ImGui::InvisibleButton("##tb_btn", b.rect.GetSize());

        bool hovered = ImGui::IsItemHovered();
        bool held    = ImGui::IsItemActive();
        bool clicked = ImGui::IsItemClicked(ImGuiMouseButton_Left);

        if (clicked && b.enabled && b.on_click) b.on_click();

        b.hover_t  = AnimTo(b.hover_t,  hovered ? 1.f : 0.f, theme_.anim_speed, dt);
        b.active_t = AnimTo(b.active_t, held    ? 1.f : 0.f, theme_.anim_speed, dt);

        if (b.draw) b.draw(b.rect, b.hover_t, b.active_t);

        ImGui::PopID();
    }

    //TODO
    // if (draw_icon_)  draw_icon_(icon_r);
    // if (draw_title_) draw_title_(text_r, title_);
    //TODO

    bool mouse_in_excluded = false;
    for (const auto& r : exclude) {
        if (PointInRect(io.MousePos, r)) {
            mouse_in_excluded = true;
            break;
        }
    }

    ImGui::SetCursorScreenPos(title_r.Min);
    ImGui::PushID("##tb_drag_zone");
    ImGui::InvisibleButton("##tb_drag", title_r.GetSize());

    if (!mouse_in_excluded)
    {
        if (ImGui::IsItemActivated() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            dragging_ = true;
            ImVec2 const m_pos = ImGui::GetIO().MousePos;
            ImVec2 const w_pos = ImGui::GetWindowPos();
            drag_offset_ = ImVec2(m_pos.x - w_pos.x, m_pos.y - w_pos.y);
        }
        if (dragging_ && ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            ImVec2 const m_pos = ImGui::GetIO().MousePos;
            auto const new_pos = ImVec2(m_pos.x - drag_offset_.x, m_pos.y - drag_offset_.y);
            ImGui::SetWindowPos(new_pos, ImGuiCond_Always);
        }
        if (dragging_ && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            dragging_ = false;
        }
    }
    else
    {
        dragging_ = false;
    }

    ImGui::PopID();

    ImGui::Dummy(ImVec2(0.0f, title_h + ImGui::GetStyle().WindowPadding.y));
}

