(
  var port = 57121;
  var appName = "/soundchamber";
  var responder;

  var oscFunc = {|matcher, port, func|
    OSCFunc.newMatching(
      path: matcher,
      recvPort: port,
      func: func,
    );
  };

  responder = "/distancebetweenhands";
  oscFunc.value(matcher: appName ++ responder, port: port,
    func: {|msg|
      //Respond to incoming message here!
      (responder + "--> User ID: " ++ msg[1]).postln;
      (responder + "--> Value: " ++ msg[2]).postln;
    }
  );

  responder = "/handheightsavg";
  oscFunc.value(matcher: appName ++ responder, port: port,
    func: {|msg|
      //Respond to incoming message here!
      (responder + "--> User ID: " ++ msg[1]).postln;
      (responder + "--> Value: " ++ msg[2]).postln;
    }
  );

  responder = "/distancefromsensor";
  oscFunc.value(matcher: appName ++ responder, port: port,
    func: {|msg|
      //Respond to incoming message here!
      (responder + "--> User ID: " ++ msg[1]).postln;
      (responder + "--> Value: " ++ msg[2]).postln;
    }
  );
)