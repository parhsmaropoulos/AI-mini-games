'''
    Generic algorithm that returns Defuzzified decision to our move
    to square problem.
'''

# Inputs:
# 1. Distance - DS_top - range from 0 to  M+N (wide+length of map)
# 2. Slope - SL - slope of the block range from 1 to 4

# Output is :
# Chance - chance to get to this block - range from 0% - 100%

M = 5
N = 5

############ MEMBERSHIP FUNCTIONS ############


# Left "slide" intervals
#                _ _ _ _ _
#               /
#              /
#             /
# - - - - - -

def left_slide(x, left, center, right):
    if x >= left and x < center:
        mul = 1
    elif x >= center and x <= right:
        mul = (right-x)/(right-center)
    else:
        mul = 0
    return mul

# Right "slide" intervals
# _ _ _ _
#         \
#          \
#           \_ _ _ _ _ _ _


def right_slide(x, left, center, right):
    if x >= left and x < center:
        mur = (x-left)/(center-left)
    elif x >= center and x <= right:
        mur = 1
    else:
        mur = 0
    return mur

# Triangle intervals
#                /\
#               /  \
#              /    \
#             /      \
# - - - - - -          - - - - - -


def triangle(x, left, center, right):
    if x >= left and x < center:
        mu = (x-left)/(center-left)
    elif x >= center and x <= right:
        mu = (right-x)/(right-center)
    else:
        mu = 0
    return mu

############ STAGE 1: DEFINE FUZZY SETS ############


def Fuzzy_Step(dst, cost):
    dst_close = left_slide(dst, 0, 1, 3)
    dst_far = right_slide(dst, 4, 6, 8)

    no_slope = left_slide(cost, 1, 1.5, 2)
    small_slope = triangle(cost, 1.5, 2, 2.5)
    mid_slope = triangle(cost, 2, 2.5, 3)
    big_slope = right_slide(cost, 2.5, 3.5, 4)

    # 8 rules - 8 intervals

    move_intervals = [[0, 10, 15], [15, 20, 25], [25, 35, 40], [
        40, 45, 50], [50, 60, 65], [65, 70, 75], [75, 85, 90], [90, 95, 100]]


############ STAGE 2: RULE BASE ############

    Rules = [0, 0, 0, 0, 0, 0, 0, 0]

# Rule 1: IF distance is  far and slope is big then chance to move is low.
    Rule_1 = max(dst_far, big_slope)
    Rules[0] = Rule_1

# Rule 2: IF distance is  far and slope is medium then chance to move is low.
    Rule_2 = max(dst_far, mid_slope)
    Rules[1] = Rule_2

# Rule 3: IF distance is  far and slope is small then chance to move is low.
    Rule_3 = max(dst_far, small_slope)
    Rules[2] = Rule_3

# Rule 4: IF distance is  far and slope is none then chance to move is low.
    Rule_4 = max(dst_far, no_slope)
    Rules[3] = Rule_4

# Rule 5: IF distance is  close and slope is big then chance to move is low.
    Rule_5 = max(dst_close, big_slope)
    Rules[4] = Rule_5

# Rule 6: IF distance is  close and slope is medium then chance to move is low.
    Rule_6 = max(dst_close, mid_slope)
    Rules[5] = Rule_6

# Rule 7: IF distance is  close and slope is small then chance to move is low.
    Rule_7 = max(dst_close, small_slope)
    Rules[6] = Rule_7

# Rule 8: IF distance is  close and slope is none then chance to move is low.
    Rule_8 = max(dst_close, no_slope)
    Rules[7] = Rule_8

    # Use scaled output approach.

    # Find the areas of each output membership.
    Outcome_Move = [0, 0, 0, 0, 0, 0, 0, 0]
    mah = -1
    max_i = -1
    for i in range(7):
        # print(Rules[i])
        if Rules[i] > mah:
            max_i = i
            mah = Rules[i]
        Outcome_Move[i] = Rules[i] * \
            (move_intervals[i][2] - move_intervals[i][0])
    Union = 0
    for i in range(7):
        Union += Outcome_Move[i]

############ STEP 3: DEFUZZIFICATION ############

    # Use center of area to realize
    Chance = 0
    for i in range(7):
        Chance += Rules[i] * move_intervals[i][1]
    Chance = Chance/Union

    return Chance


