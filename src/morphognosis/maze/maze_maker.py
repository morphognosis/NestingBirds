# For conditions of distribution and use, see copyright notice in Main.java
#
# Maze maker.
# 
# Make mazes for training and testing maze learning.
# 
# There are three learning tasks:
# 1. Maze learning: learn to navigate a sequence of rooms connected by doors.
# 2. Context learning: learn correspondences between room configurations separated
#    by intervening mazes.
# 3. Modular learning: the intervening mazes are trained independently and
#    presenting only during testing. This measures the ability to dynamically combine
#    independently learned sequences to achieve success.
#
# A maze consists of a sequence of rooms connected by doors.
# There are a fixed number of doors.
# The learner outputs a door choice or a wait.
# A room contains a room-specific set of on/off marks.
#
# These are the types of rooms:
#
# 1. Context begin room: in this room the learner is presented with marks having a single
#    on value that corresponds to the correct door choice.
#    This door leads to either a maze entry or directly to a context end room. If it leads to
#    a maze entry room, a maze must be navigated before reaching the context end room. 
#    In the context end room, the learner must choose the door that was marked on in the 
#    context begin room.
#
# 2. Maze entry room: the marks identify the configuration of the upcoming maze sequence,
#    consisting of maze interior rooms. The learner uses this information to navigate
#    the maze.
#
# 3. Maze interior room: the marks in the maze entry room determine the correct door choice 
#    sequence to move through the maze.
#
# 4. Context end room: in this room the correct door choice is determined by the context begin
#    room door marks.
#
# Input format:
# <context room marks><maze entry room marks><maze interior room mark><context end room marks>
#
# Output: 
# A door choice or a wait.
# 
# Training and testing.
# 
# There are three types of sequences:
# 1. Context: context begin room and context end room. To learn context associations.
# 2. Context maze: context begin room, maze entry and interior sequence, and context end room.
# 3. Independent maze: maze entry and maze interior sequence. To learn maze sequences.
# 
# For training, a set of the above sequences are created. This evaluates maze and context
# learning.
# 
# For testing, context-mazes are created from novel context and independent maze combinations
# taken from the training set. This evaluates modular context learning.
#
# Output dataset files:
output_dataset_module = 'maze_dataset.py'
# Contains:
# X_train_shape = [<number of sequences>, <steps per sequence>, <input size>]
# X_train_seq = [<input sequences (0|1)>]
# y_train_shape = [<number of sequences>, <steps per sequence>, <output size>]
# y_train_seq = [<output sequences>]
# X_test_shape = [<number of sequences>, <steps per sequence>, <input size>]
# X_test_seq = [<input sequences (0|1)>]
# y_test_shape = [<number of sequences>, <steps per sequence>, <output size>]
# y_test_seq = [<output sequences>]
output_dataset_csv = 'maze_dataset.csv'
# Contains:
# Training input shape and sequences:
# <number of sequences>, <steps per sequence>, <input size>
# <input sequences (0|1)>
# Training output shape and sequences:
# <number of sequences>, <steps per sequence>, <output size>
# <output sequences>]
# Testing input shape and sequences:
# <number of sequences>, <steps per sequence>, <input size>
# input sequences (0|1)>
# Testing output shape and sequences:
# <number of sequences>, <steps per sequence>, <output size>
# <output sequences>

from numpy import array
import sys, getopt, random

# Default parameters.
num_doors = 3
maze_interior_length = 1
num_context_mazes = 3
num_independent_mazes = 3
random_seed = 4517
verbose = True

# Get options.
usage = 'maze_maker.py [--num_doors <quantity>] [--maze_interior_length <length>] [--num_context_mazes <quantity>] [--num_independent_mazes <quantity>] [--random_seed <seed>] [--verbose on|off]'
try:
  opts, args = getopt.getopt(sys.argv[1:],"h",["help","num_doors=","maze_interior_length=","num_context_mazes=","num_independent_mazes=","random_seed=","verbose="])
except getopt.GetoptError:
  print(usage)
  sys.exit(1)
