var ws;

var scanners = [];

var detectedDevices = [];

function onDetected(ipAddress, deviceId)
{
   detectedDevices.push(ipAddress);

   buttonDiv = document.getElementById('buttonDiv');
   for (i = 0; i < detectedDevices.length; i++)
   {
      buttonDiv.innerHTML += '<button type="button" onclick=initializeWebSocket()">' + deviceId + " [" + ipAddress + "]</button>'";
   }
}

function Scanner(ipAddress)
{
   var ws;
   ws = new WebSocket(ipAddress);
   ws.onopen = function()
   {
      var message = '{"messageId":"ping"}';
      ws.send(message);
      //onDetected(ipAddress, "robo1");
      //setTimeout(function(){this.ws.close();}, 2000);
      //ws.close()
   }
   ws.onmessage = function(event)
   {
      s = event.data;
      console.log(s);
      startPos = s.indexOf("deviceId");
      endPos = s.indexOf(",", startPos);
      deviceId = s.substring(startPos + 10, endPos);
      onDetected(ipAddress, deviceId);
      ws.close();
   };
   ws.onclose = function(){};
   ws.onerror = function(){ws.close()};
};

/*
function onSuccess(foundIpAddress)
{
   console.log("Found one.");
   detectedDevices.push(foundIpAddress);

   buttonDiv = document.getElementById('buttonDiv');
   for (i = 0; i < detectedDevices.length; i++)
   {
      buttonDiv.innerHTML += '<button type="button">' + foundIpAddress + '</button>';
   }
}

function onFailure()
{
   lastTry++
   if (lastTry <= 255)
   {
      ipAddress = "ws://192.168.1." + lastTry + ":81";
      tryDevice(ipAddress);
   }
   else
   {
      console.log("Scan completed.");
   }
}

function onOpen(foundIpAddress)
{
   onSuccess(foundIpAddress);
}

function onMessage(event)
{
}

function onClose()
{
   //onFailure();
}

function onError()
{
   //onFailure();
}

function onTimeout()
{
   //ws.close();
   onFailure();
}

function tryDevice(ipAddress)
{
   console.log("Trying " + ipAddress);

  // Let us open a web socket
   scanners[ipAddress] = new WebSocket(ipAddress);
   openFuncs[ipAddress] = function(){onOpen(ipAddress)};

   ws.onopen = openFuncs[ipAddress];
   ws.onmessage = onMessage;
   ws.onclose = onClose;
   ws.onerror = onError;

   setTimeout(onTimeout, 100);
}
*/

function scanForDevices()
{
   console.log("Scanning ...");

   //ipAddress = ipAddress = "ws://192.168.1." + lastTry + ":81";
   //tryDevice(ipAddress);

   for (i = 1; i <= 255; i++)
   {
      ipAddress = "ws://192.168.1." + i + ":81";
      scanners[i] = new Scanner(ipAddress);
   }
}

function initializeWebSocket()
{
   console.log("here");
   if ("WebSocket" in window)
   {
      alert("WebSocket is supported by your Browser!");

      // Let us open a web socket
      ws = new WebSocket("ws://192.168.1.9:81");

      ws.onopen = function()
      {
         // Web Socket is connected, send data using send()
         alert("Connection is open...");
      };

      ws.onmessage = function (evt)
      {
         var received_msg = evt.data;
         alert("Message is received: " + event.data);
      };

      ws.onclose = function()
      {
         // websocket is closed.
         alert("Connection is closed...");
      };

      ws.onerror = function()
      {
        // websocket error.
       window.alert("Websocket connection failure");
      }
   }

   else
   {
      // The browser doesn't support WebSocket
      alert("WebSocket NOT supported by your Browser!");
   }
}

function pinMode()
{
 if (ws)
 {
    // INPUT = 0, OUTPUT = 1, INPUT_PULLUP = 2
   var message = '{"messageId":"pinMode", "pin":"16", "mode":"1"}';
   ws.send(message);
 }
}

function pinOn()
{
   if (ws)
   {
      var message = '{"messageId":"digitalWrite", "pin":"16", "value":"1"}';
      ws.send(message);
   }
}

function pinOff()
{
   if (ws)
   {
      var message = '{"messageId":"digitalWrite", "pin":"16", "value":"0"}';
      ws.send(message);
   }
}

function motor(motorId, deltaX, deltaY)
{
   if (ws)
   {
      var message = '{"messageId":"motor", "motor":"' + motorId + '", "speed":"' + deltaY + '"}';
      ws.send(message);
   }
}

function motorPair(motorPairId, speed, yaw)
{
   if (ws)
   {
      var message = '{"messageId":"motorPair", "componentId":"' + motorPairId + '", "speed":"' + speed + '", "yaw":"' + yaw + '"}';
      ws.send(message);
   }
}

function servo(servoId, angle)
{
   if (ws)
   {
      var message = '{"messageId":"servo", "componentId":"' + servoId + '", "angle":"' + angle + '"}';
      ws.send(message);
   }
}