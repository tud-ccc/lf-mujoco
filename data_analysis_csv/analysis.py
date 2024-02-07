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


def robo_joint_angles():
    df = pandas.read_csv("data.csv", sep = ',',)
    #print(df)
    #df.head()

    time = df['time'].to_list()
    jangle0 = (df['robo_joint_angles_0']).to_list()
    jangle1 = df['robo_joint_angles_1'].to_list()
    jangle2 = df['robo_joint_angles_2'].to_list()
    jangle3 = df['robo_joint_angles_3'].to_list()
    jangle4 = df['robo_joint_angles_4'].to_list()
    jangle5 = df['robo_joint_angles_5'].to_list()
    

    #plt.xlabel('Time', fontsize = 18)
    #plt.ylabel('Jangle=', fontsize = 16)
    #plt.scatter(time, jangle0)

    # Get the angles from 0 to 2 pie (360 degree) in narray object 
    X = time 
    
    # Using built-in trigonometric function we can directly plot 
    # the given cosine wave for the given angles 
    Y1 = jangle0
    Y2 = jangle1 
    Y3 = jangle2
    Y4 = jangle3
    Y5 = jangle4
    Y6 = jangle5
    
    #print(Y1)
    
    # Initialise the subplot function using number of rows and columns 
    figure, axis = plt.subplots(3, 2) 
    
    # For Sine Function 
    axis[0, 0].plot(X, Y1) 
    axis[0, 0].set_title("Joint Angles 0") 
    
    # For Cosine Function 
    axis[0, 1].plot(X, Y2) 
    axis[0, 1].set_title("Joint Angles 1") 
    
    # For Tangent Function 
    axis[1, 0].plot(X, Y3) 
    axis[1, 0].set_title("Joint Angles 2") 
    
    # For Tanh Function 
    axis[1, 1].plot(X, Y4) 
    axis[1, 1].set_title("Joint Angles 3") 
    
    axis[2, 0].plot(X, Y5) 
    axis[2, 0].set_title("Joint Angles 4") 
    
    axis[2, 1].plot(X, Y6) 
    axis[2, 1].set_title("Joint Angles 5") 
    
    
    # Combine all the operations and display 
    plt.show() 
    
def robo_joint_efforts():
    df = pandas.read_csv("data.csv", sep = ',',)

    time = df['time'].to_list()
    jeffort0 = (df['robo_joint_effort_0']).to_list()
    jeffort1 = df['robo_joint_effort_1'].to_list()
    jeffort2 = df['robo_joint_effort_2'].to_list()
    jeffort3 = df['robo_joint_effort_3'].to_list()
    jeffort4 = df['robo_joint_effort_4'].to_list()
    jeffort5 = df['robo_joint_effort_5'].to_list()
    

    #Get the angles from 0 to 2 pie (360 degree) in narray object 
    X = time 
    
    #Using built-in trigonometric function we can directly plot 
    #the given cosine wave for the given angles 
    Y1 = jeffort0
    Y2 = jeffort1 
    Y3 = jeffort2
    Y4 = jeffort3
    Y5 = jeffort4
    Y6 = jeffort5
    
    #Initialise the subplot function using number of rows and columns 
    figure, axis = plt.subplots(3, 2) 
    
    # For Sine Function 
    axis[0, 0].plot(X, Y1) 
    axis[0, 0].set_title("Joint Effort 0") 
    
    # For Cosine Function 
    axis[0, 1].plot(X, Y2) 
    axis[0, 1].set_title("Joint Effort 1") 
    
    # For Tangent Function 
    axis[1, 0].plot(X, Y3) 
    axis[1, 0].set_title("Joint Effort 2") 
    
    # For Tanh Function 
    axis[1, 1].plot(X, Y4) 
    axis[1, 1].set_title("Joint Effort 3") 
    
    axis[2, 0].plot(X, Y5) 
    axis[2, 0].set_title("Joint Effort 4") 
    
    axis[2, 1].plot(X, Y6) 
    axis[2, 1].set_title("Joint Effort 5") 
    
    
    # Combine all the operations and display 
    plt.show() 
    
    
def robo_joint_position():
    df = pandas.read_csv("data.csv", sep = ',',)
    ins = pandas.read_csv("instructions.csv", sep = ',',)
    
    
    inst_time = ins['time'].to_list()
    inst_pos_0 = ins['instruction_0'].to_list()
    inst_pos_1 = ins['instruction_1'].to_list()
    inst_pos_2 = ins['instruction_2'].to_list()
    inst_pos_3 = ins['instruction_3'].to_list()
    inst_pos_4 = ins['instruction_4'].to_list()
    inst_pos_5 = ins['instruction_5'].to_list()
  
  
    time = df['time'].to_list()
    pose0 = df['robo_pose_0'].to_list()
    pose1 = df['robo_pose_1'].to_list()
    pose2 = df['robo_pose_2'].to_list()
    pose3 = df['robo_pose_3'].to_list()
    pose4 = df['robo_pose_4'].to_list()
    pose5 = df['robo_pose_5'].to_list()
    

    X = time 
    inst_X= inst_time
    
    Y1 = pose0
    Y2 = pose1 
    Y3 = pose2
    Y4 = pose3
    Y5 = pose4
    Y6 = pose5
    
    inst_Y0= inst_pos_0
    inst_Y1= inst_pos_1 
    inst_Y2= inst_pos_2
    inst_Y3= inst_pos_3
    inst_Y4= inst_pos_4 
    inst_Y5= inst_pos_5
    
    #Initialise the subplot function using number of rows and columns 
    figure, axis = plt.subplots(3, 2) 
    
    axis[0, 0].plot(X, Y1)
    axis[0, 0].plot(inst_X, inst_Y0, 'r')
    axis[0, 0].set_title("X") 
    
    axis[0, 1].plot(X, Y2) 
    axis[0, 1].plot(inst_X, inst_Y1, 'r')
    axis[0, 1].set_title("Y") 
    
    axis[1, 0].plot(X, Y3) 
    axis[1, 0].plot(inst_X, inst_Y2, 'r')
    axis[1, 0].set_title("Z") 
    
    axis[1, 1].plot(X, Y4) 
    axis[1, 1].plot(inst_X, inst_Y3, 'r')
    axis[1, 1].set_title("roll") 
    
    axis[2, 0].plot(X, Y5) 
    axis[2, 0].plot(inst_X, inst_Y4, 'r')
    axis[2, 0].set_title("pitch") 
    
    axis[2, 1].plot(X, Y6)
    axis[2, 1].plot(inst_X, inst_Y5, 'r')
    axis[2, 1].set_title("yaw") 
    
    
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
        #robo_joint_efforts()
        #robo_joint_position()
        
    elif args[1] == 'simulator':
        print("This is yet to be implemented !")
    else:
        print("This is yet to be implemented !")

        
    
    
    
if __name__ == "__main__":
    main()
