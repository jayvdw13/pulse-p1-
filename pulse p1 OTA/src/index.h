void OTA();
const char index_html[] PROGMEM = R"=====(
<!DOCTYPE HTML><html>
    <head>
       <meta charset="utf-8">
       <title>Flex Green</title>
       <meta name="viewport" content="width=device-width, initial-scale=1">
       <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" title="main"/>
       <link href="https://gitcdn.github.io/bootstrap-toggle/2.2.2/css/bootstrap-toggle.min.css" rel="stylesheet"/>
       <link href="https://cdnjs.cloudflare.com/ajax/libs/selectize.js/0.12.6/css/selectize.bootstrap3.min.css" rel="stylesheet"/>
       <link href="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-select/1.12.4/css/bootstrap-select.min.css" rel="stylesheet"/>
       <link href=""http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd">
       <style>
          #dropdown2{
            background-color: black;
          }

          body { 
            --Tcolor: #ffffff; 
            --popup-count: 10; 
            font-family: Montserrat, sans-serif; 
            font-size: 16px; 
            overflow: hidden;
            margin-bottom: 200px;
          } 

          img {
            user-select: none;
            -moz-user-select: none;
            -webkit-user-select: none;
            -ms-user-select: none;
          }

          .btn {  
            color: var(--Tcolor); 
            font-weight: bold;
            display: flex; 
            user-select: none;
            -moz-user-select: none;
            -webkit-user-select: none;
            -ms-user-select: none; 
            text-shadow: 0 -1px rgba(0, 0, 0, 0.5); 
            cursor: pointer; 
            transition: all 150ms ease-out;
          } 

          .popup-open {  
            background-color: #39b34a;
            text-align: center;
            color: white;
            margin-left: 40px;
            margin-right: 88vw;
            font-weight: bold;
            font-size: 1.5vh;
            cursor: pointer; 
            transition: all 150ms ease-out;
            border-radius: 5px;
            border-width: 2px 2px;
            border-style: solid;
            border-color:black;
            padding: 5px 5px 5px 5px;
            user-select: none;
            -moz-user-select: none;
            -webkit-user-select: none;
            -ms-user-select: none;
          } 

          .popup-opener {  
            display: none;
          } 

          .popup-opener:checked + .popup { 
            transform: scale(1); 
            width: 84vw;   
            bottom: 0px; 
            right: 0px;  
            height: 88vh;   
            border-radius: 50px 0px 0px 0px; 
          } 
            
          .btn-popup svg { 
            margin-right: 5px;  
            display: block;
            }
              
          .popup {  
            margin: 12vh 0px 0px 16vw;
            padding: 0px 0px 0px 40px; 
            background-color: #2e2629 ; 
            color:white; 
            display: grid; 
            grid-template-rows: var(--Nheight) 30% auto;
            grid-template-columns: 100%; 
            position: absolute; 
            top: 0; left: 0; transform: scale(0); 
            transition: transform 300ms ease; 
          }

          .background_white{ 
            height: 88vh; 
            width: 84vw; 
            background-color: rgb(255, 255, 255); 
            bottom: 0px; right: 0px; 
            position: absolute; 
            border-radius: 50px 0px 0px 0px;  
            border: 1px solid rgb(197, 197, 197);
            overflow-y: scroll;
          } 

          .background_white::-webkit-scrollbar {
            display: none;
          }

          label { 
            display: block; 
          }

          label:not(.error) .error-info { 
            display: none; 
          }

          .inline { 
            display: inline-block; 
          }
            
          .line{
            background-color: black;
            height: 1px;
            border: none;
            user-select: none;
            -moz-user-select: none;
            -webkit-user-select: none;
            -ms-user-select: none;
          }

          br{
            user-select: none;
            -moz-user-select: none;
            -webkit-user-select: none;
            -ms-user-select: none;
          }

          html {
            position: relative;
            min-height: 100%;
          }

          .card { 
            background-color: #39b34a;
            color: white; 
            align-content: center;
            border-width: 2px 2px;
            border-style: solid;
            border-color: rgb(0, 0, 0);
            text-align: center;
          }

          .card2 { 
            background-color: #39b34a;
            color: white; 
            align-content: center;
            border-width: 2px 2px;
            border-style: solid;
            border-color: rgb(0, 0, 0);
            text-align: center;
            margin-top: 20px;
          }

            .card3 { 
            background-color: #39b34a;
            color: white; 
            align-content: center;
            border-width: 2px 2px;
            border-style: solid;
            border-color: rgb(0, 0, 0);
            text-align: center;
            margin-top: 40px;
          }

          .card4 { 
            background-color: #39b34a;
            color: white; 
            align-content: center;
            border-width: 2px 2px;
            border-style: solid;
            border-color: rgb(0, 0, 0);
            text-align: center;
            margin-top: 40px;
          }          

          .cards { 
            max-width: 700px; 
            margin: 0 auto; 
            display: grid; 
            grid-gap: 2rem; 
            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
            margin-top: 70px; 
          }
           
              
          .reading { 
            font-size: 2.8re; 
          }
              

          p{
            font-size: 3vh;
            margin: 5px;
          }

          .refresh{
            margin: 20px 0px 0px 36.5%;
          }

          option, select{
            color : black;
          }
       </style>
    </head>
    <body style="background-color: #E0E0E0;">
