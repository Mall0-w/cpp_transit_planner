#include <sstream>
#include <string>
#include <chrono>

inline
int timeToSeconds(const std::string& timeStr) {
    int hours, minutes, seconds;
    char colon; // for the ':' characters

    std::istringstream timeStream(timeStr);
    timeStream >> hours >> colon >> minutes >> colon >> seconds;

    return hours * 3600 + minutes * 60 + seconds;
}

inline
int getCurrentSecondsInDay() {
    using namespace std::chrono;

    // Get the current time
    auto now = system_clock::now();
    
    // Convert to time_t to get the time since epoch
    auto now_time_t = system_clock::to_time_t(now);

    // Convert to tm struct in local time zone
    tm* now_tm = localtime(&now_time_t);

    // Calculate the number of seconds since midnight
    int hours = now_tm->tm_hour;
    int minutes = now_tm->tm_min;
    int seconds = now_tm->tm_sec;

    return hours * 3600 + minutes * 60 + seconds;
}
