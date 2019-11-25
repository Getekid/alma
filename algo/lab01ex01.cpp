#include <iostream>
#include <stack>
#include <array>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
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
      vector<int> stationDistDiff; // A vector to store the diff distance between the stations.
      int getFuelDistanceMinTime(int, int);
      bool carIsValid(array<int, 2>);
      // Output.
      int minRent = -1;
      int caclulateMinRent();
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
    sort(stationDistDiff.begin(), stationDistDiff.end(), greater <>());

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

    // Calcullate the minimum distance x in economic functionality.
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

bool roadtrip::carIsValid(array<int, 2> car)
{
    int timePassed = 0;
    int pastResult[2] = {0, 0}; // A variable to avoid calculating again the same distances.
    // We added the final destination distance in the stationDist
    // vector so we iterate until numStations + 1.
    for (size_t i = 0; i < numStations + 1; i++)
    {
        int timeNeeded;
        // int distToStation = stationDist[i] - distMoved;
        int distToStation = stationDistDiff[i];
        if (pastResult[0] == distToStation)
        {
            timeNeeded = pastResult[1];
        }
        else
        {
            timeNeeded = getFuelDistanceMinTime(car[1], distToStation);
            if (timeNeeded < 0)
            {
                return false;
            }
            pastResult[0] = distToStation;
            pastResult[1] = timeNeeded;
        }

        timePassed += timeNeeded;
        if (timePassed > time)
        {
            return false;
        }
    }

    return true;
}

int roadtrip::caclulateMinRent()
{
    int fuelFail = 0;
    unsigned long int fuelSuccess = dist * cf;
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
        
        if (car[1] >= fuelSuccess || carIsValid(car))
        {
            hasValidCar = true;
            if (minRentLocal > car[0])
            {
                minRentLocal = car[0];
            }
            if (fuelSuccess > car[1])
            {
                fuelSuccess = car[1];
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
    roadtrip rt;
    rt.getInput();
    cout << rt.caclulateMinRent() << "\n";

    return 0;
}
