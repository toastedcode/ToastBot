function Motor(id)
{
   this.id = id;
   
   this.parent = null;
   
   Motor.prototype.ping = function()
   {
      var message = new Message();
      message.messageId = "ping";
      message.destination = this.id;

      this.parent.sendMessage(message);
   }
   
   Motor.prototype.drive = function(speed)
   {
      var message = new Message();
      message.messageId = "setSpeed";
      message.destination = this.id;
      message.speed = speed;
      
      this.parent.sendMessage(message);
   }
   
   Motor.prototype.stop = function()
   {
      var message = new Message();
      message.messageId = "setSpeed";
      message.destination = this.id;
      message.speed = 0;
      
      this.parent.sendMessage(message);
   }
}