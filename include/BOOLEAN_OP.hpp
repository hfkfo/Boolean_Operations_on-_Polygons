#include<bits/stdc++.h>
#include <limits>
#include "Polygon.hpp"
using namespace std;


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

