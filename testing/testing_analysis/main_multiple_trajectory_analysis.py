import sys
sys.path.append('/home/benedict/Bibliothek/7.Semester/BachelorArbeit/Lf_projects/mujoco_project/data_analysis/one_trajectory_analysis')
from analyzer_result_gatherer import AnalyzerResultGatherer 

def main():  
    arg = AnalyzerResultGatherer("testing/testing_output_data/")
    arg.analyse_all_files()

if __name__ == "__main__":
    main()

