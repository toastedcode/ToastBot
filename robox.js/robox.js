function Robox()
{
   this.clientId = "robox.js";
   
   this.adapter = null;
   
   Robox.prototype.setAdapter = function(adapter)
   {
      this.adapter = adapter;
      
      adapter.onConnected = function()
      {
         this.onConnected();
      }
      adapter.onDisconnected = function()
      {
         this.onDisconnected();
      }
      adapter.onError = function()
      {
         this.onError();
      }
      adapter.handleMessage = function(message)
      {
         this.handleMessage(message);
      }
   }
   
   Robox.prototype.connect = function(ipAddress, port)
   {
      if (this.adapter)
      {
         this.adapter.disconnect();
      }
      
      this.adapter = new WebSocketAdapter();
      this.adapter.setProtocol(new JsonProtocol());
      this.adapter.connect(ipAddress, port);
   }
   
   Robox.prototype.disconnect = function()
   {
      if (this.adapter)
      {
         this.adapter.disconnect();
      }
      
      this.adapter = null;
   }
   
   Robox.prototype.isConnected = function()
   {
      return (this.adapter && adapter.isConnected());
   }
   
   Robox.prototype.sendMessage = function(message)
   {
      if (this.adapter)
      {
         message.source = this.clientId;
         
         this.adapter.sendMessage(message);
      }
      else
      {
         console.log("Robox::sendMessage: No adapter specified for sending messages.")
      }      
   }
   
   Robox.prototype.ping = function()
   {
      var message = new Message();
      message.messageId = "ping";
      
      this.sendMessage(message);
   }
   
   Robox.prototype.motor = function(motorId, speed)
   {
      var message = new Message();
      message.messageId = "setSpeed";
      message.destination = "motor1";
      message.speed = speed;
      
      this.sendMessage(message);
   }
   
   Robox.prototype.motorPair = function(speed, yaw)
   {
      var message = new Message();
      message.messageId = "drive";
      message.destination = "motorPair";
      message.speed = speed;
      message.yaw = yaw;
      
      this.sendMessage(message);      
   }
   
   Robox.prototype.servo = function(servoId, angle)
   {
      var message = new Message();
      message.messageId = "rotate";
      message.destination = "servo1";
      message.angle = angle;
      
      this.sendMessage(message);       
   }
}