#include "handler.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 4)
    return EXIT_FAILURE;
  for (int i = 0; i < 3; i++) {

    if (!(VerifyFile(argv[i + 1], i)))
      return EXIT_FAILURE;
  }
}
