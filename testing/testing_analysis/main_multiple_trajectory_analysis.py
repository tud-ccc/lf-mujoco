import sys
sys.path.append('/home/benedict/Bibliothek/7.Semester/BachelorArbeit/Lf_projects/mujoco_project/data_analysis/one_trajectory_analysis')
from analyzer_result_gatherer import AnalyzerResultGatherer 
from analysis_interface import AnalysisInterface

def analyse_all_files():
    arg = AnalyzerResultGatherer("testing/testing_output_data/")
    arg.analyse_all_files()

def analyse_all_files_and_then_one_by_one():
    
    arg = AnalyzerResultGatherer("testing/testing_output_data/")
    arg.analyse_all_files()

    base = "testing/testing_output_data/instructions_"
    padding = ""
    for i in range(120):
        if (i >= 0):
            padding = "00";

        if (i >= 10):
            padding = "0";

        if (i >= 100):
            padding = "";

        name = base + padding + str(i) +".csv"
        interface = AnalysisInterface(name)
        # interface.combine_analysis_print()
        interface.show_points_of_vector_list_graphically_annotated()
        interface.get_max_angle_matched_to_distance()

def analyse_one_file(path):

    arg = AnalyzerResultGatherer("testing/testing_output_data/")
    arg.analyse_all_files()

    interface = AnalysisInterface(path)

    interface.show_points_of_vector_list_graphically_annotated()
    interface.get_max_angle_matched_to_distance()

    return


def main():  
    # analyse_one_file("testing/testing_output_data/instructions_000.csv")
    analyse_all_files_and_then_one_by_one()


if __name__ == "__main__":
    main()

