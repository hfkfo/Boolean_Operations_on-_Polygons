#include "BOOLEAN_OP.hpp"
#include <pybind11/stl.h>
#include<pybind11/pybind11.h>


int findIntersection(const Segment& seg0, const Segment& seg1, Point& pi0, Point& pi1)
{
	const Point& p0 = seg0.begin();
	Point d0(seg0.end().x - p0.x, seg0.end().y - p0.y);
	const Point& p1 = seg1.begin();
	Point d1(seg1.end().x - p1.x, seg1.end().y - p1.y);
	double sqrEpsilon = 0.0000001; 
	Point E(p1.x - p0.x, p1.y - p0.y);
	double kross = d0.x * d1.y - d0.y * d1.x;
	double sqrKross = kross * kross;
	double sqrLen0 = d0.x * d0.x + d0.y * d0.y;
	double sqrLen1 = d1.x * d1.x + d1.y * d1.y;
	
	if(sqrKross > sqrEpsilon * sqrLen0 * sqrLen1) {
		double s =(E.x * d1.y - E.y * d1.x) / kross;
		if((s < 0) ||(s > 1)) {
			return 0;
		}
		double t =(E.x * d0.y - E.y * d0.x) / kross;
		if((t < 0) ||(t > 1)) {
			return 0;
		}
		pi0.x = p0.x + s * d0.x;
		pi0.y = p0.y + s * d0.y;
		return 1;
	}

	double sqrLenE = E.x * E.x + E.y * E.y;
	kross = E.x * d0.y - E.y * d0.x;
	sqrKross = kross * kross;
	if(sqrKross > sqrEpsilon * sqrLen0 * sqrLenE) {
		return 0;
	}

	Point seg0_min = seg0.minPoint();
	Point seg0_max = seg0.maxPoint();
	Point seg1_min = seg1.minPoint();
	Point seg1_max = seg1.maxPoint();

	if (seg0_max < seg1_min || seg1_max < seg0_min) {
		return 0;
	}

	Point minOverlap = std::max(seg0_min, seg1_min);
	Point maxOverlap = std::min(seg0_max, seg1_max);

	if (minOverlap == maxOverlap) {
		pi0 = minOverlap;
		return 1;
	} 
	else {
		pi0 = minOverlap;
		pi1 = maxOverlap;
		return 2;
	}
}
bool Boolean_OP::SweepEventComp::operator()(SweepEvent* e1, SweepEvent* e2) {
	if(e1->p.x != e2->p.x) 
		return e1->p.x > e2->p.x;
	if(e1->p != e2->p) 
		return e1->p.y > e2->p.y;
	if(e1->left != e2->left) 
		return e1->left;
	return e1->above(e2->other->p);
}

bool Boolean_OP::SegmentComp::operator()(SweepEvent* e1, SweepEvent* e2) {
	if(e1 == e2)
		return false;
	if(signedArea(e1->p, e1->other->p, e2->p) != 0 || signedArea(e1->p, e1->other->p, e2->other->p) != 0) {
		if(e1->p == e2->p)
			return e1->below(e2->other->p);

		SweepEventComp comp;
		if(comp(e1, e2))  
			return e2->above(e1->p);
		
		return e1->below(e2->p);
	}
	if(e1->p == e2->p)
		return e1 < e2;
	SweepEventComp comp;
	return comp(e1, e2);
}

void Boolean_OP::create_SweepEvent(){
    for(unsigned int i = 0; i < subject.contours.size(); i++)
		for(unsigned int j = 0; j < subject.contours[i].size(); j++)
			processSegment(subject.segment(i,j), SUBJECT);
	for(unsigned int i = 0; i < clipping.contours.size(); i++)
		for(unsigned int j = 0; j < clipping.contours[i].size(); j++)
			processSegment(clipping.segment(i,j), CLIPPING);
}

