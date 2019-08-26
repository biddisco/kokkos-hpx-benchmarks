
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/time.h>
#include <unistd.h>
#include <chrono>
#include <iostream>
#include <vector>

#define HOST_NAME_MAX 256

using namespace std::chrono;

template<typename TimeT = microseconds>
void work_for_time(TimeT count)
{
    auto start = high_resolution_clock::now();
    do {}
    while (duration_cast<TimeT>(high_resolution_clock::now()-start)<count);
}

int main(int argc, char *argv[]) {

  int len = 100000; // length of vectors
  int nrepeat = 10; // number of repeats of the test
  bool header = false;
  int us = 10;

  for (int i = 0; i < argc; i++) {
    if ((strcmp(argv[i], "-l") == 0) || (strcmp(argv[i], "-v") == 0)) {
      len = atof(argv[++i]);
    } else if (strcmp(argv[i], "-nrepeat") == 0) {
      nrepeat = atoi(argv[++i]);
    } else if (strcmp(argv[i], "-header") == 0) {
      header = true;
    } else if (strcmp(argv[i], "-tasktime") == 0) {
        us = atof(argv[++i]);
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
  std::string benchmark = "task-overhead";
  std::string runtime = "Serial-";


  {
    std::vector<double> a(len);

    for (int64_t i=0; i<len; ++i) { a[i] = i; }

    const auto before = system_clock::now();

    for (int repeat = 0; repeat < nrepeat; repeat++) {
      for (int64_t i=0; i<len; ++i) {
        work_for_time<microseconds>(microseconds(us));
      }
    }

    const duration<double> duration = system_clock::now() - before;
    double time_per_iteration_us = 1.0E6 * duration.count() / (nrepeat*len);

    if (header) {
      std::cout << "hostname, timestamp, num_threads, benchmark, runtime "
                   "input_size_1, input_size_2, num_repeats, time, result, "
                   "specific_metric, metric_name"
                << std::endl;
    }
    std::cout << hostname << ", " << std::time(nullptr) << ", "
              << 1 << ", "
              << benchmark << ", " << runtime << ", " << len << ", " << 0
              << ", " << nrepeat << ", " << time_per_iteration_us << ", " << a[len - 1] << ", "
              << 0 << ", x" << std::endl;
  }

}
