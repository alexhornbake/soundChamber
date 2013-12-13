(
        //Synth for one instance per each user
        SynthDef.new(\soundChamber, {
	        var val1 = MouseX.kr(0, 1);
            var val2 = MouseY.kr(0, 1);
            var val3 = SinOsc(0.5, 0, 0.5, 0.5);
            var gate = 1;

            var freq = LinLin.kr(val1, 0, 1, 220, 440);
            var amp  = LinLin.kr(val2, 0, 1, 0, 1);
            var pan  = LinLin.kr(val3, 0, 1, -1, 1);
            var out  = Pan2.ar(SinOsc.ar(freq, 0, amp), pan);
            var env  = Env.asr(0.3, 1, 0.3);

            Out.ar(0, out * EnvGen.ar(env, gate: gate, doneAction: 2));
        }).play;
)

(
        //Synth for one instance per each user
        SynthDef.new(\brownAndDust, {
	        var val1 = MouseX.kr(0, 1);
            var val2 = SinOsc.kr(0.5, 0, 0.2, 0.7);
            var val3 = MouseY.kr(0, 1);
            var gate = 1;

            var brown = (
                Splay.ar(
                    Resonz.ar(
                        BrownNoise.ar,
                        { LinLin.kr(val3, 0, 1, 120, 50) } !5,
                        0.01
                    )
                ).distort
            ) * 8;

            var dust = 0.5 * (
		        Decay.ar(
                    GVerb.ar(
                        Mix.ar(
                            Dust.ar(
            			        {
            				        LinExp.kr(val1, 0, 1, 8, 0.5)
                                } !9,
                                0.75
                            ).ring3(
                                SinOsc.ar({rrand(30.1 ,66)}!5) ** Blip.ar(77.3)
                            )
                        ),
			            revtime: 6,
                        spread: 4
                    ).tanh,
                    decayTime: 0.001
		        )
            );

            var out  = Mix.new([dust, brown]) * val2;
            var env  = Env.asr(0.3, 1, 0.3);

            Out.ar(0, out * EnvGen.ar(env, gate: gate, doneAction: 2));
        }).play;
)