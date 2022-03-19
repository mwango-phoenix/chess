#ifndef _SUBJECT_H_
#define _SUBJECT_H_

#include <vector>
#include "observer.h"

class Subject {
    std::vector<Observer*> observers;

    public:
    Subject() = default;
    void attach(Observer* o);
    void detach(Observer* o);
    void notifyObservers();
    virtual ~Subject() = default;

};

#endif