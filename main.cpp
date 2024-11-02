#include "BOOLEAN_OP.hpp"

int main(){
    Polygon p1({{{4.0, 3.0}, {9.0, 7.0}, {0.0, 6.0}}});
    Polygon p2({{{4.0, 3.0}, {9.0, 7.0}, {0.0, 6.0}}});
    // Polygon p1({{{1.0, 1.0}, {3.0, 1.0}, {3.0, 3.0}, {1.0, 3.0}}});
    // Polygon p2({{{2.0, 0.0}, {4.0, 0.0}, {4.0, 1.0}, {2.0, 1.0}}});
    // Polygon p1({{{2.0, 3.0}, {8.0, 3.0}, {8.0, 7.0}, {2.0, 7.0}}, {{3.0, 4.0}, {5.0, 4.0}, {5.0, 6.0}, {3.0, 6.0}}});
    // Polygon p2({{{4.0, 1.0}, {7.0, 1.0}, {7.0, 5.0}, {4.0, 5.0}}});
    Polygon result;
    Boolean_OP BOP(p1, p2, 3);
    BOP.create_SweepEvent();
    BOP.cutting_edge();
    BOP.joining_edge();
    BOP.made_result(result);
    for(size_t i = 0 ; i < result.contours.size() ; i++){
        for(size_t j = 0 ; j < result.contours[i].size() ; j++){
            cout << result.contours[i][j].x << " " << result.contours[i][j].y<< "|";
        }
        cout << endl;
    }

    return 0;
}