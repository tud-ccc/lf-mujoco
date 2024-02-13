import pandas
from matplotlib import pyplot as plt
import numpy as np 
import math 
  

  
def print_data(coulumn_specifiers):
    
    df = pandas.read_csv("data.csv", sep = ',',)


    time = df['time'].to_list()
    jvel0 = df[coulumn_specifiers[0]].to_list()
    jvel1 = df[coulumn_specifiers[1]].to_list()
    jvel2 = df[coulumn_specifiers[2]].to_list()
    jvel3 = df[coulumn_specifiers[3]].to_list()
    jvel4 = df[coulumn_specifiers[4]].to_list()
    jvel5 = df[coulumn_specifiers[5]].to_list()
    
    # Get the angles from 0 to 2 pie (360 degree) in narray object 
    X = time 
    
    # Using built-in trigonometric function we can directly plot 
    # the given cosine wave for the given angles 
    Y1 = jvel0
    Y2 = jvel1 
    Y3 = jvel2
    Y4 = jvel3
    Y5 = jvel4
    Y6 = jvel5
    
    #print(Y1)
    
    # Initialise the subplot function using number of rows and columns 
    figure, axis = plt.subplots(3, 2) 
    
    # For Sine Function 
    axis[0, 0].plot(X, Y1) 
    axis[0, 0].set_title(coulumn_specifiers[0]) 
    
    # For Cosine Function 
    axis[0, 1].plot(X, Y2) 
    axis[0, 1].set_title(coulumn_specifiers[1]) 
    
    # For Tangent Function 
    axis[1, 0].plot(X, Y3) 
    axis[1, 0].set_title(coulumn_specifiers[2]) 
    
    # For Tanh Function 
    axis[1, 1].plot(X, Y4) 
    axis[1, 1].set_title(coulumn_specifiers[3]) 
    
    axis[2, 0].plot(X, Y5) 
    axis[2, 0].set_title(coulumn_specifiers[4]) 
    
    axis[2, 1].plot(X, Y6) 
    axis[2, 1].set_title(coulumn_specifiers[5]) 
    
    
    # Combine all the operations and display 
    plt.show() 



def sim_sensor_data():
    print("This is yet to be implemented !")


import sys

def main():
    
    args = (sys.argv)
    if args[1] == 'robo':
        print_data(['robo_joint_angles_0','robo_joint_angles_1','robo_joint_angles_2','robo_joint_angles_3','robo_joint_angles_4','robo_joint_angles_5'])
        print_data(['robo_joint_velocity_0','robo_joint_velocity_1','robo_joint_velocity_2','robo_joint_velocity_3','robo_joint_velocity_4','robo_joint_velocity_5'])
        print_data(['robo_joint_effort_0','robo_joint_effort_1','robo_joint_effort_2','robo_joint_effort_3','robo_joint_effort_4','robo_joint_effort_5'])
        print_data(['robo_pose_0','robo_pose_1','robo_pose_2','robo_pose_3','robo_pose_4','robo_pose_5'])
               
    elif args[1] == 'simulator':
        print("This is yet to be implemented !")
    else:
        print("This is yet to be implemented !")

        
    
    
    
if __name__ == "__main__":
    main()
