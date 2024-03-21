import math


class Vector:
    def __init__(self, X, Y, Z):
        self.X = X 
        self.Y = Y
        self.Z = Z
    
    def __str__(self):
        return f'X: {self.X}, Y: {self.Y}, Z: {self.Z}'

    def __repr__(self):
        return f'X: {self.X}, Y: {self.Y}, Z: {self.Z}'
            
    def get_arithmetic_length(self):
        return math.sqrt(self.X**2 + self.Y**2 + self.Z**2)

class VectorArithmentics():
    
    def two_vectors_delta_vector(self, vec1, vec2):
        return Vector(vec2.X - vec1.X , vec2.Y- vec1.Y , vec2.Z - vec1.Z)
    
    def two_vectors_distance(self, vec1, vec2):
        distance_vec = self.two_vectors_delta_vector(vec1, vec2)
        return distance_vec.get_arithmetic_length()
    
    def two_vectors_angle(self, vec1, vec2):
        scalar_prod = self.two_vectors_dot_product(vec1, vec2)
        l1 = vec1.get_arithmetic_length()
        l2 = vec2.get_arithmetic_length()
        if(l1*l2 == 0):
            return 0
        if scalar_prod / (l1*l2) > 1:
            print("Value bigger than 1, error out !")
            print("" + str(scalar_prod / (l1*l2)))
            print(vec1)
            print(vec2)
            return 0
        angle_in_rad = math.acos(scalar_prod / (l1*l2))
        return math.degrees(angle_in_rad)
    
    def two_vectors_dot_product(self, vec1, vec2):
        return vec1.X * vec2.X + vec1.Y * vec2.Y + vec1.Z * vec2.Z
    
