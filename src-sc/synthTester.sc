(
        //Synth for one instance per each user
        SynthDef.new(\soundChamber, { |val1, val2, val3, gate|

            val1 = MouseX.kr(0, 1);
            val2 = MouseY.kr(0, 1);
            val3 = SinOsc(0.5, 0, 0.5, 0.5);
            gate = 1;

            var freq = LinLin.kr(val1, 0, 1, 220, 440);
            var amp  = LinLin.kr(val2, 0, 1, 0, 1);
            var pan  = LinLin.kr(val3, 0, 1, -1, 1);
            var out  = Pan2.ar(SinOsc.ar(freq, 0, amp), pan);
            var env  = Env.asr(0.3, 1, 0.3);

            Out.ar(0, out * EnvGen.ar(env, gate: gate, doneAction: 2));
        }).play;
)