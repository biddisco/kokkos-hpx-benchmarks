#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>
#include <sys/time.h>
#include <unistd.h>
#include <vector>
#include <chrono>

int main(int argc, char *argv[]) {
  int len = 10000;
  int nrepeat = 10;
  bool header = false;

  for (int i = 0; i < argc; i++) {
    if ((strcmp(argv[i], "-l") == 0) || (strcmp(argv[i], "-v") == 0)) {
      len = atof(argv[++i]);
    } else if (strcmp(argv[i], "-nrepeat") == 0) {
      nrepeat = atoi(argv[++i]);
    } else if (strcmp(argv[i], "-header") == 0) {
        header = true;
    } else if ((strcmp(argv[i], "-h") == 0) ||
               (strcmp(argv[i], "-help") == 0)) {
      printf("ParallelFor Options:\n");
      printf("  -length (-l) <int>:       vector length (default: 10000)\n");
      printf("  -nrepeat <int>:           number of repitions (default: 10)\n");
      printf("  -help (-h):               print this message\n");
    }
  }

  char hostname[HOST_NAME_MAX];
  gethostname(hostname, HOST_NAME_MAX);
  std::string benchmark = "parallel_for";
  std::string runtime = "Serial";

  std::vector<double> a(len);

  for (std::size_t i = 0; i < len; ++i) {
    a[i] = i;
  }

  using namespace std::chrono;
  const auto before = system_clock::now();

  for (int repeat = 0; repeat < nrepeat; repeat++) {
    for (std::size_t i = 0; i < len; ++i) {
      a[i] = a[i] * std::pow(5.3, 3) / 3.14;
    }
  }

  const duration<double> duration = system_clock::now() - before;

  if (header) {
    std::cout << "hostname, timestamp, num_threads, benchmark, runtime "
                 "input_size_1, input_size_2, num_repeats, time, result, "
                 "specific_metric, metric_name"
              << std::endl;
  }
  std::cout << hostname << ", " << std::time(nullptr) << ", " << 1 << ", "
            << benchmark << ", " << runtime << ", " << len << ", " << 0 << ", "
            << nrepeat << ", " << duration.count() << ", " << a[len - 1] << ", " << 0
            << ", x" << std::endl;
}