void Boolean_OP::cutting_edge(){
	set<SweepEvent*, SegmentComp>::iterator it, sli, prev, next;
	SweepEvent* e;
	
	while(!eq.empty()) {
		e = eq.top();
		eq.pop();
		if(e->left) { 
			e->poss = it = S.insert(e).first;
			next = prev = it;
			(prev != S.begin()) ? --prev : prev = S.end();
			
			if(prev == S.end()) {           
				e->inside = e->inOut = false;
			} 
			else if((*prev)->type != NORMAL) {
				if(prev == S.begin()) { 
					e->inside = true; 
					e->inOut = false;
				} else {   
					sli = prev;
					sli--;
					if((*prev)->pl == e->pl) {
						e->inOut  = !(*prev)->inOut;
						e->inside = !(*sli)->inOut;
					} else {
						e->inOut  = !(*sli)->inOut;
						e->inside = !(*prev)->inOut;
					}
				}
			}
			else if(e->pl ==(*prev)->pl) { 
				e->inside =(*prev)->inside;
				e->inOut  = !(*prev)->inOut;
			} 
			else {                          
				e->inside = !(*prev)->inOut;
				e->inOut  =(*prev)->inside;
			}

			
			if((++next) != S.end())
				possibleIntersection(e, *next);

			
			if(prev != S.end())
				possibleIntersection(*prev, e);
		} 
		else { 
			next = prev = sli = e->other->poss; 
			++next;
			(prev != S.begin()) ? --prev : prev = S.end();
			switch(e->type) {
				case(NORMAL):
					switch(op) {
						case(0):
							if(!e->other->inside)
								result_segment.push_back(e->segment());
							break;
						case(1):
							if(e->other->inside)
								result_segment.push_back(e->segment());
							break;
						case(2):
							if(((e->pl == SUBJECT) &&(!e->other->inside)) ||(e->pl == CLIPPING && e->other->inside))
								result_segment.push_back(e->segment());
							break;
						case(3):
							result_segment.push_back(e->segment());
							break;
					}
					break;
				case(SAME_TRANSITION):
					if(op == 0 || op == 1)
						result_segment.push_back(e->segment());
					break;
				case(DIFFERENT_TRANSITION):
					if(op == 2)
						result_segment.push_back(e->segment());
					break;
				case(NON_CONTRIBUTING):
					break;
			}
			S.erase(sli);
			if(next != S.end() && prev != S.end())
				possibleIntersection(*prev, *next);
		}
	}
}

