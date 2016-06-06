#include "WorldObject.h"

#include <math.h>
#include <cstdio>

WorldObject::WorldObject(int icosphereRecursionLevelInput)
    : icosphereRecursionLevel(icosphereRecursionLevelInput)
    , dimensionVertical(pow(2, icosphereRecursionLevel - 1) + 2)
    , dimensionHorizontal(pow(2, icosphereRecursionLevel) + 2)
    , icosphereResolution(5 * dimensionHorizontal * dimensionVertical)
    , panelSize(dimensionHorizontal * dimensionVertical)
{
    //The subdivided icosphere has 2N^2 + 2 vertices, where in is the level of recursion.
    worldVerts = new IcosphereVertex[icosphereResolution]();

    GenerateIcosphere();
}

WorldObject::~WorldObject()
{
    delete[] worldVerts;
}

void WorldObject::Print()
{
	FILE *file = fopen("test.txt", "w");

	for(unsigned int i = 0; i < 5; i++)
	{
		for(unsigned int k = 0; k < dimensionVertical; k++)
		{
			for(unsigned int j = 0; j < dimensionHorizontal; j++)
			{
				if(worldVerts[i * panelSize + j * dimensionVertical + k].pointerToCoord)
				{
					glm::ivec3 &index = worldVerts[i * panelSize + j * dimensionVertical + k].pointerToCoord->index;
					fprintf(file, "(%3d,%3d,%d) ", index.x, index.y, index.z);
				}
				else
					fprintf(file, "(--NULL --) ");
			}
			fprintf(file, "\n");
		}
		fprintf(file, "\n");
	}

	fclose(file);
}

