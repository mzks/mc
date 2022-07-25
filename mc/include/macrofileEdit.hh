// Remove top two lines for generated macro file
// The top two lines are created on the source, not user-code
//
#ifndef MC_MACROFILEEDIT_H
#define MC_MACROFILEEDIT_H

#include <string>
#include <fstream>
#include <filesystem>

#include <spdlog/spdlog.h>

void EditMacroFile(const std::string& filename){
    std::string outfilename = filename + ".tmp";
    std::ofstream ofs(outfilename, std::ios::out);
    std::ifstream ifs(filename);
    if (!ifs) {
      spdlog::error("Cannot open the file, {}", filename);
      std::exit(1);
    }
    if (!ofs) {
      spdlog::error("Cannot open the file, {}", outfilename);
      std::exit(1);
    }
    std::string line_buffer;
    ofs << "# Created by mc" << std::endl;
    auto seed = G4Random::getTheSeed();
    ofs << "# The used seed is " << seed << std::endl;

    int n_lines = 0;
    while (!ifs.eof()) {
      std::getline(ifs, line_buffer);

      if (n_lines > 1){ // Skip line 0 and 1
        ofs << line_buffer << std::endl;
      } else {
        // commented out
        ofs << "# " + line_buffer << std::endl;
      }
      n_lines++;
    }
    ifs.close();
    ofs.close();
    std::filesystem::rename(outfilename, filename);
}


#endif // MC_MACROFILEEDIT_
