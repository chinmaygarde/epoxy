
#include <string>
#include <vector>

namespace epoxy {

bool Main(std::vector<std::string> args) {
  return false;
}

}  // namespace epoxy

int main(int argc, const char* argv[]) {
  std::vector<std::string> arguments;
  arguments.reserve(argc);
  for (int i = 0; i < argc; i++) {
    arguments.push_back(argv[i]);
  }
  return epoxy::Main(std::move(arguments)) ? EXIT_SUCCESS : EXIT_FAILURE;
}