void WorldObject::GenerateIcosphere()
{
    //First the first level of the icosphere is created. Then subdivide is called at the end of the function.

    //The original icosphere is made by three circumscribed rectangle of width 1 and length rectangeLength
    float rectangleLength = (1.0f + sqrt(5.0)) / 2.0;

        //The y index offset of 1 is to leave the first row empty for references to neighbor cells.
        //Subtract 1 from the horizontal displacement since the i-th unit is the (i-1)-th index.

        //north pole
        InsertVertex(glm::ivec3(0, 1, 0),                           glm::vec3(-1.0f, rectangleLength, 0));
            //Manually set the north pole of each panel to point to the north pole of the 0th panel.
            SetPointerToCoord(glm::ivec3(0, 1, 0), glm::ivec3(0, 1, 1));
            SetPointerToCoord(glm::ivec3(0, 1, 0), glm::ivec3(0, 1, 2));
            SetPointerToCoord(glm::ivec3(0, 1, 0), glm::ivec3(0, 1, 3));
            SetPointerToCoord(glm::ivec3(0, 1, 0), glm::ivec3(0, 1, 4));

        //The calls of SetPointerToCoord are paired with InsertVertex calls of the vertex they're copying the pointer of. These pair up the edges
        InsertVertex(glm::ivec3((dimensionHorizontal - 2) / 2, 1, 0),   glm::vec3(-rectangleLength, 0, 1.0f));
            SetPointerToCoord(glm::ivec3((dimensionHorizontal - 2) / 2, 1, 0),  glm::ivec3(1, dimensionVertical - 1, 1));
        InsertVertex(glm::ivec3(dimensionHorizontal - 2, 1, 0),         glm::vec3(0, -1.0f, rectangleLength));
            SetPointerToCoord(glm::ivec3(dimensionHorizontal - 2, 1, 0),                   glm::ivec3(dimensionHorizontal / 2 - 1, dimensionVertical - 1, 1));

        InsertVertex(glm::ivec3(dimensionHorizontal / 2 - 1, 1, 1), glm::vec3(0, 1.0f, rectangleLength));
            SetPointerToCoord(glm::ivec3(dimensionHorizontal / 2 - 1, 1, 1),    glm::ivec3(1, dimensionVertical - 1, 2));
        InsertVertex(glm::ivec3(dimensionHorizontal - 2, 1, 1),     glm::vec3(rectangleLength, 0, 1.0f));
            SetPointerToCoord(glm::ivec3(dimensionHorizontal - 2, 1, 1),                   glm::ivec3(dimensionHorizontal / 2 - 1, dimensionVertical - 1, 2));

        InsertVertex(glm::ivec3(dimensionHorizontal / 2 - 1, 1, 2), glm::vec3(1.0f, rectangleLength, 0));
            SetPointerToCoord(glm::ivec3(dimensionHorizontal / 2 - 1, 1, 2),    glm::ivec3(1, dimensionVertical - 1, 3));
        InsertVertex(glm::ivec3(dimensionHorizontal - 2, 1, 2),     glm::vec3(rectangleLength, 0, -1.0f));
            SetPointerToCoord(glm::ivec3(dimensionHorizontal - 2, 1, 2),                   glm::ivec3(dimensionHorizontal / 2 - 1, dimensionVertical - 1, 3));

        InsertVertex(glm::ivec3(dimensionHorizontal / 2 - 1, 1, 3), glm::vec3(0, 1.0f, -rectangleLength));
            SetPointerToCoord(glm::ivec3(dimensionHorizontal / 2 - 1, 1, 3),    glm::ivec3(1, dimensionVertical - 1, 4));
        InsertVertex(glm::ivec3(dimensionHorizontal - 2, 1, 3),     glm::vec3(0, 1.0f, -rectangleLength));
            SetPointerToCoord(glm::ivec3(dimensionHorizontal - 2, 1, 3),                   glm::ivec3(dimensionHorizontal / 2 - 1, dimensionVertical - 1, 4));

        InsertVertex(glm::ivec3(dimensionHorizontal / 2 - 1, 1, 4), glm::vec3(-rectangleLength, 0, -1.0f));
            SetPointerToCoord(glm::ivec3(dimensionHorizontal / 2 - 1, 1, 4),    glm::ivec3(1, dimensionVertical - 1, 0));
        InsertVertex(glm::ivec3(dimensionHorizontal - 2, 1, 4),     glm::vec3(-1.0f, -rectangleLength, 0));
            SetPointerToCoord(glm::ivec3(dimensionHorizontal - 2, 1, 4),                   glm::ivec3(dimensionHorizontal / 2 - 1, dimensionVertical - 1, 0));

        //South pole.
        InsertVertex(glm::ivec3(dimensionHorizontal - 2, dimensionVertical - 1, 4), glm::vec3(1.0f, -rectangleLength, 0));
            //South pole pointer is copied to the other four panels like the north pole.
            SetPointerToCoord(glm::ivec3(dimensionHorizontal - 2, dimensionVertical - 1, 4), glm::ivec3(dimensionHorizontal - 2, dimensionVertical - 1, 0));
            SetPointerToCoord(glm::ivec3(dimensionHorizontal - 2, dimensionVertical - 1, 4), glm::ivec3(dimensionHorizontal - 2, dimensionVertical - 1, 1));
            SetPointerToCoord(glm::ivec3(dimensionHorizontal - 2, dimensionVertical - 1, 4), glm::ivec3(dimensionHorizontal - 2, dimensionVertical - 1, 2));
            SetPointerToCoord(glm::ivec3(dimensionHorizontal - 2, dimensionVertical - 1, 4), glm::ivec3(dimensionHorizontal - 2, dimensionVertical - 1, 3));

    if(icosphereRecursionLevel > 1)
        SubdivideIcosphere(0);

    printf("Done");

    //Now we set the references to the top row and right column of vertices, which weren't part of any edge. These are simply references to the next panel.
    /*     /\\                                                      //\
     *    /  \\                                                    //  \
     *    \   \\                                                   \\   \
     *     \   \\ <- these vertices, which reference these cells -> \\   \
     *      \  //             on the next panel                      \\  /
     *       \//                                                      \\/
     */
     for(unsigned int loopPanels = 0; loopPanels < 5; loopPanels++)
     {
         /* First the horizontal edge, three of the corners will be dead space with no references. They should never need to be referenced because they
          * are diagonal to the corner of the panel and don't exist when the icosohedron is folded.
          * x       x
          * --------
          * |      |
          * |      |    So we will be leaving these vertices and indices (0,0), (horizontal, 0), and (horizontal, vertical) unset.
          * --------x   Therefore we start the horizontal edge at 1 and end at the (horizontalResolution - 1)-th index.
          */
         for(unsigned int cyclesX = 1; cyclesX < dimensionHorizontal - 1; cyclesX++)
         {
             /* Like matching up the other edges, the edges on one panel behave differently than the edges on the other, so we have a case for the top half
              * and bottom half.
              */
            if(cyclesX < dimensionHorizontal / 2 - 1)
                SetPointerToCoord(glm::ivec3(1, cyclesX + 1, (loopPanels == 4 ? 0 : loopPanels + 1)), glm::ivec3(cyclesX, 0, loopPanels));
            else
                SetPointerToCoord(glm::ivec3(cyclesX - dimensionHorizontal / 2 - 1, dimensionVertical - 2, (loopPanels == 4 ? 0 : loopPanels + 1)), glm::ivec3(cyclesX, 0, loopPanels));
         }

         //Now for the right-most vertical edge. Once again, the corners are dead space so we'll start at 1 and 1 prior to the last element.
         for(unsigned int cyclesY = 1; cyclesY < pow(2, icosphereRecursionLevel - 1) - 1; cyclesY++)
         {
             SetPointerToCoord(glm::ivec3(pow(2, icosphereRecursionLevel - 1) + cyclesY, pow(2, icosphereRecursionLevel - 1) + 1, (loopPanels == 4 ? 0 : loopPanels + 1)), glm::ivec3(pow(2, icosphereRecursionLevel - 1) + 1, cyclesY, loopPanels));
         }
     }

     //Define triangles for rendering.
     MakeTriangles();
}

