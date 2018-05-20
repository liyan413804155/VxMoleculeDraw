#include "VxGeamCm.h"

void GeMeshTri
(
int xStep,
int yStep,
QVector<QVector3D>& faceVertex,
QVector<short>& faceIndex,
std::function<QVector3D(int i,int j)> funcVertex,
std::function<QVector3D(int i,int j)> funcNormal
)
{
    for (int i = 0; i <= yStep; i++)
    {
        for (int j = 0; j <= xStep; j++)
        {
            faceVertex.push_back(funcVertex(i, j));
            faceVertex.push_back(funcNormal(i, j));
        }
    }

    for (int i = 0; i < yStep; i++)
    {
        for (int j = 0; j < xStep; j++)
        {
            faceIndex.push_back(i * (xStep + 1) + j);
            faceIndex.push_back((i + 1) * (xStep + 1) + j + 1);
            faceIndex.push_back((i + 1) * (xStep + 1) + j);
            faceIndex.push_back(i * (xStep + 1) + j);
            faceIndex.push_back(i * (xStep + 1) + j + 1);
            faceIndex.push_back((i + 1) * (xStep + 1) + j + 1);
        }
    }
}

VxGeam::VxGeam()
{
}

VxGeam::~VxGeam()
{

}

