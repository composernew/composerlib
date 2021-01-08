/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License (https://opensource.org/licenses/MIT)
=============================================================================*/
#include <elements.hpp>
#include <sstream>

using namespace cycfi::elements;

// Main window background color
auto constexpr bkd_color = rgba(35, 35, 37, 255);
auto background = box(bkd_color);

using slider_ptr = std::shared_ptr<basic_slider_base>;
slider_ptr vsliders[3];

using dial_ptr = std::shared_ptr<dial_base>;
dial_ptr dials[6];

template <typename E>
auto decorate(E&& e)
{
    return hsize(170, align_center(margin({ 25, 5, 25, 5 },
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

auto make_text(std::string text = "")
{
    return decorate(heading(text)
                        .font_color(get_theme().indicator_hilite_color)
                        .font_size(24)
    );
};

auto make_thumbwheel(char const* unit, float offset, float scale, int precision)
{
    auto label = make_text();

    auto&& as_string =
        [=](double val)
        {
          std::ostringstream out;
          out.precision(precision);
          out << std::fixed << ((val * scale) + offset) << unit;
          return out.str();
        };

    auto tw = share(thumbwheel(as_label<double>(as_string, label)));

    return top_margin(20,
                      layer(
                          hold(tw),
                          frame{}
                      )
    );
}

auto make_vslider(int index)
{
    vsliders[index] = share(slider(
        basic_thumb<25>(),
        make_markers<true>(),
        (index + 1) * 0.25
    ));
    return align_center(margin({ 20, 20, 20, 20 }, hold(vsliders[index])));
}

auto make_vsliders()
{
    return hmin_size(300,
                         htile(
                             vtile(
                                make_vslider(0),
                                make_label("Valence"),
                                make_thumbwheel(" ", 0, 10, 1)
                             ),
                             vtile(
                                make_vslider(1),
                                make_label("Arousal"),
                                make_thumbwheel(" ", 0, 10, 1)
                             ),
                             vtile(
                                 make_vslider(2),
                                 make_label("Originality"),
                                 make_thumbwheel(" ", 0, 10, 1)
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
    return hmargin(20,
                   htile(
                       vtile(
                           make_dial(0),
                           make_label("Tempo"),
                           make_thumbwheel(" ", 0, 10, 1),
                           make_dial(1),
                           make_label("Dynamics"),
                           make_thumbwheel(" ", 0, 10, 1),
                           make_dial(2),
                           make_label("Pitch"),
                           make_thumbwheel(" ", 0, 10, 1)
                       ),
                       vtile(
                            make_dial(3),
                            make_label("Timbre"),
                            make_thumbwheel(" ", 0, 10, 1),
                            make_dial(4),
                            make_label("Rhythm"),
                            make_thumbwheel(" ", 0, 10, 1),
                            make_dial(5),
                            make_label("Label"),
                            make_thumbwheel(" ", 0, 10, 1)
                       )
                   )
           );
}

auto make_controls()
{
    auto mbutton = button("Compose");

    auto  icon_buttons =
        group("Player",
              margin({ 10, 10, 20, 10 },
                     vtile(
                         top_margin(35,
                                    htile(
                                        align_center(icon_button(icons::pause, 1.2)),
                                        align_center(icon_button(icons::play, 1.2)),
                                        align_center(icon_button(icons::stop, 1.2)),
                                        align_center(mbutton)
                                    )
                         )
                     )
              )
        );

    return
        margin({ 20, 10, 20, 10 },
               vmin_size(400,
                         vtile(
                             htile(
                                 margin({ 20, 20, 20, 20 }, pane("Controllers", make_vsliders(), 0.8f)),
                                 hstretch(0.5, margin({ 20, 20, 20, 20 }, pane("Features", make_dials(), 0.8f)))
                             ),
                             hmin_size(250, margin({ 20, 20, 20, 20 }, icon_buttons))
                         )
               )
        );
}

void link_control(int index, view& view_)
{
    vsliders[index]->on_change =
        [index, &view_](double val)
        {

          if(index == 0){
              dials[1]->dial_base::value(val);
              view_.refresh(*dials[1]);

              dials[3]->dial_base::value(val);
              view_.refresh(*dials[3]);

              dials[4]->dial_base::value(val);
              view_.refresh(*dials[4]);

              dials[5]->dial_base::value(val);
              view_.refresh(*dials[5]);
          }

          if(index == 1){
              dials[0]->dial_base::value(val);
              view_.refresh(*dials[0]);

              dials[2]->dial_base::value(val);
              view_.refresh(*dials[2]);

              dials[3]->dial_base::value(val);
              view_.refresh(*dials[3]);

              dials[4]->dial_base::value(val);
              view_.refresh(*dials[4]);
          }

        };

    dials[index]->on_change =
        [index, &view_](double val)
        {
            if(index == 0){
                vsliders[1]->slider_base::value(val);
                view_.refresh(*vsliders[1]);
            }
            if(index == 1){
                vsliders[0]->slider_base::value(val);
                view_.refresh(*vsliders[0]);
            }
            if(index == 2){
                vsliders[1]->slider_base::value(val);
                view_.refresh(*vsliders[1]);
            }
        };
}

void link_controls(view& view_)
{
    link_control(0, view_);
    link_control(1, view_);
    link_control(2, view_);
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