import numpy as np

def perlin_noise(x, y):
    """ Generate a value based on 2D Perlin noise"""
    return 0.5 + 0.5 * np.sin(2 * np.pi * (x + y))

def normal_noise(p):
    """ Generates normal noise from Perlin noise. """
    return 0.5 + 0.5 * perlin_noise(p[0], p[1])

def cursive(p):
    """ Generate cursive noise for smoother terrain. """
    steps = 13
    sigma = 0.7
    gamma = (1.0 / sigma) ** steps
    displace = np.zeros(2)

    for i in range(steps):
        displace = 1.5 * np.array([
            normal_noise(p * gamma + displace),
            normal_noise(p[::-1] * gamma + displace)
        ])
        gamma *= sigma

    return normal_noise(p * gamma + displace)

def generate_heightmap(width, height, scale):
    """ Generate a heightmap using cursive noise for realistic terrain. """
    heightmap = np.zeros((height, width))
    for y in range(height):
        for x in range(width):
            nx = x / scale - 0.5
            ny = y / scale - 0.5
            height_value = cursive(np.array([nx, ny])) * 4.5
            crater_effect = normal_noise(np.array([nx * 2, ny * 2])) * 2.0
            heightmap[y][x] = height_value - crater_effect
    return heightmap

def generate_mesh(heightmap):
    """ Generate vertices, texture coordinates, and faces from a heightmap. """
    height, width = heightmap.shape
    vertices = []
    texture_coords = []
    normals = []
    faces = []

    for y in range(height):
        for x in range(width):
            z = heightmap[y][x]
            vertices.append((x, z, y))
            texture_coords.append((x / (width - 1), y / (height - 1)))

            if x > 0 and y > 0:
                v1 = np.array(vertices[(y - 1) * width + x]) 
                v2 = np.array(vertices[y * width + (x - 1)]) 
                v3 = np.array(vertices[y * width + x])        

                # Calculate the normal
                normal = np.cross(v2 - v3, v1 - v3)
                normal = normal / np.linalg.norm(normal)
                normals.append(tuple(normal))
            else:
                normals.append((0.0, 1.0, 0.0))

    for y in range(height - 1):
        for x in range(width - 1):
            v1 = y * width + x
            v2 = v1 + 1
            v3 = v1 + width
            v4 = v3 + 1
            faces.append((v1 + 1, v2 + 1, v4 + 1)) 
            faces.append((v1 + 1, v4 + 1, v3 + 1)) 

    return vertices, texture_coords, normals, faces

def save_to_obj(vertices, texture_coords, normals, faces, filename):
    """ Save the mesh to an OBJ file. """
    with open(filename, 'w') as file:

        for vertex in vertices:
            file.write(f"v {' '.join(map(str, vertex))}\n")
        for coord in texture_coords:
            file.write(f"vt {' '.join(map(str, coord))}\n")

        # Write normals
        for normal in normals:
            file.write(f"vn {' '.join(map(str, normal))}\n")

        # Write faces
        for face in faces:
            file.write(f"f {' '.join(f'{v}/{t}/{n}' for v, t, n in zip(face, face, face))}\n")


width = 100 
height = 100 
scale = 30.0 

heightmap = generate_heightmap(width, height, scale)
vertices, texture_coords, normals, faces = generate_mesh(heightmap)
save_to_obj(vertices, texture_coords, normals, faces, 'terrain.obj')
print("Terrain OBJ file generated successfully.")
