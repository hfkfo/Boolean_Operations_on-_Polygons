from _BOOLEAN_OP import Polygon, Boolean_OP
import pytest


p1 = Polygon([[(4.0, 3.0), (9.0, 7.0), (0.0, 6.0)]])
p2 = Polygon([[(4.0, 3.0), (0.0, 6.0), (9.0, 7.0)]])

p3 = Polygon([[(2.0,3.0),(8.0,3.0),(8.0,7.0),(2.0,7.0)],[(3.0,4.0),(5.0,4.0),(5.0,6.0),(3.0,6.0)]])
p4 = Polygon([[(4.0,1.0),(7.0,1.0),(7.0,5.0),(4.0,5.0)]])
p5 = Polygon([[(2.0, 7.0), (2.0, 3.0), (4.0, 3.0), (4.0, 1.0), (7.0, 1.0), (7.0, 3.0), (8.0, 3.0), (8.0, 7.0)],
              [(3.0, 6.0), (3.0, 4.0), (4.0, 4.0), (4.0, 5.0), (5.0, 5.0), (5.0, 6.0)]])

p6 = Polygon([[(4.0,1.0),(7.0,1.0),(7.0,5.0),(4.0,5.0)]])
p7 = Polygon([[(1.0,1.0),(4.0,1.0),(4.0,5.0),(1.0,5.0)]])
p8 = Polygon([[(4.0,5.0), (1.0,5.0), (1.0,1.0), (4.0,1.0), (7.0,1.0), (7.0,5.0)]])

def test_zero():
    result = Polygon()
    BOP = Boolean_OP(p1, p2, 3)
    BOP.create_SweepEvent()
    BOP.cutting_edge()
    BOP.joining_edge()
    BOP.made_result(result)
    assert result.is_empty()

def test_equal():
    result = Polygon()
    BOP = Boolean_OP(p3, p4, 0)
    BOP.create_SweepEvent()
    BOP.cutting_edge()
    BOP.joining_edge()
    BOP.made_result(result)
    assert result == p5
    
def test_merge():
    result = Polygon()
    BOP = Boolean_OP(p6, p7, 0)
    BOP.create_SweepEvent()
    BOP.cutting_edge()
    BOP.joining_edge()
    BOP.made_result(result)
    assert result == p8