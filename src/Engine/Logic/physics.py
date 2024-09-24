import glm

def check_collision(a_min, a_max, b_min, b_max):
    """Check if two AABBs overlap."""
    return (a_min.x <= b_max.x and a_max.x >= b_min.x and
            a_min.y <= b_max.y and a_max.y >= b_min.y and
            a_min.z <= b_max.z and a_max.z >= b_min.z)

def handle_collision(obj1, obj2):
    """Handle collision between two objects."""
    obj1.velocity = glm.vec3(0, 0, 0)
    obj2.velocity = glm.vec3(0, 0, 0)

def check_collisions(objects, camera=None):
    """
    Check collisions between all objects and optionally with the camera.
    If a camera is provided, check collisions between the camera and objects.
    """
    for i, obj1 in enumerate(objects):
        for j, obj2 in enumerate(objects):
            if i != j:
                obj1_min, obj1_max = obj1.get_aabb()
                obj2_min, obj2_max = obj2.get_aabb()
                if check_collision(obj1_min, obj1_max, obj2_min, obj2_max):
                    handle_collision(obj1, obj2)
    if camera:
        camera_min, camera_max = camera.get_aabb()
        for obj in objects:
            obj_min, obj_max = obj.get_aabb()
            if check_collision(camera_min, camera_max, obj_min, obj_max):
                camera.position -= camera.forward * camera.speed
