import sys

class DataSystem:
    def __init__(self,Interpreter):
        self.memory = {}
        self.datatypes = ["int","str","float","char","array","bool"]
    def register_Data(self,key_VAR_Name,datatype,data,isthere=False):
        name = key_VAR_Name
        dtype = datatype
        if isthere:
            self.memory[name] = {}
        self.memory[name]["dtype"] = dtype
        self.memory[name]["data"] = self.format_dtype(data=data,dtype=dtype)
    def kill_Data(self,key_VAR_Name):
        self.memory.pop[key_VAR_Name]
    def get_Data_Location(self,key_VAR_Name,absolute = False):
        if absolute:
           return hex(id(self.memory[key_VAR_Name]))
        return hex(id(self.memory[key_VAR_Name]["data"]))
    def get_Sizeof_Data(self,key_VAR_Name):
        return sys.getsizeof(self.memory[key_VAR_Name]["data"])
    def format_dtype(self,dtype,data):
        if dtype in self.datatypes:
            if dtype == "str":
               return str(data)
            elif dtype == "int":
               return int(data)
            elif dtype == "float":
               return float(data)
            elif dtype == "char":
               return data[0]
            elif dtype == "array":
               return list(data)
            elif dtype == "bool":
                if data == "true":
                    return int(1)
                elif data == "false":
                    return int(0)
                elif data == "maybe":
                    return int(2)
            else: return
# s = DataSystem("x")
# s.register_Data("h","int",15)
# print(s.memory)
# print(s.get_Data_Location("h"))
# print(s.get_Sizeof_Data("h"))