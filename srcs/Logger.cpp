#include <Logger.h>

#include <sstream>

// Initialization of the display pointer, a pointer to std::cout.
std::ostream *Logger::p_output_ = nullptr;

// Initialization of the display protection mutex.
std::mutex Logger::output_mutex_;

void Logger::set_output(std::ostream *p_output) {
  auto &&lock __attribute__((unused)) =
      std::lock_guard<std::mutex>(output_mutex_);
  p_output_ = p_output;
}

Logger &Logger::get_instance() {
  // The logger variable is initialized once with a move assignment of a logger
  // object build with the private constructor.
  static auto &&logger = Logger();
  return (logger);
}

void Logger::print(Logger::level level, const std::string &message) {
  // Initializing a string stream.
  auto ss = std::stringstream();

  // Buffering the log level in the string stream.
  switch (level) {
    case (INFO):ss << "INFO";
      break;
    case (WARN):ss << "WARN";
      break;
    case (ERROR):ss << "ERROR";
      break;
  }
  // Buffering the message in the string stream.
  ss << ": " << message;

  // Waiting for output access.
  auto &&lock __attribute__((unused)) =
      std::lock_guard<std::mutex>(output_mutex_);
  if (p_output_ != nullptr) {
    // If the output is set, then display the string stream.
    (*p_output_) << ss.rdbuf() << std::endl;
  }

  // In a real application, log might also be saved in file.
}
