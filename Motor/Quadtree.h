#include <vector>
#include <glm/glm.hpp>

class Quadtree {
private:
    const int MAX_OBJECTS = 4;
    const int MAX_LEVELS = 5;

    int level;
    std::vector<glm::vec2> objects;
    float bounds[4]; // [x, y, width, height]
    Quadtree* nodes[4];

    // o(1)
    void split();
    // o(1)
    int getIndex(glm::vec2 p);

public:
    Quadtree() {

    }
    // o(1)
    void init(int pLevel, float pBounds[4]);

    // o(n)
    void clear();
    // o(log n)
    void insert(glm::vec2 p);
    // o(n) y theta(n)
    std::vector<glm::vec2> retrieve(std::vector<glm::vec2>& returnObjects, glm::vec2 p);
};
