#pragma once
#include <string>
#include <vector>
#include "Geometry.h"  // ��� ������ � Vertex � Face

class HandlerPLY {
public:
    HandlerPLY(const std::string& fileName);  // �����������
    void process() const;                     // �������� ����� ��� ������� PLY �����

private:
    std::string _fileName;                    // ����� ����� PLY
};
