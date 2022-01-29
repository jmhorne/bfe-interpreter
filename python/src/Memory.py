class Memory:
    def __init__(self, program_file:str=None, size:int=None) -> None:
        self.pointer_rollover = True
        self.data_rollover = True

        if program_file:
            with open(program_file, 'r') as input:
                self.memory = [ord(x) for x in input.read()]
            self.pointer_rollover = False
            self.data_rollover = False
        elif size:
            self.memory = [0 for x in range(size)]
        else:
            self.memory = [0 for x in range(1_048_576)]
        
        self.size = len(self.memory)
        self.pointer = 0

    def increase_data(self) -> None:
        self.memory[self.pointer] += 1

        if self.memory[self.pointer] > 255 and self.data_rollover:
            self.memory[self.pointer] = 0
    
    def decrease_data(self) -> None:
        self.memory[self.pointer] -= 1

        if self.memory[self.pointer] < 0 and self.data_rollover:
            self.memory[self.pointer] = 255
    
    def increase_pointer(self) -> None:
        self.pointer += 1

        if self.pointer >= self.size and self.pointer_rollover:
            self.pointer = 0

    def decrease_pointer(self) -> None:
        self.pointer -= 1

        if self.pointer < 0 and self.pointer_rollover:
            self.pointer = self.size - 1
    
    def set_pointer(self, pos:int) -> None:
        self.pointer = pos
    
    def get_pointer(self) -> int:
        return self.pointer

    def get_data(self) -> int:
        return self.memory[self.pointer]
    
    def get_data_at(self, pos:int) -> int:
        if pos >= self.size:
            return None
        return self.memory[pos]
    
    def set_data(self, data:int) -> None:
        self.memory[self.pointer] = data
    
    def set_pointer_rollover(self, rollover:bool) -> None:
        self.pointer_rollover = rollover
    
    def set_data_rollover(self, rollover:bool) -> None:
        self.data_rollover = rollover
    
    def get_size(self) -> int:
        return self.size