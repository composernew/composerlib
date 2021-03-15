/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License (https://opensource.org/licenses/MIT)
=============================================================================*/
#include <elements.hpp>
#include <sstream>
#include <iostream>
#include <array>

using namespace cycfi::elements;

// Main window background color
auto constexpr bkd_color = rgba(35, 35, 37, 255);
auto background = box(bkd_color);

using slider_ptr = std::shared_ptr<basic_slider_base>;
std::array<slider_ptr,3> vertical_sliders;

using dial_ptr = std::shared_ptr<dial_base>;
std::array<dial_ptr,6> dials;

using label_ptr = decltype(share(label("")));
std::array<label_ptr,9> labels;

template <typename E>
auto decorate(E&& e)
{
    return hsize(90, align_center(margin({ 20, 5, 20, 5 },
                  std::forward<E>(e)
    )));
}

template <bool is_vertical>
auto make_markers()
{
    auto track = basic_track<5, is_vertical>();
    return slider_labels<10>(
        slider_marks<40>(track),         // Track with marks
        0.8,                             // Label font size (relative size)
        "-5", "-4", "-3", "-2", "-1",         // Labels
        "0", "1", "2", "3", "4", "5"
    );
}

auto make_label(std::string label_name){
    return vmargin({20,20},label(label_name));
}

auto make_text(int index, std::string default_value){

    labels[index] = share(label(default_value));

    return layer(
                decorate(hold(labels[index])),
                frame{}
    );
}

auto make_vslider(int index)
{
    vertical_sliders[index] = share(slider(
        basic_thumb<25>(),
        make_markers<true>(),
        (index + 1) * 0.25
    ));
    return align_center(hold(vertical_sliders[index]));
}

auto make_vsliders()
{
    return margin({20,0,20,20},
                        htile(
                             vtile(
                                 make_label("Valence"),
                                 make_vslider(0),
                                 margin({20,20,20,0}, make_text(0, "-5.0"))
                             ),
                             vtile(
                                 make_label("Arousal"),
                                 make_vslider(1),
                                 margin({20,20,20,0}, make_text(1, "-5.0"))
                             ),
                             vtile(
                                 make_label("Originality"),
                                 make_vslider(2),
                                 margin({20,20,20,0}, make_text(2, "-5.0"))
                             )
                        )
    );
}

auto make_dial(int index)
{
    dials[index] = share(
        dial(
            radial_marks<20>(basic_knob<50>()),
            (index + 1) * 0.25
        )
    );

    auto markers = radial_labels<15>(
        hold(dials[index]),
        0.7,                                   // Label font size (relative size)
        "0", "1", "2", "3", "4",               // Labels
        "5", "6", "7", "8", "9", "10"
    );

    return align_center_middle(hmargin({20,20},markers));
}

auto make_dials()
{
    return margin({20,0,20,20},
                   htile(
                       vtile(
                            make_label("Tempo"),
                            make_dial(0),
                            margin({20,20,20,0}, make_text(3, "0")),
                            make_label("Dynamics"),
                            make_dial(1),
                            margin({20,20,20,0}, make_text(4, "0")),
                            make_label("Pitch"),
                            make_dial(2),
                            margin({20,20,20,0}, make_text(5, "0"))
                       ),
                       vtile(
                            make_label("Timbre"),
                            make_dial(3),
                            margin({20,20,20,0}, make_text(6, "0")),
                            make_label("Rhythm"),
                            make_dial(4),
                            margin({20,20,20,0}, make_text(7, "0")),
                            make_label("Label"),
                            make_dial(5),
                            margin({20,20,20,0}, make_text(8, "0"))
                       )
                   )
           );
}

auto make_buttons(){

    auto mbutton = button("Compose", 1.2);

    return
         vtile(
             margin({20, 0, 20, 20},
                        htile(
                            icon_button(icons::pause, 1.2),
                            icon_button(icons::play, 1.2),
                            right_margin(20, icon_button(icons::stop, 1.2)),
                            fixed_size(
                                {140,20},
                                mbutton
                            )
                        )
             )
        );
}

auto make_controls()
{
    return
        margin({ 20, 10, 20, 10 },
               vmin_size(400,
                             htile(
                                 vstretch(0.5, margin({ 10, 10, 10, 10 }, pane("Controllers", make_vsliders(), 1.0f))),
                                 vtile(
                                     hstretch(0.5, margin({ 10, 10, 10, 10 }, pane("Features", make_dials(), 1.0f))),
                                     margin({ 10, 10, 10, 10 }, pane("Player", make_buttons()))
                                 )
                             )
               )
        );
}

void dial_value(int dial_index, double val, view &view_){

    dials[dial_index]->dial_base::value(val);
    view_.refresh(*dials[dial_index]);

    labels[(dial_index+3)]->set_text(std::to_string(val));
    view_.refresh(*labels[(dial_index+3)]);
}

void slider_value(int slider_index, double val, view &view_){

    vertical_sliders[slider_index]->slider_base::value(val);
    view_.refresh(*vertical_sliders[slider_index]);

    labels[slider_index]->set_text(std::to_string(val));
    view_.refresh(*labels[slider_index]);
}

void link_control(int index, view& view_)
{
    if(index <= 2){

        vertical_sliders[index]->on_change =
            [index, &view_](double val){

                labels[index]->set_text(std::to_string(val));
                view_.refresh(*labels[index]);

                if(index == 0){
                    dial_value(1, val, view_);
                    dial_value(3, val, view_);
                    dial_value(4, val, view_);
                }

                if(index == 1){
                    dial_value(0, val, view_);
                    dial_value(2, val, view_);
                    dial_value(3, val, view_);
                    dial_value(4, val, view_);
                    dial_value(5, val, view_);
                }
            };
    }

    int dials_index = index + 3;

    dials[index]->on_change =
        [index, dials_index, &view_](double val){

            labels[dials_index]->set_text(std::to_string(val));
            view_.refresh(*labels[dials_index]);

            if(index == 0 || index == 2 || index == 3 || index == 4 || index == 5){
                slider_value(1, val,view_);
                dial_value(2, val, view_);
                dial_value(3, val, view_);
                dial_value(4, val, view_);
                dial_value(5, val, view_);
            }

            if(index == 1 || index == 3 || index == 4){
                slider_value(0, val,view_);
                dial_value(3, val, view_);
                dial_value(4, val, view_);
            }
        };
}

void link_controls(view& view_)
{
    link_control(0, view_);
    link_control(1, view_);
    link_control(2, view_);
    link_control(3, view_);
    link_control(4, view_);
    link_control(5, view_);
}

int main(int argc, char* argv[])
{
    app _app(argc, argv, "Composer", "com.cycfi.basic-sliders-and-knobs");
    window _win(_app.name());
    _win.on_close = [&_app]() { _app.stop(); };

    view view_(_win);

    view_.content(
        make_controls(),
        background

    );

    link_controls(view_);
    _app.run();
    return 0;
}