<svg version="1.1" id="Layer_1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" x="0px" y="0px" width="300px" height="170px" viewBox="0 0 300 170" enable-background="new 0 0 300 170" xml:space="preserve">  
</svg>
       <div class="dropdown2">
           <label class="popup-open" style=" margin-top: 20px;" for="netwerk">NETWERK SETTINGS</label>
        </div>
           <br>
         <hr class="line">
         <br>
        <div class="dropdown2">
           <label class="popup-open"  for="server">SERVER SETTINGS</label>
        </div>
        <br>
        <br>
        <hr class="line">
        <div class="dropdown2">
          <label class="popup-open"  for="setup">SETUP</label>
        </div>  
        <br>
        <hr class="line">

     <div class="background_white">
      <div class="cards">
        <!--meter en pulsen hier-->
         <div class="card">   
            <p>
              <span>Pulse & P1</span><br>
              <span class="ds-labels"></span> 
              <span id="pulse1">%count1%</span>
            </p>
             </div>
      </div>
       </div>

     <input type="checkbox" id="netwerk" class="popup-opener" />
     <div class="popup">
      <div id="networkcontent" >
      <br>
      <br>
      <legend style="color: white;">Wi-Fi Settings</legend>
      <h6 class="text-muted" style="color: white;">Type your Wi-Fi Networks SSID or Scan for nearby Wireless Networks to join.</h6>
      <br>
      <div class="row form-group">
       <label class="col-xs-3">Wi-Fi Mode<i style="margin-left: 10px;"  aria-hidden="true" data-toggle="popover" data-trigger="hover" data-placement="right" data-content="You can run your ESP in AP Mode or Client Mode. In client mode you will need to connect to an existing Wi-Fi network, in AP Mode ESP creates a Wi-Fi network itself."></i></label>
       <div class="col-xs-9">
        <form>
         <label class="radio-inline">
          <input type="radio" value="1" name="wmode" id="wmodeap" onclick="handleAP();" checked>Access Point </label>
         <label class="radio-inline">
          <input type="radio" value="0" name="wmode" id="wmodesta" onclick="handleSTA();">Client</label>
        </form>
      </div>
      </div>
      <div class="row form-group">
       <label class="col-xs-3">SSID<i style="margin-left: 10px;"  aria-hidden="true" data-toggle="popover" data-trigger="hover" data-placement="right" data-content="Wi-Fi Network's Name"></i></label>
       <span class="col-xs-7 col-md-5">
         <input class="form-control input-sm" id="inputtohide" type="text" name="ap_ssid">
         <select class="form-control input-sm" style="display:none;" id="ssid" onchange="listBSSID();"></select>
          </span>
       <span class="col-xs-2">
         <button id="scanb" type="button" class="btn btn-primary btn-xs" style="display:none;" onclick="scanWifi()">Scan</button>
          </span>
      </div>
      <div class="row form-group" style="display:none" id="hideBSSID">
       <label class="col-xs-3">BSSID<i style="margin-left: 10px;"  aria-hidden="true" data-toggle="popover" data-trigger="hover" data-placement="right" data-content="If you have two Wi-Fi networks with the same SSID you may want to choose the specific one by BSSID"></i></label>
       <span class="col-xs-9 col-md-5">
          <input id="wifibssid" class="form-control input-sm" type="text" name="ap_bssid">
          </span>
      </div>
      <div class="row form-group">
       <label class="col-xs-3">Password<i style="margin-left: 10px;"  aria-hidden="true" data-toggle="popover" data-trigger="hover" data-placement="right" data-content="Wi-Fi Password"></i></label>
       <span class="col-xs-9 col-md-5">
          <input id="wifipass" class="form-control input-sm" name="ap_passwd" type="password">
          </span>
      </div>
      <br>
      <div class="row form-group" id="hideap">
       <label class="col-xs-3">Hide Network Name<i style="margin-left: 10px;"  aria-hidden="true" data-toggle="popover" data-trigger="hover" data-placement="right" data-content="You can choose to hide Wireless Network name, so your ESP does not broadcast SSID, keep in mind that people can still find your network if they are insist."></i></label>
       <div class="col-xs-9">
        <form>
         <label class="radio-inline">
          <input type="radio" value="1" name="hideapenable">Yes</label>
         <label class="radio-inline">
          <input type="radio" value="0" name="hideapenable" checked>No</label>
        </form>
       </div>
      </div>
      <br>
      <div class="row form-group" id="dhcp">
       <label class="col-xs-3">Use DHCP<i style="margin-left: 10px;"  aria-hidden="true" data-toggle="popover" data-trigger="hover" data-placement="right" data-content="Choose if you want to get an IP address from DHCP server or not."></i></label>
       <div class="col-xs-9">
        <form>
         <label class="radio-inline">
          <input type="radio" value="1" name=",dhcpenabled," onclick="handleDHCP();" checked>Yes</label>
         <label class="radio-inline">
          <input type="radio" value="0" name=",dhcpenabled," onclick="handleDHCP();">No</label>
        </form>
       </div>
      </div>
      <div id="staticip1">
       <div class="row form-group">
        <label class="col-xs-3">IP Address<i style="margin-left: 10px;" aria-hidden="true" data-html="true" data-toggle="popover" data-trigger="hover" data-placement="right" data-content="Enter IP Address<BR>(for example 192.168.4.1)"></i></label>
        <span class="col-xs-9 col-md-5">
          <input id="ipaddress" class="form-control input-sm">
          </span>
       </div>
       <div class="row form-group">
        <label class="col-xs-3">Subnet Mask<i style="margin-left: 10px;"  aria-hidden="true" data-html="true" data-toggle="popover" data-trigger="hover" data-placement="right" data-content="Enter Subnet Mask<BR>(for example 255.255.255.0)"></i></label>
        <span class="col-xs-9 col-md-5">
          <input id="subnet" class="form-control input-sm">
          </span>
       </div>
      </div>
      <div id="staticip2" style="display:none">
       <div class="row form-group">
        <label class="col-xs-3">DNS Server</label>
        <span class="col-xs-9 col-md-5">
          <input id="dnsadd" class="form-control input-sm">
          </span>
       </div>
       <div class="row form-group">
        <label class="col-xs-3">Gateway</label>
        <span class="col-xs-9 col-md-5">
          <input id="gateway" class="form-control input-sm">
          </span>
       </div>
      </div>
      <br>
      <div class="row form-group">
       <label class="col-xs-3">Auto Disable Wifi<i style="margin-left: 10px;"  aria-hidden="true" data-toggle="popover" data-trigger="hover" data-placement="right" data-content="Disable WiFi after a time interval, to conserve power in battery powered units. WiFi is re-enabled when an admin card is scanned, or device is rebooted. Remember that you need to have enough time to upload your configuration, so don't set it too low."></i></label>
       <span class="col-xs-9 col-md-5">
         <select class="form-control input-sm" id="disable_wifi_after_seconds" name="auto_restart_interval_seconds">
           <option value='0'>Always on</option>
           <option value='180'>3 min</option>
           <option value='240'>4 min</option>
           <option value='300'>5 min</option>
           <option value='600'>10 min</option>
           <option value='900'>15 min</option>
           <option value='1800'>30 min</option>
         </select>
          </span>
       <br>
       <br>
       <div class="col-xs-9 col-md-8">
        <button onclick="savenetwork()" class="btn btn-primary btn-sm pull-right">Save</button>
       </div>
      </div>
      <br>
      <br>
     </div>
      </div>
      <input type="checkbox" id="server" class="popup-opener" />
     <div class="popup">
     <h1>welkom</h1>
     <h1>welkom</h1>
     <h1>welkom</h1>
     <h1>welkom</h1>
     <h1>welkom</h1>
     <h1>welkom</h1>
     <h1>welkom</h1>
     <h1>welkom</h1> 
     <h1>welkom</h1>
     <h1>welkom</h1>
     <h1>welkom</h1>
     <h1>welkom</h1>
     <h1>welkom</h1>
    </div>
    <input type="checkbox" id="setup" class="popup-opener"/>
    <div class="popup">
      </div>

      <script>

 setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pulse1").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/count1", true);
  xhttp.send();
} ) ;
  </script>

    </body>
</html>
)=====";