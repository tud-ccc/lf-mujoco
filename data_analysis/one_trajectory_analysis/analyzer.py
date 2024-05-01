from vector import VectorArithmentics


class Analyzer:
    def __init__(self, vec_list):
            
        self.vec_list = vec_list
        self.vector_arithmetics = VectorArithmentics()
        
        self.list_of_vec_distances = self.get_list_of_distances_between_vectors()
        self.list_of_angles_between_vectors = self.get_list_of_angles_between_vectors()
        

    def get_list_of_distances_between_vectors(self):
        distance_list = list()
        for i in range(0, self.get_vector_list_len()-1):
            distance =  self.vector_arithmetics.two_vectors_distance(self.vec_list[i], self.vec_list[i+1])
            distance_list.append(distance)
        
        return distance_list

    def get_list_of_angles_between_vectors(self):
        angle_list = list()
        for i in range(0, self.get_vector_list_len()-2):
            angle =  self.vector_arithmetics.two_vectors_angle(self.vector_arithmetics.two_vectors_delta_vector(self.vec_list[i],self.vec_list[i+1]), self.vector_arithmetics.two_vectors_delta_vector(self.vec_list[i+1],self.vec_list[i+2]))
            angle_list.append(angle)
        
        return angle_list
    

    def get_vector_list_len(self):
        return len(self.vec_list)
    
    def print_vector_list(self):
        for elem in self.vec_list:
            print(elem)
        
    def get_max_distance(self):
        return max(self.list_of_vec_distances) 
    def get_min_distance(self):
        return min(self.list_of_vec_distances) 
    def get_mean_distance(self):
        return float(sum(self.list_of_vec_distances)) / max(len(self.list_of_vec_distances), 1)    
    
    def get_max_angle(self):
        return max(self.list_of_angles_between_vectors)    
    def get_min_angle(self):
        return min(self.list_of_angles_between_vectors)
    def get_mean_angle(self):
        return float(sum(self.list_of_angles_between_vectors)) / max(len(self.list_of_angles_between_vectors), 1)
    