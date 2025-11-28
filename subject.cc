export module Subject;

import Display;

import <vector>;
import <algorithm>;

using namespace std;

// subject (observable) class for Observer pattern
export class Subject {
    vector<Display*> observers;
    
public:
    virtual ~Subject() = default;
    
    void attach(Display* obs) {
        observers.push_back(obs);
    }
    
    void detach(Display* obs) {
        auto it = find(observers.begin(), observers.end(), obs);
        if (it != observers.end()) {
            observers.erase(it);
        }
    }
    
protected:
    // called by derived class when state changes
    void notifyObservers() {
        for (auto obs : observers) {
            obs->notify();
        }
    }
};