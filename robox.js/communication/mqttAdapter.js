function MqttAdapter()
{
   this.protocol = null;

   this.host = null;
   
   this.port = null;
   
   this.sendTopic = null;
   
   this.receiveTopic = null;
   
   this.mqttClient = null;

   // Callback functions to be implemented by client.
   this.onConnected = null;
   this.onDisconnected = null;
   this.onError = null;
   this.handleMessage = null;

   MqttAdapter.prototype.setProtocol = function(protocol)
   {
      this.protocol = protocol;
   }

   MqttAdapter.prototype.connect = function(host, port, topic)
   {
      this.host = host;
      this.port = port;
      this.sendTopic = topic + "/to";
      this.receiveTopic = topic + "/from";
      
      if ("WebSocket" in window)
      {      
         this.mqttClient = new MqttClient({
            host : this.host,
            port: this.port,
            clientId: "roboxclient"});
         
         this.mqttClient.on('connecting', function()
         {
            console.log('MqttAdapter::connecting'); 
         });
         
         this.mqttClient.on('connect', function()
         {
            console.log("MqttAdapter::connect");
            
            this.mqttClient.subscribe(this.receiveTopic);
            
            if (typeof this.onConnected === "function")
            {
               this.onConnected();
            }
         }.bind(this));  // bind function to MqttAdapter object
         
         this.mqttClient.on('disconnect', function()
         {
            console.log("MqttAdapter::disconnect");
            
            if (typeof this.onDisconnected === "function")
            {
               this.onDisconnected();
            }
            
            this.mqttClient = null;
         }.bind(this));  // bind function to MqttAdapter object
         
         this.mqttClient.on('offline',    function()
         {
            console.log('MqttAdapter::offine'); 
         });
         
         this.mqttClient.on('message', function(message)
         {
            console.log("MqttAdapter::message: " + message);
         });
         
         this.mqttClient.connect();
      }
      else
      {
         alert("Your browser does not support web sockets.");
      }
   }

   MqttAdapter.prototype.disconnect = function()
   {
      this.mqttClient.disconnect();
   }

   MqttAdapter.prototype.isConnected = function()
   {
      return (this.mqttClient && this.mqttClient.connected);
   }

   MqttAdapter.prototype.sendMessage = function(message)
   {
      if (this.mqttClient && this.protocol)
      {
         var serialized = this.protocol.serialize(message);

         console.log("Sent: " + serialized);

         this.mqttClient.publish(this.sendTopic, serialized);
      }
   }
}