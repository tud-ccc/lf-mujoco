import sys
sys.path.append('/home/benedict/Bibliothek/7.Semester/BachelorArbeit/Lf_projects/mujoco_project/data_analysis/one_trajectory_analysis')
from analysis_interface import AnalysisInterface




  
def analyse_the_only_single_instruction_file():
    interface = AnalysisInterface("data_analysis/data/instructions.csv")
    interface.combine_analysis_print()
    # interface.show_points_of_vector_list_graphically()
    interface.get_max_angle_matched_to_distance()

def main():  
    analyse_the_only_single_instruction_file()

if __name__ == "__main__":
    main()

