function Scanner()
{
   // Public members

   this.isScanning = false;

   this.progress = 0;

   // Private members

   var scanners = [];

   var onDetected;

   var onCompleted;

   var onProgress;

   var scanningCount = 0;

   var that = this;

   const NUM_SCANNERS = 255;

   function reset()
   {
      console.log("Reset");
      that.isScanning = false;
      that.progress = 0;
      scanningCount = 0;

      for (i = 0; i <= NUM_SCANNERS; i++)
      {
         if (scanners[i])
         {
            scanners[i].disconnect();
            delete scanners[i];
         }
      }
   }

   // Public methods

   Scanner.prototype.cancel = function()
   {
      reset();
      this.onCompleted();
   }

   Scanner.prototype.scanForDevices = function(subnet)
   {
      console.log("Scanning for Toast Bots ...\n");

      if (this.isScanning)
      {
         console.log("Currently scanning.");
      }
      else
      {
         this.isScanning = true;
         this.progress = 0;
         scanningCount = NUM_SCANNERS;

         for (i = 0; i <= NUM_SCANNERS; i++)
         {
            var url = "ws://" + subnet + "." + i + ":81";

            scanners[i] = new ToastBot();

            scanners[i].onConnect = function()
            {
               console.log("Pinging ToastBot at " + this.url);
               this.ping();
               setTimeout(function(){this.disconnect()}.bind(this), 2000);
            }.bind(scanners[i]);

            scanners[i].onDisconnect = function()
            {
               if (this.isScanning)
               {
                  scanningCount--;

                  this.progress = Math.round(((NUM_SCANNERS - scanningCount) * 100) / NUM_SCANNERS);
                  this.onProgress(this.progress);

                  if (scanningCount == 0)
                  {
                     reset();
                     this.onCompleted();
                  }
               }
            }.bind(this);

            scanners[i].onMessage = function(json)
            {
               if (json.messageId == "pong")
               {
                  this.onDetected(json.ipAddress, json.deviceId);
               }
            }.bind(this);

            console.log("Scanning for ToastBot at " + url);
            scanners[i].connect(url);
         }
      }
   }
};