/**
 * @file    tree_mesh_builder.cpp
 *
 * @author  Miroslav Válka <xvalka05@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP tasks + octree early elimination
 *
 * @date    DATE
 **/

#include <iostream>
#include <math.h>
#include <limits>

#include "tree_mesh_builder.h"

TreeMeshBuilder::TreeMeshBuilder(unsigned gridEdgeSize)
    : BaseMeshBuilder(gridEdgeSize, "Octree")
{

}

unsigned TreeMeshBuilder::marchCubesHelper(
    const Vec3_t<float> &startPoint, 
    const ParametricScalarField &field, 
    unsigned cubeSize
) {
    // A. Test na pruchod touto krychli
    // A.1 Najit stred krychle
    float halfCubeSize = cubeSize / 2.0;
    Vec3_t<float> middlePoint(
        (startPoint.x + halfCubeSize) * mGridResolution,
        (startPoint.y + halfCubeSize) * mGridResolution,
        (startPoint.z + halfCubeSize) * mGridResolution
        );
    // A.2 spocitat vzorec a vyhodnotit: F(p) > l + (sqrt(3)/2) * a 
    // float a = cubeSize * mGridResolution
    float f = field.getIsoLevel() + cubeSize * mGridResolution * sqrt(3) / 2;
    if (evaluateFieldAt(middlePoint, field) > f)
        return 0;

    // B. Je cas jiz sestavit krychlicky? CUT-OFF
    if (cubeSize <= 1) 
        return buildCube(startPoint, field);
    
    // C. Rozlozit krychli na osum podkrychli
    // C.1 Kazdy task bude mit svoji promenou pro vysledek
    unsigned triangles[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    // C.2 Rozdeleni na osum casti
    for(unsigned i = 0; i < 8; i++) {
        #pragma omp task shared(startPoint, triangles, field) 
        {
            Vec3_t<float> newStartPoint(
                startPoint.x + ((i & 1)? halfCubeSize : 0),
                startPoint.y + ((i & 2)? halfCubeSize : 0),
                startPoint.z + ((i & 4)? halfCubeSize : 0)
            );
            triangles[i] = marchCubesHelper(newStartPoint, field, halfCubeSize);
        }
    }
    // D. Spocitat vysledky vsech tasku
    unsigned totalTriangles = 0;
    #pragma omp taskwait 
    {
        for(unsigned i = 0; i < 8; i++) {
            totalTriangles += triangles[i];
        }
        return totalTriangles;
    }
}


unsigned TreeMeshBuilder::marchCubes(const ParametricScalarField &field)
{
    // Suggested approach to tackle this problem is to add new method to
    // this class. This method will call itself to process the children.
    // It is also strongly suggested to first implement Octree as sequential
    // code and only when that works add OpenMP tasks to achieve parallelism.
    
    unsigned totalTriangles = 0;
    #pragma omp parallel
    #pragma omp single
    totalTriangles = marchCubesHelper(Vec3_t<float>{0, 0, 0}, field, mGridSize);
    return totalTriangles;
}

float TreeMeshBuilder::evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field)
{
    // viz. loop_mesh_builder
    const Vec3_t<float> *pPoints = field.getPoints().data();
    const unsigned count = unsigned(field.getPoints().size());
    float value = std::numeric_limits<float>::max();

    for(unsigned i = 0; i < count; ++i)
    {
        float distanceSquared  = (pos.x - pPoints[i].x) * (pos.x - pPoints[i].x);
        distanceSquared       += (pos.y - pPoints[i].y) * (pos.y - pPoints[i].y);
        distanceSquared       += (pos.z - pPoints[i].z) * (pos.z - pPoints[i].z);
        value = std::min(value, distanceSquared);
    }
    return sqrt(value);
}

void TreeMeshBuilder::emitTriangle(const BaseMeshBuilder::Triangle_t &triangle)
{
    // viz. loop_mesh_builder
    #pragma omp critical
    mTriangles.push_back(triangle);
}
