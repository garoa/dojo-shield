/*
  WildSnakeUNO
  
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

*/

// {first_choice, second_choice*}
// * -1 means there is no second_choice
int moves[][2] = {
  {1, -1},  // 0
  {13, 2},  // 1
  {3, -1},  // 2
  {4, -1},  // 3
  {6, 5},   // 4
  {0, -1},  // 5
  {2, 8},   // 6
  {12, -1}, // 7
  {7, -1},  // 8
  {13, 8},  // 9
  {9, -1},  // 10
  {10, -1}, // 11
  {6, 11},  // 12
  {5, 11}   // 13
};

/*     A
     -----
  F |     | B
    |  G  |
     -----
  E |     | C
    |  D  |
     ----- 
*/

//                A   B  C  D  E   F   G
int display[] = {8, 9, 10, 11, 12, 13, 7}; // Garoa Dojo Shield
const int SEGMENTS = 7;

int head = 1; // segment B
int tail = 0; // segment A

void setup() {
  for (int i=0; i<SEGMENTS; i++) {
    pinMode(display[i], OUTPUT);
  }
  digitalWrite(display[tail % 7], HIGH);
}

void loop() {
  digitalWrite(display[head % 7], HIGH);
  delay(analogRead(A0));
  digitalWrite(display[tail % 7], LOW);
  tail = head;
  int* choices = moves[head];
  if (choices[1] == -1) {
    head = choices[0];  // no second choice
  } else {
    head = choices[random(2)];  // random move
  }
}

