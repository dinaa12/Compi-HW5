#include "stringCreator.h"

stringCreator::stringCreator() : name(std::string()) {
    name = std::string("s") + std::to_string(string_counter);
    string_counter++;
}