#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <glm/glm.hpp>

#include <vector>

class WorldObject
{
public:

    WorldObject(int icosphereRecursionLevelInput);

    ~WorldObject();

    void Print();

private:

    void GenerateIcosphere();

    //Creates the first level of the icosphere, or only if recursion level is one.
    //void CreateLevelOneIcosphere();

    //Creates a list of Triangle structs
    void MakeTriangles();

    //Subdivides icosphere, creating new vertices, for each level of recursion.
    void SubdivideIcosphere(unsigned int recursionLevelCurrent);

    //Subfunction of subdivideicosphere. Takes two points of a triangle, forming an edge, and subdivides it.
    //void SubdivideEdge(glm::ivec3 point1, glm::ivec3 point2);

    //Finds the midway point of the two vec and normalizes it to the sphere's radius
    glm::vec3 AverageCoords(glm::ivec3 coordIndex1, glm::ivec3 coordIndex2);

    //Inserts the coord into worldVerts using the index.
    void InsertVertex(glm::ivec3 index, glm::vec3 coord);

    //Sets the pointer specified by indexPointer point to the coord specified by indexCoord;
    void SetPointerToCoord(glm::ivec3 indexCoord, glm::ivec3 indexPointer);

    glm::vec3 GetVertex(glm::ivec3 index);


    struct IcosphereVertex
    {
    	glm::ivec3 index;
        glm::vec3 modelCoord;
        IcosphereVertex *pointerToCoord;
    };

    //Triangle object keeps the indexes for its three points. Used in subdividing icosphere.
    struct Triangle
    {
        glm::ivec3 point1;
        glm::ivec3 point2;
        glm::ivec3 point3;
    };


    IcosphereVertex* worldVerts;

    std::vector<Triangle> icosphereTriangles;

    const unsigned int icosphereRecursionLevel;
    const unsigned int dimensionVertical;
    const unsigned int dimensionHorizontal;
    const unsigned int icosphereResolution;
    const unsigned int panelSize;
};

#endif
