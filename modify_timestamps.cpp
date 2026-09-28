#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <ctime>

namespace fs = std::filesystem;

// --------------------------------------------------
// Read date from .date
// --------------------------------------------------
bool readDate(int &year, int &month, int &day)
{
    std::ifstream input(".date");

    if (!input)
    {
        std::cerr << "Could not open .date" << std::endl;
        return false;
    }

    char dash1, dash2;

    input >> year >> dash1 >> month >> dash2 >> day;

    if (!input || dash1 != '-' || dash2 != '-')
    {
        std::cerr
            << "Invalid date in .date. "
            << "Expected YYYY-MM-DD"
            << std::endl;

        return false;
    }

    return true;
}

// --------------------------------------------------
// Increment date by exactly one calendar day
// --------------------------------------------------
bool incrementDate(int &year, int &month, int &day)
{
    std::tm date = {};

    date.tm_year = year - 1900;
    date.tm_mon = month - 1;
    date.tm_mday = day;

    date.tm_mday += 1;

    if (std::mktime(&date) == -1)
    {
        std::cerr << "Could not increment date" << std::endl;
        return false;
    }

    year = date.tm_year + 1900;
    month = date.tm_mon + 1;
    day = date.tm_mday;

    return true;
}

// --------------------------------------------------
// Decrement date by exactly one calendar day
// --------------------------------------------------
bool decrementDate(int &year, int &month, int &day)
{
    std::tm date = {};

    date.tm_year = year - 1900;
    date.tm_mon = month - 1;
    date.tm_mday = day;

    date.tm_mday -= 1;

    if (std::mktime(&date) == -1)
    {
        std::cerr << "Could not decrement date" << std::endl;
        return false;
    }

    year = date.tm_year + 1900;
    month = date.tm_mon + 1;
    day = date.tm_mday;

    return true;
}

// --------------------------------------------------
// Save date to .date
// --------------------------------------------------
bool saveDate(int year, int month, int day)
{
    std::ofstream output(".date");

    if (!output)
    {
        std::cerr << "Could not write to .date" << std::endl;
        return false;
    }

    output << year << "-"
           << std::setfill('0') << std::setw(2) << month << "-"
           << std::setfill('0') << std::setw(2) << day
           << std::endl;

    return true;
}

// --------------------------------------------------
// Format timestamp for EXIF
// --------------------------------------------------
std::string formatTimestamp(
    const std::chrono::system_clock::time_point &time)
{
    std::time_t tt =
        std::chrono::system_clock::to_time_t(time);

    std::tm tm = *std::localtime(&tt);

    std::ostringstream oss;

    oss << std::put_time(
        &tm,
        "%Y:%m:%d %H:%M:%S");

    return oss.str();
}

// --------------------------------------------------
// Modify photo timestamps
// --------------------------------------------------
void modifyTimestamps(
    const std::string &directory,
    int year,
    int month,
    int day)
{
    std::vector<fs::path> jpgFiles;

    for (const auto &entry :
         fs::directory_iterator(directory))
    {

        if (entry.path().extension() == ".jpg" ||
            entry.path().extension() == ".JPG")
        {

            jpgFiles.push_back(entry.path());
        }
    }

    std::sort(jpgFiles.begin(), jpgFiles.end());

    std::tm start = {};

    start.tm_year = year - 1900;
    start.tm_mon = month - 1;
    start.tm_mday = day;

    // Start at midnight.
    start.tm_hour = 0;
    start.tm_min = 0;
    start.tm_sec = 0;

    auto baseTime =
        std::chrono::system_clock::from_time_t(
            std::mktime(&start));

    int incrementSeconds = 0;
    int updatedCount = 0;

    for (const auto &file : jpgFiles)
    {

        auto newTime =
            baseTime +
            std::chrono::seconds(incrementSeconds);

        std::string timestamp =
            formatTimestamp(newTime);

        std::string command =
            "exiftool -overwrite_original "
            "-AllDates=\"" +
            timestamp +
            "\" \"" +
            file.string() +
            "\"";

        int result = std::system(command.c_str());

        if (result == 0)
        {

            std::cout
                << file.filename().string()
                << " updated to "
                << timestamp
                << std::endl;

            updatedCount++;
        }
        else
        {

            std::cerr
                << "Failed to update: "
                << file.string()
                << std::endl;
        }

        incrementSeconds++;
    }

    std::cout
        << updatedCount
        << " image files updated"
        << std::endl;
}

