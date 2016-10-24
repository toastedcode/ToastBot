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

   ToastBot.prototype.ping = function()
   {
      console.log("ToastBot.ping: Url: " + this.url);
      console.log("ToastBot.ping: Ready state: " + this.webSocket.readyState);
      if (this.webSocket)
      {
         var message = '{"messageId":"ping"}';
         this.webSocket.send(message);
      }
   };

   ToastBot.prototype.motorPair = function(motorPairId, speed, yaw)
   {
      if (this.webSocket)
      {
         var message = '{"messageId":"drive", "destination":"' + motorPairId + '", "speed":"' + speed + '", "yaw":"' + yaw + '"}';
         this.webSocket.send(message);
      }
   }.bind(this);

   ToastBot.prototype.servo = function(servoId, angle)
   {
      if (this.webSocket)
      {
         var message = '{"messageId":"servo", "destination":"' + servoId + '", "angle":"' + angle + '"}';
         this.webSocket.send(message);
      }
   }.bind(this);
   
   ToastBot.prototype.follow = function(id, enable)
   {
      if (this.webSocket)
      {
         var message = '{"messageId":"enable", "destination":"' + id + '"}';
         this.webSocket.send(message);
      }
   }.bind(this);
};