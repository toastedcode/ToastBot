function JsonProtocol()
{
   JsonProtocol.prototype.serialize = function(message)
   {
      var serialized = JSON.stringify(message);
      
      return (serialized);
   }
   
   JsonProtocol.prototype.parse = function(serializedMessage)
   {
      var message = JSON.parse(serializedMessage);
      
      return (message);
   }
}