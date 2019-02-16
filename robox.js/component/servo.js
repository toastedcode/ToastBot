function Servo(id)
{
   this.id = id;
   
   this.parent = null;
   
   Servo.prototype.ping = function()
   {
      var message = new Message();
      message.messageId = "ping";
      message.destination = this.id;

      parent.sendMessage(message);
   }
   
   Servo.prototype.rotate = function(angle)
   {
      var message = new Message();
      message.messageId = "rotate";
      message.destination = this.id;
      message.angle = angle;
      
      this.parent.sendMessage(message);
   }
   
   Servo.prototype.panTo = function(angle, seconds)
   {
      var message = new Message();
      message.messageId = "panTo";
      message.destination = this.id;
      message.angle = angle;
      message.seconds = seconds;
      
      this.parent.sendMessage(message);
   }
   
   Servo.prototype.oscillate = function(startAngle, endAngle, seconds)
   {
      var message = new Message();
      message.messageId = "oscillate";
      message.destination = this.id;
      message.startAngle = endAngle;
      message.endAngle = endAngle;
      message.seconds = seconds;
      
      this.parent.sendMessage(message);
   }
   
   Servo.prototype.stop = function()
   {
      var message = new Message();
      message.messageId = "stop";
      message.destination = this.id;
      
      this.parent.sendMessage(message);
   }
   
   Servo.prototype.limit = function(limitMin, limitMax)
   {
      var message = new Message();
      message.messageId = "limit";
      message.destination = this.id;
      message.limitMin = limitMin;
      message.limitMax = limitMax;
      
      this.parent.sendMessage(message);
   }
   
   Servo.prototype.reverse = function(isReversed)
   {
      var message = new Message();
      message.messageId = "reverse";
      message.destination = this.id;
      message.isReversed = isReversed;
      
      this.parent.sendMessage(message);
   }
}