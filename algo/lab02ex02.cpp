#include <iostream>
#include <map>
#include <vector>
using namespace std;

class Node
{
    public:
        vector<int> children;
        int parent = 0;
        int countEdges();
};

int Node::countEdges()
{
    return children.size() + 1;
}

class xmas_lighting
{
    public:
        int N, K;
        map<int, Node*> intersections;
        void getInput();
        int getMinLighting();
        int getMinLightingAll(int begin);
};

void xmas_lighting::getInput()
{
    // Get the number of the intersections and the minimum number of streets to light.
    cin >> N >> K;

    for (int i = 1; i <= N; i++)
    {
        // Get the i edge.
        int parent, child;
        cin >> parent >> child;
        // Create any node necessary.
        if (i == 1)
        {
            intersections.insert(make_pair(parent, new Node()));
        }
        intersections.insert(make_pair(child, new Node()));
        // Populate the parent and children values.
        intersections[parent]->children.insert(intersections[parent]->children.end(), child);
        intersections[child]->parent = parent;
    }
}

int xmas_lighting::getMinLighting()
{
    int lights = 0;
    if (K == N - 1)
    {
        return getMinLightingAll(1);
    }

    return lights;
}

int xmas_lighting::getMinLightingAll(int begin)
{
    if (intersections[begin]->children.empty())
    {
        return 0;
    }

    int lights = 0;
    bool hasLeaves = false;

    // Get the sum of the min lighting of the node's children and check if it has any leaves.
    // If it has no leaves, then we treat that node as one. This way we can use the nodes
    // that link to nodes with leaves as leaves themselves and complete the recursion.
    for (int child : intersections[begin]->children)
    {
        lights = lights + getMinLightingAll(child);
        if (intersections[child]->children.empty())
        {
            hasLeaves = true;
        }
    }

    if (hasLeaves) // If it has leaves then that node will be "lighted" so we add 1.
    {
        return lights + 1;
    }
    else // If it doesn't have leaves, then make this a leaf.
    {
        intersections[begin]->children.clear();
        return lights;
    }
}

int main()
{
    xmas_lighting xl;
    xl.getInput();
    cout << xl.getMinLighting() << "\n";

    return 0;
}
