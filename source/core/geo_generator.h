//
// Created by ABDERRAHIM ZEBIRI on 2024-07-21.
//

#ifndef THREEDIVE_GEO_GENERATOR_H
#define THREEDIVE_GEO_GENERATOR_H

#include <vector>

namespace geo {

    std::vector<float> generateDetailedGridVertices(float size, float step, float extensionSize, int fadeSteps);

} // geo

#endif //THREEDIVE_GEO_GENERATOR_H
