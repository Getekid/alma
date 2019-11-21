#include <iostream>
#include <stack>
#include <array>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

class rentCar
{
    public:
      // Input.
      int numCars, numStations, dist, time;
      stack <array<int, 2>> cars;
      vector<int> stationDist;
      int ts, cs, tf, cf;
      void getInput();
      // Helper.
      float getFuelDistanceMinTime(int, int);
      bool carIsValid(array<int, 2>);
      // Output.
      int minRent = -1;
      int caclulateMinRent();
};

void rentCar::getInput()
{
    // Get the first line of data.
    cout << "Type the # of cars, stations, city distance and the time available.\n";
    cin >> numCars;
    cin >> numStations;
    cin >> dist;
    cin >> time;

    // Get the car data.
    for (size_t i = 0; i < numCars; i++)
    {
        cout << "Type the renting cost and fuel capacity of car " << i+1 << "\n";
        array<int, 2> specs;
        cin >> specs[0];
        cin >> specs[1];
        cars.push(specs);
    }

    // Get the station distance data.
    cout << "Type the distance of each station.\n";
    int distance;
    for (size_t i = 0; i < numStations; i++)
    {
        cin >> distance;
        stationDist.insert(stationDist.end(), distance);
    }
    // Sort the vector to put the destinations in order.
    sort(stationDist.begin(), stationDist.end());
    // Add the final destination distance as a last element.
    stationDist.insert(stationDist.end(), dist);

    // Get the time and fuel cost data.
    cout << "Type the time and fuel costs of the economic and spor functionality respectively\n";
    cin >> ts;
    cin >> cs;
    cin >> tf;
    cin >> cf;
}

float rentCar::getFuelDistanceMinTime(int fuel, int distance)
{
    // Calcullate the minimum distance x in economic functionality.
    // If x is bigger than the distance then the car cannot cross, return -1.
    float x = (float)(distance * cf - fuel)/(cf - cs);
    x = (x < 0) ? 0 : x;
    if (x > distance)
    {
        return -1;
    }
    else
    {
        return distance * tf + x * (ts - tf);
    }
}

bool rentCar::carIsValid(array<int, 2> car)
{
    int distMoved = 0;
    float timePassed = 0;
    // We added the final destination distance in the stationDist
    // vector so we iterate until numStations + 1.
    for (size_t i = 0; i < numStations + 1; i++)
    {
        int distToStation = stationDist[i] - distMoved;
        int timeNeeded = getFuelDistanceMinTime(car[1], distToStation);
        if (timeNeeded < 0)
        {
            return false;
        }

        timePassed += timeNeeded;
        if (timePassed > time)
        {
            return false;
        }
        distMoved = stationDist[i];
    }

    return true;
}

int rentCar::caclulateMinRent()
{
    int fuelFail = 0;
    int minRentLocal = 1000000000;
    bool hasValidCar = false;
    while(!cars.empty())
    {
        array<int, 2> car = cars.top();
        cars.pop();

        // If the car has:
        // - cost higher that the one of a valid car or
        // - fuel capacity is lower that one of an invalid car
        // then this car is invalid.
        if ((hasValidCar && (car[0] >= minRentLocal)) || car[1] <= fuelFail)
        {
            continue;
        }
        
        if (carIsValid(car))
        {
            hasValidCar = true;
            if (minRentLocal > car[0])
            {
                minRentLocal = car[0];
            }
        }
        else
        {
            fuelFail = car[1];
        }
    }
    if (hasValidCar)
    {
        minRent = minRentLocal;
    }

    return minRent;
}

int main()
{
    rentCar rc;
    rc.getInput();
    cout << rc.caclulateMinRent() << "\n";

    return 0;
}
