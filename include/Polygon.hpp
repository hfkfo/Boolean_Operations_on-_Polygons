#include<bits/stdc++.h>
#include <limits>
using namespace std;



class Point{
public:
	double x, y;
	Point (): x(0), y (0) {}
	Point (double ax, double ay): x (ax), y (ay) {}

	double dist(const Point& p) const
	{
		double dx = x - p.x;
		double dy = y - p.y;
		return sqrt (dx * dx + dy * dy);
	}

    bool operator==(const Point & other) const{
		return x == other.x && y == other.y;
	}
    bool operator!= (const Point& p) const { return !(*this == p); }
	bool operator<(const Point& other) const {
        if (x == other.x) {
            return y < other.y;
        }
        return x < other.x;
    }
};
inline double signedArea (const Point& p0, const Point& p1, const Point& p2){ 
		return (p0.x - p2.x)*(p1.y - p2.y) - (p1.x - p2.x) * (p0.y - p2.y);
}
class Segment {
public:
	Point p1, p2;

public:
	/** Default constructor */
	Segment () {}
	~Segment () {}

	/** Constructor from two points **/
	Segment(const Point& ap1, const Point& ap2) : p1 (ap1), p2 (ap2) {}

	/** Get the beginning point */
	const Point& begin() const { return p1; }
	/** Get the end point */
	const Point& end() const { return p2; }
	Point minPoint() const {
        return min(p1, p2);
    }

    Point maxPoint() const {
        return max(p1, p2);
    }

};

class Polygon{
public:
    vector<vector<Point>> contours = {};

	vector<vector<Point>> get_polygons() const{
		return contours;
	}
	Segment segment(unsigned j ,unsigned p) const { 
		return (p == contours[j].size() - 1) ? Segment (contours[j].back (), contours[j].front ()) : Segment (contours[j][p], contours[j][p+1]); 
	}
    Polygon(){}
    Polygon(vector<vector<pair<double, double>>> polygons){
        for(auto i : polygons){
            vector<Point> polygon;
            for(auto v : i){
				polygon.push_back(Point(v.first, v.second));
			}
            contours.push_back(polygon);
        }
    }
	bool is_empty() const{
		return contours.empty();
	}
	bool operator == (Polygon const &other){
		if(contours.size() != other.contours.size())
			return false;
		size_t j;
		for(auto p : contours){
			bool judge = false;
			for(auto po : other.contours){
				size_t i;
				if(po.size() != p.size())
					continue;
				for(i = 0 ; i < po.size() ; i++){
					if(p[0] == po[i]){
						break;
					}
				}
				if(i == po.size())
					continue;
				j = 1;
				size_t tmp = i;
				if(i == 0){
					tmp = po.size() - 1;
					if(po[tmp] == p[j]){
						while (po[tmp] == p[j] && j < p.size())
						{
							tmp--;
							j++;
						}
						if(j == p.size()){
							judge = true;
							break;
						}
					}
				}
				else if(po[tmp - 1] == p[j]){
					tmp--;
					while (po[tmp] == p[j] && j < p.size())
					{
						tmp--;
						j++;
						if(tmp < 0){
							tmp = po.size() - 1;
						}
					}
					if(j == p.size()){
						judge = true;
						break;
					}
				}
				j = 1;
				tmp = i;
				if(po[tmp + 1] == p[1]){
					tmp++;
					while (po[tmp] == p[j] && j < p.size())
					{
						tmp++;
						j++;
						if(tmp == po.size()){
							tmp = 0;
						}
					}
					if(j == p.size()){
						judge = true;
						break;
					}
				}
			}
			if(judge == false){
				return false;
			}
		}
		return true;
	}
};