/*
    SoundChamber 1
    ==============
    - Read in OSC values from the given port and addresses.
    - For each distinct userId detected, create a new synth instance.
    - Bus all subsequent normalised values to the appropriate argument in the synth
      (as defined by the mapping-key in the address).

    OSC message format:
    ------------------
    Address: /appName/mapping-key
    Value 1: integer userId
    Value 2: normalised value
*/
(
    //Global variables
    ~port = 57121;
    ~appName = "/soundchamber";
    ~debug = true;

    ~users = Dictionary.new;

    ~mappings = Dictionary.new;
    ~mappings.add("/distancebetweenhands" -> \val1);
    ~mappings.add("/handheightsavg"       -> \val2);
    ~mappings.add("/distancefromsensor"   -> \val3);
    ~mappings.add("/lostuser"             -> \gate);

    //Declare synth definitions
    {
        //Synth for one instance per each user
        SynthDef.new(\brownAndDust, { |val1, val2, val3, gate|

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
        }).add;

        //Synth for one instance period - static on/off
        SynthDef.new(\static, { |mul|
            var out = PinkNoise.ar(mul);
            Out.ar(0, out);
        }).add;

        //Ensure server has loaded the definitions
        s.sync;
    }.fork;

    //Create a static synth instance
    ~staticBus = Bus.control(s);
    ~staticSynth = Synth.new(\static);
    ~staticSynth.map(\mul, ~staticBus);

    //Create OSC handlers for each of the mapped Kinect values
    ~mappings.keys.do({ |key|
        ~createOSCHandler.value(key, { |msg|
            var userId = msg[1];
            var val = msg[2];
            var user = ~users.at(userId);

            if(key == "/lostuser", {
			    if(user != nil, {
                    user.at(\busses).at(key).setSynchronous(0);
                    ~users.removeAt(userId);
                    ~trace.value(key, userId, val);
			    });
            }, {
                if(user == nil, {
                    ~users.add(userId -> ~createNewUser.value(userId));
                }, {
                    user.at(\busses).at(key).set(val);
                    ~trace.value(key, userId, val);
                });
		    });
        });
    });

    //Create an OSC handler for receiving static on/off messages
    ~createOSCHandler.value("/static", { |msg|
        var command = msg[1];
        if(command == 1, {
            ~staticBus.set(exprand(0.2, 0.5));
        }, {
            ~staticBus.set(0);
        });
    });

    //Reuse the same function to generate OSC handlers
    ~createOSCHandler = { |key, func|
        OSCFunc.newMatching(
            path: ~appName ++ key, recvPort: ~port, func: func
        );
    };

    //Build new user -> synth -> bus dictionary tree
    ~createNewUser = { |userId|
        var synth = Synth.new(\soundChamber);
        var busses = Dictionary.new;
        var user = Dictionary.new;

        var keys = ~mappings.keys;
        keys.do({ |key|
            busses.add(key -> Bus.control(s));
            busses.at(key).setSynchronous(1);
            synth.map(~mappings.at(key), busses.at(key));
        });

        user.add(\synth -> synth);
        user.add(\busses -> busses);
        user;
    };

    //Output debug messages
    ~trace = { |key, userId, val|
        if(~debug, {
            (key + "--> User: " ++ userId).postln;
            (key + "--> Value: " ++ val).postln;
        });
    }
)