for opt, arg in opts:
  if opt == '-h' or opt == '--help':
     print(usage)
     sys.exit(0)
  if opt == "--num_doors":
     num_doors = int(arg)
  elif opt == "--maze_interior_length":
     maze_interior_length = int(arg)
  elif opt == "--num_context_mazes":
     num_context_mazes = int(arg)
  elif opt == "--num_independent_mazes":
     num_independent_mazes = int(arg)
  elif opt == "--random_seed":
     random_seed = int(arg)
  elif opt == "--verbose":
     if arg == 'on':
         verbose = True
     elif arg == 'off':
         verbose = False
  else:
     print(usage)
     sys.exit(1)

# Number of maze ids for contexxt and independent mazes.
num_maze_ids = num_context_mazes  + num_independent_mazes

random.seed(random_seed)

if verbose:
    print("Parameters:")
    print("num_doors =", num_doors)
    print("maze_interior_length =", maze_interior_length)
    print("num_context_mazes =", num_context_mazes)
    print("num_independent_mazes =", num_independent_mazes)
    print("random_seed =", random_seed)

off_doors = []
for i in range(num_doors):
    off_doors += [0]
off_maze_id = []
for i in range(num_maze_ids):
    off_maze_id += [0]
off_interior = [0]
on_interior = [1]

# Generated maze ids.
generated_maze_id_index = 0

# Generate a maze.
def generate_maze():

    # Generate maze id.
    def generate_maze_id():
        global generated_maze_id_index
        maze_id = []
        for i in range(num_maze_ids):
            if i == generated_maze_id_index:
                maze_id += [1]
            else:
                maze_id += [0]
        generated_maze_id_index += 1
        return maze_id

    X_seq = []
    y_seq = []
    X_seq += (off_doors + generate_maze_id() + off_interior + off_doors)
    door = random.randrange(0, num_doors)
    for i in range(num_doors + 1):
        if i == door:
            y_seq += [1]
        else:
            y_seq += [0]
    for room in range(maze_interior_length):
        X_seq += (off_doors + off_maze_id + on_interior + off_doors)
        door = random.randrange(0, num_doors)
        for i in range(num_doors + 1):
            if i == door:
                y_seq += [1]
            else:
                y_seq += [0]
    return X_seq, y_seq

# Create training set:

# Create shapes.
sequence_steps = maze_interior_length + 3
num_context_sequences = num_doors
num_context_maze_sequences = num_doors * num_context_mazes
num_independent_maze_sequences = num_independent_mazes
num_train_sequences = num_context_sequences + num_context_maze_sequences + num_independent_maze_sequences
num_inputs = num_doors + num_maze_ids + 1 + num_doors
num_outputs = num_doors + 1
X_train_shape = [num_train_sequences, sequence_steps, num_inputs]
X_train_seq = []
y_train_shape = [num_train_sequences, sequence_steps, num_outputs]
y_train_seq = []

# Create context sequences.
for door in range(num_context_sequences):
    for i in range(num_doors):
        if i == door:
            X_train_seq += [1]
        else:
            X_train_seq += [0]
    for i in range(num_doors + 1):
        if i == door:
            y_train_seq += [1]
        else:
            y_train_seq += [0]
    X_train_seq += (off_maze_id + off_interior + off_doors)
    X_train_seq += (off_doors + off_maze_id + off_interior)
    for i in range(num_doors):
        X_train_seq += [1]
    for i in range(num_doors + 1):
        if i == door:
            y_train_seq += [1]
        else:
            y_train_seq += [0]
    for i in range(sequence_steps - 2):
        X_train_seq += (off_doors + off_maze_id + off_interior + off_doors)
        for i in range(num_doors + 1):
            if i == door:
                y_train_seq += [1]
            else:
                y_train_seq += [0]

# Create context-maze sequences.

# Create mazes for contexts.
X_maze_context_seqs = []
y_maze_context_seqs = []
n = 0
while n < num_context_mazes:
    X, y = generate_maze()
    X_maze_context_seqs.append(X)
    y_maze_context_seqs.append(y)
    n += 1

