function WebSocketAdapter()
{
   this.protocol = null;

   this.webSocket = null;

   this.ipAddress = null;

   this.port = null;

   // Callback functions to be implemented by client.
   this.onConnected = null;
   this.onDisconnected = null;
   this.onError = null;
   this.handleMessage = null;

   WebSocketAdapter.prototype.setProtocol = function(protocol)
   {
      this.protocol = protocol;
   }

   WebSocketAdapter.prototype.connect = function(ipAddress, port)
   {
      this.ipAddress = ipAddress;
      this.port = port;

      if ("WebSocket" in window)
      {
         // Let us open a web socket
         var address = "ws://" + ipAddress + ":" + port;
         console.log(address);
         this.webSocket = new WebSocket(address);

         this.webSocket.onopen = function()
         {
            console.log("WebSocketAdapter::onopen");
            
            if (typeof this.onConnected === "function")
            {
               this.onConnected();
            }
         }.bind(this);  // bind function to webSocketAdapter object

         this.webSocket.onmessage = function(event)
         {
            console.log("WebSocketAdapter::onmessage");
            
            if (this.protocol)
            {
               console.log("Received: " + event.data);
               
               var message = this.protocol.parse(event.data);

               if (typeof this.handleMessage === "function")
               {
                  this.handleMessage(message);
               }
            }
         }.bind(this);  // bind function to webSocketAdapter object

         this.webSocket.onclose = function()
         {
            console.log("WebSocketAdapter::onclose");
            
            if (typeof this.onDisconnected === "function")
            {
               this.onDisconnected();
            }
         }.bind(this);  // bind function to webSocketAdapter object

         this.webSocket.onerror = function()
         {
            console.log("WebSocketAdapter::onerror");
            
            if (typeof this.onError === "function")
            {
               this.onError();
            }
         }.bind(this);  // bind function to webSocketAdapter object
      }
      else
      {
         alert("Your browser does not support web sockets.");
      }
   }

   WebSocketAdapter.prototype.disconnect = function()
   {
      this.webSocket.close();
   }

   WebSocketAdapter.prototype.isConnected = function()
   {
      return (this.webSocket.readyState == WebSocket.OPEN);
   }

   WebSocketAdapter.prototype.sendMessage = function(message)
   {
      if (this.protocol && this.isConnected())
      {
         var serialized = this.protocol.serialize(message);

         console.log("Sent: " + serialized);

         this.webSocket.send(serialized);
      }
   }
}