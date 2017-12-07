class AjaxTestPage : public Webpage
{

public:
  
  AjaxTestPage();

  virtual bool handle(
      const HTTPMethod& requestMethod,
      const String& requestUri,
      MessagePtr message,
      String& responsePath);
};

AjaxTestPage::AjaxTestPage() :
   Webpage("/ajax_info.txt",
           "/ajax_info.txt")
{
   // Nothing to do here.  
}

 bool AjaxTestPage::handle(
    const HTTPMethod& requestMethod,
    const String& requestUri,
    MessagePtr message,
    String& responsePath)
{
   bool success = false;
   responsePath = "/tmp/ajaxTest";

   Logger::logDebug("AjaxTestPage::handle: %d, %s", requestMethod, requestUri.c_str());

   if (canHandle(requestMethod, requestUri))
   {
       File responseFile = SPIFFS.open(responsePath, "w");

       if (!responseFile)
       {
          Logger::logWarning(F("Webpage::handle: Failed to create temp file: %s"), responsePath.c_str());
          responsePath = "";
       }
       else
       {
          responseFile.println("{\"json\":\"jasontost\"}");
          responseFile.close();

          success = true;
       }
   }

   return (success);
}
