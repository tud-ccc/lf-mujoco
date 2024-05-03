
import pandas


from analyzer import Analyzer
from plotter_and_parser import Plotter, Parser

class AnalysisInterface:
    def __init__(self, instruction_file_path):
        self.instruction_file_path = instruction_file_path
        self.instructions_csv = pandas.read_csv(instruction_file_path, sep = ',')    

        self.parser = Parser(self.instructions_csv)
        self.analyzer = Analyzer(self.parser.parse_csv_to_vectors_of_points())
        self.plotter = Plotter(self.instructions_csv)
    
    def get_list_of_distances_between_vectors(self):
        return self.analyzer.get_list_of_distances_between_vectors()
    def get_list_of_angles_between_vectors(self):
        return self.analyzer.get_list_of_angles_between_vectors()
    
    def get_vector_list_len(self):
        return self.analyzer.get_vector_list_len()
    
    def print_vector_list(self):
        return self.analyzer.print_vector_list()
        
    def get_max_distance(self):
        return self.analyzer.get_max_distance()
    def get_min_distance(self):
        return self.analyzer.get_min_distance() 
    def get_mean_distance(self):
        return self.analyzer.get_mean_distance()
    
    def get_max_angle(self):
        return self.analyzer.get_max_angle()
    def get_min_angle(self):
        return self.analyzer.get_min_angle()
    def get_mean_angle(self):
        return self.analyzer.get_mean_angle()
    
    def show_X_Y_Z_diagrams(self):
        return self.plotter.show_X_Y_Z_diagrams()    

    def show_points_of_vector_list_graphically(self):
        return self.plotter.show_points_of_vector_list_graphically(self.parser.parse_csv_to_vectors_of_points())

    def show_points_of_vector_list_graphically_annotated(self):
        return self.plotter.show_points_of_vector_list_graphically_annotated(self.parser.parse_csv_to_vectors_of_points())

    def get_number_of_single_steps(self):
        return self.analyzer.get_vector_list_len()
       
    def get_max_angle_matched_to_distance(self):
        return self.analyzer.get_max_angle_matched_to_distance()
    

    def combine_analysis_print(self):
        print(f"Minimal distance : {self.get_min_distance()}")
        print(f"Maximum distance : {self.get_max_distance()}")
        print(f"Mean distance : {self.get_mean_distance()}")
        print(f"Minimal angle : {self.get_min_angle()}")
        print(f"Maximum angle : {self.get_max_angle()}")
        print(f"Mean angle : {self.get_mean_angle()}")
        
    
    def dump_into_dict(self):
        max_angle_match_to_dis_dict = self.get_max_angle_matched_to_distance()
        list_of_keys = list(max_angle_match_to_dis_dict.keys())
        assert len(list_of_keys) == 4, "The dictionary does not have the correct size"
        dumped_information = {    
        "Minimal distance": self.get_min_distance(),
        "Maximum distance": self.get_max_distance(),
        "Mean distance": self.get_mean_distance(),
        "Minimal angle":  self.get_min_angle(),
        "Maximum angle": self.get_max_angle(),
        "Mean angle":self.get_mean_angle(),
        "Number of steps" : self.get_number_of_single_steps(),
        str(list_of_keys[0]) : max_angle_match_to_dis_dict[list_of_keys[0]],
        str(list_of_keys[1]) : max_angle_match_to_dis_dict[list_of_keys[1]],
        str(list_of_keys[2]) : max_angle_match_to_dis_dict[list_of_keys[2]],
        str(list_of_keys[3]) : max_angle_match_to_dis_dict[list_of_keys[3]]
        }
        return dumped_information
 