#include <iostream>
#include <string>
using namespace std;

/*
    Mission Class

    Simulates a space mission resource system.
    Demonstrates dynamic memory management, deep copy constructor,
    destructor cleanup, and use of static members in C++.
*/

class Mission {
	private:
	    int missionID;
	    string missionName;
	    int astronautCount;
	
	    // Dynamically allocated resources
	    int* oxygenLevels;
	    double* fuelUnits;
	
	    static int totalMissions;
	
	public:
	    // Constructor
	    Mission(int id, string name, int astronauts, int oxygenPerAstronaut, double fuel);
	
	    // Deep copy constructor to prevent shared memory issues
	    Mission(const Mission& other);
	
	    // Destructor to free allocated memory
	    ~Mission();
	
	    void consumeOxygen(int amountPerAstronaut);
	    void consumeFuel(double amount);
	    void displayMissionStatus() const;
	
	    int getMissionID() const;
	    string getMissionName() const;
	    int getAstronautCount() const;
	    int getOxygenLevel(int index) const;
	    double getFuel() const;
	
	    static int showTotalMissions();
};

Mission::Mission(int id, string name, int astronauts, int oxygen, double fuel) {
    missionID = id;
    missionName = name;
    astronautCount = astronauts;

    // Allocate oxygen levels for each astronaut
    oxygenLevels = new int[astronautCount];
    for (int i = 0; i < astronautCount; i++)
        oxygenLevels[i] = oxygen;

    fuelUnits = new double;
    *fuelUnits = fuel;

    totalMissions++;
}

// Deep copy implementation
Mission::Mission(const Mission& other) {
    missionID = other.missionID;
    missionName = other.missionName;
    astronautCount = other.astronautCount;

    oxygenLevels = new int[astronautCount];
    for (int i = 0; i < astronautCount; i++)
        oxygenLevels[i] = other.oxygenLevels[i];

    fuelUnits = new double;
    *fuelUnits = *(other.fuelUnits);

    totalMissions++;
}

// Release dynamically allocated memory
Mission::~Mission() {
    delete[] oxygenLevels;
    delete fuelUnits;
}

int Mission::getMissionID() const {
    return missionID;
}

string Mission::getMissionName() const {
    return missionName;
}

int Mission::getAstronautCount() const {
    return astronautCount;
}

int Mission::getOxygenLevel(int index) const {
    return oxygenLevels[index];
}

double Mission::getFuel() const {
    return *fuelUnits;
}

// Reduce oxygen for each astronaut
void Mission::consumeOxygen(int amount) {
    for (int i = 0; i < astronautCount; i++)
        oxygenLevels[i] -= amount;
}

// Reduce total mission fuel
void Mission::consumeFuel(double amount) {
    *fuelUnits -= amount;
}

void Mission::displayMissionStatus() const {
    cout << "Mission ID: " << missionID << endl;
    cout << "Mission Name: " << missionName << endl;
    cout << "Astronauts: " << astronautCount << endl;

    cout << "Oxygen Levels: ";
    for (int i = 0; i < astronautCount; i++)
        cout << oxygenLevels[i] << " ";
    cout << endl;

    cout << "Fuel: " << *fuelUnits << endl;
}

int Mission::showTotalMissions() {
    return totalMissions;
}

int Mission::totalMissions = 0;

int main() {
    // Create initial mission
    Mission m1(1, "Mars Exploration", 3, 100, 500.5);

    cout << "Initial Mission m1 Status:\n";
    m1.displayMissionStatus();

    cout << "\n----------------------------\n";

    // Clone mission using copy constructor
    Mission m2 = m1;

    // Modify original mission
    m1.consumeOxygen(20);

    cout << "\nAfter reducing oxygen in m1:\n";

    cout << "\nMission m1 Status:\n";
    m1.displayMissionStatus();

    cout << "\nMission m2 Status (should remain unchanged):\n";
    m2.displayMissionStatus();

    return 0;
}