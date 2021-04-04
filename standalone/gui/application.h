//
// Created by Carla on 15/03/2021.
//

#ifndef COMPOSER_APPLICATION_H
#define COMPOSER_APPLICATION_H

#include <elements.hpp>
#include <sstream>
#include <iostream>
#include <vector>

class application {

    template <typename E>
    auto decorate(E&& e);

    template <bool is_vertical>
    auto make_markers();

    static auto make_label(std::string const &label_name);
    auto make_text(std::string const &default_value);
    auto make_dial();
    auto make_toggle_button(std::string const &label_name, cycfi::elements::color color);
    auto make_toggle_buttons();
    auto make_vertical_slider();
    auto make_control(std::string const &label_name);
    auto make_feature(std::string const &name);

    auto make_controllers();
    auto make_features();
    static auto make_player();

    void dial_value(int dial_index, double value, cycfi::elements::view &view_);
    void slider_value(int slider_index, double value, cycfi::elements::view &view_);
    void toggle_button_value(size_t index, bool status, cycfi::elements::view &view_);
    void toggle_button_values(size_t index, double value, cycfi::elements::view &view_);

    void link_sliders(int index, cycfi::elements::view &view_);
    void link_dials(int index, cycfi::elements::view &view_);
    void link_buttons(int index, cycfi::elements::view &view_);
    void link_controls(cycfi::elements::view& view_);

    using dial_ptr = std::shared_ptr<cycfi::elements::dial_base>;
    std::vector<dial_ptr> dials;

    using label_ptr = decltype(share(cycfi::elements::label("")));
    std::vector<label_ptr> labels;

    using slider_ptr = std::shared_ptr<cycfi::elements::basic_slider_base>;
    std::vector<slider_ptr> vertical_sliders;

    using button_ptr = std::shared_ptr<cycfi::elements::basic_toggle_button<>>;
    std::vector<button_ptr> toggle_buttons;

    // Colors
    static constexpr auto bgreen = cycfi::elements::colors::green.level(0.7).opacity(0.4);
    static constexpr auto bred   = cycfi::elements::colors::red.opacity(0.4);

    cycfi::elements::app _app;

  public:

    application(const auto &background_color, int argc, char* argv[]) :
        _background(background_color),
        _view(_window),
        _app(argc, argv, "Composer", "com.composer.composer"),
        _window(_app.name())
    {initialize_application();}

    auto make_application();

    void initialize_application(); //initialize the application

    void run();

    cycfi::elements::box_element    _background;
    cycfi::elements::window         _window;
    cycfi::elements::view           _view;
};

#endif // COMPOSER_APPLICATION_H