// --------------------------------------------------
// Main
// --------------------------------------------------

int main(int argc, char *argv[])
{

    bool increment = false;
    bool decrement = false;
    std::string directoryPath;

    // ----------------------------------------------
    // Parse arguments
    // ----------------------------------------------
    for (int i = 1; i < argc; ++i)
    {

        std::string arg = argv[i];

        if (arg == "--incr")
        {

            increment = true;
        }
        else if (arg == "--decr")
        {

            decrement = true;
        }
        else
        {

            if (!directoryPath.empty())
            {
                std::cerr
                    << "Error: multiple directory paths provided."
                    << std::endl;

                return 1;
            }

            directoryPath = arg;
        }
    }

    // ----------------------------------------------
    // Cannot increment and decrement simultaneously
    // ----------------------------------------------
    if (increment && decrement)
    {

        std::cerr
            << "Error: cannot use --incr and --decr "
            << "at the same time."
            << std::endl;

        return 1;
    }

    // ----------------------------------------------
    // Read current date
    // ----------------------------------------------

    int year;
    int month;
    int day;

    if (!readDate(year, month, day))
    {
        return 1;
    }

    // ----------------------------------------------
    // Increment date
    // ----------------------------------------------
    if (increment)
    {
        std::cout
            << "Current date: "
            << year << "-"
            << std::setfill('0') << std::setw(2) << month << "-"
            << std::setfill('0') << std::setw(2) << day
            << std::endl;

        if (!incrementDate(year, month, day))
        {
            std::cerr
                << "Failed to increment date."
                << std::endl;

            return 1;
        }

        if (!saveDate(year, month, day))
        {
            std::cerr
                << "Failed to save date."
                << std::endl;

            return 1;
        }

        std::cout
            << "Date incremented to: "
            << year << "-"
            << std::setfill('0') << std::setw(2) << month << "-"
            << std::setfill('0') << std::setw(2) << day
            << std::endl;
    }

    // ----------------------------------------------
    // Decrement date
    // ----------------------------------------------
    if (decrement)
    {

        std::cout
            << "Current date: "
            << year << "-"
            << std::setfill('0') << std::setw(2) << month << "-"
            << std::setfill('0') << std::setw(2) << day
            << std::endl;

        if (!decrementDate(year, month, day))
        {
            std::cerr
                << "Failed to decrement date."
                << std::endl;

            return 1;
        }

        if (!saveDate(year, month, day))
        {
            std::cerr
                << "Failed to save date."
                << std::endl;

            return 1;
        }

        std::cout
            << "Date decremented to: "
            << year << "-"
            << std::setfill('0') << std::setw(2) << month << "-"
            << std::setfill('0') << std::setw(2) << day
            << std::endl;
    }

    // ----------------------------------------------
    // If directory supplied, update photos
    // ----------------------------------------------
    if (!directoryPath.empty())
    {

        if (!fs::exists(directoryPath) ||
            !fs::is_directory(directoryPath))
        {
            std::cerr
                << "The provided path does not exist "
                << "or is not a directory."
                << std::endl;

            return 1;
        }

        std::cout
            << "Updating photos using date: "
            << year << "-"
            << std::setfill('0') << std::setw(2) << month << "-"
            << std::setfill('0') << std::setw(2) << day
            << std::endl;

        modifyTimestamps(
            directoryPath,
            year,
            month,
            day
        );

        return 0;
    }

    // ----------------------------------------------
    // No directory supplied
    // ----------------------------------------------

    if (!increment && !decrement)
    {
        std::cerr
            << "Usage:"
            << std::endl
            << "  " << argv[0] << " /path/to/photos"
            << std::endl
            << "  " << argv[0] << " --incr"
            << std::endl
            << "  " << argv[0] << " --decr"
            << std::endl
            << "  " << argv[0] << " --incr /path/to/photos"
            << std::endl
            << "  " << argv[0] << " --decr /path/to/photos"
            << std::endl;

        return 1;
    }

    return 0;
}
