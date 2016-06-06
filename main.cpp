#include "WorldObject.h"

int main()
{
    unsigned int icosphereRecursionLevel = 8;

    WorldObject worldGenerating(icosphereRecursionLevel);

    worldGenerating.Print();
}
