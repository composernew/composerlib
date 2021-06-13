//
// Created by Carla on 08/05/2021.
//

#include "midi_stream.h"
#include <iostream>
#include <sstream>

auto to_raw_message(smf::MidiMessage &event) {

    int x;
    cycfi::q::midi::raw_message message{};
    std::stringstream stream;

    // Converts messages to hexadecimal
    for(size_t i = event.size(); i > 0; --i){
        stream << std::hex << static_cast<int>(event[i-1]);
    }

    stream >> x;

    message.data = x;

    return message;
}

bool is_harmony(smf::MidiEventList &event_list) {

    for (int event = 1; event < event_list.size(); ++event) {
        if (event_list[event][0] == note_on && event_list[event-1][0] == note_on) {
            return true;
        }
    }

    return false;
}

void midi_stream::process(midi_processor &processor) {

    // Compositions with harmonies will not be supported at first
    if(is_harmony()){
        std::cout << "This MIDI file contains harmonies." << std::endl;
        return;
    }

    event_list.linkNotePairs();

    for (int event = 0; event < event_list.size(); ++event) {

        cycfi::q::midi::raw_message message = to_raw_message(event_list[event]);
        cycfi::q::midi::dispatch(message, event_list[event].tick, processor);
        cycfi::q::sleep(event_list[event].getDurationInSeconds());
    }
}