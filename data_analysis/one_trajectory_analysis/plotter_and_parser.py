from matplotlib import pyplot as plt
from .vector import Vector

class Plotter: 
    def __init__(self, instructions_csv):
        self.instructions_csv = instructions_csv

    def show_X_Y_Z_diagrams(self):
        
        inst_time = self.instructions_csv['time'].to_list()


        current_position_0 = self.instructions_csv['current_position_0'].to_list()
        current_position_1 = self.instructions_csv['current_position_1'].to_list()
        current_position_2 = self.instructions_csv['current_position_2'].to_list()   
        
        raw_instruction_0 = self.instructions_csv['raw_instruction_0'].to_list()
        raw_instruction_1 = self.instructions_csv['raw_instruction_1'].to_list()
        raw_instruction_2 = self.instructions_csv['raw_instruction_2'].to_list()   
        

        checked_instruction_0 = self.instructions_csv['checked_instruction_0'].to_list()
        checked_instruction_1 = self.instructions_csv['checked_instruction_1'].to_list()
        checked_instruction_2 = self.instructions_csv['checked_instruction_2'].to_list()   

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


    def show_points_of_vector_list_graphically(self, vector_list):

        ax = plt.axes(projection='3d')

        x_values = [float(x.X) for x in vector_list]
        y_values = [float(y.Y) for y in vector_list]
        z_values = [float(z.Z) for z in vector_list]

        ax.scatter3D(x_values, y_values, z_values,color = "r")

        # ax.set_zlim(0,300)

        plt.show()

    
    def show_points_of_vector_list_graphically_annotated(self, vector_list):

        ax = plt.axes(projection='3d')

        x_values = [float(x.X) for x in vector_list]
        y_values = [float(y.Y) for y in vector_list]
        z_values = [float(z.Z) for z in vector_list]

        ax.scatter3D(x_values, y_values, z_values,color = "r")

        time_stamp = 0

        for x, y, z in zip( x_values, y_values, z_values):
            label = time_stamp
            ax.text(x, y, z, label)
            time_stamp = time_stamp + 1

        # ax.set_zlim(0,300)

        plt.show()


class Parser:
    def __init__(self, instructions_csv):
        self.instructions_csv = instructions_csv
    
    def parse_csv_to_vectors_of_points(self):
        vector_list = list()

        x = self.instructions_csv['current_position_0'].to_list()
        y = self.instructions_csv['current_position_1'].to_list()
        z = self.instructions_csv['current_position_2'].to_list() 

        assert(len(x) == len(y) == len(z))

        for i in range(0,len(x)):
            vector_list.append(Vector(float(x[i]), float(y[i]), float(z[i])))
        # print(vector_list)
        return vector_list