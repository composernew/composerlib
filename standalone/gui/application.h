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

    // Labels
    static auto make_title_label(std::string const &label_name);
    auto push_value_label(std::string const &default_value);

    // GUI components
    auto push_mood_button(std::string const &label_name, cycfi::elements::color color);
    auto push_vertical_slider();
    auto push_dial();
    auto make_composition_control(std::string const &label_name);
    auto make_feature(std::string const &name);

    // GUI
    auto make_mood_buttons();
    auto make_composition_controls();
    auto make_features();
    static auto make_player();

    // Components values
    void feature_value(int dial_index, double value);
    void control_value(int slider_index, double value);
    void mood_button_value(size_t index, bool status);
    void mood_button_values(size_t index, double value);

    // Links
    void link_sliders(int index);
    void link_features(int index);
    void link_mood_buttons(int index);
    void link_controls();

    using dial_ptr = std::shared_ptr<cycfi::elements::dial_base>;
    std::vector<dial_ptr> dials;

    using label_ptr = std::shared_ptr<cycfi::elements::basic_label>;
    std::vector<label_ptr> labels;

    using slider_ptr = std::shared_ptr<cycfi::elements::basic_slider_base>;
    std::vector<slider_ptr> vertical_sliders;

    using button_ptr = std::shared_ptr<cycfi::elements::basic_toggle_button<>>;
    std::vector<button_ptr> mood_buttons;

    // Colors
    static constexpr auto green = cycfi::elements::colors::green.level(0.7).opacity(0.4);
    static constexpr auto red   = cycfi::elements::colors::red.opacity(0.4);

    cycfi::elements::app app_;

  public:

    application(const auto &background_color, int argc, char** argv) :
        background_(background_color), view_(window_),
        app_(argc, argv, "Composer", "com.composer.composer"),
        window_(app_.name())
    {initialize_application();}

    auto make_application();

    void initialize_application(); //initialize the application

    void run();

    cycfi::elements::box_element    background_;
    cycfi::elements::window         window_;
    cycfi::elements::view           view_;
};

#endif // COMPOSER_APPLICATION_H