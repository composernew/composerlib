//
// Created by Carla on 15/03/2021.
//

#include "application.h"

template <typename E>
auto application::decorate(E&& e) {
    return hsize(100, align_center(vmargin({5, 5}, std::forward<E>(e))));
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

auto application::make_title_label(std::string const &label_name) {
    return bottom_margin({10.},cycfi::elements::label(label_name));
}

auto application::push_value_label(std::string const &default_value) {

    labels.emplace_back(share(cycfi::elements::label(default_value)));

    return margin({10., 10., 10., 10.},
           layer(
           decorate(hold(labels.back())),
           cycfi::elements::frame{}
    ));
}

auto application::push_dial() {

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

    return align_center_middle(markers);
}

auto application::push_mood_button(std::string const &label_name, cycfi::elements::color color) {

    mood_buttons.emplace_back(share(toggle_button(label_name, 1.0, color)));

    return margin({10.,0.,10.,10.},hold(mood_buttons.back()));
}

auto application::make_mood_buttons() {

    return
        vmargin(
            {10.,10.},
            vtile(
                push_mood_button("Alert", green),
                push_mood_button("Excited", green),
                push_mood_button("Enthusiastic", green),
                push_mood_button("Elated", green),
                push_mood_button("Happy", green),
                push_mood_button("Contented", green),
                push_mood_button("Serene", green),
                push_mood_button("Relaxed", green),
                push_mood_button("Calm", green),
                push_mood_button("Bored", red),
                push_mood_button("Sluggish", red),
                push_mood_button("Depressed", red),
                push_mood_button("Sad", red),
                push_mood_button("Upset", red),
                push_mood_button("Stressed", red),
                push_mood_button("Nervous", red),
                push_mood_button("Tense", red)
            )
        );
}

auto application::push_vertical_slider() {
    vertical_sliders.emplace_back(share(slider(
        cycfi::elements::basic_thumb<25>(),
        make_markers<true>(),
        0.5
    )));

    return align_center(hold(vertical_sliders.back()));
}

auto application::make_composition_control(const std::string &label_name) {
    return
        vtile(
            make_title_label(label_name),
            push_vertical_slider(),
            push_value_label("-5.0")
        );
}

auto application::make_player() {
    auto compose_button = cycfi::elements::button("Compose", 1.2);

    return
        vtile(
            margin({20., 0., 20., 20.},
                   htile(
                       cycfi::elements::icon_button(cycfi::elements::icons::pause, 1.2),
                       cycfi::elements::icon_button(cycfi::elements::icons::play, 1.2),
                       right_margin(20.0, cycfi::elements::icon_button(cycfi::elements::icons::stop, 1.2)),
                       fixed_size(
                           {140.0,20.0},
                           compose_button
                       )
                   )
            )
        );
}

auto application::make_feature(std::string const  &name) {
    return
        margin({10., 10., 10., 10.},
            vtile(
                make_title_label(name),
                push_dial(),
                push_value_label("0")
            )
        );
}

auto application::make_features() {
    return
          htile(
              vtile(
                  make_feature("Tempo"),
                  make_feature("Dynamics"),
                  make_feature("Pitch")
              ),
              align_right(
                  vtile(
                      make_feature("Timbre"),
                      make_feature("Rhythm"),
                      make_feature("Label")
                  )
              )
          );
}

auto application::make_composition_controls() {
    return
        margin({10., 10., 10., 10.},
            cycfi::elements::htile(
                make_composition_control("Valence"),
                make_composition_control("Arousal"),
                make_composition_control("Originality")
            )
        );
}

auto application::make_application() {
    return
        margin({10., 10., 10., 10.},
               vmin_size(400.,
                         htile(
                             vstretch(0.5,
                                    margin({ 10., 10., 10., 10.},
                                    pane("Emotions", make_mood_buttons(), 1.0F))
                             ),
                             vstretch(0.5,
                                      margin({10., 10., 10., 10.},
                                      pane("Controllers", make_composition_controls(), 1.0F))
                             ),
                             vtile(
                                 hstretch(0.5,
                                          margin({10., 10., 10., 10.},
                                          pane("Features", make_features(), 1.0F))
                                 ),
                                 margin({ 10., 10., 10., 10. }, pane("Player", make_player()))
                             )
                         )
               )
        );
}

void application::feature_value(int dial_index, double value) {

    dials[dial_index]->dial_base::value(value);
    view_.refresh(*dials[dial_index]);

    labels[dial_index]->set_text(std::to_string(value));
    view_.refresh(*labels[dial_index]);
}

void application::control_value(int slider_index, double value) {

    /* The index need to be calculated here because the application creates the
     * features first and then the controls
     */
    size_t label_index = dials.size() + slider_index;

    vertical_sliders[slider_index]->slider_base::value(value);
    view_.refresh(*vertical_sliders[slider_index]);

    labels[label_index]->set_text(std::to_string(value));
    view_.refresh(*labels[label_index]);
}

void application::mood_button_value(size_t index, bool status) {
    mood_buttons[index]->value(status);
        view_.refresh(*mood_buttons[index]);
}

void application::mood_button_values(size_t index, double value) {

    if(index == 0){
        if(value > 0.5){

            mood_button_value(0, true);
            mood_button_value(2, true);
            mood_button_value(9, false);
            mood_button_value(11, false);
        }
        else{
            mood_button_value(0, false);
            mood_button_value(2, false);
            mood_button_value(9, true);
            mood_button_value(11, true);
        }
    }

    if(index == 1){
        if(value > 0.5){

            mood_button_value(1, true);
            mood_button_value(3, true);
            mood_button_value(10, false);
            mood_button_value(12, false);
        }
        else{
            mood_button_value(1, false);
            mood_button_value(3, false);
            mood_button_value(10, true);
            mood_button_value(12, true);
        }
    }
}

void application::link_sliders(int index) {

    /* The index need to be calculated here because the application creates the
     * features first and then the controls
     */

    size_t label_index = dials.size() + index;

    vertical_sliders[index]->on_change =
        [index, label_index, this](double val){

          labels[label_index]->set_text(std::to_string(val));
          view_.refresh(*labels[label_index]);

          if(index == 2){
              feature_value(1, val);
              feature_value(0, val);
              feature_value(4, val);
              mood_button_values(index, val);
          }

          if(index == 1){
              feature_value(0, val);
              feature_value(2, val);
              feature_value(3, val);
              feature_value(1, val);
              feature_value(5, val);
              mood_button_values(index, val);
          }
        };
}

void application::link_features(int index) {

    dials[index]->on_change =
        [index, this](double val){

          labels[index]->set_text(std::to_string(val));
          view_.refresh(*labels[index]);

          if(index == 0 || index == 2 || index == 3 || index == 1 || index == 5){
              control_value(1, val);
              feature_value(2, val);
              feature_value(0, val);
              feature_value(1, val);
              feature_value(5, val);
              mood_button_values(1, val);
          }

          if(index == 1 || index == 0 || index == 4){
              control_value(2, val);
              feature_value(0, val);
              feature_value(1, val);
              mood_button_values(0, val);
          }
        };
}

void application::link_mood_buttons(int index) {

    double size          = mood_buttons.size();
    double value         = (size - index)*(1.0/size);
    double value_arousal = index*(1.0/size);

    mood_buttons[index]->on_click =
        [index, value, value_arousal, this](double status) {

              control_value(2, value);
              feature_value(3, value);
              feature_value(4, value);
              feature_value(5, value);
              feature_value(0, value_arousal);
              feature_value(1, value_arousal);
              feature_value(2, value_arousal);

              if(index <= mood_buttons.size()/2){
                  control_value(1, value);
              }
              else{
                  control_value(1, value_arousal);
              }
        };
}

void application::link_components() {

    for(size_t i = 0; i < vertical_sliders.size(); ++i){
        link_sliders(i);
    }

    for(size_t i = 0; i < dials.size(); ++i){
        link_features(i);
    }

    for(size_t i = 0; i < mood_buttons.size(); ++i){
        link_mood_buttons(i);
    }
}

// Initialize the application
// The application will stop when the window is closed
void application::initialize_application() {

    window_.on_close = [this]() { app_.stop(); };

    view_.content(
        make_application(), background_);

    link_components();
}

void application::run() {
    app_.run();
}