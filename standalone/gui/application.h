//
// Created by Carla on 15/03/2021.
//

#ifndef COMPOSER_APPLICATION_H
#define COMPOSER_APPLICATION_H

#include <elements.hpp>
#include <sstream>
#include <iostream>
#include <array>

class application {

    template <typename E>
    auto decorate(E&& e);

    template <bool is_vertical>
    auto make_markers();

    static auto make_label(std::string const &label_name);
    auto make_text(int index, std::string const &default_value);
    auto make_dial(int index);
    auto make_vertical_slider(int index);

    auto make_control(std::string const &label_name, int slider_index, int text_index);

    auto make_controllers();
    auto make_features();
    static auto make_player();

    void dial_value(int dial_index, double val, cycfi::elements::view &view_);
    void slider_value(int slider_index, double val, cycfi::elements::view &view_);
    void link_control(int index, cycfi::elements::view &view_);

    void link_controls(cycfi::elements::view& view_);

    using dial_ptr = std::shared_ptr<cycfi::elements::dial_base>;
    std::array<dial_ptr,6> dials;

    using label_ptr = decltype(share(cycfi::elements::label("")));
    std::array<label_ptr,9> labels;

    using slider_ptr = std::shared_ptr<cycfi::elements::basic_slider_base>;
    std::array<slider_ptr,3> vertical_sliders;

  public:

    application(const auto &background_color, cycfi::elements::app &_app) :
        _background(background_color),
        _window(_app.name()),
        _view(_window)
    {init_application(_app);}

    auto make_application();

    void init_application(cycfi::elements::app &_app); //initialize the application

    cycfi::elements::box_element    _background;
    cycfi::elements::window         _window;
    cycfi::elements::view           _view;
};

#endif // COMPOSER_APPLICATION_H
