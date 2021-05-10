//
// Created by Carla on 08/05/2021.
//

#include "midi_stream.h"
#include <iostream>

auto midi_stream::convert_message(smf::MidiMessage &event) {

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

void midi_stream::process(midi_processor &processor) {

    event_list.linkNotePairs();

    for (int event = 0; event < event_list.size(); ++event) {

        cycfi::q::midi::raw_message message = convert_message(event_list[event]);
        cycfi::q::midi::dispatch(message, event_list[event].tick, processor);
        cycfi::q::sleep(event_list[event].getDurationInSeconds());
    }
}