# Combine contexts with mazes.
for door in range(num_context_sequences):
    for maze in range(num_context_mazes):
        for i in range(num_doors):
            if i == door:
                X_train_seq += [1]
            else:
                X_train_seq += [0]
        for i in range(num_doors + 1):
            if i == door:
                y_train_seq += [1]
            else:
                y_train_seq += [0]
        X_train_seq += (off_maze_id + off_interior + off_doors)
        X_train_seq += X_maze_context_seqs[maze]
        y_train_seq += y_maze_context_seqs[maze]
        X_train_seq += (off_doors + off_maze_id + off_interior)
        for i in range(num_doors):
            X_train_seq += [1]
        for i in range(num_doors + 1):
            if i == door:
                y_train_seq += [1]
            else:
                y_train_seq += [0]

# Create independent mazes.
X_maze_independent_seqs = []
y_maze_independent_seqs = []
n = 0
while n < num_independent_mazes:
    X, y = generate_maze()
    X_maze_independent_seqs.append(X)
    y_maze_independent_seqs.append(y)
    n += 1

for maze in range(num_independent_mazes):
    X_train_seq += X_maze_independent_seqs[maze]
    y_train_seq += y_maze_independent_seqs[maze]
    for i in range(2):
        X_train_seq += (off_doors + off_maze_id + off_interior + off_doors)
        for i in range(num_doors):
            y_train_seq += [0]
        y_train_seq += [1]

# Create testing set:

# Create shapes.
num_test_sequences = num_doors * num_independent_mazes
X_test_shape = [num_test_sequences, sequence_steps, num_inputs]
X_test_seq = []
y_test_shape = [num_test_sequences, sequence_steps, num_outputs]
y_test_seq = []

# Combine contexts with independent mazes.
for door in range(num_context_sequences):
    for maze in range(num_independent_mazes):
        for i in range(num_doors):
            if i == door:
                X_test_seq += [1]
            else:
                X_test_seq += [0]
        X_test_seq += (off_maze_id + off_interior + off_doors)
        for i in range(num_doors + 1):
            if i == door:
                y_test_seq += [1]
            else:
                y_test_seq += [0]
        X_test_seq += X_maze_independent_seqs[maze]
        y_test_seq += y_maze_independent_seqs[maze]
        X_test_seq += (off_doors + off_maze_id + off_interior)
        for i in range(num_doors):
            X_test_seq += [1]
        for i in range(num_doors + 1):
            if i == door:
                y_test_seq += [1]
            else:
                y_test_seq += [0]

# Write dataset.
if verbose:
    print("Writing maze dataset to ", output_dataset_module, " and ", output_dataset_csv, sep='')
