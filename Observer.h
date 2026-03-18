//
// Created by Michael Pachowski on 2026-03-17.
//

#ifndef GROUP55_OBSERVER_H
#define GROUP55_OBSERVER_H

#include <string>

class Observer {
public:
    virtual void update(const std::string& event) = 0;
    virtual ~Observer() = default;
};

#endif //GROUP55_OBSERVER_H