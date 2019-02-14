function ConsoleAdapter()
{
   this.handler = null;
   this.protocol = null;
   
   ConsoleAdapter.prototype.setProtocol = function(protocol)
   {
      this.protocol = protocol;
   }
   
   ConsoleAdapter.prototype.setMessageHandler = function(handler)
   {
   }
   
   /*
   ConsoleAdapter.prototype.connect = function()
   {
      // Nothing to do here.
   }
   
   ConsoleAdapter.prototype.disconnect = function()
   {
      // Nothing to do here.
   }
   
   ConsoleAdapter.prototype.isConnected() = function()
   {
      return (true);
   }
   */
      
   ConsoleAdapter.prototype.sendMessage = function(message)
   {
      if (this.protocol)
      {
         var serialized = this.protocol.serialize(message);
         
         console.log(serialized);
      }
   }
}