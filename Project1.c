    /***********************************************************************
     * Auther: (Maren Gingerich and Mingi Lee)                             *
     *                Feb 17, 2020                                         *
     ***********************************************************************/

#include <MyroC.h>
#include <stdio.h>
//DANCE
/* This is the short dance w/music that will play three times after the robot 
 *   has completed 10 moves
 *
 * note: there will be "motion for robot superceded by later motion" messages in
 *   the terminal, but this does not affect the robot's overall motion
 */
void dance(void){
  rBackward(1.0,-0.5);
  rBeep(0.5,523);
  rTurnLeft(1.0,-0.5);
  rBeep(0.5,659);
  rBackward(1.0,-0.5);
  rBeep(0.5,784);
  rTurnRight(1.0,-0.5);
  rBeep(0.6,1047);

}


 
// GENERAL MOTION
/* This function uses an infinite while loop to instruct the robot to keep 
 * moving and avoid obstacles. The robot will do its best to avoid obstacles.
 * Every 10 moves it does the dance, and if it has to turn twice in the same
 * direction, it  will beep and turn for longer before continuing motion.
 */
int motion_default (void){

  int left, right;
  left = rGetIRTxt ("left", 3);
  right = rGetIRTxt ("right", 3);
  int count_move = 0;
  int count_turn = 0;
  while (1)
    {
      /* if the count_turn counter ever reaches -2 or 2, it must have turned 
       * in the same direction twice consecutively. When this happens, the robot       * will beep and turn right for 2 seconds.
       */
      while (count_turn == 2 || count_turn == -2){
        printf("The robot has made 2 consecutive turns in the same direction. Turning right and beeping.\n");
        rBeep(2.0, 1047);
        rTurnRight(0.7, 2.0);
        count_turn = 0;
      }
      
      /* DANCE
       * if the count_move counter ever reaches 10, it must have made 10 
       * consecutive moves. When this happens, the robot will do its dance 
       * 3 times
       */
      if (count_move % 10 == 0 && count_move != 0){
         printf("The robot has made 10 consecutive moves. Now performing short dance 3 times.\n");
        for(int i = 0; i < 3; i++){
          dance();
        }
        count_move = 0;
      }
      
      //if no obstacle on left or right, move forward
      else if (left == 0 && right == 0){
        printf("No obstacle on the right or left. Moving forward.\n");
        rForward(1.0, 1.0);
        left = rGetIRTxt ("left", 3);
        right = rGetIRTxt ("right", 3);
        count_move++;
        count_turn = 0;
      }
      
      // if left is blocked but not right, turn right
      else if (left && right == 0)
        {
          printf("Obstacle on the left. Turning right.\n");
          rTurnRight(1.0, 1.0);
          left = rGetIRTxt ("left", 3);
          right = rGetIRTxt ("right", 3);
          count_move++;
          count_turn++;
        }
      
      //if right is blcoked but not left, turn left
      else if (left == 0 && right)
        {
          printf("Obstacle on the right. Turning left.\n");
          rTurnLeft(1.0, 1.0);
          left = rGetIRTxt ("left", 3);
          right = rGetIRTxt ("right", 3);
          count_move++;
          count_turn--;
        }
      
      //if both left and right are blocked, turn right
      else if (left && right)
        {
          printf("Obstacle on both sides; turning right\n");
          rTurnRight(1.0, 1.0);
          left = rGetIRTxt ("left", 3);
          right = rGetIRTxt ("right", 3);
          count_move++;
          count_turn++;
        }
    }
  return 0;
} //motion_default

//MAIN
/* connect to robot, call forward_default
 */
int main (void) {

  rConnect("/dev/rfcomm0");
  printf("Robot connected.\n");
  motion_default();
  rDisconnect();

  return 0;
} //main


