#include "Utils.h"

#include <cmath>

double toRadians(const double& angle) {
    return angle * M_PI / 180.0;
}

double haversineDistance(Vertex* v1, Vertex* v2) {
    double EARTH_RADIUS = 6371.0 * 1000.0;

    double dLat = toRadians(v2->getLat() - v1->getLat());
    double dLon = toRadians(v2->getLon() - v1->getLon());

    double lat1 = toRadians(v1->getLat());
    double lat2 = toRadians(v2->getLat());

    double a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
    double c = 2 * asin(sqrt(a));

    return EARTH_RADIUS * c;
}
