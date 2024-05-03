from analysis_interface import AnalysisInterface
import os
import json



class AnalyzerResultGatherer:
    def __init__(self, trajectories_file_path):
        self.trajectories_file_path = trajectories_file_path
        
    def analyse_all_files(self):
        # The directory
        #
        #
        directory = os.fsencode(self.trajectories_file_path) 
        
        # Get the analysis form every single file
        #
        #
        complete_analysis = {}
        for file in sorted(os.listdir(directory)):
            filename = os.fsdecode(file)
            if filename.endswith(".csv"): 
                interface = AnalysisInterface(self.trajectories_file_path + filename)
                complete_analysis[filename] = interface.dump_into_dict()

        print("#### All trajectories have been analyzed #####")
        
        # Write the results to a JSON File to make it readable
        #
        #
        with open("testing/testing_output_data/0000-overview.json", "w") as outfile:
            json.dump(complete_analysis, outfile)  