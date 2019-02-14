function RestfulProtocol()
{
   RestfulProtocol.prototype.serialize = function(message)
   {
      var serializedMessage = "";
      
      var properties = [];
      
      for (var property in message)
      {
         if ((message.hasOwnProperty(property)) && (message[property]))
         {
            properties.push(encodeURIComponent(property) + "=" + encodeURIComponent(message[property]));
         }
      }
      
      serializedMessage = properties.join("&");
      
      return (serializedMessage);
   }
   
   Message.prototype.parse = function(serializedMessage)
   {
      // TODO.
   }
}