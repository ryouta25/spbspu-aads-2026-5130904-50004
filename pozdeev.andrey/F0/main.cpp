#include "application.hpp"

#include <iostream>
#include <exception>

int main()
{
  try {
    pozdeev::Application app;
    app.run();
  } catch (const std::invalid_argument& ex) {
    std::cerr << "Input Error: " << ex.what() << "\n";
    return 1;
  } catch (const std::exception& ex) {
    std::cerr << "Fatal Error: " << ex.what() << "\n";
    return 2;
  } catch (...) {
    std::cerr << "Unknown Fatal Error.\n";
    return 2;
  }

  return 0;
}
