<html>
   <head>
   <style>
      .centered
      {
         display: block;
         margin-left: auto;
         margin-right: auto;
      }
   </style>
   </head>
   <body>
      <img class="centered" src="roboxes.jpg"/>
      <?php
      ini_set('display_errors', 'On');
      error_reporting(E_ALL | E_STRICT);
   	  include 'shopSense.php';
      ?>
   </body>
</html>