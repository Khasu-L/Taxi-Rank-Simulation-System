//Name: Lebohang Khasu
//Std No.: 17955572

#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <vector>
#include <iomanip>
using namespace std;

struct Passenger {
    int arrivalTime, boardingTime;
    char route;
};

class TaxiRank {
public:
    void initialiseTaxi(char route);
    bool isTaxiFull(char route) const;
    void passengerBoard(Passenger p);
    int getTaxiSize(char route) const;
    TaxiRank();

private:
    const int maxPassengers = 5;
    vector<Passenger> taxiS, taxiC, taxiL;
};

void TaxiRank::initialiseTaxi(char route) {
    if (route == 'S') taxiS.clear();
    else if (route == 'C') taxiC.clear();
    else if (route == 'L') taxiL.clear();
}

bool TaxiRank::isTaxiFull(char route) const {
    if (route == 'S') return taxiS.size() >= maxPassengers;
    else if (route == 'C') return taxiC.size() >= maxPassengers;
    else if (route == 'L') return taxiL.size() >= maxPassengers;
    return true;
}

void TaxiRank::passengerBoard(Passenger p) {
    if (isTaxiFull(p.route)) {
        initialiseTaxi(p.route);
    }

    if (p.route == 'S') taxiS.push_back(p);
    else if (p.route == 'C') taxiC.push_back(p);
    else if (p.route == 'L') taxiL.push_back(p);
}

int TaxiRank::getTaxiSize(char route) const {
    if (route == 'S') return taxiS.size();
    else if (route == 'C') return taxiC.size();
    else if (route == 'L') return taxiL.size();
    return 0;
}

TaxiRank::TaxiRank() {}

class TaxiQueue {
public:
    void newElement(Passenger p);
    vector<Passenger>& getQueue(char route);
    void removeFront(char route);

private:
    vector<Passenger> queueS, queueC, queueL;
};

void TaxiQueue::newElement(Passenger p) {
    if (p.route == 'S') queueS.push_back(p);
    else if (p.route == 'C') queueC.push_back(p);
    else if (p.route == 'L') queueL.push_back(p);
}

vector<Passenger>& TaxiQueue::getQueue(char route) {
    if (route == 'S') return queueS;
    else if (route == 'C') return queueC;
    else return queueL;
}

void TaxiQueue::removeFront(char route) {
    if (route == 'S' && !queueS.empty()) queueS.erase(queueS.begin());
    else if (route == 'C' && !queueC.empty()) queueC.erase(queueC.begin());
    else if (route == 'L' && !queueL.empty()) queueL.erase(queueL.begin());
}

int main() {
    ifstream file("C:/data/taxiData.txt");
    if (!file) {
        cout << "Failed to open file\n";
        return 1;
    }
    cout << "File open\n";

    vector<Passenger> passengers;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string timeStr, routeStr, boardStr;
        getline(ss, timeStr, ',');
        getline(ss, routeStr, ',');
        getline(ss, boardStr, ',');
        Passenger p;
        p.arrivalTime = stoi(timeStr);
        p.route = routeStr[0];
        p.boardingTime = stoi(boardStr);
        passengers.push_back(p);
    }

    TaxiQueue queue;
    TaxiRank rank;
    int currentTime = 0;
    int simEndTime = 300;

    cout << left << setw(8) << "Time"
         << setw(12) << "Next"
         << setw(6) << "S"
         << setw(6) << "L"
         << setw(6) << "C"
         << setw(10) << "WQ_S"
         << setw(10) << "WQ_L"
         << setw(10) << "WQ_C"
         << setw(12) << "TaxiCap_S"
         << setw(12) << "TaxiCap_L"
         << setw(12) << "TaxiCap_C" << endl;

    while (currentTime <= simEndTime) {
        vector<char> newPassengers;
        for (auto& p : passengers) {
            if (p.arrivalTime == currentTime) {
                queue.newElement(p);
                newPassengers.push_back(p.route);
            }
        }

        for (char route : {'S', 'L', 'C'}) {
            vector<Passenger>& q = queue.getQueue(route);
            if (!q.empty()) {
                Passenger& front = q.front();
                if (!rank.isTaxiFull(route)) {
                    rank.passengerBoard(front);
                    queue.removeFront(route);
                }
            }
        }

        cout << left << setw(8) << currentTime;
        for (string r : {"S", "L", "C"}) {
            string nexts;
            for (char c : newPassengers) {
                if (c == r[0]) nexts += c;
            }
            cout << setw(4) << nexts;
        }
        cout << setw(6) << queue.getQueue('S').size();
        cout << setw(6) << queue.getQueue('L').size();
        cout << setw(6) << queue.getQueue('C').size();
        cout << setw(10) << (queue.getQueue('S').size() > 0 ? queue.getQueue('S').size() - 1 : 0);
        cout << setw(10) << (queue.getQueue('L').size() > 0 ? queue.getQueue('L').size() - 1 : 0);
        cout << setw(10) << (queue.getQueue('C').size() > 0 ? queue.getQueue('C').size() - 1 : 0);
        cout << setw(12) << rank.getTaxiSize('S');
        cout << setw(12) << rank.getTaxiSize('L');
        cout << setw(12) << rank.getTaxiSize('C') << endl;

        this_thread::sleep_for(chrono::seconds(1));
        currentTime++;
    }

    return 0;
}

