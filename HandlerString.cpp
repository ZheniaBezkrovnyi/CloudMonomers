#include "HandlerString.h"


std::string changeExtensionPLYtoTXT(const std::string& path) {
    std::stringstream ss(path);
    std::string segment;
    std::string lastSegment;

    while (std::getline(ss, segment, '/')) {
        lastSegment = segment;
    }

    size_t pos = lastSegment.rfind(".ply");
    if (pos != std::string::npos) {
        lastSegment.replace(pos, 4, ".txt");
    }

    return lastSegment;
}
std::string getNameFile(const std::string& path) {
    std::stringstream ss(path);
    std::string segment;
    std::string lastSegment;

    while (std::getline(ss, segment, '/')) {
        lastSegment = segment;
    }

    size_t pos = lastSegment.rfind(".ply");
    if (pos != std::string::npos) {
        lastSegment.replace(pos, 4, "");
    }

    return lastSegment;
}