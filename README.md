# singleton_example

Modern C++ singleton implementation through an example article [published](https://hnrck.io/cpp/2018/11/05/1-modern-cpp-singleton.html) on my blog.

**[tl;dr]** Modern C++ best practices allows an explicit and beautiful implementation of the singleton design pattern.

---

Code source of the full example available on [hnrck/singleton_example](https://github.com/hnrck/singleton_example).

---

Table of Contents
=================

   * [Motivations](#motivations)
   * [Context](#context)
   * [Related work](#related-work)
      * [Blogs and websites](#blogs-and-websites)
      * [Books](#books)
   * [Introduction](#introduction)
      * [About design patterns](#about-design-patterns)
      * [The singleton pattern](#the-singleton-pattern)
         * [Why using a singleton](#why-using-a-singleton)
         * [How to implement a singleton](#how-to-implement-a-singleton)
   * [The Logger class](#the-logger-class)
      * [The Logger class declaration](#the-logger-class-declaration)
         * [Constructor and assignment](#constructor-and-assignment)
         * [Copy constructor and assignment](#copy-constructor-and-assignment)
         * [Move constructor and assignment](#move-constructor-and-assignment)
      * [Singleton functionality](#singleton-functionality)
      * [Logger functionality](#logger-functionality)
         * [Log level](#log-level)
         * [Print method](#print-method)
         * [Output setting](#output-setting)
         * [Output protection](#output-protection)
         * [Logger class final result](#logger-class-final-result)
      * [Test application](#test-application)
         * [Global parameters](#global-parameters)
         * [Test routine](#test-routine)
         * [A small exception](#a-small-exception)
         * [Main function](#main-function)
         * [Test application final result](#test-application-final-result)
   * [Experimentation](#experimentation)
      * [GitHub repository](#github-repository)
      * [Compilation](#compilation)
      * [Execution](#execution)
   * [Conclusion](#conclusion)
      * [Summary](#summary)
      * [Limitations](#limitations)
      * [Future work](#future-work)

---

# Motivations

First of all, *pardon my French*, as a French native speaker, with a fairly solid background in English, some of my sentences may seem flawed. 

For a few years now, I've been thinking about maintaining a blog on programming.
I am now a reasonably experienced engineer.
I have worked on stimulating projects at Thales and Airbus in France, and I want to share some visions I have on my field.
As I am finishing my Ph.D. thesis, I have started my job search process.
Latterly, I participated in a test, during which I had a few tens of minutes to answer questions about implementation in modern C++.
From my point of view, I had no difficulty answering the questions, and to be honest, I even had much fun.
Nevertheless, once the test was over, I took my books from C++ to check my answers, and I noticed that they were rather old on their recommendation for design pattern implementation.

I, therefore, take this opportunity to express my point of view on modern design in C++.

My contribution on this blog is particularly minimal, and I'm sure that some books I don't have yet offer even better implementation methods than mine.
Furthermore, some C++ blogs are way more interesting than mine. I prepared a small related work section in this article.
Anyway, I think it's time for me to stop hiding behind the excuse that someone somewhere must have already done better not to publish.
If by any chance you are interested in my vision, and you identify gaps in my proposals, I would be particularly glad to discuss this in the comments.

---

# Context

One of the questions in my test was, from memory:
> Implement a Logger, based on the singleton.
>
> The Logger should have a print method taking as input a log level (INFO, WARN, ERROR), and a message.
>
> The use of C++11 and a thread-safe implementation will be appreciated.

This question was particularly inspiring to me.
I implemented a distributed simulation framework in C++11, in which I made extensive use of design patterns.

By the greatest coincidence, I had made in this framework a similar Logger class to trace the execution of simulation components.
The significant differences being that :
* Using a simulation framework distributed between my components, I did not need to make sure my implementation was thread-safe.
* The singleton part was at the simulation components level.

I rewrote a singleton in much the same way as I did in my framework, making sure to make it thread-safe.

---

# Related work

Here is a non-exhaustive list of resources among my favorite blogs and sites, as well as some books I have read, which I find appropriate for this article.

All these resources are much more professional and educational than my work, and I highly recommend them if you want to go further.

&nbsp;

## Blogs and websites

* [C++ Programming: Code patterns design](https://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns)
* [Fluent\{C\+\+\}](https://www.fluentcpp.com/)
* [Andrzej's C++ blog](https://akrzemi1.wordpress.com/)
* [The view from aristeia](http://scottmeyers.blogspot.com/)
* [ISO C++ blog](https://isocpp.org/blog)
* [C++ Core guidelines](https://github.com/isocpp/CppCoreGuidelines)
* ... And much more, just google 

&nbsp;

## Books

* [The C++ Programming Language](http://www.stroustrup.com/4th.html)
* [Design Patterns: Elements of Reusable Object-Oriented Software](https://www.oreilly.com/library/view/design-patterns-elements/0201633612/)
* [Programmer en langage C++](https://www.editions-eyrolles.com/Livre/9782212673869/programmer-en-langage-c)
* [Effective Modern C++](https://www.oreilly.com/library/view/effective-modern-c/9781491908419/)
* [Safe C++](https://www.oreilly.com/library/view/safe-c/9781449321338/)
* [C++ Concurrency in Action](https://www.manning.com/books/c-plus-plus-concurrency-in-action)
* ... And much more than I have not yet had the opportunity to read ...

---

# Introduction

## About design patterns

In software engineering, [design patterns](https://en.wikipedia.org/wiki/Software_design_pattern) are software bricks, recognized as good practice, and addressing common design issues.

There exist many design patterns, and most of the time, we think first and foremost of the [GoF design patterns](http://wiki.c2.com/?GangOfFour).

Design patterns can be split into different categories, in simple words:
* Creational patterns - these are the design patterns that deal with the creation of objects, which requires particular control.
* Structural patterns - these are the design patterns that deal with the relationships between software entities.
* Behavioral patterns - these are the design patterns that deal with the communication and behavior of objects.

But we can still hear about concurrency patterns or even anti-patterns.

&nbsp;

## The singleton pattern

The [singleton](https://en.wikipedia.org/wiki/Singleton_pattern) is a member of the creational patterns.

This is one of the most straighforward patterns, and it aims to ensure that there is only one instance of a given class.

### Why using a singleton

The singleton is used when you need to have the same object to perform specific actions.
The singleton is also very often used in the implementation of other design patterns.

Besides, the singleton makes it possible to maintain a global state without going through a global variable and saves time and allocation space.

Unfortunately, the singleton is also very often misused, and many articles can be found on the Internet presenting arguments against the singleton.

It is not for me to say if the singleton is a good or bad design pattern.
I focus here on my vision of its implementation.


### How to implement a singleton

The object should only exist in one instance, so the constructor of its class is inaccessible, and an interface is provided.
The interface  builds the object on the first call, and always returns the same instance to those who call it.

The following class diagram illustrates this mechanism:

![singleton class diagram](https://hnrck.io/static/assets/img/blog/cpp/singleton_example/singleton.png "Singleton class diagram")

In this diagram, the instance is a Singleton object belonging to the Singleton class.
The `get_instance` class method allows to build this instance on the first call, via the private constructor, and to return this same singleton every call.

---

# The Logger class

Let's get to the crux of the matter, the implementation of an example of a singleton.

We will first focus on the elements essential to any class, constructors and destructors, then deal with the addition of the instance and the get_instance method, and finally add the functionalities of the Logger.

![Logger class diagram](https://hnrck.io/static/assets/img/blog/cpp/singleton_example/logger.png "Logger class diagram")

The Logger should be a singleton; its class diagram strongly resembles the singleton ones, and the pattern of the singleton can be figured behind it.
We add a print method as requested, which takes a log level and a message and displays them.
We also add the possibility to configure the output of this print via a private class attribute, `output_`, which can be modified by a class method, as well as a mutex to protect the critical section that is `output_` access.
Let's now look at its implementation.

&nbsp;

## The Logger class declaration

The first step in creating a class in C++, its declaration.

```cpp
class Logger final {
};
```

Since C++11, the specifier [`final`](https://en.cppreference.com/w/cpp/language/final) can be added to classes.

This specifier will be useful at the design level.
It explicitly prohibits inheritance from the specified class.

This deletion of the inheritance is entirely relative, an evil developer wishing to inherit from this class will be able to delete the `final` and do what he wants, nevertheless, it allows to make explicit the desire of the initial developer.
It might be tricky to adapt our singleton into an inherited class, and it is easier to redesign if necessary, for instance by using a singleton adapter.
A developer wanting to manipulate this part of the code will see that the original developer has not planned to allow the inheritance of his class, without having to integrate this into comments, and a less experienced developer will be protected at compilation time.

### Constructor and assignment

The necessary elements of a C++ class are [constructors](https://en.cppreference.com/w/cpp/language/initializer_list) and [destructors](https://en.cppreference.com/w/cpp/language/destructor).
By default these are generated by the compiler, and public.

Here we have subtleties that make us want the explicit declarations: we want to hide the constructor.

```cpp
 private: // Private default constructor.
  Logger() = default;

 public:

  // Default logger destructor. No need for virtual destructor as the logger
  // class cannot be inherited.
  ~Logger() = default;
```

C++11 introduces new mechanisms for declaring constructors and destructors, the [`= default` and `= delete` specifiers](http://www.stroustrup.com/C++11FAQ.html#default).
The first one explicitly generates a default function, will the second delete a function.

In the previous snippet, the constructor, declared by default, is hidden in the private part of the class.
The compiler should have generated a default constructor implicitly, as there is no other constructor in this class, but this one would have been publicly accessible.

The destructor is free to access. 
It's a design choice.
Making it private would have prevented a user from deleting the instance that everyone uses, but some compilers don't know how to handle private destructor.
In this specific case, I trust users more than compilers.

One could also have not make the destructor explicit, the compiler would have generated one, but from my point of view, it is cleaner to express all constructors and destructors in a class whose creation and destruction mechanisms are essential, and this is the case of the singleton which is a creational pattern.

Finally, it is useless to declare the `virtual` destructor since we have defined our `final` class, and none can inherit it.


### Copy constructor and assignment

Singletons must not be copyable, and only the `get_instance` interface must allow access to the instance.
By default, the compiler will generate [copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor) and [copy assignment](https://en.cppreference.com/w/cpp/language/copy_assignment), so we have to handle their declaration.

```cpp
 public:
  // Deleted copy constructor.
  // Only the get_instance class method is allowed to give a logger.
  Logger(const Logger &) = delete;

  // Deleted copy assignment.
  // Only the get_instance class method is allowed to give a logger.
  void operator=(const Logger &) = delete;
```

In an earlier version of C++, one would have put these constructors and assignments in the `private` part of the class, as we did to the constructor, but C++11 allows a cleaner method to prevent the use of copy.

The `delete' specifier that we have seen in the previous sub-section allows us to remove these functions explicitly.
They are therefore not only inaccessible, they no longer exist, even for the singleton.

### Move constructor and assignment

This part is not mandatory, but as I wrote above, the singleton is a creational pattern, and all its construction, destruction and assignment mechanisms should be made explicit.
We will also make the [move constructors](https://en.cppreference.com/w/cpp/language/move_constructor) and [assignments](https://en.cppreference.com/w/cpp/language/move_assignment) explicit.

There is no need to delete the latter.
Once an instance has been provided to a client, the client can do whatever he wants with it, including transferring the instance.

```cpp
 public:
  // Default move constructor.
  Logger(Logger &&) noexcept = default;

  // Default move assignment.
  Logger &operator=(Logger &&) noexcept = default;
```

Just as we did for the destructor, these are `public` and `default`.
Only one subtlety, the presence of the [`noexcept` specifier](https://en.cppreference.com/w/cpp/language/noexcept_spec).

Since C++11, a function can be specified whether it could throw exceptions or not.
Defaulted functions can potentially throw an exception.
By adding the `noexcept` specifier, move constructor and assignment are explicitly non-throwing.

No specifying `noexecpt` is not a problem in our design since STL libraries containers choose copy constructors and assignments instead of move ones, unless they are marked `noexcept` or the copy constructor is not accessible, which is our case.

&nbsp;

## Singleton functionality

The two crucial elements of implementing a singleton are its instance and its interface.

For the interface, we implement a class method that will initialize the instance at the first call, and return this instance at each call.

```cpp
 public:
  // Initialize the Logger singleton once, and return it each time this class
  // method is called.
  static Logger &get_instance();
```

[`static` is the keyword](https://en.cppreference.com/w/cpp/language/static) used in C++ to declare a class method, and the return type is `Logger &`, a reference to the instance.

At the level of the instance implementation, C++ allows to hide it apart from its attributes.
The instance can be linked to the class method `get_instance`:

```cpp
Logger &Logger::get_instance() {
  // The logger variable is initialized once with a move assignment of a logger
  // object build with the private constructor.
  static auto &&logger = Logger();
  return (logger);
}
```

Variables declared with the [`static` specifier](https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables) have static storage duration.
They are initialized the first time the execution goes through the declaration.

[`auto` is a variable specifier](https://en.cppreference.com/w/cpp/language/auto) which means that the type of the variable will be deduced from its initializer.

The [`&&` token](https://en.cppreference.com/w/cpp/language/reference) is used to mean an rvalue reference.
An rvalue reference behaves like an lvalue reference, but can be bind to a temporary rvalue.
The initialized object that would have been a temporary object copied can now be moved in our variable.

`Logger()` is the Logger class private constructor, creating the logger object.

`static auto &&logger = Logger();` means that the first time the declaration is executed, a logger object will be created by the private constructor, and moved into the logger variable, which is deducted to be a logger object.

From the compiler's point of view `static auto &&logger = Logger();` will be translated into `static Logger &&logger = Logger();`.
I remain satisfied with the disappearance of the bureaucratic aspect of rewriting the class name several times to declare it, then initialize it with its constructor.

__Note:__

It would also have been possible to call the private constructor directly:

```cpp
  static Logger logger;
```

From my point of view, this is a bad practice.
I want a consistent language. 
I like that all my variable declaration are:

```cpp
[static] [const] auto [&[&]] <name> = <initializer>;
```

Language is consistent, it makes it impossible for objects to be uninitialized, and this is way more stylish.

I recommend you this [great article](https://herbsutter.com/2013/08/12/gotw-94-solution-aaa-style-almost-always-auto/) from Herb Sutter.


&nbsp;

## Logger functionality

### Log level

C++ inherits from C a straightforward mechanism for a set of constants declaration.

```cpp
 public:
  // The different logger levels.
  enum level : unsigned char {
    INFO,
    WARN,
    ERROR,
  };
};
```

Three unsigned char constants are declared, with respectively 0, 1 and 2 as value.

### Print method

The print method will take one of the enum level constants as entered, as well as a message.
There's nothing particular here.

```cpp
 public:
  // Print a message with a certain logger level on the output.
  void print(enum level level, const std::string &message);
```

This method is implemented as follow:
```cpp
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

  if (p_output_ != nullptr) {
    // If the output is set, then display the string stream.
    (*p_output_) << ss.rdbuf() << std::endl;
  }

  // In a real application, log might also be saved in file.
}
```

In the simulation framework I designed, I also added a possible output in a file, but I won't complicate the problem here much more.

### Output setting

I then wanted to add a class setting for the print output.

I declared the `p_output` pointer on [std::ostream class](https://en.cppreference.com/w/cpp/io/basic_ostream) as private and static in the class:
```cpp
 private:
  static std::ostream *p_output_;
```

Initialized to `nullptr` in the implementation:
```cpp
// Initialization of the display pointer.
std::ostream *Logger::p_output_ = nullptr;
```

And add a public class method to change the output:
```cpp
 public:
  // A function that allows the setting of the logger output.
  // Nothing by default.
  static void set_output(std::ostream *p_output = nullptr);
```

```cpp
void Logger::set_output(std::ostream *p_output) {
  p_output_ = p_output;
}
```

The `p_output_` pointer must be initialized outside the class.
It's a C++ limitation that I regret.

### Output protection

In our code, we currently have a critical resource that is not protected: the output pointed to by `p_output_`.
The simple answer to protecting a resource is to use the  [`std::mutex` class](https://en.cppreference.com/w/cpp/thread/mutex):

```cpp
 private:
  // A mutex to protect the display resource.
  static std::mutex output_mutex_;
```

Initialized in the implementation:

```cpp
// Initialization of the display protection mutex.
std::mutex Logger::output_mutex_;
```

Nevertheless, C+++11 includes thread library support, with more specific blocking mechanisms, in particular, the [`std::lock_guard` class](https://en.cppreference.com/w/cpp/thread/lock_guard), which locks a mutex until the end of the scope:

```cpp
void Logger::set_output(std::ostream *p_output) {
  auto &&lock __attribute__((unused)) =
      std::lock_guard<std::mutex>(output_mutex_);
  p_output_ = p_output;
}
```

```cpp
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
```

### Logger class final result

You can find on GitHub the final result of the Logger class [header](https://github.com/hnrck/singleton_example/blob/master/include/Logger.h) and [implementation](https://github.com/hnrck/singleton_example/blob/master/srcs/Logger.cpp).

&nbsp;

## Test application 

A small program repeatedly calling the print method of the logger instance concurrently is built to test our singleton Logger.

### Global parameters

First of all, the limits of our program are set through global parameters.
We will set a maximum number of threads, no need to go beyond 1000 threads.

```cpp
// An arbitrary maximum number of threads.
#define MAX_NB_THREADS 1000
```

### Test routine

Then, a small routine that retrieves the instance of the Logger, generates a level and a message, waits for 10 milliseconds, and then calls the print method is implemented:

```cpp
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
```

### A small exception

Finally, for cleanliness, we create an exception to return if the program user forgets to enter the number of threads.

```cpp
// An exception to use when the main class does not have enough argument.
class missing_argument final : public std::exception {};
```

### Main function

All the bricks are now ready for our application.
First, we start the main body, and set the future exit code:

```cpp
int main(int argc, char const *argv[]) {
  // Initializing the program exit code, success by default.
  auto exit_code = int(EXIT_SUCCESS);
```

Then we retrieve the instance of our Logger, and set the output to the standard error output:

```cpp
  // Using our logger for error logging.
  auto &logger = Logger::get_instance();

  // Setting the logger output to std::cerr, so the log can be save from
  // command line using " 2> <log_file>"
  Logger::set_output(&std::cerr);
```

We can now parse the user's entry to retrieve the number of threads to launch.
We are preparing to potentially receive an error from this parsing with a `try {...} catch` block, in which case we can use our logger instance to display an error message.

```cpp
  auto arg = std::string();

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
```

We can now launch `nb_threads` routine:

```cpp
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
```

Finally, if everything went well, we can leave the execution of the program.

```cpp
  // Exiting the program with the exit code.
  return (exit_code);
}
```

### Test application final result

The final result of the test application is available on [GitHub](https://github.com/hnrck/singleton_example/blob/master/app/main.cpp)

---

# Experimentation

## GitHub repository

The code source of the full example available is on [hnrck/singleton_example](https://github.com/hnrck/singleton_example).

The GitHub repository is structured as following:
```
.
├── app
│   └── main.cpp
├── CMakeLists.txt
├── include
│   └── Logger.h
├── LICENSE
└── srcs
    └── Logger.cpp
```

* app - containing the application [`main.cpp`](https://github.com/hnrck/singleton_example/blob/master/app/main.cpp), using the Logger class.
* [`CMakeLists.txt`](https://github.com/hnrck/singleton_example/blob/master/CMakeLists.txt) - allowing to build the project quickly.
* include - containing the [Logger header](https://github.com/hnrck/singleton_example/blob/master/include/Logger.h).
* [`LICENSE`](https://github.com/hnrck/singleton_example/blob/master/LICENSE) - the license of the project.
* srcs - containing the [Logger implementation](https://github.com/hnrck/singleton_example/blob/master/srcs/Logger.cpp).

To test the project, the easiest way is to clone the github repository:
```
git clone https://github.com/hnrck/singleton_example.git
```

&nbsp;

## Compilation

```bash
cd singleton_example
mkdir build
cd build
cmake ..
cmake --build . 
```

These commands build the logger library `liblogger.so.1.0.0` in `singleton_example/build/usr/lib/` and an executable `main` in `singleton_example/build/usr/bin/`.

*Alternative*

As a huge big fan of clang and ninja, I use the following commands.
In the future, I will try to make an article to explain my tool preferences.

```bash
cd singleton_example
mkdir build
cd build
cmake -G "Ninja" -DCMAKE_CXX_COMPILER=clang++ ..
cmake --build . 
```

&nbsp;

## Execution

From `singleton_example/build/`, the main application can be executed with:

```bash 
usr/bin/main <number of thread> 2> <log file>
```

Where `<number of thread>` is a number from 0 to 1000. Trying to enter incorrect values causes errors, but that is not the point of this article
And `<log file>` is the file where the log will be written

*Note:*

```bash 
usr/bin/main <number of thread>
```

*Will output the logs in the terminal.*

For instance:

```bash 
usr/bin/main 10 2> log.txt
```

returns on the terminal:

```
Logger singleton example.
Take a number of thread from first argument (from 0 to 1000), and execute concurrently simple routines.
Setting the logger output to std::cerr, so the log can be extracted and saved in a file.
Creating  and joining 10 threads.
All the threads finished their routines.
Done with success.
```

and `log.txt` contains:

```
WARN: this is the message n° 1.
INFO: this is the message n° 0.
ERROR: this is the message n° 2.
INFO: this is the message n° 3.
WARN: this is the message n° 4.
ERROR: this is the message n° 5.
INFO: this is the message n° 6.
WARN: this is the message n° 7.
ERROR: this is the message n° 8.
INFO: this is the message n° 9.
```

Video of the demo on [asciinema](https://asciinema.org/a/6FnXYa8zZV1tpdK2RyxAF7Xbm):

[![asciicast](https://asciinema.org/a/6FnXYa8zZV1tpdK2RyxAF7Xbm.svg)](https://asciinema.org/a/6FnXYa8zZV1tpdK2RyxAF7Xbm)
<!-- ![demo](/static/assets/img/blog/cpp/singleton_example/demo.gif "demo") -->
<!-- <p align="center"> -->
<!--   <img src="/static/assets/img/blog/cpp/singleton_example/demo.gif" height="1000" width="968"> -->
<!-- </p> -->

---

# Conclusion

## Summary

I hope you had as much fun reading this article as I did writing it.

Design patterns are essential building blocks of software engineering.
In this article, we presented one of the most simple design patterns, the singleton.

C++ does not have direct support for these design patterns, and recent modernizations do not provide this support.
Nevertheless, modern C++ best practices allow an explicit and beautiful implementation of the singleton design pattern.

&nbsp;

## Limitations

First of all, the singleton I decided to illustrate in this article is often considered as bad practice.

Then, the Logger class and main application I built are very simplistic examples. The Internet is full of examples of more interesting applications.

&nbsp;

## Future work

In any order, I would like to write articles about other design patterns implementation, write articles about the C++ tools I used, and write articles about my C++ preferred resources.

---