with open(output_dataset_module, 'w') as module, open(output_dataset_csv, 'w') as csv:
    if verbose:
        print('Training data:')
        print('X_train_shape = [', num_train_sequences, ',', sequence_steps, ',', num_inputs, ']')
    print('X_train_shape = [', num_train_sequences, ',', sequence_steps, ',', num_inputs, ']', file=module)
    print(num_train_sequences, ',', sequence_steps, ',', num_inputs, sep='', file=csv)
    module.write('X_train_seq = [ ')
    first = True
    for value in X_train_seq:
        if first:
            first = False
        else:
            module.write(", ")
            csv.write(",")
        module.write("%s" % value)
        csv.write("%s" % value)
    module.write(' ]\n')
    csv.write('\n')
    if verbose:
        print('X_train_seq = [ ', end='')
        first = True
        for value in X_train_seq:
            if first:
                first = False
            else:
                print(", ", end='')
            print(value, end='')
        print(' ]')
        print('y_train_shape = [', num_train_sequences, ',', sequence_steps, ',', num_outputs, ']')
    print('y_train_shape = [', num_train_sequences, ',', sequence_steps, ',', num_outputs, ']', file=module)
    print(num_train_sequences, ',', sequence_steps, ',', num_outputs, sep='', file=csv)
    module.write('y_train_seq = [ ')
    first = True
    for value in y_train_seq:
        if first:
            first = False
        else:
            module.write(", ")
            csv.write(",")
        module.write("%s" % value)
        csv.write("%s" % value)
    module.write(' ]\n')
    csv.write('\n')
    if verbose:
        print('y_train_seq = [ ', end='')
        first = True
        for value in y_train_seq:
            if first:
                first = False
            else:
                print(", ", end='')
            print(value, end='')
        print(' ]')
        print('Testing data:')
        print('X_test_shape = [', num_test_sequences, ',', sequence_steps, ',', num_inputs, ']')
    print('X_test_shape = [', num_test_sequences, ',', sequence_steps, ',', num_inputs, ']', file=module)
    print(num_test_sequences, ',', sequence_steps, ',', num_inputs, sep='', file=csv)
    module.write('X_test_seq = [ ')
    first = True
    for value in X_test_seq:
        if first:
            first = False
        else:
            module.write(", ")
            csv.write(",")
        module.write("%s" % value)
        csv.write("%s" % value)
    module.write(' ]\n')
    csv.write('\n')
    if verbose:
        print('X_test_seq = [ ', end='')
        first = True
        for value in X_test_seq:
            if first:
                first = False
            else:
                print(", ", end='')
            print(value, end='')
        print(' ]')
        print('y_test_shape = [', num_test_sequences, ',', sequence_steps, ',', num_outputs, ']')
    print('y_test_shape = [', num_test_sequences, ',', sequence_steps, ',', num_outputs, ']', file=module)
    print(num_test_sequences, ',', sequence_steps, ',', num_outputs, sep='', file=csv)
    module.write('y_test_seq = [ ')
    first = True
    for value in y_test_seq:
        if first:
            first = False
        else:
            module.write(", ")
            csv.write(",")
        module.write("%s" % value)
        csv.write("%s" % value)
    module.write(' ]\n')
    csv.write('\n')
    if verbose:
        print('y_test_seq = [ ', end='')
        first = True
        for value in y_test_seq:
            if first:
                first = False
            else:
                print(", ", end='')
            print(value, end='')
        print(' ]')

# Print interpreted dataset.
if verbose:

    print('Training mazes:')
    seq = array(X_train_seq)
    X = seq.reshape(X_train_shape[0], X_train_shape[1], X_train_shape[2])
    seq = array(y_train_seq)
    y = seq.reshape(y_train_shape[0], y_train_shape[1], y_train_shape[2])
    for seq in range(X_train_shape[0]):
        print('maze =', seq)
        for step in range(X_train_shape[1]):
            marks = X[seq][step][0:]
            print('input: =', marks[0:num_doors], end='')
            print('', marks[num_doors:num_doors + num_maze_ids], end='')
            print('', marks[num_doors + num_maze_ids:num_doors + num_maze_ids + 1 ], end='')
            print('', marks[num_doors + num_maze_ids + 1:num_doors + num_maze_ids + 1 + num_doors], end='')
            print(' ', end='')
            output = y[seq][step]
            print(' output =', output, end='')
            door = list(output).index(1)
            if door < num_doors:
                s = ' (door ' + str(door) + ')'
                print(s)
            else:
                print(' (wait)')

    print('Test mazes:')
    seq = array(X_test_seq)
    X = seq.reshape(X_test_shape[0], X_test_shape[1], X_test_shape[2])
    seq = array(y_test_seq)
    y = seq.reshape(y_test_shape[0], y_test_shape[1], y_test_shape[2])
    for seq in range(X_test_shape[0]):
        print('maze =', seq)
        for step in range(X_test_shape[1]):
            marks = X[seq][step][0:]
            print('input =', marks[0:num_doors], end='')
            print('', marks[num_doors:num_doors + num_maze_ids], end='')
            print('', marks[num_doors + num_maze_ids:num_doors + num_maze_ids + 1 ], end='')
            print('', marks[num_doors + num_maze_ids + 1:num_doors + num_maze_ids + 1 + num_doors], end='')
            print(' ', end='')
            output = y[seq][step]
            print(' output =', output, end='')
            door = list(output).index(1)
            if door < num_doors:
                s = ' (door ' + str(door) + ')'
                print(s)
            else:
                print(' (wait)')
