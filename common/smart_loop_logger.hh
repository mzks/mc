#include <cmath>
#include <spdlog/spdlog.h>

void smart_loop_logger(long nTotal, long iLoop, int each_display_percent=10){
    if (
           ((100 * iLoop / nTotal % each_display_percent == 0) && ((100 * iLoop / nTotal) != (100 * (iLoop-1) / nTotal))) ||
           ((int(std::log10(iLoop-1))) != (int(std::log10(iLoop))))
       ){
        spdlog::info("{:<6} percent events ({:d}) have been done.", 100. * (iLoop) / nTotal, iLoop);
    }
}
