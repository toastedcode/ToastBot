function MotorPair(id)
{
   this.id = id;
   
   this.parent = null;
   
   MotorPair.prototype.ping = function()
   {
      var message = new Message();
      message.messageId = "ping";
      message.destination = this.id;

      parent.sendMessage(message);
   }
   
   MotorPair.prototype.drive = function(speed, yaw)
   {
      var message = new Message();
      message.messageId = "drive";
      message.destination = this.id;
      message.speed = speed;
      message.yaw = yaw;
      
      this.parent.sendMessage(message);
   }
   
   MotorPair.prototype.rotate = function(speed)
   {
      var message = new Message();
      message.messageId = "rotate";
      message.destination = this.id;
      message.speed = speed;
      
      this.parent.sendMessage(message);
   }
   
   MotorPair.prototype.stop = function()
   {
      var message = new Message();
      message.messageId = "drive";
      message.destination = this.id;
      message.speed = 0;
      message.yaw = 0;
      
      this.parent.sendMessage(message);
   }
}