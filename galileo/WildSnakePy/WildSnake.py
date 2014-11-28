"""
  WildSnakePy - WildSnakeUNO ported to Galileo (mraa native API)
  
A two-segment snake slithers aimlessly over the display. On intersections, 
the next step is chosen at random.

Most of the logic is encoded in the `moves` array which lists possible next
steps for each current position and direction. Snake head position and 
direction is coded like pictured below, i.e. when the snake head is at the 
middle segment going right, the code is 6, going left in the same place is 
code 13.

       >:0
       <:7
      -----
^:5  |     | v:1
v:12 | >:6 | ^:8
      -----
^:4  | <:13| v:2
v:11 |     | ^:9
      -----
       <:3
       >:10

To understand this diagram, read:

> as a right arrow
< as a left arrow
v as a down arrow
^ as an up arrow

"""

# TODO: works, but needs cleanup

from mraa import *
import time, random

# {first_choice, second_choice*}
# * -1 means there is no second_choice
moves = [
  [1, -1],  # 0
  [13, 2],  # 1
  [3, -1],  # 2
  [4, -1],  # 3
  [6, 5],   # 4
  [0, -1],  # 5
  [2, 8],   # 6
  [12, -1], # 7
  [7, -1],  # 8
  [13, 8],  # 9
  [9, -1],  # 10
  [10, -1], # 11
  [6, 11],  # 12
  [5, 11]   # 13
]
print moves

"""    A
     -----
  F |     | B
    |  G  |
     -----
  E |     | C
    |  D  |
     ----- 
"""

#                  A   B  C  D  E   F   G
#int display[] = {11, 10, 8, 7, 6, 12, 13}; # DojoXXG

#                 A  B  G  E   D   C   G  F 
#int display[] = {8, 9, 7, 12, 11, 10, 7, 13};

#                 A   B  C  D  E   F   G
SEGMENTS = 7
pins = [8, 9, 10, 11, 12, 13, 7]
display = [Gpio(i) for i in pins]
print repr(display)


head = 8 # segment B
tail = 9 # segment A

pot = Aio(0)

def setup():
  for i in range(SEGMENTS):
    display[i].dir(DIR_OUT) #MRAA
    print repr(display[i])
    display[i].write(1)
    print 'pin %d on' %pins[i]
    time.sleep(2)
    display[i].write(0)
    print 'tested i %d' %pins[i]
  display[tail % 7].write(1) #MRAA

def loop():
  global head
  global tail
  print ' head ' + str(head%7)
  print ' tail ' + str(tail%7)
  display[head % 7].write(1) #MRAA

  delay = pot.read()
  print 'delay %s' % str(delay)
  time.sleep(delay/1000.0)
  display[tail % 7].write(0) #MRAA
  tail = head
  choices = moves[head]
  if (choices[1] == -1) :
    head = choices[0]  # no second choice
  else:
    choice = random.choice([0,1])
    print ' choice ' + str(choice)
    head = choices[choice]


print 'setup()'
setup()
print 'loop()'
while True:
  loop()

