#ifndef SINGLETON_EXAMPLE_LOGGER_H
#define SINGLETON_EXAMPLE_LOGGER_H

#include <mutex>
#include <ostream>

// The C++11 singleton-based logger class.
// Allows the displaying of message with different log level support.
class Logger final {
 private:
  // Private default constructor.
  Logger() = default;

  // Pointer to the logger output. NULL by default. Private for protection.
  static std::ostream *p_output_;

  // A mutex to protect the display resource.
  static std::mutex output_mutex_;

 public:
  // A function that allows the setting of the logger output.
  // Nothing by default.
  static void set_output(std::ostream *p_output = nullptr);

  // The different logger levels.
  enum level : unsigned char {
    INFO,
    WARN,
    ERROR,
  };

  // Initialize the Logger singleton once, and return it each time this class
  // method is called.
  static Logger &get_instance();

  // Default logger destructor. No need for virtual destructor as the logger
  // class cannot be inherited.
  ~Logger() = default;

  // Deleted copy constructor.
  // Only the get_instance class method is allowed to give a logger.
  Logger(const Logger &) = delete;

  // Deleted copy assignment.
  // Only the get_instance class method is allowed to give a logger.
  void operator=(const Logger &) = delete;

  // Default move constructor.
  Logger(Logger &&) noexcept = default;

  // Default move assignment.
  Logger &operator=(Logger &&) noexcept = default;

  // Print a message with a certain logger level on the output.
  void print(enum level level, const std::string &message);
};

#endif //SINGLETON_EXAMPLE_LOGGER_H
