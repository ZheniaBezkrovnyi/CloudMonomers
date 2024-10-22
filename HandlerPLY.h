#pragma once
#include <string>
#include <vector>
#include "Geometry.h"  // Для роботи з Vertex і Face

class HandlerPLY {
public:
    HandlerPLY(const std::string& fileName);  // Конструктор
    void process() const;                     // Основний метод для обробки PLY файлу

private:
    std::string _fileName;                    // Назва файлу PLY
};
