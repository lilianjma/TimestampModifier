#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cstdlib>

namespace fs = std::filesystem;

// std::string formatTimestamp(const std::chrono::system_clock::time_point& time) {
//     std::time_t tt = std::chrono::system_clock::to_time_t(time);
//     std::tm tm = *std::localtime(&tt);

//     std::ostringstream oss;
//     oss << std::put_time(&tm, "%Y:%m:%d-%H:%M:%S");
//     return oss.str();
// }

std::string formatTimestamp(const std::chrono::system_clock::time_point& time) {
    std::time_t tt = std::chrono::system_clock::to_time_t(time);
    std::tm tm = *std::localtime(&tt);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y:%m:%d %H:%M:%S"); // Space between date and time
    return oss.str();
}

void modifyTimestamps(const std::string& directory) {
    std::vector<fs::path> jpgFiles;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.path().extension() == ".jpg" || entry.path().extension() == ".JPG") {
            jpgFiles.push_back(entry.path());
        }
    }

    std::sort(jpgFiles.begin(), jpgFiles.end());

    int month = 4;
    int day = 28;
    int year = 1980;
    auto baseTime = std::chrono::system_clock::from_time_t(std::mktime(new std::tm{0, 0, 0, day, month-1, year-1900}));
    int incrementSeconds = 0;
    int updatedCount = 0;  // Counter for updated images

    for (const auto& file : jpgFiles) {
        auto newTime = baseTime + std::chrono::seconds(incrementSeconds);
        std::string timestamp = formatTimestamp(newTime);

        std::string command = "exiftool -overwrite_original -AllDates=\"" + timestamp + "\" \"" + file.string() + "\"";

        // std::cout << "Executing command: " << command << std::endl; // Debug output

        int result = std::system(command.c_str());

        if (result == 0) {
            std::cout << file.filename().string() << " updated" << std::endl;
        } else {
            perror("System command error");
            std::cerr << "Failed to execute command: " << command << " with code: " << result << std::endl;
        }

        incrementSeconds += 1;
    }

    // Print the total number of files updated
    std::cout << updatedCount << " image files updated" << std::endl;
}



int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <directory_path>" << std::endl;
        return 1;
    }

    std::string directoryPath = argv[1];

    if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath)) {
        std::cerr << "The provided path does not exist or is not a directory." << std::endl;
        return 1;
    }

    // Check if jhead is available
    // int jheadCheck = std::system("jhead -h > /dev/null 2>&1");
    // if (jheadCheck != 0) {
    //     std::cerr << "jhead command is not available. Please install jhead and try again." << std::endl;
    //     return 1;
    // }

    modifyTimestamps(directoryPath);

    return 0;
}
