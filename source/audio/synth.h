#ifndef COMPOSER_SYNTH_H
#define COMPOSER_SYNTH_H

#include <q/synth/sin.hpp>
#include <q/synth/square.hpp>
#include <q/synth/triangle.hpp>
#include <q/synth/saw.hpp>
#include <q/support/literals.hpp>
#include <q_io/audio_stream.hpp>
#include <vector>

// Based on Q 'sin_synth' example - https://github.com/cycfi/q/blob/master/example/sin_synth.cpp

class synth : public cycfi::q::port_audio_stream
{
    void process(out_channels const& out) override;



    cycfi::q::phase_iterator   phase;   // The phase iterator

  public:

    enum class timbre {sine, triangle, square, saw};

    timbre timbre_;

    explicit synth(timbre timbre = timbre::sine)
        : port_audio_stream( 0, 2)
        , timbre_(timbre)
    {}

    void play(std::vector<std::tuple<int,cycfi::q::frequency,cycfi::q::duration>> &notes);

    void set_timbre(timbre timbre);

    void set(cycfi::q::frequency const &freq, cycfi::q::duration const &dur);
};

#endif // COMPOSER_SYNTH_H