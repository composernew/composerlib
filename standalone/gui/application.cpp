//
// Created by Carla on 15/03/2021.
//

#include "application.h"

template <typename E>
auto application::decorate(E&& e)
{
    return hsize(90, align_center(margin({ 20, 5, 20, 5 },
                                         std::forward<E>(e)
    )));
}

template <bool is_vertical>
auto application::make_markers()
{
    auto track = cycfi::elements::basic_track<5, is_vertical>();
    return cycfi::elements::slider_labels<10>(
        cycfi::elements::slider_marks<40>(track),         // Track with marks
        0.8,                             // Label font size (relative size)
        "-5", "-4", "-3", "-2", "-1",    // Labels
        "0", "1", "2", "3", "4", "5"
    );
}

auto application::make_label(std::string const &label_name){
    return vmargin({20.,20.},cycfi::elements::label(label_name));
}

auto application::make_text(std::string const &default_value){

    labels.emplace_back(cycfi::elements::share(cycfi::elements::label(default_value)));

    return layer(
        hold(labels.back()),
        cycfi::elements::frame{}
    );
}

auto application::make_dial()
{
    dials.emplace_back(share(
        dial(
            radial_marks<20>(cycfi::elements::basic_knob<50>()),
            0.5
        ))
    );

    auto markers = radial_labels<15>(
        hold(dials.back()),
        0.7,                                   // Label font size (relative size)
        "0", "1", "2", "3", "4",               // Labels
        "5", "6", "7", "8", "9", "10"
    );

    return align_center_middle(hmargin({20.,20.},markers));
}

auto application::make_toggle_button(std::string const &label_name, cycfi::elements::color color) {

    toggle_buttons.emplace_back(cycfi::elements::share(toggle_button(label_name, 1.0, color)));
    
    return margin({10.,10.,10.,0.},hold(toggle_buttons.back()));
}

auto application::make_toggle_buttons() {

    return
        vmargin(
            {60.,20.},
            vtile(
                 make_toggle_button("Alert", bgreen),
                 make_toggle_button("Excited", bgreen),
                 make_toggle_button("Enthusiastic", bgreen),
                 make_toggle_button("Elated", bgreen),
                 make_toggle_button("Happy", bgreen),
                 make_toggle_button("Contented", bgreen),
                 make_toggle_button("Serene", bgreen),
                 make_toggle_button("Relaxed", bgreen),
                 make_toggle_button("Calm", bgreen),
                 make_toggle_button("Bored", bred),
                 make_toggle_button("Sluggish", bred),
                 make_toggle_button("Depressed", bred),
                 make_toggle_button("Sad", bred),
                 make_toggle_button("Upset", bred),
                 make_toggle_button("Stressed", bred),
                 make_toggle_button("Nervous", bred),
                 make_toggle_button("Tense", bred)
            )
        );
}

auto application::make_vertical_slider() {
    vertical_sliders.emplace_back(cycfi::elements::share(slider(
        cycfi::elements::basic_thumb<25>(),
        make_markers<true>(),
        0.5
    )));

    return align_center(hold(vertical_sliders.back()));
}

auto application::make_control(const std::string &label_name) {
    return
        vtile(
            make_label(label_name),
            make_vertical_slider(),
            cycfi::elements::margin({20.,20.,20.,0.}, make_text("-5.0"))
        );
}

auto application::make_player() {
    auto mbutton = cycfi::elements::button("Compose", 1.2);

    return
        vtile(
            margin({20., 0., 20., 20.},
                   htile(
                       cycfi::elements::icon_button(cycfi::elements::icons::pause, 1.2),
                       cycfi::elements::icon_button(cycfi::elements::icons::play, 1.2),
                       right_margin(20.0, cycfi::elements::icon_button(cycfi::elements::icons::stop, 1.2)),
                       fixed_size(
                           {140.0,20.0},
                           mbutton
                       )
                   )
            )
        );
}

auto application::make_feature(std::string const  &name) {

    return vtile(make_label(name),
    make_dial(),
    cycfi::elements::margin({20.,20.,20.,0.}, make_text("0")));
}

auto application::make_features() {
    return margin({20.,0.,20.,20.},
                  htile(
                      vtile(
                          make_feature("Tempo"),
                          make_feature("Dynamics"),
                          make_feature("Pitch")
                      ),
                      vtile(
                          make_feature("Timbre"),
                          make_feature("Rhythm"),
                          make_feature("Label")
                      )
                  )
    );
}

auto application::make_controllers() {

    return margin({20.,0.,20.,20.},
                  cycfi::elements::htile(
                      make_control("Valence"),
                      make_control("Arousal"),
                      make_control("Originality")
                  )
    );
}

