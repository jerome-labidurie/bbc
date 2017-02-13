/**
 *  This file is part of BBC Notif.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2017 Jérôme Labidurie jerome@labidurie.fr
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>


#define HOSTNAME "BBCnotif"
#define baseSSID "BBC-"
#define VERSION "BBC Notif v1.0"
char mySSID[13];

ESP8266WebServer server(80);

void setup (void) {
	uint8_t r = 0;
	IPAddress myIP;
	char mySSID[13];
	uint8_t mac[6];

	EEPROM.begin(512);
	Serial.begin (115200);
	Serial.println (VERSION);

	// create uniq ssid
	WiFi.macAddress(mac);
	snprintf (mySSID, 13, "%s%02X%02X", baseSSID, mac[4], mac[5]);

	// setup wifi
	do {
		r = wicoWifiConfig (0, mySSID, &myIP);
		Serial.println(myIP);
		if (r == 0 ) {
			// AP has been created, start web server
			Serial.write (mySSID);
			wicoSetupWebServer ();
		}
	} while (!r);

	// setup webserver
	WiFi.hostname (HOSTNAME);
	server.on ("/", handleRoot);
	server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println("HTTP server started");

	// Set up mDNS responder:
	// - first argument is the domain name, in this example
	//   the fully-qualified domain name is "esp8266.local"
	// - second argument is the IP address to advertise
	//   we send our IP address on the WiFi network
	if (!MDNS.begin(HOSTNAME, myIP)) {
		Serial.println("Error setting up MDNS responder!");
		while(1) {
			delay(1000);
		}
	}
	Serial.println("mDNS responder started");
	// Add service to MDNS-SD
	MDNS.addService("http", "tcp", 80);
} // setup()

void loop(void) {
	server.handleClient();
} // loop()


void handleRoot(void) {
	char ssid[32];
	char pwd[64];
	char email[100];

	// get current config
	memset (ssid,  0,  32);
	memset (pwd,   0,  64);
	memset (email, 0, 100);
	wicoReadWifiConfig (0, ssid, pwd, email);

	String s = "<html><body><h1>BBC</h1>";
	s += "<br/>SSID: ";  s += ssid;
	s += "<br/>email: "; s += email;
	s += "<p><form>Reset config <input type='checkbox' name='reset'><input type='submit' value='send'></form></p>\n";
	s += "</body></html>";
	if (server.hasArg("reset") ) {
		Serial.println("reset");
		wicoResetWifiConfig (0);
	}

	server.send ( 200, "text/html", s );
} // handleRoot()

void handleNotFound() {
  String message = "Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  server.send ( 404, "text/plain", message );
}
