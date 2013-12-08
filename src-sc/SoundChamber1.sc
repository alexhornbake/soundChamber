(
var port = 57121;
var appName = "/soundchamber";
var responder1, responder2, responder3;

var oscFunc = {|matcher, port, func|
	OSCFunc.newMatching(
		path: matcher,
		recvPort: port,
		func: func,
	);
};

responder1 = "/distancebetweenhands";
oscFunc.value(matcher: appName ++ responder1, port: port,
	func: {|msg|
		//Respond to incoming message here!
		(responder1 + "--> User ID: " ++ msg[1]).postln;
		(responder1 + "--> Value: " ++ msg[2]).postln;
	}
);

responder2 = "/handheightsavg";
oscFunc.value(matcher: appName ++ responder2, port: port,
	func: {|msg|
		//Respond to incoming message here!
		(responder2 + "--> User ID: " ++ msg[1]).postln;
		(responder2 + "--> Value: " ++ msg[2]).postln;
	}
);

responder3 = "/distancefromsensor";
oscFunc.value(matcher: appName ++ responder3, port: port,
	func: {|msg|
		//Respond to incoming message here!
		(responder3 + "--> User ID: " ++ msg[1]).postln;
		(responder3 + "--> Value: " ++ msg[2]).postln;
	}
);
)