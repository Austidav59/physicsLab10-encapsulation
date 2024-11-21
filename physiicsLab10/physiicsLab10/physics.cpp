/***********************************************************************
 * Source File:
 *    PHYSICS
 * Author:
 *    <your name here>
 * Summary:
 *    Laws of motion, effects of gravity, wind resistance, etc.
 ************************************************************************/

#include "physics.h"  // for the prototypes
#include <stdexcept>
using namespace std;// Ensure this header is included

/*********************************************************
 * LINEAR INTERPOLATION
 * From a list of domains and ranges, linearly interpolate.
 *********************************************************/
double linearInterpolation(const Mapping mapping[], int numMapping, double domain)
{
    // Ensure there are at least two mappings
    if (numMapping < 2) {
        throw invalid_argument("At least two mappings are required for interpolation.");
    }

    // Find the correct range of mappings
    for (int i = 0; i < numMapping - 1; ++i) {
        if (domain >= mapping[i].domain && domain <= mapping[i + 1].domain) {
            return linearInterpolation(mapping[i].domain, mapping[i].range,
                                       mapping[i + 1].domain, mapping[i + 1].range,
                                       domain);
        }
    }

    // Out of bounds: return the closest range (optional)
    if (domain < mapping[0].domain)
        return mapping[0].range; // Below the first domain
    if (domain > mapping[numMapping - 1].domain)
        return mapping[numMapping - 1].range; // Above the last domain

    throw out_of_range("Domain is out of range for the provided mappings.");
}

/*********************************************************
 * GRAVITY FROM ALTITUDE
 * Determine gravity coefficient based on altitude
 *********************************************************/
double gravityFromAltitude(double altitude)
{
    Mapping gravityMap[] = {
        {0, 9.807},    // Sea level
        {10000, 9.764}, // 10 km altitude
        {20000, 9.721}  // 20 km altitude
    };
    const int numMapping = sizeof(gravityMap) / sizeof(gravityMap[0]);
    return linearInterpolation(gravityMap, numMapping, altitude);
}

/*********************************************************
 * DENSITY FROM ALTITUDE
 * Determine the density of air based on altitude
 *********************************************************/
double densityFromAltitude(double altitude)
{
    Mapping densityMap[] = {
        {0, 1.225},    // Sea level
        {10000, 0.4135}, // 10 km altitude
        {20000, 0.08891} // 20 km altitude
    };
    const int numMapping = sizeof(densityMap) / sizeof(densityMap[0]);
    return linearInterpolation(densityMap, numMapping, altitude);
}

/*********************************************************
 * SPEED OF SOUND FROM ALTITUDE
 * Determine the speed of sound at a given altitude.
 ********************************************************/
double speedSoundFromAltitude(double altitude)
{
    Mapping speedSoundMap[] = {
        {0, 340.3},  // Sea level
        {10000, 299.5}, // 10 km altitude
        {20000, 295.0}  // 20 km altitude
    };
    const int numMapping = sizeof(speedSoundMap) / sizeof(speedSoundMap[0]);
    return linearInterpolation(speedSoundMap, numMapping, altitude);
}

/*********************************************************
 * DRAG FROM MACH
 * Determine the drag coefficient based on Mach number.
 *********************************************************/
double dragFromMach(double speedMach)
{
    Mapping dragMap[] = {
        {0.8, 0.2},   // Subsonic region
        {1.0, 0.5},   // Transonic region
        {2.0, 0.3}    // Supersonic region
    };
    const int numMapping = sizeof(dragMap) / sizeof(dragMap[0]);
    return linearInterpolation(dragMap, numMapping, speedMach);
}
