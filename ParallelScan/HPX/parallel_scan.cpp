#include <hpx/hpx_main.hpp>
#include <hpx/include/iostreams.hpp>
#include <hpx/include/parallel_for_loop.hpp>
#include <hpx/include/parallel_scan.hpp>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/time.h>
#include <unistd.h>

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
      printf("ParallelScan Options:\n");
      printf("  -length (-l) <int>:       vector length (default: 10000)\n");
      printf("  -nrepeat <int>:           number of repetitions (default: 10)\n");
      printf("  -help (-h):               print this message\n");
    }
  }

  char hostname[HOST_NAME_MAX];
  gethostname(hostname, HOST_NAME_MAX);
  std::string benchmark = "parallel_scan";
  std::string runtime = "HPX";

  std::vector<double> a(len);
  std::vector<double> b(len);

  hpx::parallel::for_loop(hpx::parallel::execution::par, 0, len,
                          [&a](const size_t i) { a[i] = i; });

  struct timeval begin, end;

  gettimeofday(&begin, NULL);

  for (int repeat = 0; repeat < nrepeat; repeat++) {
    hpx::parallel::inclusive_scan(hpx::parallel::execution::par, std::begin(a),
                                  std::end(a), std::begin(b));
  }

  gettimeofday(&end, NULL);

  double time = 1.0 * (end.tv_sec - begin.tv_sec) +
                1.0e-6 * (end.tv_usec - begin.tv_usec);

  if (header) {
    hpx::cout << "hostname,timestamp,num_threads,benchmark,runtime,"
                 "input_size_1,input_size_2,num_repeats,time,result,"
                 "specific_metric,metric_name"
              << hpx::endl;
  }
  hpx::cout << hostname << "," << std::time(nullptr) << ","
            << hpx::resource::get_num_threads("default") << "," << benchmark
            << "," << runtime << "," << len << "," << 0 << "," << nrepeat << ","
            << time << "," << b[len - 1] << "," << 0 << ",x" << std::endl;

  return hpx::finalize();
}
