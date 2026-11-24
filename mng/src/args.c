#include "args.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_usage(const char *arg0) {
  printf("Usage: %s [OPTION] <PATH>\n"
         "\n"
         "Options:\n"
         "  -h, --help\t\tShow this help message and exit.\n",
         arg0);
}

bool parse_args(int argc, char **argv, Args *args) {
  args->init_path = (argc > 1 && argv[1]) ? argv[1] : NULL;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {0, 0, 0, 0}};

  int opt;
  int option_idx = 0;
  while ((opt = getopt_long(argc, argv, "h:", long_options, &option_idx)) !=
         -1) {
    switch (opt) {
    case 'h':
      print_usage(argv[0]);
      exit(EXIT_SUCCESS);
    case '?':
      print_usage(argv[0]);
      exit(EXIT_FAILURE);
    default:
      abort();
    }
  }

  return true;
}
