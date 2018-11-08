#include "client_socket.h"
#include <cstdlib>

using namespace std;

#define DRIVER_KEY "$$$### "
#define DONE "done\n"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    cerr << "valid operation: " << argv[0] << " [server address] [server port]\n";
    exit(-1);
  }
  
  ClientSocket socket(string(argv[1]), Tools::mystoi(argv[2]));
  cout << "# connection established\n";
  
  string line;
  while (getline(cin, line)) {
    if (line == ":q")
      break;
    line = DRIVER_KEY + line;
    socket.send(line); 
    string message = socket.receive();
    if(message != DONE)
      cout << message;
  }
  
  return 0;
}
