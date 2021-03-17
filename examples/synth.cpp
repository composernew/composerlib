/*=============================================================================
   Copyright (c) 2014-2020 Joel de Guzman. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/

#include <array>
#include <audio/synthesizer.h>
#include <audio/midi_processor.h>
#include <audio/devices.h>

int main(){

    q::midi_input_stream::set_default_device(get_midi_device());
    auto audio_device_id = get_audio_device();

    auto env_cfg = q::envelope::config
        {
            100_ms      // attack rate
            , 1_s         // decay rate
            , -12_dB      // sustain level
            , 5_s         // sustain rate
            , 1_s         // release rate
        };

    sin_synth synth{env_cfg, audio_device_id};
    q::midi_input_stream stream;
    midi_processor proc{synth};

    if (!stream.is_valid())
        return -1;

    synth.start();
    while (running)
        stream.process(proc);
    synth.stop();

    return 0;
}