import pandas
from matplotlib import pyplot as plt
import numpy as np 
import math 
from enum import Enum  

import matplotlib.pyplot as plt
import numpy as np

def show_position_evaluator_data():
      
    ins = pandas.read_csv("data_analysis_csv/data/instructions.csv", sep = ',')    
    
    inst_time = ins['time'].to_list()


    current_position_0 = ins['current_position_0'].to_list()
    current_position_1 = ins['current_position_1'].to_list()
    current_position_2 = ins['current_position_2'].to_list()   
    
    raw_instruction_0 = ins['raw_instruction_0'].to_list()
    raw_instruction_1 = ins['raw_instruction_1'].to_list()
    raw_instruction_2 = ins['raw_instruction_2'].to_list()   
    

    checked_instruction_0 = ins['checked_instruction_0'].to_list()
    checked_instruction_1 = ins['checked_instruction_1'].to_list()
    checked_instruction_2 = ins['checked_instruction_2'].to_list()   

    figure, axis = plt.subplots(2, 2) 
    
    axis[0, 0].plot(inst_time, current_position_0, 'b') 
    axis[0, 0].plot(inst_time, raw_instruction_0, 'r')
    axis[0, 0].plot(inst_time, checked_instruction_0, 'g')
    axis[0, 0].set_title("X__ Blue : current pos, Red : raw pos, Green : checked pos") 
    
    axis[0, 1].plot(inst_time, current_position_1, 'b') 
    axis[0, 1].plot(inst_time, raw_instruction_1, 'r')
    axis[0, 1].plot(inst_time, checked_instruction_1, 'g')
    axis[0, 0].set_title("Y__ Blue : current pos, Red : raw pos, Green : checked pos") 

    axis[1, 0].plot(inst_time, current_position_2, 'b') 
    axis[1, 0].plot(inst_time, raw_instruction_2, 'r')
    axis[1, 0].plot(inst_time, checked_instruction_2, 'g')
    axis[0, 0].set_title("Z__ Blue : current pos, Red : raw pos, Green : checked pos") 
    
    
    
    plt.show() 


def show_points_graphically():


    ax = plt.axes(projection='3d')

    ins = pandas.read_csv("data_analysis_csv/data/instructions.csv", sep = ',')    

    current_position_0 = ins['current_position_0'].to_list()
    current_position_1 = ins['current_position_1'].to_list()
    current_position_2 = ins['current_position_2'].to_list()   

    x = current_position_0
    y = current_position_1
    z = current_position_2

    ax.scatter3D(x, y, z,color = "r") 
    ax.set_zlim(0,300)

    plt.show()

def main():

    # show_position_evaluator_data()
    show_points_graphically()
   
if __name__ == "__main__":
    main()
