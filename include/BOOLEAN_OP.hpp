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
	
};
class Boolean_OP{
public:
	
	Boolean_OP(Polygon a, Polygon b,int op):subject(a), clipping(b), op(op){};
    ~Boolean_OP(){};
	void create_SweepEvent();
    void cutting_edge();
    void joining_edge();
	void made_result(Polygon& p);
	
private:
	enum PolygonType{ SUBJECT, CLIPPING };
	enum EdgeType { NORMAL, NON_CONTRIBUTING, SAME_TRANSITION, DIFFERENT_TRANSITION };
	struct SweepEvent;
	struct SegmentComp : public binary_function<SweepEvent*, SweepEvent*, bool> { // for sorting edges in the sweep line
		bool operator() (SweepEvent* e1, SweepEvent* e2);
	};
	struct SweepEventComp : public binary_function<SweepEvent*, SweepEvent*, bool> { // for sortening events
		bool operator() (SweepEvent* e1, SweepEvent* e2);
	};
	struct SweepEvent {
		Point p;           
		bool left;         
		PolygonType pl;    
		SweepEvent *other; 
		bool inOut;
		bool inside;
		EdgeType type;
		set<SweepEvent*, SegmentComp>::iterator poss;
		int pos;
    	bool processed = false;
		/** Class constructor */
		SweepEvent (const Point& pp, bool b, PolygonType apl, SweepEvent* o, EdgeType t = NORMAL) : p (pp), left (b), pl (apl), other (o), type (t) {}
 		/** Return the line segment associated to the SweepEvent */
		Segment segment () { return Segment (p, other->p); }
		/** Is the line segment (p, other->p) below point x */
		bool below (const Point& x) const { return (left) ? signedArea (p, other->p, x) > 0 : signedArea (other->p, p, x) > 0; }
		/** Is the line segment (p, other->p) above point x */
		bool above (const Point& x) const { return !below (x); }
	};
	void processSegment (const Segment& s, PolygonType pl);
	void possibleIntersection (SweepEvent *e1, SweepEvent *e2);
    void divideSegment (SweepEvent *e, const Point& p);
	std::priority_queue <SweepEvent*, std::vector<SweepEvent*>, SweepEventComp> eq;
	deque<SweepEvent> eventHolder;
	SweepEventComp sec;
    std::set<SweepEvent*, SegmentComp> S;
    std::vector<Segment> result_segment;
	SweepEvent *storeSweepEvent(const SweepEvent& e) { eventHolder.push_back (e); return &eventHolder.back (); }
	Polygon subject; 
	Polygon clipping;
	size_t op=0; // Union = 0 , Intersection = 1, Difference = 2, XOR = 3
	list<pair<list<Point>, bool>> closedPolygons = {};
};

