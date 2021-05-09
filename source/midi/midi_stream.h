//
// Created by Carla on 08/05/2021.
//

#ifndef COMPOSER_MIDI_STREAM_H
#define COMPOSER_MIDI_STREAM_H

#include <MidiEventList.h>
#include <MidiMessage.h>
#include <utility>
#include "midi_processor.h"

class midi_stream {

    static auto convert_message(smf::MidiMessage &event);

    smf::MidiEventList event_list;

  public:

    explicit midi_stream(smf::MidiEventList eventList)
        : event_list(std::move(eventList)) {}

    void process(midi_processor &processor);
};

#endif // COMPOSER_MIDI_STREAM_H
