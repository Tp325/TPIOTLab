#include "MeshLora.h"
MeshLora a(1, 9600, 915E6, 5, 2, 3, 4);
void setup() {
}

void loop() {
  a.sendMessage("help",1);
}