void WorldObject::MakeTriangles()
{
    //The Triangle object we will be using to temporarily store until pushing them into the container.
    Triangle triangle;

    for(unsigned int loopPanels = 0; loopPanels < 5; loopPanels++)
    {
        //cyclesX starts at 0 because there are no reference cells left of the edge.
        for(unsigned int cyclesX = 0; cyclesX < dimensionHorizontal - 1; cyclesX++)
        {
            /* cyclesY starts at 1 because there is 1 row of reference cells above the panel's edge. The loop test ends one above the bottom row because
             * triangulation ends on the row below. There is nothing below the bottom row to make triangles from.
             * +-------+
             * |_______|
             * |\|/|\|/|
             * +-------+
             */
            for(unsigned int cyclesY = 1; cyclesY < dimensionVertical - 2; cyclesY++)
            {
                //Each cycle contains two triangles, forming one quad.
                triangle.point1 = glm::ivec3(cyclesX, cyclesY, loopPanels);
                triangle.point2 = glm::ivec3(cyclesX + 1, cyclesY, loopPanels);
                triangle.point3 = glm::ivec3(cyclesX, cyclesY + 1, loopPanels);
                    icosphereTriangles.push_back(triangle);

                triangle.point1 = glm::ivec3(cyclesX + 1, cyclesY, loopPanels);
                triangle.point2 = glm::ivec3(cyclesX + 1, cyclesY + 1, loopPanels);
                triangle.point3 = glm::ivec3(cyclesX, cyclesY + 1, loopPanels);
                    icosphereTriangles.push_back(triangle);
            }
        }
    }
}

