#pragma once
#include <vector>
#include <memory>
#include "Object.h"

class ObjectCleaner {
public:
    ObjectCleaner() = default;
    ~ObjectCleaner() = default;

    void cleanup(std::vector<std::unique_ptr<Object>>& objects, float playerX);
};