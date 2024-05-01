

from one_trajectory_analysis.analysis_interface import AnalysisInterface




  
def analyse_the_only_single_instruction_file():
    interface = AnalysisInterface("data_analysis/data_testing/instructions.csv")
    interface.combine_analysis_print()
    interface.show_points_of_vector_list_graphically_annotated()

def main():  
    analyse_the_only_single_instruction_file()

if __name__ == "__main__":
    main()