auto application::make_application() {

    return
        margin({ 20., 10., 20., 10. },
               vmin_size(400.,
                         htile(
                             valign(0.5, margin({ 10., 10., 10., 10. }, pane("Emotions",
                                                make_toggle_buttons(), 1.0F))),
                             vstretch(0.5, margin({ 10., 10., 10., 10. }, pane("Controllers", make_controllers(), 1.0F))),
                             vtile(
                                 hstretch(0.5, margin({ 10., 10., 10., 10. }, cycfi::elements::pane("Features", make_features(), 1.0F))),
                                 margin({ 10., 10., 10., 10. }, cycfi::elements::pane("Player", make_player()))
                             )
                         )
               )
        );
}

void application::dial_value(int dial_index, double value) {

    dials[dial_index]->dial_base::value(value);
    view_.refresh(*dials[dial_index]);

    labels[(dial_index+3)]->set_text(std::to_string(value));
    view_.refresh(*labels[(dial_index+3)]);
}

void application::slider_value(int slider_index, double value) {

    vertical_sliders[slider_index]->slider_base::value(value);
    view_.refresh(*vertical_sliders[slider_index]);

    labels[slider_index]->set_text(std::to_string(value));
    view_.refresh(*labels[slider_index]);
}

void application::toggle_button_value(size_t index, bool status) {
        toggle_buttons[index]->value(status);
        view_.refresh(*toggle_buttons[index]);
}

void application::toggle_button_values(size_t index, double value) {

    if(index == 0){
        if(value > 0.5){

            toggle_button_value(0, true);
            toggle_button_value(2, true);
            toggle_button_value(9, false);
            toggle_button_value(11, false);
        }
        else{
            toggle_button_value(0, false);
            toggle_button_value(2, false);
            toggle_button_value(9, true);
            toggle_button_value(11,true);
        }
    }

    if(index == 1){
        if(value > 0.5){

            toggle_button_value(1, true);
            toggle_button_value(3, true);
            toggle_button_value(10, false);
            toggle_button_value(12, false);
        }
        else{
            toggle_button_value(1, false);
            toggle_button_value(3, false);
            toggle_button_value(10, true);
            toggle_button_value(12, true);
        }
    }
}

void application::link_sliders(int index) {

    vertical_sliders[index]->on_change =
        [index, this](double val){

          labels[index]->set_text(std::to_string(val));
          view_.refresh(*labels[index]);

          if(index == 0){
              dial_value(1, val);
              dial_value(3, val);
              dial_value(4, val);
              toggle_button_values(index, val);
          }

          if(index == 1){
              dial_value(0, val);
              dial_value(2, val);
              dial_value(3, val);
              dial_value(4, val);
              dial_value(5, val);
              toggle_button_values(index, val);
          }
        };
}

void application::link_dials(int index) {
    int dials_index = index + 3;

    dials[index]->on_change =
        [index, dials_index, this](double val){

          labels[dials_index]->set_text(std::to_string(val));
          view_.refresh(*labels[dials_index]);

          if(index == 0 || index == 2 || index == 3 || index == 4 || index == 5){
              slider_value(1, val);
              dial_value(2, val);
              dial_value(3, val);
              dial_value(4, val);
              dial_value(5, val);
              toggle_button_values(1, val);
          }

          if(index == 1 || index == 3 || index == 4){
              slider_value(0, val);
              dial_value(3, val);
              dial_value(4, val);
              toggle_button_values(0, val);
          }
        };
}

void application::link_buttons(int index) {

    double size          = toggle_buttons.size();
    double value         = (size - index)*(1.0/size);
    double value_arousal = index*(1.0/size);

    toggle_buttons[index]->on_click =
        [index, value, value_arousal, this](double status) {

              slider_value(0, value);
              dial_value(0,value);
              dial_value(1,value);
              dial_value(2,value);
              dial_value(3,value_arousal);
              dial_value(4,value_arousal);
              dial_value(5,value_arousal);

              if(index <= toggle_buttons.size()/2){
                  slider_value(1, value);
              }
              else{
                  slider_value(1, value_arousal);
              }
        };
}

void application::link_controls() {

    link_sliders(0);
    link_sliders(1);
    link_sliders(2);
    link_dials(0);
    link_dials(1);
    link_dials(2);
    link_dials(3);
    link_dials(4);
    link_dials(5);

    for(size_t i = 0; i < toggle_buttons.size(); ++i){
        link_buttons(i);
    }
}

// Initialize the application
// The application will stop when the window is closed
void application::initialize_application() {

    window_.on_close = [this]() { app_.stop(); };

    view_.content(
        make_application(), background_);

    link_controls();
}

void application::run() { app_.run();
}