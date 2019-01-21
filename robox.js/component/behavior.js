function Behavior(id)
{
   this.id = id;
   
   this.parent = null;
   
   Behavior.prototype.ping = function()
   {
      var message = new Message();
      message.messageId = "ping";
      message.destination = this.id;

      parent.sendMessage(message);
   }
   
   Behavior.prototype.enable = function()
   {
      var message = new Message();
      message.messageId = "enable";
      message.destination = this.id;
      
      this.parent.sendMessage(message);
   }
   
   Behavior.prototype.disable = function()
   {
      var message = new Message();
      message.messageId = "disable";
      message.destination = this.id;
      
      this.parent.sendMessage(message);
   }
}