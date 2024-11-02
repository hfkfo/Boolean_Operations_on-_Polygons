from _BOOLEAN_OP import Polygon, Boolean_OP
import pytest


p1 = Polygon([[(4.0, 3.0), (9.0, 7.0), (0.0, 6.0)]])
p2 = Polygon([[(4.0, 3.0), (9.0, 7.0), (0.0, 6.0)]])

def test_zero():
    result = Polygon()
    BOP = Boolean_OP(p1, p2, 3)
    BOP.create_SweepEvent()
    BOP.cutting_edge()
    BOP.joining_edge()
    BOP.made_result(result)
    assert result.is_empty()
    
