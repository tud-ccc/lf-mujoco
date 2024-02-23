import pandas
from matplotlib import pyplot as plt
import numpy as np 
import math 
from enum import Enum  

class ColoumnSpecifier(Enum):
    ROBO_JOINT_ANGLES = 0
    ROBO_JOINT_VELOCITY = 1
    ROBO_JOINT_EFFORT = 2
    ROBO_POSE = 3
  
def print_data(what_to_print):
    
    enum_to_col_specs = {}
    enum_to_col_specs[ColoumnSpecifier.ROBO_JOINT_ANGLES] = ['robo_joint_angles_0','robo_joint_angles_1','robo_joint_angles_2','robo_joint_angles_3','robo_joint_angles_4','robo_joint_angles_5']
    enum_to_col_specs[ColoumnSpecifier.ROBO_JOINT_VELOCITY] = ['robo_joint_velocity_0','robo_joint_velocity_1','robo_joint_velocity_2','robo_joint_velocity_3','robo_joint_velocity_4','robo_joint_velocity_5']
    enum_to_col_specs[ColoumnSpecifier.ROBO_JOINT_EFFORT] = ['robo_joint_effort_0','robo_joint_effort_1','robo_joint_effort_2','robo_joint_effort_3','robo_joint_effort_4','robo_joint_effort_5']
    enum_to_col_specs[ColoumnSpecifier.ROBO_POSE] = ['robo_pose_0','robo_pose_1','robo_pose_2','robo_pose_3','robo_pose_4','robo_pose_5']
    
    
    df = pandas.read_csv("data.csv", sep = ',',)
    

    time = df['time'].to_list()
    jvel0 = df[enum_to_col_specs[what_to_print][0]].to_list()
    jvel1 = df[enum_to_col_specs[what_to_print][1]].to_list()
    jvel2 = df[enum_to_col_specs[what_to_print][2]].to_list()
    jvel3 = df[enum_to_col_specs[what_to_print][3]].to_list()
    jvel4 = df[enum_to_col_specs[what_to_print][4]].to_list()
    jvel5 = df[enum_to_col_specs[what_to_print][5]].to_list()
    
    ins = pandas.read_csv("instructions.csv", sep = ',',)
    
    
    inst_time = ins['time'].to_list()
    inst_pos_0 = ins['pe_adjusted_position_0'].to_list()
    inst_pos_1 = ins['pe_adjusted_position_1'].to_list()
    inst_pos_2 = ins['pe_adjusted_position_2'].to_list()
    inst_pos_3 = ins['pe_adjusted_position_3'].to_list()
    inst_pos_4 = ins['pe_adjusted_position_4'].to_list()
    inst_pos_5 = ins['pe_adjusted_position_5'].to_list()
    
    
    
    # Get the angles from 0 to 2 pie (360 degree) in narray object 
    X = time 
    inst_X= inst_time
    
    
    # Using built-in trigonometric function we can directly plot 
    # the given cosine wave for the given angles 
    Y1 = jvel0
    Y2 = jvel1 
    Y3 = jvel2
    Y4 = jvel3
    Y5 = jvel4
    Y6 = jvel5
    
    inst_Y0= inst_pos_0
    inst_Y1= inst_pos_1 
    inst_Y2= inst_pos_2
    inst_Y3= inst_pos_3
    inst_Y4= inst_pos_4 
    inst_Y5= inst_pos_5
     
    figure, axis = plt.subplots(3, 2) 
    
    axis[0, 0].plot(X, Y1) 
    if what_to_print is ColoumnSpecifier.ROBO_POSE : axis[0, 0].plot(inst_X, inst_Y0, 'r')
    axis[0, 0].set_title(enum_to_col_specs[what_to_print][0]) 
    
    axis[0, 1].plot(X, Y2) 
    if what_to_print is ColoumnSpecifier.ROBO_POSE : axis[0, 1].plot(inst_X, inst_Y1, 'r')
    axis[0, 1].set_title(enum_to_col_specs[what_to_print][1]) 

    axis[1, 0].plot(X, Y3) 
    if what_to_print is ColoumnSpecifier.ROBO_POSE : axis[1, 0].plot(inst_X, inst_Y2, 'r')
    axis[1, 0].set_title(enum_to_col_specs[what_to_print][2]) 
    
    axis[1, 1].plot(X, Y4)
    if what_to_print is ColoumnSpecifier.ROBO_POSE :axis[1, 1].plot(inst_X, inst_Y3, 'r')
    axis[1, 1].set_title(enum_to_col_specs[what_to_print][3]) 
    
    axis[2, 0].plot(X, Y5) 
    if what_to_print is ColoumnSpecifier.ROBO_POSE : axis[2, 0].plot(inst_X, inst_Y4, 'r')
    axis[2, 0].set_title(enum_to_col_specs[what_to_print][4]) 
    
    axis[2, 1].plot(X, Y6) 
    if what_to_print is ColoumnSpecifier.ROBO_POSE : axis[2, 1].plot(inst_X, inst_Y5, 'r')
    axis[2, 1].set_title(enum_to_col_specs[what_to_print][5]) 
    
    
    # Combine all the operations and display 
    plt.show() 


