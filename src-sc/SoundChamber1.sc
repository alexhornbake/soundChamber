(
  ~netAddr = NetAddr("127.0.0.1", NetAddr.langPort);
  ~matcher = "/soundchamber";

  OSCFunc.newMatching(
    path: ~matcher,
    srcID: ~netAddr,
    func: {
      arg msg, time, addr, recvPort;
      ("-->" + msg[1]).postln;
    },
  );

  ~netAddr.sendMsg(~matcher, "Now recieving OSC on matcher" + ~matcher);
)