void WorldObject::SubdivideIcosphere(unsigned int recursionLevelCurrent)
{
    glm::ivec3 newCoordIndex;
    glm::vec3 newCoord;

    unsigned int step = pow(2, icosphereRecursionLevel - 1 - recursionLevelCurrent);

    //First, we'll subdivide the vertical edges, or to further clarify the orientation, the edges spanning the width of the panel.
    for(unsigned int loopPanels = 0; loopPanels < 5; loopPanels++)
    {
        //cyclesX starts at 0 because there are no reference cells left of the edge.
        for(unsigned int cyclesX = 0; cyclesX < pow(2, icosphereRecursionLevel) + 1; cyclesX += step)
        {
            //cyclesY starts at 1 because there is 1 row of reference cells above the panel's edge.
            //Note the limit of the y cycles is half, or one power of 2 less than that of x.
            for(unsigned int cyclesY = 1; cyclesY < pow(2, icosphereRecursionLevel - 1) + 1; cyclesY += step)
            {
                //Find new subdivided vertex.
                newCoordIndex = (glm::ivec3(cyclesX, cyclesY, loopPanels) + glm::ivec3(cyclesX, cyclesY + step, loopPanels)) / 2;
                //Left most edge, these are just setting the pointer to the edge on another face where they're actually contained.
                if(cyclesX == 0)
                {
                    /* The coord is set to point to the coord on the edge of the panel it lines up with. That's the top edge of the
                     * panel that is panel# - 1, or if panel 0, then panel 4.
                     */
                    SetPointerToCoord(glm::ivec3(newCoordIndex.y - 1, 1, (loopPanels == 0 ? 4 : loopPanels - 1)), newCoordIndex);
                }
                else
                {
                    InsertVertex(newCoordIndex, AverageCoords(glm::ivec3(cyclesX, cyclesY, loopPanels), glm::ivec3(cyclesX, cyclesY + step, loopPanels)));
                }
            }
        }
    }
    //Next we'll subdivide the horizontal edges, or the edges spanning the length of the panel.
    for(int loopPanels = 0; loopPanels < 5; loopPanels++)
    {
        for(unsigned int cyclesY = 1; cyclesY <= pow(2, icosphereRecursionLevel - 1) + 1; cyclesY += step)
        {
            //Limit for cyclesX is different because the last point on the horizontal edge is only subdivided with the previous point.
            for(unsigned int cyclesX = 0; cyclesX < pow(2, icosphereRecursionLevel); cyclesX += step)
            {
                //Find new subdivided vertex.
                newCoordIndex = (glm::ivec3(cyclesX, cyclesY, loopPanels) + glm::ivec3(cyclesX + step, cyclesY, loopPanels)) / 2;

                //If we are subdividing the bottom horizontal line which shares its face with another panel...
                if(newCoordIndex.y == pow(2, icosphereRecursionLevel - 1) + 1)
                {
                    /* The coord is set to point to the coord on the edge of the panel it lines up with. That's the right edge of the
                     * panel that is panel# - 1, or if panel 0, then panel 4. The bottom two edges line up to the edges on the other
                     * panel such that they behave differently.
                     *          /\       /\
                     *         /  \_____/  \
                     *         \   \____\   \
                     *          \   \____\   \
                     *           \  /_____\  /
                     *            \/_______\/
                     * Mapping scheme for bottom edge of panel.
                     */
                    if(newCoordIndex.x <= pow(2, icosphereRecursionLevel - 1))
                        //The top half of the edge maps to the bottom half of the top edge of the previous panel.
                        SetPointerToCoord(glm::ivec3(newCoordIndex.x + pow(2, icosphereRecursionLevel - 1), 1, (loopPanels - 1 < 0 ? 4 : loopPanels - 1)), newCoordIndex);
                    else
                        //The bottom half of the edge maps to the bottom edge of the previous panel.
                        SetPointerToCoord(glm::ivec3(pow(2, icosphereRecursionLevel), newCoordIndex.x - pow(2, icosphereRecursionLevel - 1) + 1, (loopPanels - 1 < 0 ? 4 : loopPanels - 1)), newCoordIndex);
                }
                else
                {
                     InsertVertex(newCoordIndex, AverageCoords(glm::ivec3(cyclesX, cyclesY, loopPanels), glm::ivec3(cyclesX + step, cyclesY, loopPanels)));
                }
            }
        }
    }
    /* Now after subdividing the exterior edges, we get the vertex of the intersection of the edges originating from the new vertices.
     * +-----+-----+-----+-----+       +-----+-----+-----+-----+
     * |		   |   		   |       |	 |     |	 |     |
     * |		   |  		   |       |	 |	   |  	 |	   |
     * +		   + 		   +  -->  +-----+-----+-----+-----+
     * |		   | 		   |       |	 |	   | 	 |	   |
     * |		   | 		   |       |	 |	   | 	 |	   |
     * +-----+-----+-----+-----+       +-----+-----+-----+-----+
     * This is identical to the previous loop, except we only do the vertices that weren't subdivided previously by changing the initial cyclesX.
     * We have to do this as a second pass because the first subdivision must be done on all panels so that any vertices referencing another
     * panel don't have a null reference before they are used in a further subdivision.
     */
	for(int loopPanels = 0; loopPanels < 5; loopPanels++)
	{
		for(unsigned int cyclesY = 1 + step / 2; cyclesY <= pow(2, icosphereRecursionLevel - 1) + 1; cyclesY += step)
		{
			for(unsigned int cyclesX = 0; cyclesX < pow(2, icosphereRecursionLevel); cyclesX += step)
			{
                //Find new subdivided vertex.
                newCoordIndex = (glm::ivec3(cyclesX, cyclesY, loopPanels) + glm::ivec3(cyclesX + step, cyclesY, loopPanels)) / 2;

                //If we are subdividing the bottom horizontal line which shares its face with another panel...
                if(newCoordIndex.y == pow(2, icosphereRecursionLevel - 1) + 1)
                {
                    /* The coord is set to point to the coord on the edge of the panel it lines up with. That's the right edge of the
                     * panel that is panel# - 1, or if panel 0, then panel 4. The bottom two edges line up to the edges on the other
                     * panel such that they behave differently.
                     *          /\       /\
                     *         /  \_____/  \
                     *         \   \____\   \
                     *          \   \____\   \
                     *           \  /_____\  /
                     *            \/_______\/
                     * Mapping scheme for bottom edge of panel.
                     */
                    if(newCoordIndex.x <= pow(2, icosphereRecursionLevel - 1))
                        //The top half of the edge maps to the bottom half of the top edge of the previous panel.
                        SetPointerToCoord(glm::ivec3(newCoordIndex.x + pow(2, icosphereRecursionLevel - 1), 1, (loopPanels - 1 < 0 ? 4 : loopPanels - 1)), newCoordIndex);
                    else
                        //The bottom half of the edge maps to the bottom edge of the previous panel.
                        SetPointerToCoord(glm::ivec3(pow(2, icosphereRecursionLevel), newCoordIndex.x - pow(2, icosphereRecursionLevel - 1) + 1, (loopPanels - 1 < 0 ? 4 : loopPanels - 1)), newCoordIndex);
                }
                else
                {
                     InsertVertex(newCoordIndex, AverageCoords(glm::ivec3(cyclesX, cyclesY, loopPanels), glm::ivec3(cyclesX + step, cyclesY, loopPanels)));
                }
            }
        }
    }

    /*for(icosphereTriangles::iterator it = icosphereTriangles.begin(); it != icosphereTriangles.end(); it++)
    {
        triangle = *it;

        //Edge 1
            //Determine indices, find the actual coord in space, and insert into the struct
            newCoordIndex = (triangle.point1 + triangle.point2) / 2;
            newCoord = AverageCoords(triangle.point1, triangle.point2);

            InsertVertex(newCoordIndex, newCoord);

        //Edge 2
            newCoordIndex = (triangle.point2 + triangle.point3) / 2;
            newCoord = AverageCoords(triangle.point2, triangle.point3);

            InsertVertex(newCoordIndex, newCoord);

        //Edge 3
            newCoordIndex = (triangle.point3 + triangle.point1) / 2;
            newCoord = AverageCoords(triangle.point3, triangle.point1);

            InsertVertex(newCoordIndex, newCoord);
    }*/

    if(recursionLevelCurrent + 1 < icosphereRecursionLevel)
        SubdivideIcosphere(recursionLevelCurrent + 1);
}

