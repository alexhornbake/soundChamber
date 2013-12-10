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
    ~port = 57121;
    ~appName = "/soundchamber";
    ~debug = false;

    ~users = Dictionary.new;

    ~mappings = Dictionary.new;
    ~mappings.add("/distancebetweenhands" -> \val1);
    ~mappings.add("/handheightsavg"       -> \val2);
    ~mappings.add("/distancefromsensor"   -> \val3);

    {
        SynthDef.new(\soundChamber, { |val1, val2, val3|
            //Go crazy and creative with your snyth def in here!!
            var freq = LinLin.kr(val1, 0, 1, 220, 440);
            var amp  = LinLin.kr(val2, 0, 1, 0, 1);
            var pan  = LinLin.kr(val3, 0, 1, -1,  1);
            var out  = Pan2.ar(SinOsc.ar(freq, 0, amp), pan);
            Out.ar(0, out);
        }).add;
        s.sync;
    }.fork;

    ~mappings.keys.do({ |key|
        ~createOSCHandler.value(key);
    });

    ~createOSCHandler = { |key|
        (~appName ++ key).postln;
        OSCFunc.newMatching(
            path: ~appName ++ key, recvPort: ~port,
            func: { |msg|
                var userId = msg[1];
                var val = msg[2];
                var user = ~users.at(userId);

                if(user == nil, {
                    ~createNewUser.value(userId);
                }, {
                    user.at(\busses).at(key).set(val);
                    ~trace.value(key, userId, val);
                });
            }
        );
    };

    ~createNewUser = { |userId|
        var synth = Synth.new(\soundChamber);
        var busses = Dictionary.new;
        var user = Dictionary.new;

        var keys = ~mappings.keys;
        keys.do({ |key|
            busses.add(key -> Bus.control(s));
            synth.map(~mappings.at(key), busses.at(key));
        });

        user.add(\synth -> synth);
        user.add(\busses -> busses);
        ~users.add(userId -> user);
    };

    ~trace = { |key, userId, val|
        if(~debug, {
            (key + "--> User: " ++ userId).postln;
            (key + "--> Value: " ++ val).postln;
        });
    }
)