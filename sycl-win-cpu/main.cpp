#include <sycl/sycl.hpp>
#include <iostream>

int main() {
  try {
    sycl::queue q{sycl::cpu_selector_v};
    std::cout << "Device: "
              << q.get_device().get_info<sycl::info::device::name>()
              << "\n";

    int data = 0;
    {
      sycl::buffer<int> buf(&data, sycl::range<1>(1));
      q.submit([&](sycl::handler &h) {
        auto acc = buf.get_access<sycl::access::mode::write>(h);
        h.single_task([=]() { acc[0] = 42; });
      });
    }

    std::cout << "Result: " << data << "\n";
    return 0;
  } catch (const sycl::exception &e) {
    std::cerr << "SYCL exception: " << e.what() << "\n";
    return 1;
  }
}
