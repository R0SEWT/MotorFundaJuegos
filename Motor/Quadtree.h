#include <vector>
#include <iostream>

struct Point {
    float x, y;
};

class Quadtree {
private:
    const int MAX_OBJECTS = 4;
    const int MAX_LEVELS = 5;

    int level;
    std::vector<Point> objects;
    float bounds[4]; // [x, y, width, height]
    Quadtree* nodes[4];

    void split() {
        float subWidth = (bounds[2] / 2);
        float subHeight = (bounds[3] / 2);
        float x = bounds[0];
        float y = bounds[1];

        float aux[4][4] = {
            { x + subWidth, y, subWidth, subHeight },
			{ x, y, subWidth, subHeight },
			{ x, y + subHeight, subWidth, subHeight },
			{ x + subWidth, y + subHeight, subWidth, subHeight }
		};

        for (int i = 0; i < 4; i++) {
			nodes[i] = new Quadtree();
            nodes[i]->init(level + 1, aux[i]);
		}

    }

    int getIndex(Point p) {
        int index = -1;
        float verticalMidpoint = bounds[0] + (bounds[2] / 2);
        float horizontalMidpoint = bounds[1] + (bounds[3] / 2);

        bool topQuadrant = (p.y < horizontalMidpoint && p.y >= bounds[1]);
        bool bottomQuadrant = (p.y >= horizontalMidpoint);

        if (p.x < verticalMidpoint && p.x >= bounds[0]) {
            if (topQuadrant) {
                index = 1; // Nodo superior derecho
            }
            else if (bottomQuadrant) {
                index = 2; // Nodo inferior derecho
            }
        }
        else if (p.x >= verticalMidpoint) {
            if (topQuadrant) {
                index = 0; // Nodo superior izquierdo
            }
            else if (bottomQuadrant) {
                index = 3; // Nodo inferior izquierdo
            }
        }

        return index;
    }

public:
    Quadtree() {

    }

    void init(int pLevel, float pBounds[4]) {
        level = pLevel;
		bounds[0] = pBounds[0];
		bounds[1] = pBounds[1];
		bounds[2] = pBounds[2];
		bounds[3] = pBounds[3];
		for (int i = 0; i < 4; i++) nodes[i] = nullptr;
	}

    void clear() {
        objects.clear();

        for (int i = 0; i < 4; i++) {
            if (nodes[i] != nullptr) {
                nodes[i]->clear();
                delete nodes[i];
                nodes[i] = nullptr;
            }
        }
    }
    // Insert the object into the quadtree. If the node
    // exceeds the capacity, it will split and add all
    // objects to their corresponding nodes.

    // produce stack overflow if the point is outside the bounds
    // of the quadtree eso sucede porque el punto no se inserta en el nodo
    // sino en el objeto, por lo que si el punto no esta en el rango del objeto
    // se produce un stack overflow
    void insert(Point p) {
        if (nodes[0] != nullptr) {
            int index = getIndex(p);

            if (index != -1) {
                nodes[index]->insert(p);
                return;
            }
        }

        objects.push_back(p);

        if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
            if (nodes[0] == nullptr) {
                split();
            }

            int i = 0;
            while (i < objects.size()) {
                int index = getIndex(objects[i]);
                if (index != -1) {
                    nodes[index]->insert(objects[i]);
                    objects.erase(objects.begin() + i);
                }
                else {
                    i++;
                }
            }
        }
    }

    std::vector<Point> retrieve(std::vector<Point>& returnObjects, Point p) {
        int index = getIndex(p);
        if (index != -1 && nodes[0] != nullptr) {
            nodes[index]->retrieve(returnObjects, p);
        }

        returnObjects.insert(returnObjects.end(), objects.begin(), objects.end());

        return returnObjects;
    }
};
