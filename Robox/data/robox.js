function Robox()
{
   // **************************************************************************
   //                                  Messaging
   
   Robox.prototype.sendMessage = function(message)
   {
   };
   
   Robox.prototype.sendRawMessage = function(rawMessage)
   {
      var success = true;
      
      var xhttp = new XMLHttpRequest();
      xhttp.robox = this;
      
      xhttp.onreadystatechange = function()
      {
         if ((this.readyState == 4) && (this.status == 200))
         {
            this.robox.handleMessage(this.responseText);
         }
      };

      xhttp.open("GET", rawMessage, true);
      xhttp.send();
      
      return (success);
   };
   
   Robox.prototype.handleMessage = function(rawMessage)
   {
      console.log(rawMessage);
   };
   
   // **************************************************************************
   //                                  Control
   
   Robox.prototype.ping = function()
   {
      return (this.sendRawMessage("/robox/ping"));
   };
   
   Robox.prototype.pong = function()
   {
      return (this.sendRawMessage("/robox/pong"));
   };
   
   Robox.prototype.reset = function()
   {
      return (this.sendRawMessage("/robox/reset"));
   };
   
   Robox.prototype.analogWrite = function(pin, value)
   {
   };
   
   Robox.prototype.digitalWrite = function(pin, value)
   {
   };
   
   // **************************************************************************
   //                             Motor Pair
   
   Robox.prototype.drive = function(speed)
   {
      return (this.sendRawMessage("/motorPair/drive?speed=" + speed));
   }
   
   Robox.prototype.rotate = function(speed)
   {
      return (this.sendRawMessage("/motorPair/rotate?speed=" + speed));
   }
};