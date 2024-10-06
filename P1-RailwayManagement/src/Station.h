#ifndef DA_RAILWAYMANAGEMENT_STATION_H
#define DA_RAILWAYMANAGEMENT_STATION_H

#include <string>

class Station {
private:
    std::string name;
    std::string district;
    std::string municipality;
    std::string township;
    std::string line;
public:
    Station();

    Station(
            const std::string &name,
            const std::string &district,
            const std::string &municipality,
            const std::string &township,
            const std::string &line
    );

    std::string getName() const;

    std::string getDistrict() const;

    std::string getMunicipality() const;

    bool operator==(const Station &station) const;
};

#endif //DA_RAILWAYMANAGEMENT_STATION_H
