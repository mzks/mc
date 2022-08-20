
#ifndef SMART_LOOP_LOGGER_HH
#define SMART_LOOP_LOGGER_HH

#include <cmath>
#include <spdlog/spdlog.h>
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

static void smart_loop_logger(long nTotal, long iLoop, int each_display_percent=10){
    if (
           ((100 * iLoop / nTotal % each_display_percent == 0) && ((100 * iLoop / nTotal) != (100 * (iLoop-1) / nTotal))) ||
           ((int(std::log10(iLoop-1))) != (int(std::log10(iLoop))))
       ){
        spdlog::info("{:<6} percent events ({:d}) have been done.", 100. * (iLoop) / nTotal, iLoop);
    }
}

/**
 * @fn
 * @brief Find the best unit to provide a message.
 * @param [in] value
 * @param [in] unit
 * @return std::string string value with the best unit
 * @detail This function finds the best units to show
 * the value as a message.
 */
static std::string BestUnit(double value, std::string unit){

  // time -- default unit : sec
  std::map<std::string, double> time_units {
      {"second", 1}, // default
      {"milli second", 1.e-3},
      {"micro second", 1.e-6},
      {"nano second", 1.e-9},
      {"pico second", 1.e-12},
      {"minute", 60},
      {"hour", 3600},
      {"day", 3600*24},
      {"month", 365.2422 * 24 * 3600./12},
      {"year", 365.2422 * 24 * 3600.},
  };
  std::map<std::string, double> time_short_units {
      {"s", 1},
      {"ms", 1.e-3},
      {"us", 1.e-6},
      {"ns", 1.e-9},
      {"ps", 1.e-12},
      {"min", 60},
      {"h", 3600},
      {"d", 3600*24},
      {"mon", 365.2422 * 24 * 3600./12},
      {"y", 365.2422 * 24 * 3600.},
  };
  // data -- default unit : byte
  std::map<std::string, double> data_units {
      {"bit", 1/8.},
      {"byte", 1},
      {"killo byte", 1.e3},
      {"Mega byte", 1.e6},
      {"Giga byte", 1.e9},
      {"Tera byte", 1.e12},
  };
  std::map<std::string, double> data_short_units {
      {"b", 1/8.},
      {"B", 1},
      {"kB", 1.e3},
      {"MB", 1.e6},
      {"GB", 1.e9},
      {"TB", 1.e12},
  };

  std::map<std::string, double> units;
  // ## time_units.count(unit)==1 // check contains
  if (time_units.count(unit)==1) units = time_units;
  else if (time_short_units.count(unit)==1) units = time_short_units;
  else if (data_units.count(unit)==1) units = data_units;
  else if (data_short_units.count(unit)==1) units = data_short_units;
  else std::cout << "The unit is not available" << std::endl;

  auto value_with_default_unit = value * units.at(unit);
  double value_with_best_unit = 1.e100; // large value to be updated
  std::string best_unit;

  for(auto u:units){
    std::string unit = u.first;
    auto value_with_a_unit = value_with_default_unit / u.second;
    auto ndigits = log10(value_with_a_unit);
    if ((0<=ndigits) && (ndigits < log10(value_with_best_unit))){
      value_with_best_unit = value_with_a_unit;
      best_unit = unit;
    }
  }
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(1) << value_with_best_unit;
  std::string value_str = oss.str();
  return value_str + " " + best_unit;
}

#endif