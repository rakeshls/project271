#include <MD_MAX72xx.h>
#include <ezButton.h>

// matrix controls
const byte data_pin = 11;
const byte chip_select_pin = 10;
const byte clock_pin = 13;
const byte max_devices = 1;

int y = 7;
int x = 0;
int direction = "";

ezButton button(8);

long int current_time, prev_time_random_object = 0, prev_time_player_movement = 0;
int threshold = 400;

int random_object1_position_x = 3;
int random_object_position_y = 8;
int random_object2_position_x = 3;
int random_object3_position_x = 3;

// creating matrix object
MD_MAX72XX matrix = MD_MAX72XX(MD_MAX72XX::PAROLA_HW, chip_select_pin, max_devices);

// define the joystick controls
const byte hpin = A0;
const byte vpin = A1;

void setup() {
  Serial.begin(9600);

  matrix.begin();
  matrix.clear();

  button.setDebounceTime(25);
}

void loop() {

  // put your main code here, to run repeatedly:
  button.loop();
  current_time = millis();
  check_direction();

  //control the movement the player sprite and draws all the objects
  if (current_time - prev_time_player_movement >= 100) {
    prev_time_player_movement = current_time;

    matrix.clear();

    move_sprite();
    matrix.setPoint(y, x, true);
    
    /* write code to show all three random objects
     on the LED Dot Matrix Display*/

    matrix.setPoint(random_object_position_y, random_object1_position_x, true);
    matrix.setPoint(random_object_position_y, random_object2_position_x, true);
    matrix.setPoint(random_object_position_y, random_object3_position_x, true);
     
  }

  //controls the movement of the random_object
  if (current_time - prev_time_random_object >= threshold) {
    prev_time_random_object = current_time;
    random_object_position_y++;

    /* when the random object reaches the bottom, 
    generate a new random object and
    increase the threshold to make the game faster with falling object*/
     if (random_object_position_y > 7) {
      generate_random_objects();
      threshold = threshold - 10;
      if (threshold < 150) {
        threshold = 150;
      }
    }

  }

}

void check_direction() {
  if (analogRead(hpin) > 512)
    direction = "left";
  else if (analogRead(hpin) < 512)
    direction = "right";
  else
    direction = "";
}

void window_check() {
  if (x > 7 ) x = 7;
  else if (x < 0) x = 0;
}

void move_sprite() {
  if (direction == "left") x++;
  else if (direction == "right") x--;

  window_check();
}

/* Write a function that generates the random x positions for the three
falling objects, the three objects shouldn't have same x position.
Variable random_object_position_y controls y position of all 3 objects.
Variables random_object1_position_x,random_object2_position_x and
random_object3_position_x controls the x position of 
object1, object2, object3 respectively */

void generate_random_objects() {

  random_object_position_y = 0;

  while (true) {

    random_object1_position_x = random(0, 8);
    random_object2_position_x = random(0, 8);
    random_object3_position_x = random(0, 8);

    if (random_object1_position_x != random_object2_position_x &&
        random_object2_position_x != random_object3_position_x &&
        random_object3_position_x != random_object1_position_x) {
      break;
    }
  }
}