void Boolean_OP::processSegment(const Segment& s, PolygonType pl)
{
	if(s.begin() == s.end()) 
		return;                 
	SweepEvent* e1 = storeSweepEvent(SweepEvent(s.begin(), true, pl, 0));
	SweepEvent* e2 = storeSweepEvent(SweepEvent(s.end(), true, pl, e1));
	e1->other = e2;
	
	if(e1->p.x < e2->p.x) {
		e2->left = false;
	} else if(e1->p.x > e2->p.x) {
		e1->left = false;
	} else if(e1->p.y < e2->p.y) { 
		e2->left = false;
	} else {
		e1->left = false;
	}
	eq.push(e1);
	eq.push(e2);
}
void Boolean_OP::possibleIntersection(SweepEvent *e1, SweepEvent *e2){
	Point ip1, ip2;  
	int nintersections;

	if(!(nintersections = findIntersection(e1->segment(), e2->segment(), ip1, ip2)))
		return;

	if((nintersections == 1) &&((e1->p == e2->p) ||(e1->other->p == e2->other->p)))
		return; 

	if(nintersections == 2 && e1->pl == e2->pl) {  
		std::cerr << "A polygon has overlapping edges. Sorry, but the program does not work yet with this kind of polygon\n";
		exit(1); 
	}
	if(nintersections == 1) {
		if(e1->p != ip1 && e1->other->p != ip1)  // if ip1 is not an endpoint of the line segment associated to e1 then divide "e1"
			divideSegment(e1, ip1);
		if(e2->p != ip1 && e2->other->p != ip1)  // if ip1 is not an endpoint of the line segment associated to e2 then divide "e2"
			divideSegment(e2, ip1);
		return;
	}

	// The line segments overlap
	vector<SweepEvent *> sortedEvents;
	if(e1->p == e2->p) {
		sortedEvents.push_back(0);
	} 
	else if(sec(e1, e2)) {
		sortedEvents.push_back(e2);
		sortedEvents.push_back(e1);
	} 
	else {
		sortedEvents.push_back(e1);
		sortedEvents.push_back(e2);
	}
	if(e1->other->p == e2->other->p) {
		sortedEvents.push_back(0);
	} 
	else if(sec(e1->other, e2->other)) {
		sortedEvents.push_back(e2->other);
		sortedEvents.push_back(e1->other);
	}
	else {
		sortedEvents.push_back(e1->other);
		sortedEvents.push_back(e2->other);
	}

	if(sortedEvents.size() == 2) { 
		e1->type = e1->other->type = NON_CONTRIBUTING;
		e2->type = e2->other->type =(e1->inOut == e2->inOut) ? SAME_TRANSITION : DIFFERENT_TRANSITION;
		return;
	}
	if(sortedEvents.size() == 3) { // the line segments share an endpoint
		sortedEvents[1]->type = sortedEvents[1]->other->type = NON_CONTRIBUTING;
		if(sortedEvents[0])         // is the right endpoint the shared point?
			sortedEvents[0]->other->type =(e1->inOut == e2->inOut) ? SAME_TRANSITION : DIFFERENT_TRANSITION;
		 else 								// the shared point is the left endpoint
			sortedEvents[2]->other->type =(e1->inOut == e2->inOut) ? SAME_TRANSITION : DIFFERENT_TRANSITION;
		divideSegment(sortedEvents[0] ? sortedEvents[0] : sortedEvents[2]->other, sortedEvents[1]->p);
		return;
	}
	if(sortedEvents[0] != sortedEvents[3]->other) { // no line segment includes totally the other one
		sortedEvents[1]->type = NON_CONTRIBUTING;
		sortedEvents[2]->type =(e1->inOut == e2->inOut) ? SAME_TRANSITION : DIFFERENT_TRANSITION;
		divideSegment(sortedEvents[0], sortedEvents[1]->p);
		divideSegment(sortedEvents[1], sortedEvents[2]->p);
		return;
	}
	 // one line segment includes the other one
	sortedEvents[1]->type = sortedEvents[1]->other->type = NON_CONTRIBUTING;
	divideSegment(sortedEvents[0], sortedEvents[1]->p);
	sortedEvents[3]->other->type =(e1->inOut == e2->inOut) ? SAME_TRANSITION : DIFFERENT_TRANSITION;
	divideSegment(sortedEvents[3]->other, sortedEvents[2]->p);
}
void Boolean_OP::divideSegment(SweepEvent* e, const Point& p)
{
	// "Right event" of the "left line segment" resulting from dividing e(the line segment associated to e)
	SweepEvent *r = storeSweepEvent(SweepEvent(p, false, e->pl, e, e->type));
	// "Left event" of the "right line segment" resulting from dividing e(the line segment associated to e)
	SweepEvent *l = storeSweepEvent(SweepEvent(p, true, e->pl, e->other, e->other->type));
	if(sec(l, e->other)) { // avoid a rounding error. The left event would be processed after the right event
		cout << "Oops" << endl;
		e->other->left = true;
		l->left = false;
	}
	if(sec(e, r)) { // avoid a rounding error. The left event would be processed after the right event
		cout << "Oops2" << endl;
//		cout << *e << endl;
	}
	e->other->other = l;
	e->other = r;
	eq.push(l);
	eq.push(r);
}