def show_position_evaluator_data():
      
    ins = pandas.read_csv("instructions.csv", sep = ',',)    
    
    inst_time = ins['time'].to_list()


    pe_last_position_0 = ins['pe_last_position_0'].to_list()
    pe_last_position_1 = ins['pe_last_position_1'].to_list()
    pe_last_position_2 = ins['pe_last_position_2'].to_list()   
    
    pe_raw_position_0 = ins['pe_raw_position_0'].to_list()
    pe_raw_position_1 = ins['pe_raw_position_1'].to_list()
    pe_raw_position_2 = ins['pe_raw_position_2'].to_list()   
    

    pe_adjusted_position_0 = ins['pe_adjusted_position_0'].to_list()
    pe_adjusted_position_1 = ins['pe_adjusted_position_1'].to_list()
    pe_adjusted_position_2 = ins['pe_adjusted_position_2'].to_list()   

    figure, axis = plt.subplots(2, 2) 
    
    axis[0, 0].plot(inst_time, pe_last_position_0) 
    axis[0, 0].plot(inst_time, pe_raw_position_0, 'r')
    axis[0, 0].plot(inst_time, pe_adjusted_position_0, 'g')
    axis[0, 0].set_title("X__ Blue : last_pos, Red : raw_pos, Green : adjusted_pos") 
    
    axis[0, 1].plot(inst_time, pe_last_position_1) 
    axis[0, 1].plot(inst_time, pe_raw_position_1, 'r')
    axis[0, 1].plot(inst_time, pe_adjusted_position_1, 'g')
    axis[0, 1].set_title("Y__ Blue : last_pos, Red : raw_pos, Green : adjusted_pos") 

    axis[1, 0].plot(inst_time, pe_last_position_2) 
    axis[1, 0].plot(inst_time, pe_raw_position_2, 'r')
    axis[1, 0].plot(inst_time, pe_adjusted_position_2, 'g')
    axis[1, 0].set_title("Z__ Blue : last_pos, Red : raw_pos, Green : adjusted_pos") 
    
    plt.show() 




def sim_sensor_data():
    print("This is yet to be implemented !")


import sys

def main():

    show_position_evaluator_data()
    
    args = (sys.argv)
    if args[1] == 'robo':
        print_data(ColoumnSpecifier.ROBO_JOINT_ANGLES)
        print_data(ColoumnSpecifier.ROBO_JOINT_VELOCITY)
        print_data(ColoumnSpecifier.ROBO_JOINT_EFFORT)
        print_data(ColoumnSpecifier.ROBO_POSE)
               
    elif args[1] == 'simulator':
        print("This is yet to be implemented !")
    else:
        print("This is yet to be implemented !")

        
    
    
    
if __name__ == "__main__":
    main()
