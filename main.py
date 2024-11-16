import matplotlib.pyplot as plt
from matplotlib.patches import Polygon
from _BOOLEAN_OP import Boolean_OP, Polygon as P
from matplotlib.path import Path

def is_inner_polygon(inner, outer):
    outer_path = Path(outer)
    return all(outer_path.contains_point(point) for point in inner)

p1 = P([[(2.0,3.0),(8.0,3.0),(8.0,7.0),(2.0,7.0)],[(3.0,4.0),(5.0,4.0),(5.0,6.0),(3.0,6.0)]])
p2 = P([[(4.0,1.0),(7.0,1.0),(7.0,5.0),(4.0,5.0)]])

result = P()
BOP = Boolean_OP(p1, p2, 0)
BOP.create_SweepEvent()
BOP.cutting_edge()
BOP.joining_edge()
BOP.made_result(result)

fig, ax = plt.subplots()
# 創建多邊形對象
InOut = []
vertices = []
for k in result.get_polygons():
    tmp = []
    for i in k:
        tmp.append((i.x, i.y))
    vertices.append(tmp)
    InOut.append(True)
    # print(tmp)



for i, poly1 in enumerate(vertices):
    for j, poly2 in enumerate(vertices):
        if i != j and is_inner_polygon(poly1, poly2):
            if InOut[i] == True:
                InOut[i] = False
            else:
                InOut[i] = True
    # print(InOut[i])

for i, poly in enumerate(vertices):
    if InOut[i] == True:
        polygon = Polygon(poly, closed=True, edgecolor='blue', facecolor='blue', zorder=1)
    else:
        polygon = Polygon(poly, closed=True, edgecolor='white', facecolor='white', zorder=2)
    ax.add_patch(polygon)


ax.add_patch(polygon)
# 設置軸的範圍
ax.set_xlim(0, 10)
ax.set_ylim(0, 10)

# 顯示圖形
plt.gca().set_aspect('equal', adjustable='box')  # 設置等比例
plt.show()