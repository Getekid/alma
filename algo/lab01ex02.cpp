#include <iostream>
#include <stack>
#include <vector>

using namespace std;

class poleodomia
{
    public:
      // Input.
      int numSkyscrapers;
      vector<unsigned long long int> skyscraperHeight;
      void getInput();
      // Helper.
      vector<unsigned long long int> costLeft;
      vector<unsigned long long int> costRight;
      void calculateCostLeft();
      void calculateCostRight();
      // Output.
      unsigned long long int caclulateMinCost();
};

void poleodomia::getInput()
{
    // Get the number of the skyscrapers.
    // cout << "Type the # of skyscrapers.\n";
    cin >> numSkyscrapers;

    // Get the height of each skyscraper.
    // cout << "Type the height of each skyscraper.\n";
    int height;
    for (size_t i = 0; i < numSkyscrapers; i++)
    {
        cin >> height;
        skyscraperHeight.insert(skyscraperHeight.end(), height);
        // Initiate the costLeft and costRight vectors as well.
        costLeft.insert(costLeft.end(), 0);
        costRight.insert(costRight.end(), 0);
    }
}

void poleodomia::calculateCostLeft()
{

    stack<int> skyscraperIndexHelper;
    for (int i = 0; i < numSkyscrapers; i++)
    {
        unsigned long long int height = skyscraperHeight[i];
        while (!skyscraperIndexHelper.empty() && skyscraperHeight[skyscraperIndexHelper.top()] < height)
        {
            skyscraperIndexHelper.pop();
        }
        if (skyscraperIndexHelper.empty())
        {
            costLeft[i] = (i + 1) * height;
        }
        else
        {
            int index = skyscraperIndexHelper.top();
            costLeft[i] = costLeft[index] + (i - index) * height;
        }
        skyscraperIndexHelper.push(i);
    }
}

void poleodomia::calculateCostRight()
{

    stack<int> skyscraperIndexHelper;
    for (int i = numSkyscrapers - 1; i >= 0 ; i--)
    {
        unsigned long long int height = skyscraperHeight[i];
        while (!skyscraperIndexHelper.empty() && skyscraperHeight[skyscraperIndexHelper.top()] < height)
        {
            skyscraperIndexHelper.pop();
        }
        if (skyscraperIndexHelper.empty())
        {
            costRight[i] = (numSkyscrapers - i) * height;
        }
        else
        {
            int index = skyscraperIndexHelper.top();
            costRight[i] = costRight[index] + (index - i) * height;
        }
        skyscraperIndexHelper.push(i);
    }
}

unsigned long long int poleodomia::caclulateMinCost()
{
    calculateCostLeft();
    calculateCostRight();
    unsigned long long int minCost = 0;
    for (int i = 0; i < numSkyscrapers; i++)
    {
        unsigned long long int cost = costLeft[i] + costRight[i] - skyscraperHeight[i];
        if (minCost == 0 || cost < minCost)
        {
            minCost = cost;
        }
    }

    return minCost;
}

int main()
{
    poleodomia p;
    p.getInput();
    cout << p.caclulateMinCost() << "\n";

    return 0;
}
