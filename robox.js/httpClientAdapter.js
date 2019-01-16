function HttpClientAdapter()
{
   this.handler = null;
   this.protocol = null;
   this.url = null;
   
   HttpClientAdapter.prototype.setProtocol = function(protocol)
   {
      this.protocol = protocol;
   }
   
   HttpClientAdapter.prototype.setMessageHandler = function(handler)
   {
      this.handler = handler;
   }
   
   HttpClientAdapter.prototype.setUrl = function(url)
   {
      this.url = url;
   }
   
   HttpClientAdapter.prototype.sendMessage = function(message)
   {
      var xhttp = new XMLHttpRequest();
      
      xhttp.onreadystatechange = function()
      {
         if (this.readyState == 4 && this.status == 200)
         {
           var message = this.protocol.parse(this.responseText);
           
           if (this.messageHandler)
           {
              this.messageHandler.handleMessage(message);
           }
         }
      };
      
      var restfulProtocol = new RestfulProtocol();
      
      var request = this.url + "/" + restfulProtocol.serialize(message);
      
      xhttp.open("GET", request, true);
      
      xhttp.send();      
   }
}