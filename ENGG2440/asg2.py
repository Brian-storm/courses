# Checking answers
# ENGG2440A assignment2

from math import log, floor


class Solutions():
    
    def __init__(self, *args):
        self.n = args[0]
        self.b = args[1]

    def q3_2 (self):
        return sum(1 / (k+1) / (k+2) for k in range(1, self.n+1))
    
    def q3_3(self):
        return sum(1 / k / (k+1) / (k+2) for k in range(1, self.n+1))
    
    def q3_4(self):
        return sum(sum(2 ** k for k in range(j, self.n+1)) for j in range(1, self.n+1))
    
    def q3_5(self):
        return sum(floor(log(k, self.b)) for k in range(1, self.n+1))

def prompt_answer():
    sup_num = [3]
    sub_num = range(2, 6)
    
    args = (10, 5)
    solution = Solutions(*args)
    
    for n0 in sup_num:
        for n1 in sub_num:
            q_num = 'q' + str(n0) + '_' + str(n1)
            if hasattr(solution, q_num):
                sol = getattr(solution, q_num)
                ans = sol()
                print(f"Q{n0}_{n1}: answer = {ans}, with args({args}))")
    print("\n=================================")

print("=================================")
print("This is Assignment 2 of ENGG2440A")
print("=================================\n")

prompt_answer()
