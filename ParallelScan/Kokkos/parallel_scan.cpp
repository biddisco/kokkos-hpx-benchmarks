#include <Kokkos_Core.hpp>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/time.h>
#include <unistd.h>
#include <chrono>

#if defined(KOKKOS_ENABLE_DEFAULT_DEVICE_TYPE_OPENMP)
#define KOKKOS_DEVICE "OpenMP"
#elif defined(KOKKOS_ENABLE_DEFAULT_DEVICE_TYPE_OPENMPTARGET)
#define KOKKOS_DEVICE "OpenMPTarget"
#elif defined(KOKKOS_ENABLE_DEFAULT_DEVICE_TYPE_HPX)
#define KOKKOS_DEVICE "HPX"
#include <hpx/hpx_main.hpp>
#elif defined(KOKKOS_ENABLE_DEFAULT_DEVICE_TYPE_THREADS)
#define KOKKOS_DEVICE "Threads"
#elif defined(KOKKOS_ENABLE_DEFAULT_DEVICE_TYPE_SERIAL)
#define KOKKOS_DEVICE "Serial"
#else
#define KOKKOS_DEVICE "Unknown"
#endif

int main(int argc, char *argv[]) {

  int len = 100000; // length of vectors
  int nrepeat = 10; // number of repeats of the test
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
      printf("  -nrepeat <int>:           number of repitions (default: 10)\n");
      printf("  -help (-h):               print this message\n");
    }
  }

  char hostname[HOST_NAME_MAX];
  gethostname(hostname, HOST_NAME_MAX);
  std::string benchmark = "parallel_scan";
  std::string runtime = "Kokkos-" + std::string(KOKKOS_DEVICE);

  Kokkos::initialize(argc, argv);

  {
    Kokkos::View<double *> a("A", len);
    Kokkos::View<double *> b("B", len);

    Kokkos::parallel_for(len, KOKKOS_LAMBDA(const int &i) { a(i) = i; });

    using namespace std::chrono;
    const auto before = system_clock::now();

    for (int repeat = 0; repeat < nrepeat; repeat++) {
      Kokkos::parallel_scan(
          len, KOKKOS_LAMBDA(const int &i, double &upd, const bool &final) {
            upd += a(i);
            if (final) {
              b(i) = upd;
            }
          });
    }

    const duration<double> duration = system_clock::now() - before;

    if (header) {
      std::cout << "hostname, timestamp, num_threads, benchmark, runtime "
                   "input_size_1, input_size_2, num_repeats, time, result, "
                   "specific_metric, metric_name"
                << std::endl;
    }
    std::cout << hostname << ", " << std::time(nullptr) << ", "
              << Kokkos::DefaultExecutionSpace::concurrency() << ", "
              << benchmark << ", " << runtime << ", " << len << ", " << 0
              << ", " << nrepeat << ", " << duration.count() << ", " << b(len - 1) << ", "
              << 0 << ", x" << std::endl;
  }

  Kokkos::finalize();
}