void Boolean_OP::joining_edge(){
	list<pair<list<Point>, bool>> openPolygons = {};
	// for(auto s : result_segment){		
	// 	cout << s.begin().x << " " << s.begin().y << endl;
	// 	cout << s.end().x << " " << s.end().y << endl;
	// 	cout << endl;
	// }
	for(auto s : result_segment){
		bool if_poly = false;
		auto j = openPolygons.begin();
		// if(j != openPolygons.end()){
		// 	cout << openPolygons.size() << endl;
		// 	for(auto c : j->first){
		// 		cout << c.x << " " << c.y << endl;
		// 	}
		// 	cout << endl;
		// }
		
		while(j != openPolygons.end()) {
			if(s.begin() == j->first.front()) {
				if_poly = true;
				if(s.end() == j->first.back())
					j->second = true;
				else
					j->first.push_front(s.end());
			}
			else if(s.end() == j->first.back()) {
				if_poly = true;
				if(s.begin() == j->first.front())
					j->second = true;
				else
					j->first.push_back(s.begin());
			}
			else if(s.end() == j->first.front()) {
				if_poly = true;
				if(s.begin() == j->first.back())
					j->second = true;
				else
					j->first.push_front(s.begin());
			}
			else if(s.begin() == j->first.back()) {
				if_poly = true;
				if(s.end() == j->first.front())
					j->second = true;
				else
					j->first.push_back(s.end());
			}
			if(j->second){
				closedPolygons.splice(closedPolygons.end(), openPolygons, j);
				break;
			}
			else{
				auto k = j;
				k++;
				for(; k != openPolygons.end(); k++) {
					if(k->first.front() == j->first.back()) {
						k->first.pop_front();
						j->first.splice(j->first.end(), k->first);
						openPolygons.erase(k);
						break;
					}
					if(k->first.back() == j->first.front()) {
						j->first.pop_front();
						j->first.splice(j->first.begin(), k->first);
						openPolygons.erase(k);
						break;
					}
					if(k->first.front() == j->first.front()) {
						j->first.pop_front();
						reverse(k->first.begin(), k->first.end());
						j->first.splice(j->first.begin(), k->first);
						openPolygons.erase(k);
						break;
					}
					if(k->first.back() == j->first.back()) {
						j->first.pop_back();
						reverse(k->first.begin(), k->first.end());
						j->first.splice(j->first.end(), k->first);
						openPolygons.erase(k);
						break;
					}
				}
			}
			j++;
		}
		if(!if_poly){
			list<Point> tmp = {};
			tmp.push_back(s.begin());
			tmp.push_back(s.end());
			openPolygons.push_back(make_pair(tmp,false));
		}
	}
}

void Boolean_OP::made_result(Polygon& p){
	for (auto it = closedPolygons.begin() ; it != closedPolygons.end() ; it++) {
		vector<Point> contour;
		for (auto it2 = it->first.begin(); it2 != it->first.end(); it2++)
			contour.push_back(*it2);
		p.contours.push_back(contour);
	}
}

PYBIND11_MODULE(_BOOLEAN_OP, m){
    pybind11::class_<Boolean_OP>(m, "Boolean_OP")
        .def(pybind11::init<Polygon , Polygon ,int >())
        .def("create_SweepEvent", &Boolean_OP::create_SweepEvent)
        .def("cutting_edge", &Boolean_OP::cutting_edge)
        .def("joining_edge", &Boolean_OP::joining_edge)
		.def("made_result", &Boolean_OP::made_result);
    pybind11::class_<Polygon>(m, "Polygon")
        .def(pybind11::init<vector<vector<pair<double, double>>>>())
		.def(pybind11::init<>())
		.def("get_polygons", &Polygon::get_polygons)
		.def("is_empty", &Polygon::is_empty);
	pybind11::class_<Point>(m, "Point")
		.def_readwrite("x", &Point:: x)
		.def_readwrite("y", &Point:: y);
}