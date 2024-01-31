import pandas
from matplotlib import pyplot as plt
import numpy as np 
import math 
  
def split_list(a_list, divisor):
    half = len(a_list)//divisor
    return a_list[:half], a_list[half:]


  
def joint_velocity():
    df = pandas.read_csv("run1.csv", sep = ',',)
    #print(df)
    #df.head()

    time = df['time'].to_list()
    jvel0 = (df['joint_velocity_0']).to_list()
    jvel1 = df['joint_velocity_1'].to_list()
    jvel2 = df['joint_velocity_2'].to_list()
    jvel3 = df['joint_velocity_3'].to_list()
    jvel4 = df['joint_velocity_4'].to_list()
    jvel5 = df['joint_velocity_5'].to_list()
    
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
    axis[0, 0].set_title("Joint velocity 0") 
    
    # For Cosine Function 
    axis[0, 1].plot(X, Y2) 
    axis[0, 1].set_title("Joint velocity 1") 
    
    # For Tangent Function 
    axis[1, 0].plot(X, Y3) 
    axis[1, 0].set_title("Joint velocity 2") 
    
    # For Tanh Function 
    axis[1, 1].plot(X, Y4) 
    axis[1, 1].set_title("Joint velocity 3") 
    
    axis[2, 0].plot(X, Y5) 
    axis[2, 0].set_title("Joint velocity 4") 
    
    axis[2, 1].plot(X, Y6) 
    axis[2, 1].set_title("Joint velocity 5") 
    
    
    # Combine all the operations and display 
    plt.show() 


def joint_angles():
    df = pandas.read_csv("run1.csv", sep = ',',)
    #print(df)
    #df.head()

    time = df['time'].to_list()
    jangle0 = (df[' joint_angles_0']).to_list()
    jangle1 = df['joint_angles_1'].to_list()
    jangle2 = df['joint_angles_2'].to_list()
    jangle3 = df['joint_angles_3'].to_list()
    jangle4 = df['joint_angles_4'].to_list()
    jangle5 = df['joint_angles_5'].to_list()
    

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
    
def joint_efforts():
    df = pandas.read_csv("run1.csv", sep = ',',)

    time = df['time'].to_list()
    jeffort0 = (df['joint_effort_0']).to_list()
    jeffort1 = df['joint_effort_1'].to_list()
    jeffort2 = df['joint_effort_2'].to_list()
    jeffort3 = df['joint_effort_3'].to_list()
    jeffort4 = df['joint_effort_4'].to_list()
    jeffort5 = df['joint_effort_5'].to_list()
    

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
    
    print(Y1)
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
    
    
def joint_position():
    df = pandas.read_csv("run1.csv", sep = ',',)

    time = df['time'].to_list()
    pose0 = (df['pose_0']).to_list()
    pose1 = df['pose_1'].to_list()
    pose2 = df['pose_2'].to_list()
    pose3 = df['pose_3'].to_list()
    pose4 = df['pose_4'].to_list()
    pose5 = df['pose_5'].to_list()
    

    #Get the angles from 0 to 2 pie (360 degree) in narray object 
    X = time 
    
    #Using built-in trigonometric function we can directly plot 
    #the given cosine wave for the given angles 
    Y1 = pose0
    Y2 = pose1 
    Y3 = pose2
    Y4 = pose3
    Y5 = pose4
    Y6 = pose5
    
    print(Y1)
    #Initialise the subplot function using number of rows and columns 
    figure, axis = plt.subplots(3, 2) 
    
    # For Sine Function 
    axis[0, 0].plot(X, Y1) 
    axis[0, 0].set_title("X") 
    
    # For Cosine Function 
    axis[0, 1].plot(X, Y2) 
    axis[0, 1].set_title("Y") 
    
    # For Tangent Function 
    axis[1, 0].plot(X, Y3) 
    axis[1, 0].set_title("Z") 
    
    # For Tanh Function 
    axis[1, 1].plot(X, Y4) 
    axis[1, 1].set_title("roll") 
    
    axis[2, 0].plot(X, Y5) 
    axis[2, 0].set_title("pitch") 
    
    axis[2, 1].plot(X, Y6) 
    axis[2, 1].set_title("yaw") 
    
    
    # Combine all the operations and display 
    plt.show() 

def main():
    joint_angles()
    joint_velocity()
    joint_efforts()
    joint_position()

if __name__ == "__main__":
    main()