# c_tp = Fuzzy_Step(6, 3)
# c_r = Fuzzy_Step(4, 1)
# c_d = Fuzzy_Step(4, 4)

# print(
#     f'top_tile chance :{c_tp} \n right_tile chance: {c_r} \n left_tile chance: {c_d}')


# Main solution algorithm
# Given a map a starting position and a desired end postiion
# Using the fuzzy step function to calculate chance to take each step
# We get our optimal path.
def Fuzzy_Solution(start_pos, end_pos, map):
    cur_x = start_pos[0]
    cur_y = start_pos[1]
    e_x = end_pos[0]
    e_y = end_pos[1]

    # An array with 4 chances that updates on each step
    # Left - Right - Up - Down
    chances = [0, 0, 0, 0]
    left_dst = 0
    right_dst = 0
    top_dst = 0
    down_dst = 0
    while((cur_x != e_x or cur_y != e_y)):
        # Get for each of the four posible positions:
        # cost = the value that we get from the new coords of our map
        # distance = use the absolute distance which is abs(e_x - cur_x) + abs(e_y-cur_y)
        # chance to move = Fuzzy_Step(distance,step)
        # If cur_y is lower than 1 there is no left block
        if cur_y > 0:
            left = [cur_x, cur_y-1]
            left_cost = map[left[0]][left[1]]
            left_dst = abs(e_x - (cur_x)) + abs(e_y - (cur_y-1))
            chances[0] = Fuzzy_Step(left_dst, left_cost)

        # If cur_y is equal to map size x then there is no right block
        if cur_y < len(map)-1:
            right = [cur_x, cur_y+1]
            right_cost = map[right[0]][right[1]]
            right_dst = abs(e_x - (cur_x)) + abs(e_y - (cur_y+1))
            chances[1] = Fuzzy_Step(right_dst, right_cost)

        # If cur_x is lower than 1 then there is no up block
        if cur_x > 0:
            top = [cur_x-1, cur_y]
            top_cost = map[top[0]][top[1]]
            top_dst = abs(e_x - (cur_x-1)) + abs(e_y - (cur_y))
            chances[2] = Fuzzy_Step(top_dst, top_cost)

        # If cur_x is equal to map size y then there is no right block
        if cur_x < len(map[0])-1:
            down = [cur_x+1, cur_y]
            down_cost = map[down[0]][down[1]]
            down_dst = abs(e_x - (cur_x+1)) + abs(e_y - (cur_y))
            chances[3] = Fuzzy_Step(down_dst, down_cost)
        print(
            f'Left Distance: {left_dst}\n Right Distance: {right_dst}\n Down Distance: {down_dst}\n Up Distance: {top_dst}\n')
        print(
            f' Left Chance: {chances[0]}\n Right Chance: {chances[1]}\n Up Chance: {chances[2]}\n Down Chance: {chances[3]}\n')

        max = 0
        move = -1
        # Get the max likely step
        for i in range(len(chances)):
            if chances[i] > max:
                max = chances[i]
                move = i
        # Move indicates the pos of the max value
        # 0 -> Left move
        # 1 -> Right move
        # 2 -> Top move
        # 3 -> Down move
        if move == 0:
            cur_y = cur_y - 1
        if move == 1:
            cur_y = cur_y + 1
        if move == 2:
            cur_x = cur_x - 1
        if move == 3:
            cur_x = cur_x + 1
        print(f'New position: {cur_x} , {cur_y}')
        # Zero chances for the next iteration
        chances = [0, 0, 0, 0]


if __name__ == '__main__':
    map = [[1, 1, 1, 1, 2], [1, 2, 2, 2, 2], [
        3, 3, 4, 4, 3], [1, 1, 4, 3, 3], [4, 1, 1, 1, 1]]
    start_pos = [3, 0]
    end_pos = [4, 4]
    Fuzzy_Solution(start_pos, end_pos, map)
    # Current 4.1
    # print(f'tp chance: {Fuzzy_Step(3,6)}')
    # print(f'down chance: {Fuzzy_Step(4,4)}')
    # print(f'right chance: {Fuzzy_Step(1,4)}')
