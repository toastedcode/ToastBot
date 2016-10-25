function ToastBot()
{
   var url;

   var webSocket;

   var onConnect;

   var onDisconnect;

   var onMessage;

   ToastBot.prototype.connect = function(url)
   {
      this.url = url;

      this.webSocket = new WebSocket(this.url);

      this.webSocket.onopen = function()
      {
         console.log("ToastBot.connect: Url: " + this.url);
         console.log("ToastBot.connect: Ready state: " + this.webSocket.readyState);
         if (this.onConnect)
         {
            this.onConnect();
         }
      }.bind(this);

      this.webSocket.onmessage = function(event)
      {
         var json = JSON.parse(event.data);
         this.onMessage(json);
      }.bind(this);

      this.webSocket.onerror = function(event)
      {
         this.webSocket.close();
      }.bind(this);

      this.webSocket.onclose = function()
      {
         if (this.onDisconnect)
         {
            this.onDisconnect();
         }
      }.bind(this);
   };

   ToastBot.prototype.disconnect = function()
   {
      if (this.webSocket)
      {
         this.webSocket.close();
      }
   };
   
   ToastBot.prototype.isConnected = function()
   {
      return ((this.websocket) && (this.websocket.readyState == 1));
   }

   ToastBot.prototype.ping = function()
   {
      console.log("ToastBot.ping: Url: " + this.url);
      console.log("ToastBot.ping: Ready state: " + this.webSocket.readyState);
      
      if (this.isConnected())
      {
         var message = '{"messageId":"ping"}';
         this.webSocket.send(message);
      }
   };

   ToastBot.prototype.motorPair = function(motorPairId, speed, yaw)
   {
      if (this.isConnected())
      {
         var message = '{"messageId":"drive", "destination":"' + motorPairId + '", "speed":"' + speed + '", "yaw":"' + yaw + '"}';
         this.webSocket.send(message);
      }
   }.bind(this);
   
   ToastBot.prototype.rotate = function(motorPairId, speed)
   {
      if (this.isConnected())
      {
         var message = '{"messageId":"rotate", "destination":"' + motorPairId + '", "speed":"' + speed + '"}';
         this.webSocket.send(message);
      }
   }.bind(this);

   ToastBot.prototype.servo = function(servoId, angle)
   {
      if (this.isConnected())
      {
         var message = '{"messageId":"servo", "destination":"' + servoId + '", "angle":"' + angle + '"}';
         this.webSocket.send(message);
      }
   }.bind(this);
   
   ToastBot.prototype.follow = function(id, enable)
   {
      if (this.isConnected())
      {
         var message = '{"messageId":"enable", "destination":"' + id + '"}';
         this.webSocket.send(message);
      }
   }.bind(this);
};