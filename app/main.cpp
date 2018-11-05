#include <iostream>
#include <thread>
#include <vector>

#include <Logger.h>

// An arbitrary maximum number of threads.
#define MAX_NB_THREADS 1000

// A simple test routine.
static void test_routine(int i) {
  // Getting the logger instance.
  auto &logger = Logger::get_instance();

  // Generating a level from i, with a round-robin strategy.
  const auto level =
      static_cast<Logger::level>(Logger::INFO
          + i % (Logger::ERROR + 1 - Logger::INFO));

  // Generating a message from i.
  const auto message =
      std::string("this is the message n° " + std::to_string(i) + ".");

  // Waiting for 10ms.
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  // Logging the message with a the round-robin logger level.
  logger.print(level, message);
}

// An exception to use when the main class does not have enough argument.
class missing_argument final : public std::exception {};

int main(int argc, char const *argv[]) {
  // Initializing the program exit code, success by default.
  auto exit_code = int(EXIT_SUCCESS);
  // Using our logger for error logging.
  auto &logger = Logger::get_instance();
  // Syntactic sugar for first parameter.
  auto arg = std::string();

  std::cout << "Logger singleton example." << std::endl;
  std::cout << "Take a number of thread from first argument (from 0 to "
            << (MAX_NB_THREADS)
            << "), and execute concurrently simple routines." << std::endl;

  std::cout << "Setting the logger output to std::cerr, so the log can be "
               "extracted and saved in a file." << std::endl;

  // Setting the logger output to std::cerr, so the log can be save from
  // command line using " 2> <log_file>"
  Logger::set_output(&std::cerr);

  try {

    // Checking the number of arguments.
    if (argc < 2) {
      throw (missing_argument());
    }
    arg.assign(argv[1]); // Pointer arithmetic is a bad practice, but I am lazy.

    // Converting the number of thread from the first argument to an unsigned
    // int.
    auto nb_threads = std::stoul(arg);

    if (nb_threads > (MAX_NB_THREADS)) {
      // If the number of threads is above MAX_NB_THREADS, throw an error.
      throw (std::out_of_range(""));
    }

    std::cout << "Creating  and joining " << nb_threads << " thread"
              << (nb_threads == 1 ? "" : "s") << "." << std::endl;

    // Pre-allocation of the threads vector.
    auto threads = std::vector<std::thread>(nb_threads);

    // Initialization of the threads.
    for (auto i = 0U; i < nb_threads; ++i) {
      threads[i] = std::thread(test_routine, i);
    }

    // Joining the threads.
    for (auto &thread : threads) {
      thread.join();
    }

    std::cout << "All the threads finished their routines." << std::endl;

  } catch (missing_argument &e) {

    // Bad number of argument, at least one needed.
    logger.print(Logger::ERROR, "1 argument needed (the number of threads).");
    exit_code = EXIT_FAILURE; // Setting the program exit code to failure.

  } catch (std::invalid_argument &e) {

    // Invalid argument, should be a number.
    logger.print(Logger::ERROR, "No conversion could be performed from " + arg);
    exit_code = EXIT_FAILURE; // Setting the program exit code to failure.

  } catch (std::out_of_range &e) {

    // Argument out of range, below 0, or above MAX_NB_THREADS.
    logger.print(Logger::ERROR,
                 "Value out of range. Argument provided ( " + arg +
                     " ) is not in the acceptable range [0, "
                     + std::to_string(MAX_NB_THREADS) + "].");
    exit_code = EXIT_FAILURE; // Setting the program exit code to failure.

  } catch (...) {

    // Unknown error.
    logger.print(Logger::ERROR, "Unknown error.");

    throw; // Re-throw the unknown error. The execution will be aborted.
  }

  std::cout << "Done with "
            << (exit_code == (EXIT_SUCCESS) ? "success" : "failure") << "."
            << std::endl;

  // Exiting the program with the exit code.
  return (exit_code);
}
