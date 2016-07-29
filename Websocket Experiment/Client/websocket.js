function initializeWebSocket()
{
   console.log("here");
   if ("WebSocket" in window)
   {
      alert("WebSocket is supported by your Browser!");

      // Let us open a web socket
      ws = new WebSocket("ws://192.168.1.18:81");

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
      var message = '{"messageId":"motor", "deltaX":"' + deltaX + '", "deltaY":"' + deltaY + '0"}';
      ws.send(message);
   }
}