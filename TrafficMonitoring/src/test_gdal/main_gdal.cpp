/**
 *  @file    main.hpp
 *  @author  Alessandra Fais
 *  @date    13/06/2019
 *
 *  @brief Definition of main class to test Road_Grid_List functionalities.
 */

#include <iostream>

#include "ogr_geometry.h"
#include "../../includes/geographic_model/road_grid_list.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    /// read shapefile
    Road_Grid_List road_grid_list("../data/beijing/roads.shp");

    /// process 4 records of the original dataset
    string file = "4186589,33556,2009-01-06T05:19:07,40.0426,116.609,0,0\n"
                  "4186590,33556,2009-01-06T05:20:07,40.0427,116.606,18.41,291\n"
                  "4186591,33556,2009-01-06T05:21:07,40.0442,116.602,3.44488,297\n"
                  "4186592,33556,2009-01-06T05:22:07,40.0438,116.602,1.14084,159";
    istringstream is(file);
    string line;
    cout << "Process records: " << file << endl;
    while (getline(is, line)) {
        cout << "Processing line: " << line << endl;
        int token_count = 0;
        vector<string> tokens;

        size_t last = 0;
        size_t next = 0;
        while ((next = line.find(',', last)) != string::npos) {
            tokens.push_back(line.substr(last, next - last));
            last = next + 1;
            token_count++;
        }
        tokens.push_back(line.substr(last));
        token_count++;
        if (token_count >= 7) {
            int taxi_id = atoi(tokens.at(0).c_str());
            double lat = atof(tokens.at(3).c_str());
            double lon = atof(tokens.at(4).c_str());
            double speed = atof(tokens.at(5).c_str());
            double dir = atoi(tokens.at(6).c_str());

            cout << "Process point <" << lat << ", " << lon << "> - taxi_id: " << taxi_id << " speed: " << speed << " dir: " << dir << endl;
            OGRPoint p(lon, lat);
            int road_id = road_grid_list.fetch_road_ID(p);
            cout << "RoadID found is " << road_id << endl;
        }
    }

    return 0;
}