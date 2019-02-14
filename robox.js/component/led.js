function Led(id)
{
   this.id = id;
   
   this.parent = null;
   
   Led.prototype.ping = function()
   {
      var message = new Message();
      message.messageId = "ping";
      message.destination = this.id;

      parent.sendMessage(message);
   }
   
   Led.prototype.setBrightness = function(brightness)
   {
      var message = new Message();
      message.messageId = "setBrightness";
      message.destination = this.id;
      message.brightness = brightness;
      
      this.parent.sendMessage(message);
   }
   
   Led.prototype.blink = function(pattern)
   {
      var message = new Message();
      message.messageId = "blink";
      message.destination = this.id;
      message.pattern = pattern;
      
      this.parent.sendMessage(message);
   }
   
   Led.prototype.pulse = function(period)
   {
      var message = new Message();
      message.messageId = "pulse";
      message.destination = this.id;
      message.period = period;
      
      this.parent.sendMessage(message);
   }
}