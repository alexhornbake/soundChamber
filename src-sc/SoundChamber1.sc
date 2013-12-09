(
    ~port = 57121;
    ~appName = "/soundchamber";

    //Synth factory function definition
    ~oscSynthFactory = { |sender, func|

        //New control bus
        var bus = Bus.control(s);
        bus.set(0);

        //New synth with given SynthDef; map in the control bus
        {
            SynthDef.new(sender, func).add;
            s.sync;
            Synth.new(sender).map(\val, bus);
        }.fork;

        //New OSC reciever feeding incoming values to conntrol bus
        OSCFunc.newMatching(
            path: ~appName ++ sender, recvPort: ~port,
            func: { |msg|
                var user = msg[1];
                var value = msg[2];
                bus.set(value);
                (sender + "--> User: " ++ user).postln;
                (sender + "--> Value: " ++ value).postln;
            }
        );
    };

    //Call synth factory for each sender
    ~oscSynthFactory.value("/distancebetweenhands", {|val = 0|
        Out.ar(0, SinOsc.ar(440, 0, val));
    });

    /*~oscSynthFactory.value("/handheightsavg", {|val = 0|
        Out.ar(0, SinOsc.ar(440, 0, val));
    });

    ~oscSynthFactory.value("/distancefromsensor", {|val = 0|
        Out.ar(0, SinOsc.ar(440, 0, val));
    });*/
)