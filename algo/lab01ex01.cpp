#include <iostream>
#include <stack>
#include <array>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

class roadtrip
{
    public:
      // Input.
      int numCars, numStations, dist, time;
      stack <array<int, 2>> cars;
      vector<int> stationDist;
      int ts, cs, tf, cf;
      void getInput();
      // Helper.
      int fuelMin = 0, fuelMax = 0;
      vector<int> stationDistDiff; // A vector to store the diff distance between the stations.
      map<int, bool> fuelStatus; // A helper map to store the status for each fuel value.
      int getFuelDistanceMinTime(int, int);
      bool fuelIsValid(int);
      int calculateMinFuelSuccess(int, int);
      // Output.
      int minRent = -1;
      int calculateMinRent();
};

void roadtrip::getInput()
{
    // Get the first line of data.
    // cout << "Type the # of cars, stations, city distance and the time available.\n";
    cin >> numCars >> numStations >> dist >> time;

    // Get the car data.
    for (size_t i = 0; i < numCars; i++)
    {
        // cout << "Type the renting cost and fuel capacity of car " << i+1 << "\n";
        array<int, 2> specs;
        cin >> specs[0] >> specs[1];
        cars.push(specs);
        fuelMin = (fuelMin == 0 || fuelMin > specs[1]) ? specs[1] : fuelMin;
        fuelMax = (fuelMin == 0 || fuelMax < specs[1]) ? specs[1] : fuelMax;
    }

    // Get the station distance data.
    // To make the algorithm simpler, We will calculate the data as if
    // the trip was from the town of Choice to the town of Sorting.
    // cout << "Type the distance of each station.\n";
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
    // Build the diff distance vector.
    stationDistDiff.insert(stationDistDiff.end(), stationDist[0]);
    for (int i = 1; i < numStations + 1; i++)
    {
        stationDistDiff.insert(stationDistDiff.end(), stationDist[i] - stationDist[i - 1]);
    }

    // Get the time and fuel cost data.
    // cout << "Type the time and fuel costs of the economic and spor functionality respectively\n";
    cin >> ts >> cs >> tf >> cf;
}

int roadtrip::getFuelDistanceMinTime(int fuel, int distance)
{
    int distXcf = distance * cf;
    int distXcs = distance * cs;
    if (distXcf < fuel)
    {
        return distance * tf;
    }
    if (distXcs > fuel)
    {
        return -1;
    }

    // Calculate the minimum distance x in economic functionality.
    // If x is bigger than the distance then the car cannot cross, return -1.
    int x = (distXcf - fuel)/(cf - cs);
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

bool roadtrip::fuelIsValid(int fuel)
{
    if (fuelStatus.count(fuel) == 1)
    {
        return fuelStatus[fuel];
    }
    int timePassed = 0;
    int pastResult[2] = {0, 0}; // A variable to avoid calculating again the same distances.
    // We added the final destination distance in the stationDist
    // vector so we iterate until numStations + 1.
    for (size_t i = 0; i < numStations + 1; i++)
    {
        int timeNeeded;
        int distToStation = stationDistDiff[i];
        if (pastResult[0] == distToStation)
        {
            timeNeeded = pastResult[1];
        }
        else
        {
            timeNeeded = getFuelDistanceMinTime(fuel, distToStation);
            if (timeNeeded < 0)
            {
                fuelStatus.insert({ fuel, 0 });
                return false;
            }
            pastResult[0] = distToStation;
            pastResult[1] = timeNeeded;
        }

        timePassed += timeNeeded;
        if (timePassed > time)
        {
            fuelStatus.insert({ fuel, 0 });
            return false;
        }
    }

    fuelStatus.insert({ fuel, 1 });
    return true;
}

int roadtrip::calculateMinFuelSuccess(int min, int max)
{
    if (max - min == 1)
    {
        return fuelIsValid(min) ? min : (fuelIsValid(max) ? max : -1);
    }

    if (!fuelIsValid(max))
    {
        return -1;
    }
    if (fuelIsValid(min))
    {
        return min;
    }

    // Binary search: Check the media value and recurse accordingly.
    int median = ((min + max) / 2);
    return (!fuelIsValid(median)) ? calculateMinFuelSuccess(median, max) : calculateMinFuelSuccess(min, median);
}

int roadtrip::calculateMinRent()
{
    int minRentLocal = 1000000000;
    int minFuelSuccess = calculateMinFuelSuccess(fuelMin, fuelMax);
    if (minFuelSuccess < 0)
    {
        return -1;
    }
    bool hasValidCar = false;
    while(!cars.empty())
    {
        array<int, 2> car = cars.top();
        cars.pop();

        // If the car has:
        // - cost higher that the one of a valid car or
        // - fuel capacity is lower that one of an invalid car
        // then this car is invalid.
        if ((hasValidCar && (car[0] >= minRentLocal)) || car[1] < minFuelSuccess)
        {
            continue;
        }

        hasValidCar = true;
        minRentLocal = (minRentLocal > car[0]) ? car[0] : minRentLocal;
    }
    if (hasValidCar)
    {
        minRent = minRentLocal;
    }

    return minRent;
}

int main()
{
    roadtrip rt;
    rt.getInput();
    cout << rt.calculateMinRent() << "\n";

    return 0;
}
