from Memory import Memory
import sys, tty, termios

class Interpreter:
    def __init__(self, memory: Memory) -> None:
        self.memory = memory
        self.jump_stack = list()
        self.program:Memory = None
        self.commands = {
            '>': self.memory.increase_pointer,
            '<': self.memory.decrease_pointer,
            '+': self.memory.increase_data,
            '-': self.memory.decrease_data,
            '.': self.output,
            ',': self.input,
            '[': self.jump_if_zero,
            ']': self.jump_unconditional,
            '/': self.comment,
            '?': self.print_memory,
            '^': self.debug
        }
    
    def execute(self, program: Memory) -> None:
        self.program = program
        self.program.set_pointer(0)

        while self.program.get_pointer() < self.program.get_size():
            command = chr(self.program.get_data())

            if command in self.commands:
                self.commands[command]()
            
            self.program.increase_pointer()

    def output(self) -> None:
        data = self.memory.get_data()

        if (32 <= data <= 176) or data == 10:
            print(chr(data), end='')
        else:
            print("\\x{:02x}".format(data), end='')
    
    def input(self) -> None:
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        tty.setraw(fd)
        data = sys.stdin.read(1)
        if data == '\r':
            print()
        print(data, '\b', end='', flush=True)
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        self.memory.set_data(ord(data))
    
    def jump_if_zero(self) -> None:
        data = self.memory.get_data()
        start = '['
        end = ']'

        if data != 0:
            self.jump_stack.append(self.program.get_pointer())
        else:
            inner_loop_count = -1
            command = chr(self.program.get_data())

            while command != end or inner_loop_count:
                if command == start:
                    inner_loop_count += 1
                
                if command == end:
                    inner_loop_count -= 1
                
                self.program.increase_pointer()
                command = chr(self.program.get_data())
    
    def jump_unconditional(self) -> None:
        if len(self.jump_stack):
            self.program.set_pointer(self.jump_stack.pop() - 1)
    
    def print_memory(self) -> None:
        self.program.increase_pointer()
        start = self.pull_number()
        self.program.increase_pointer() # skip delimeter
        end = self.pull_number()

        print("\n***** MEMORY *****\n")

        [print(x, end='\t') for x in range(start, end)]
        print()
        [print(self.memory.get_data_at(x), end='\t') for x in range(start, end)]
        print()
    
    def pull_number(self) -> int:
        num = "0"

        while self.program.get_pointer() < self.program.get_size() and chr(self.program.get_data()).isnumeric():
            num += chr(self.program.get_data())
            self.program.increase_pointer()
        
        return int(num)
    
    def debug(self) -> None:
        print("\n***** DEBUG *****\n")
        print(f"{'Program pointer:':>16} {self.program.get_pointer()}")
        print(f"{'Memory pointer:':>16} {self.memory.get_pointer()}")
    
    def comment(self) -> None:
        self.program.increase_pointer()

        if chr(self.program.get_data()) == '/':
            while self.program.get_data() != 10 and self.program.get_pointer() < self.program.get_size():
                self.program.increase_pointer()
        elif chr(self.program.get_data()) == '*':
            self.program.increase_pointer()
            while chr(self.program.get_data()) != '*' and self.program.get_pointer() < self.program.get_size():
                self.program.increase_pointer()
            self.program.increase_pointer()