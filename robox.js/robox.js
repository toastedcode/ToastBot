function Robox()
{
   this.clientId = "robox.js";

   this.adapter = null;

   this.components = new Object();

   Robox.prototype.addComponent = function(component)
   {
      this.components[component.id] = component;
      component.parent = this;

      console.log("Robox::addComponent: Added component [" + component.id + "].");
   }

   // **************************************************************************
   //                            Robox connection API

   Robox.prototype.setAdapter = function(adapter)
   {
      this.adapter = adapter;

      this.adapter.onConnected = function()
      {
         if (typeof this.onConnected === "function")
         {
            this.onConnected();
         }
      }.bind(this);
      
      this.adapter.onDisconnected = function()
      {
         if (typeof this.onDisconnected === "function")
         {
            this.onDisconnected();
         }
      }.bind(this);
      
      this.adapter.onError = function()
      {
         if (typeof this.onError === "function")
         {
            this.onError();
         }
      }.bind(this);
      
      this.adapter.handleMessage = function(message)
      {
         if (typeof this.handleMessage === "function")
         {
            this.handleMessage(message);
         }
      }.bind(this);
   }

   Robox.prototype.connect = function(ipAddress, port)
   {
      if (this.adapter)
      {
         this.adapter.disconnect();
      }

      this.setAdapter(new WebSocketAdapter());
      
      this.adapter.setProtocol(new JsonProtocol());
      this.adapter.connect(ipAddress, port);
   }
   
   Robox.prototype.connect = function(host, port, topic)
   {
      if (this.adapter)
      {
         this.adapter.disconnect();
      }

      this.setAdapter(new MqttAdapter());
      
      this.adapter.setProtocol(new JsonProtocol());
      this.adapter.connect(host, port, topic);
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

   // **************************************************************************
   //                            Robox messaging API

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

   Robox.prototype.reset = function()
   {
      var message = new Message();
      message.messageId = "reset";

      this.sendMessage(message);
   }
   
   Robox.prototype.heap = function()
   {
      var message = new Message();
      message.messageId = "heap";

      this.sendMessage(message);
   }
   
   Robox.prototype.setLogLevel = function(logLevel)
   {
      var message = new Message();
      message.messageId = "setLogLevel";
      message.name = name;
      message.logLevel = logLevel;

      this.sendMessage(message);
   }

   Robox.prototype.setProperty = function(name, value)
   {
      var message = new Message();
      message.messageId = "setProperty";
      message.name = name;
      message.value = value;

      this.sendMessage(message);
   }

   Robox.prototype.getProperty = function(name)
   {
      var message = new Message();
      message.messageId = "getProperty";
      message.name = name;

      this.sendMessage(message);
   }

   Robox.prototype.pinMode = function(pin, mode)
   {
      var message = new Message();
      message.messageId = "pinMode";
      message.pin = pin;
      message.mode = mode;

      this.sendMessage(message);
   }
   
   Robox.prototype.digitalRead = function(pin)
   {
      var message = new Message();
      message.messageId = "digitalRead";
      message.pin = pin;

      this.sendMessage(message);
   }

   Robox.prototype.digitalWrite = function(pin, value)
   {
      var message = new Message();
      message.messageId = "digitalWrite";
      message.pin = pin;
      message.value = value;

      this.sendMessage(message);
   }
   
   Robox.prototype.analogRead = function(pin)
   {
      var message = new Message();
      message.messageId = "analogRead";
      message.pin = pin;

      this.sendMessage(message);
   }

   Robox.prototype.analogWrite = function(pin, value)
   {
      var message = new Message();
      message.messageId = "analogWrite";
      message.pin = pin;
      message.value = value;

      this.sendMessage(message);
   }

   Robox.prototype.create = function(addComponent, description)
   {
      var message = new Message();
      message.messageId = "create";
      message.description = description;

      this.sendMessage(message);
   }
   
   Robox.prototype.subscribe = function(topic)
   {
      var message = new Message();
      message.messageId = "subscribe";
      message.toTopic = topic;

      this.sendMessage(message);
   }

   // **************************************************************************

   this.addComponent(new Motor("motor1"));
   this.addComponent(new Motor("motor2"));
   this.addComponent(new MotorPair("motorPair"));
   this.addComponent(new Servo("servo1"));
   this.addComponent(new Servo("servo2"));
   this.addComponent(new Led("statusLed"));
}