glm::vec3 WorldObject::AverageCoords(glm::ivec3 coordIndex1, glm::ivec3 coordIndex2)
{
    glm::vec3 coordAvg = (GetVertex(coordIndex1) + GetVertex(coordIndex2)) / 2.0f;

    float length = coordAvg.x * coordAvg.x + coordAvg.y * coordAvg.y + coordAvg.z * coordAvg.z;

    //Dividing by length of the vector from origin to the point puts coordinates on unit sphere.
    coordAvg /= length;

    return coordAvg;
}

void WorldObject::InsertVertex(glm::ivec3 index, glm::vec3 coord)
{
    worldVerts[index.z * panelSize + index.x * dimensionVertical + index.y].index = index;
    worldVerts[index.z * panelSize + index.x * dimensionVertical + index.y].modelCoord = coord;
    worldVerts[index.z * panelSize + index.x * dimensionVertical + index.y].pointerToCoord = &(worldVerts[index.z * panelSize + index.x * dimensionVertical + index.y]);
}

void WorldObject::SetPointerToCoord(glm::ivec3 indexCoord, glm::ivec3 indexPointer)
{
    worldVerts[indexPointer.z * panelSize + indexPointer.x * dimensionVertical + indexPointer.y].pointerToCoord = &(worldVerts[indexCoord.z * panelSize + indexCoord.x * dimensionVertical + indexCoord.y]);
}

glm::vec3 WorldObject::GetVertex(glm::ivec3 index)
{
    return worldVerts[index.z * panelSize + index.x * dimensionVertical + index.y].pointerToCoord->modelCoord;
}
