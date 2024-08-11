#include "argparse.h"
#include "traffic.h"

int main(int args, char **argv) {
  ArgParse argparse(args, argv);
  if (argparse.isValid()) {
    argparse.parse();
    if (argparse.run) {
      test();
    }
  }
  return 0;
}
