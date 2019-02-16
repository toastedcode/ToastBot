function Sensor(id)
{
   this.id = id;
   
   this.parent = null;
   
   Sensor.prototype.ping = function()
   {
      var message = new Message();
      message.messageId = "ping";
      message.destination = this.id;

      parent.sendMessage(message);
   }
   
   Sensor.prototype.read = function()
   {
      var message = new Message();
      message.messageId = "read";
      message.destination = this.id;
      
      this.parent.sendMessage(message);
   }
   
   Sensor.prototype.poll = function(period)
   {
      var message = new Message();
      message.messageId = "poll";
      message.destination = this.id;
      message.period = period;
      
      this.parent.sendMessage(message);
   }
}