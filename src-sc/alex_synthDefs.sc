//BASS CHOPPER
(
        //Synth for one instance per each user
        SynthDef.new(\bassChopper, {
	        var val1 = MouseX.kr(0, 1);
            var val2 = SinOsc.kr(0.5, 0, 0.2, 0.7);
            var val3 = MouseY.kr(0, 1);
            var gate = 1;

            var freq = LinLin.kr(val3, 0, 1, 40, 80);
	        var freq2 = LinLin.kr(val3, 0, 1, 100, 200);
	        var amp  = LFPulse.kr(LinLin.kr(val1, 0, 1, 12, 1), 0.5, 0.5);
            var env  = Env.asr(0.3, 1, 0.3);

	        var out = Pan2.ar(
		                (PMOsc.ar(freq) +
			            (SyncSaw.ar(freq2)*0.05) +
			            (BrownNoise.ar()*0.06))
		                * amp, 0);

            Out.ar(0, val2 * out * EnvGen.ar(env, gate: gate, doneAction: 2));
        }).play;
)

//TRANSFORMER
(
        //Synth for one instance per each user
        SynthDef.new(\transformer, {
	        var val1 = MouseX.kr(0, 1);
            var val2 = MouseY.kr(0, 1);
            var val3 = SinOsc.kr(0.5, 0, 0.5, 0.5);
            var gate = 1;

            var freq = LinLin.kr(val1, 0, 1, 0.1, 0.001);
            var filter  = LinLin.kr(val2, 0, 1, 0.1, 1);
            var dur  = LinLin.kr(val3, 0, 1, -1, 1);
            var env  = Env.asr(0.3, 1, 0.3);
	        var out = MoogFF.ar(
                        PitchShift.ar(
		                  CombN.ar(
			                Formant.ar(
					          101, // fundamental
				              4**LFNoise1.kr(freq)*450, // formant freq
				              200*freq), // pulse-width (bw) frequency
			                  freq, // CombN maxdelay time
			                  0.5*freq, // CombN delaytime
			                  99), // CombN decaytime
		                      1, // PitchShift windowSize
			                  Duty.kr( // PitchShift pitchRatio (1-4)
			                  4*dur, // dur
			                  0, // reset
			                  Dseq([[6,8,10],[6,7.2,7]]/8,
					          inf))).sum/25!2, 100+(5000*filter));

            Out.ar(0, 0.4 * out * EnvGen.ar(env, gate: gate, doneAction: 2));
        }).play;
)