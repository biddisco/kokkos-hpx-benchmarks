#include <hpx/hpx_main.hpp>
#include <hpx/include/iostreams.hpp>
#include <hpx/include/parallel_for_loop.hpp>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int num_vectors = 1000;
  int len = 10000;
  int nrepeat = 10;
  bool header = false;

  for (int i = 0; i < argc; i++) {
    if ((strcmp(argv[i], "-v") == 0) ||
        (strcmp(argv[i], "-num_vectors") == 0)) {
      num_vectors = atoi(argv[++i]);
    } else if ((strcmp(argv[i], "-l") == 0) || (strcmp(argv[i], "-v") == 0)) {
      len = atof(argv[++i]);
    } else if (strcmp(argv[i], "-nrepeat") == 0) {
      nrepeat = atoi(argv[++i]);
    } else if (strcmp(argv[i], "-header") == 0) {
      header = true;
    } else if ((strcmp(argv[i], "-h") == 0) ||
               (strcmp(argv[i], "-help") == 0)) {
      printf("ArrayOfDotProducts Options:\n");
      printf("  -num_vectors (-v)  <int>: number of vectors (default: 1000)\n");
      printf("  -length (-l) <int>:       vector length (default: 10000)\n");
      printf("  -nrepeat <int>:           number of repitions (default: 10)\n");
      printf("  -help (-h):               print this message\n");
    }
  }

  char hostname[HOST_NAME_MAX];
  gethostname(hostname, HOST_NAME_MAX);
  std::string benchmark = "array_of_dot_products";
  std::string runtime = "HPX";

  // allocate space for vectors to do num_vectors dot products of length len
  double *a = new double[num_vectors * len];
  double *b = new double[num_vectors * len];
  double *c = new double[num_vectors];

  // Initialize vectors
  hpx::parallel::for_loop(hpx::parallel::execution::par, 0, num_vectors,
                          [len, a, b, c](const size_t i) {
                            for (int j = 0; j < len; j++) {
                              a[i * len + j] = i + 1;
                              b[i * len + j] = j + 1;
                            }
                            c[i] = 0.0;
                          });

  // Time dot products
  struct timeval begin, end;

  gettimeofday(&begin, NULL);

  for (int repeat = 0; repeat < nrepeat; repeat++) {
    hpx::parallel::for_loop(hpx::parallel::execution::par, 0, num_vectors,
                            [len, a, b, c](const size_t i) {
                              double ctmp = 0.0;
                              for (int j = 0; j < len; j++) {
                                ctmp += a[i * len + j] * b[i * len + j];
                              }
                              c[i] = ctmp;
                            });
  }

  gettimeofday(&end, NULL);

  // Calculate time
  double time = 1.0 * (end.tv_sec - begin.tv_sec) +
                1.0e-6 * (end.tv_usec - begin.tv_usec);

  // Error check
  int error = 0;
  for (int i = 0; i < num_vectors; i++) {
    double diff = ((c[i] - 1.0 * (i + 1) * len * (len + 1) / 2)) /
                  ((i + 1) * len * (len + 1) / 2);
    if (diff * diff > 1e-20) {
      error = 1;
      printf("Error: %i %i %i %lf %lf %e %lf\n", i, num_vectors, len, c[i],
             1.0 * (i + 1) * len * (len + 1) / 2,
             c[i] - 1.0 * (i + 1) * len * (len + 1) / 2, diff);
    }
  }

  if (error != 0) {
    hpx::cerr << "Error!" << hpx::endl;
  }

  if (header) {
    hpx::cout << "hostname, timestamp, num_threads, benchmark, runtime "
                 "input_size_1, input_size_2, num_repeats, time, result, "
                 "specific_metric, metric_name"
              << hpx::endl;
  }
  hpx::cout << hostname << ", " << std::time(nullptr) << ", "
            << hpx::resource::get_num_threads("default") << ", " << benchmark
            << ", " << runtime << ", " << len << ", " << num_vectors << ", "
            << nrepeat << ", " << time << ", " << error << ", "
            << 1.0e-9 * num_vectors * len * 2 * 8 * nrepeat / time
            << ", bandwidth_GB_per_s" << std::endl;

  return hpx::finalize();
}
