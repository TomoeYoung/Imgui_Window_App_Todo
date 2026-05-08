#pragma once
#include <vector>
#include <functional>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <imgui_internal.h>


#ifndef TitelbarMy_h
#define TitelbarMy_h

struct TBTheme {
    float height_mul = 1.0f;
    float pad_x = 30.0f; //offset from the left end of the titelbar
    float gap = 30.0f; //the gap between the buttons
    float btn_size_mul = 0.9f; //the size of the button relative to the titelbar
    float anim_speed = 2.f;
};

struct TBButtonDesc {
    uint32_t id = 0;
    bool visible = true;
    bool enabled = true;
    std::function<void(const ImRect& r, float hover_t, float active_t)> draw; //button draw function
    std::function<void()> on_click; //button logic function
};

struct TBButtonState {
    bool visible = true;
    bool enabled = true;
    ImRect rect{}; //We calculate the button positions on each frame
    float hover_t = 0.f;
    float active_t = 0.f;
    std::function<void(const ImRect&, float, float)> draw;
    std::function<void()> on_click;
};


class TitleBar {
public:
    explicit TitleBar(const TBTheme& theme = {}) : theme_(theme) {}
    void AddOrUpdateButton(const TBButtonDesc& desc);
    void SetOrder(const std::vector<uint32_t>& ids) { order_ = ids; } //Setting up a queue drawing buttons each frame
    void SetVisible(uint32_t id, bool visible_state);
    void SetEnabled(uint32_t id, bool enable_state);
    void SetAction(uint32_t id, std::function<void()> fn); //Adding actions for the button, send arg as function
    void SetTitle(std::string t) { title_ = std::move(t); }
    void SetDrawIcon(std::function<void(const ImRect&)> fn) { draw_icon_ = std::move(fn); }
    void SetDrawTitle(std::function<void(const ImRect&, const std::string&)> fn) { draw_title_ = std::move(fn); }
    void Draw(); //Main functional, start between begin-and in primary script
    TBTheme theme_;
private:
    bool  dragging_ = false;
    ImVec2 drag_offset_{0,0};
    std::unordered_map<uint32_t, TBButtonState> btn_;
    std::vector<uint32_t> order_;
    std::string title_; // Name programm on titelbar
    std::function<void(const ImRect&)> draw_icon_;
    std::function<void(const ImRect&, const std::string&)> draw_title_;
    static float AnimTo(float v, float target, float speed, float dt);
};

#endif
