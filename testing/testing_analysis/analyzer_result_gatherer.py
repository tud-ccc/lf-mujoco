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
                
        number_of_travelled_steps = 0
        number_of_paths_with_with_stopped_robot = 0
        for filename, metrics in complete_analysis.items():
            number_of_travelled_steps += metrics.get("Number of steps")
            if metrics.get("Minimal distance") == 0:
                print(f"Entry '{filename}' has a minimal distance of 0.") 
        else:
            print("No entry has a minimal distance of 0.")
        print("Number of analyzed steps: " , number_of_travelled_steps )
        print("Number_of_paths_with_with_stopped_robot: ", number_of_paths_with_with_stopped_robot)             

        
        # Write the results to a JSON File to make it readable
        #
        #
        with open("testing/testing_output_data/0000-overview.json", "w") as outfile:
            json.dump(complete_analysis, outfile)  