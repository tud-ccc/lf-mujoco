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
    
    def get_list_of_angles_annotated_with_distance(self):
        angle_distance_list = list()
        for i in range(0, self.get_vector_list_len()-2):
            vec_1 = self.vector_arithmetics.two_vectors_delta_vector(self.vec_list[i],self.vec_list[i+1])
            vec_2 = self.vector_arithmetics.two_vectors_delta_vector(self.vec_list[i+1],self.vec_list[i+2])
            angle =  self.vector_arithmetics.two_vectors_angle(vec_1,vec_2)
            current_distance = vec_1.get_arithmetic_length()
            angle_distance_list.append((angle, current_distance))        
        return angle_distance_list
    
    def get_max_angle_matched_to_distance(self):        
        if not self.list_of_vec_distances: 
            dict= {}
            dict[0] = float('nan') 
            dict[1] = float('nan') 
            dict[2] = float('nan') 
            dict[3] = float('nan')
            return dict

        angle_to_distance = self.get_list_of_angles_annotated_with_distance()
        one_fourth_of_max_distance = self.get_max_distance()/4
        
        small_distances_cap = one_fourth_of_max_distance*1
        small_middle_distances_cap = one_fourth_of_max_distance*2
        huge_middle_distances_cap = one_fourth_of_max_distance*3
        huge_distances_cap = one_fourth_of_max_distance*4

        small_distances = [(angle,dis) for (angle,dis) in angle_to_distance if dis <= small_distances_cap ]
        small_middle_distances = [(angle,dis) for (angle,dis) in angle_to_distance if dis <= small_middle_distances_cap and dis > small_distances_cap ]
        huge_middle_distances = [(angle,dis) for (angle,dis) in angle_to_distance if dis <= huge_middle_distances_cap and dis > small_middle_distances_cap]
        huge_distances = [(angle,dis) for (angle,dis) in angle_to_distance if dis <=  huge_distances_cap and dis > huge_middle_distances_cap]

        dict = {}
        dict[small_distances_cap] = max([angle for (angle, dis) in small_distances]) if small_distances else None
        dict[small_middle_distances_cap] = max([angle for  (angle, dis) in small_middle_distances]) if small_middle_distances else None
        dict[huge_middle_distances_cap] = max([angle for  (angle, dis) in huge_middle_distances]) if huge_middle_distances else None
        dict[huge_distances_cap] =max([angle for  (angle, dis) in huge_distances]) if huge_distances else None
        
        return dict

    def get_vector_list_len(self):
        return len(self.vec_list)
    
    def print_vector_list(self):
        for elem in self.vec_list:
            print(elem)
        
    def get_max_distance(self):
        return max(self.list_of_vec_distances) if self.list_of_vec_distances else float('nan')
    def get_min_distance(self):
        return min(self.list_of_vec_distances) if self.list_of_vec_distances else float('nan')
    def get_mean_distance(self):
        return float(sum(self.list_of_vec_distances)) / max(len(self.list_of_vec_distances), 1) if self.list_of_vec_distances else float('nan')
    
    def get_max_angle(self):
        return max(self.list_of_angles_between_vectors) if self.list_of_angles_between_vectors else float('nan')   
    def get_min_angle(self):
        return min(self.list_of_angles_between_vectors) if self.list_of_angles_between_vectors else float('nan')
    def get_mean_angle(self):
        return float(sum(self.list_of_angles_between_vectors)) / max(len(self.list_of_angles_between_vectors), 1) if self.list_of_angles_between_vectors else float('nan')
    