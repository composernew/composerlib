//
// Created by Carla on 08/05/2021.
//

#ifndef COMPOSER_MIDI_STREAM_H
#define COMPOSER_MIDI_STREAM_H

#include <MidiEventList.h>
#include <MidiMessage.h>
#include <utility>
#include "midi_processor.h"

// This library uses the midi_processor class to process a smf::MidiEventList.
// That way is possible to cycfi::q::midi to handle all MIDI messages to play the event list.

auto to_raw_message(smf::MidiMessage &event);
bool is_harmony(smf::MidiEventList &event_list);
void process(midi_processor &processor, smf::MidiEventList &event_list);

#endif // COMPOSER_MIDI_STREAM_H
