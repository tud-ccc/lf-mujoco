import sys
sys.path.append('/home/benedict/Bibliothek/7.Semester/BachelorArbeit/Lf_projects/mujoco_project/data_analysis/one_trajectory_analysis')
from analyzer_result_gatherer import AnalyzerResultGatherer 
from analysis_interface import AnalysisInterface

def analyse_all_files():
    arg = AnalyzerResultGatherer("testing/testing_output_data/")
    arg.analyse_all_files()

def analyse_the_only_single_instruction_file():
    interface = AnalysisInterface("testing/testing_output_data/instructions_054.csv")
    # interface.combine_analysis_print()
    interface.show_points_of_vector_list_graphically_annotated()
    interface.get_max_angle_matched_to_distance()

def main():  

    analyse_the_only_single_instruction_file()


if __name__ == "__main__":
    main()

