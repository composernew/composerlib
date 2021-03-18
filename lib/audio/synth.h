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

    void set(cycfi::q::frequency const &freq, int const &dur);

    cycfi::q::phase_iterator   phase;   // The phase iterator

  public:

    explicit synth(int timbre = 0)
        : port_audio_stream( 0, 2)
        , _timbre(timbre)
    {}

    void play(std::vector<std::pair<int,cycfi::q::frequency>> &notes);

    int _timbre; // sine, triangle, square or saw
};

#endif // COMPOSER_SYNTH_H
