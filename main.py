import matplotlib.pyplot as plt
from matplotlib.patches import Polygon
from _BOOLEAN_OP import Boolean_OP, Polygon as P
from matplotlib.path import Path

def is_inner_polygon(inner, outer):
    outer_path = Path(outer)
    return all(outer_path.contains_point(point) for point in inner)

current_polygon = []
operation = None
with open('input.txt', 'r') as file:
    content = file.read()
    lines = content.strip().split('\n')
    for line in lines:
        line = line.strip()
        if not line:
            continue
        if line.startswith('P1'):
            continue
        if line.startswith('P2'):
            if current_polygon:
                poly1 = current_polygon
                current_polygon = []
                
        elif line.startswith('OP'):
            operation = line.split(' ')[1]
        else:
            points = line.strip('()').split('),(')
            contour = [(float(p.split(',')[0]), float(p.split(',')[1])) for p in points]
            current_polygon.append(contour)

poly2 = current_polygon


p1 = P(poly1)
p2 = P(poly2)

if operation == 'union':
    op = 0
elif operation == 'intersection':
    op = 1
elif operation == 'difference':
    op = 2
else:
    op = 3

result = P()
BOP = Boolean_OP(p1, p2, op)
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