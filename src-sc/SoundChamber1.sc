(
  ~port = 57121;
  ~netAddr = NetAddr("127.0.0.1", ~port);
  ~matcher = "/soundchamber/distancebetweenhands";

  OSCFunc.newMatching(
    path: ~matcher,
    recvPort: ~port,
    func: {
      arg msg, time, addr, recvPort;
      ("--> User ID: " + msg[1]).postln;
      ("--> Value: " + msg[2]).postln;
    },
  );

  ~netAddr.sendMsg(~matcher, "Now recieving OSC on matcher" + ~matcher);
)