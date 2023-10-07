import matplotlib.pyplot as plt

with open("PolygonPoints/res.txt", "r") as file:
    n = int(file.readline().strip()) 
    polygon_vertices = [tuple(map(int, file.readline().strip().split())) for _ in range(n)]

    points = []  
    for line in file:
        x, y = map(int, line.strip().split())
        points.append((x, y))

polygon_x = [vertex[0] for vertex in polygon_vertices]
polygon_y = [vertex[1] for vertex in polygon_vertices]

points_x = [point[0] for point in points]
points_y = [point[1] for point in points]

plt.figure(figsize=(8, 6))
plt.plot(polygon_x + [polygon_x[0]], polygon_y + [polygon_y[0]], marker='o', linestyle='-', color='b', label='Polygon')
plt.scatter(points_x, points_y, color='r', label='Points')

plt.xlabel('X-axis')
plt.ylabel('Y-axis')
plt.title('Convex Polygon and Points')
plt.legend()

plt.grid()
plt.show()
