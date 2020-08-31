#include <Arduino.h>
#include "M5Atom.h"
#include "painlessMesh.h"

#define   MESH_PREFIX       "GramsiOnAir"
#define   MESH_PASSWORD   "eumel0815"
#define   MESH_PORT       5555

painlessMesh mesh;

String nodeName = "onAirButton"; // Name 
String bmsg = "none";

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void setup() {
  M5.begin(true, false, true);
  delay(50);
  M5.dis.drawpix(0, 0x707070);

  // Mesh stuff
  Serial.begin(115200);
  mesh.setDebugMsgTypes(ERROR | STARTUP | DEBUG | CONNECTION);  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, 6 );

  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.setRoot();
  mesh.sendBroadcast(bmsg);
}

uint8_t FSM = 0;

void loop()
{
 
    if (M5.Btn.wasPressed())
    {

        switch (FSM)
        {
        case 0:
            M5.dis.drawpix(0, 0xf00000); //green
            bmsg = "one";
            mesh.sendBroadcast(bmsg);
            break;
        case 1:
            M5.dis.drawpix(0, 0x0000f0); //blue
            bmsg = "two";
            mesh.sendBroadcast(bmsg);
            break;
        case 2:
            M5.dis.drawpix(0, 0x707070); //white
            bmsg = "none";
            mesh.sendBroadcast(bmsg);
            break;
        default:
            break;
        }
        FSM++;
        if (FSM >= 3)
        {
            FSM = 0;
        }
    }

    //delay(50);
    M5.update();
    mesh.update();
}