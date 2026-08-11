#include "main.h"
/*#include <cstdlib>
#include "pros/colors.h"
#include "pros/misc.h"
#include "pros/screen.hpp"
#include <cmath> */

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Controller partner(pros::E_CONTROLLER_PARTNER);

using namespace std;
// if (ball.x + ball.radius >= player.x - player.width)
int player_score = 0;
int opponent_score = 0;
const int fat = 480;
const int notfat = 200;

class Ball {
public:
float x, y;
int speed_x, speed_y;
int radius;
//pros::Color color = pros::Color::white;
std::uint32_t color = pros::c::COLOR_WHITE;
//You're welcome Phoebe
void pride() {
static const std::uint32_t gay[21] = {
    pros::c::COLOR_DARK_GRAY,
    pros::c::COLOR_MAROON,
    pros::c::COLOR_ORANGE,
    pros::c::COLOR_DARK_GREEN,
    pros::c::COLOR_DARK_BLUE,
    pros::c::COLOR_DARK_VIOLET,
    pros::c::COLOR_SADDLE_BROWN,
    pros::c::COLOR_GRAY,
    pros::c::COLOR_RED,
    pros::c::COLOR_GOLD,
    pros::c::COLOR_LIME,
    pros::c::COLOR_BLUE,
    pros::c::COLOR_VIOLET,
    pros::c::COLOR_BROWN,
    pros::c::COLOR_LIGHT_GRAY,
    pros::c::COLOR_PINK,
    pros::c::COLOR_YELLOW,
    pros::c::COLOR_GREEN,
    pros::c::COLOR_SKY_BLUE,
    pros::c::COLOR_PURPLE,
    pros::c::COLOR_BEIGE
};


    color = gay[rand() % 21];
}

void Draw() {
    pros::screen::set_pen(color); 
    pros::screen::draw_circle(x, y, radius);
}

void Update() {
    x += speed_x;
    y += speed_y;

    if(y + radius >= notfat || y - radius <= 0) {
        pride();
        speed_y *= -1;
    }
    if(x + radius >= fat) { // If the ball hits the right side the Opponent Scores
        opponent_score ++;
        ResetBall();
    }

    if (x - radius <= 0) { // If ball hits left side P1 wins
        player_score ++;
        ResetBall();
    }
  } 
  void ResetBall() {
    x = fat/2;
    y = fat/2;

    pride();
    int speed_choices[2] = {-7,7};
    speed_x = speed_choices[rand() % 2];
    speed_y = speed_choices[rand() % 2];
  }
};

class Racket {

protected:
pros::Controller& master;
//THIS FUNCTION IS THE GOAT
void LimitMovement() {
    if(y <= 0) {
            y = 0;
            speed = maxspeed;
        }
        if(y + height >= notfat) {
            y = notfat - height;
            speed = -maxspeed;
        }
}

public:
float x, y;
float width, height;
float speed = 0.0f;
float acceleration = 0.125f;   // how fast you accelerate per second/frame
float maxspeed = 1.5f;   // top speed
float damping = 0.225f; // slows when no key pressed (tune)
std::uint32_t color = pros::c::COLOR_WHITE;

    Racket(pros::Controller& m) : master(m) {}

    void CHEATS(std::uint32_t& opponent_color) {
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
            player_score ++;
        }

        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
            opponent_score --;
        }

        //if (IsKeyPressed(KEY_SLASH)) {
        //    opponent_color = BLACK;
        //} 

        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
            /*if (IsKeyPressed(KEY_DOWN)) {
                y = y + 240;
            } else if (IsKeyPressed(KEY_UP)) {
                y = y - 240;
            } */
            acceleration = 0.25;
            maxspeed = 6;
            width = 5;
            height = 25;
            x = fat - width - 5;
        } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
            acceleration = 0;
            width = 10;
            height = 30;
            x = fat - width - 5;
        } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y) && master.get_digital(pros::E_CONTROLLER_DIGITAL_A)){
            acceleration = 0.125;
            maxspeed = 6;
            //ball_color = BLACK;
            width = 10;
            height = 30;
            x = fat - width - 5;
        } else {
            acceleration = 0.125;
            maxspeed = 6;
            width = 10;
            height = 30;
            x = fat - width - 5;
        }
    }
    
    void pride() {
static const std::uint32_t gay[21] = {
    pros::c::COLOR_DARK_GRAY,
    pros::c::COLOR_MAROON,
    pros::c::COLOR_ORANGE,
    pros::c::COLOR_DARK_GREEN,
    pros::c::COLOR_DARK_BLUE,
    pros::c::COLOR_DARK_VIOLET,
    pros::c::COLOR_SADDLE_BROWN,
    pros::c::COLOR_GRAY,
    pros::c::COLOR_RED,
    pros::c::COLOR_GOLD,
    pros::c::COLOR_LIME,
    pros::c::COLOR_BLUE,
    pros::c::COLOR_VIOLET,
    pros::c::COLOR_BROWN,
    pros::c::COLOR_LIGHT_GRAY,
    pros::c::COLOR_PINK,
    pros::c::COLOR_YELLOW,
    pros::c::COLOR_GREEN,
    pros::c::COLOR_SKY_BLUE,
    pros::c::COLOR_PURPLE,
    pros::c::COLOR_BEIGE
};


    color = gay[rand() % 21];
}

    void Draw() {
        pros::screen::set_pen(color); 
        pros::screen::draw_rect(x, y, width, height);
    }

    

    void Update() {
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {   
        speed -= acceleration;
    }
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
        speed += acceleration;
    }
    if (!master.get_digital(pros::E_CONTROLLER_DIGITAL_X) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
        speed *= damping; // momentum
    }

    /* if (speed >  maxspeed) {
        speed =  maxspeed;
    }
    if (speed < -maxspeed) {
        speed = -maxspeed;
    }*/
    y += speed;

    LimitMovement();
    }

};

class Opponent: public Racket {
    public:

    bool game_mode;

    Opponent(pros::Controller& c) : Racket(c) {}

    void ChangeGameMode() {
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2) && master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
            game_mode = !game_mode;
        }
    }

    void CHEATS(std::uint32_t& player_color) {
        if (partner.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
            player_score --;
        }

        if (partner.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
            opponent_score ++;
        }

        if (partner.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
            player_color = pros::c::COLOR_BLACK;
        }

        if (partner.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT) && !partner.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
            acceleration = 0.25;
            maxspeed = 6;
            width = 5;
            height = 25;
            x = 5;
        } else if (partner.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT) && !partner.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
            acceleration = 0;
            width = 10;
            height = 30;
            x = 5;
        } else if (partner.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT) && partner.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
            acceleration = 0.125;
            maxspeed = 6;
            //ball_color = BLACK;
            width = 10;
            height = 30;
            x = 5;
        } else {
            acceleration = 0.125;
            maxspeed = 6;
            //ball_color = BLACK;
            width = 10;
            height = 30;
            x = 5;
        }
    }

    void UpdateP2() {
    if (partner.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {   
        speed -= acceleration;
    }
    if (partner.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        speed += acceleration;
    }
    if (!partner.get_digital(pros::E_CONTROLLER_DIGITAL_UP) && !partner.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        speed *= damping; // momentum
    }

    /*if (speed >  maxspeed) {
        speed =  maxspeed;
    }
    if (speed < -maxspeed) {
        speed = -maxspeed;
    }*/
    y += speed;


        LimitMovement();
    }

    void UpdateAlgorithim(int ball_y, std::uint32_t ball_color) { 
        bool isBlack = (ball_color == pros::c::COLOR_BLACK);
        if (isBlack) {
            pride();
        } else {  
            if (y + height/2 > ball_y) { //ball is above
                    speed -= acceleration;
            }

            if(y + height/2 <= ball_y) { //ball is below
                    speed += acceleration;
            }
            y += speed;
        }
        /*if (speed >  maxspeed) {
        speed =  maxspeed;
        }

        if (speed < -maxspeed) {
        speed = -maxspeed;
        }*/
        LimitMovement();
    }
};

Ball ball;
Racket player(master);
Opponent opponent(partner);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    std::srand(pros::millis());

    ball.radius = 5;
    ball.x = fat/2;
    ball.y = notfat/2;
    ball.speed_x = 2;
    ball.speed_y = 2;

    player.width = 10;
    player.height = 30;
    player.x = fat - player.width - 5;
    player.y = notfat/2 - player.height/2;
    player.speed = 0;

    opponent.height = 30;
    opponent.width = 10;
    opponent.x = 5;
    opponent.y = notfat/2  - opponent.height/2;
    opponent.speed = 0;
    opponent.game_mode = true;

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

 
	while (true) {
        //For updating ball position
        ball.Update();
        player.Update();

        //For Choosing Game Mode
        opponent.ChangeGameMode();
        
        if (opponent.game_mode == false) {
            opponent.UpdateP2();
        } else {
            opponent.UpdateAlgorithim(ball.y, ball.color);
        }
        // cHEATING
        player.CHEATS(opponent.color);
        opponent.CHEATS(player.color);

        //For Checking Collisions
        if (ball.x >= player.x - ball.radius && ball.y >= player.y && ball.y <= player.y + player.height) {
            if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y) && master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
            ball.color = pros::c::COLOR_BLACK;
            }
            player.pride();
            ball.speed_y = player.speed;
            if (player.speed <= 0.1 && player.speed >= -0.1) {
                ball.speed_x = -16;
            } else {
                if (player.speed < 0) {
                ball.speed_x = ball.speed_x/player.speed;
                } else if (player.speed > 0) {
                    ball.speed_x = ball.speed_x/-player.speed;
                }
                if (ball.speed_x > -1) {
                    ball.speed_x = -3;
                }

                
            }

            if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
                ball.speed_x *= 2; 
            }
            /*if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
            ball.speed_x = -16;
        } else {
            ball.speed_x = -7;
        }*/
            
        }

        if (ball.x <= opponent.x + opponent.width + ball.radius && ball.y >= opponent.y && ball.y <= opponent.y + opponent.height) {
            if (partner.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT) && partner.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
            ball.color = pros::c::COLOR_BLACK; 
            }
            opponent.pride();
            ball.speed_y = opponent.speed;
            if (opponent.speed <= 0.1 && opponent.speed >= -0.1) {
                ball.speed_x = 16;
            } else {
                if (opponent.speed < 0) {
                ball.speed_x = ball.speed_x/-opponent.speed;
                } else if (opponent.speed > 0) {
                    ball.speed_x = ball.speed_x/opponent.speed;
                }
                if (ball.speed_x < 1) {
                    ball.speed_x = 3;
                }

                
            }
            
            if (partner.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
                ball.speed_x *= 2; 
            }
            /*if (partner.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
            ball.speed_x = 16;
        } else {
            ball.speed_x = 7;
        } */
            
        }

        //This for Drawing
        pros::screen::set_eraser(pros::c::COLOR_BLACK);
        pros::screen::erase();
        pros::screen::set_pen(pros::c::COLOR_WHITE);
        pros::screen::draw_line(fat/2,0,fat/2,notfat);
        ball.Draw();
        opponent.Draw();
        player.Draw();
        pros::screen::print(pros::E_TEXT_LARGE, 113, 5, "%d", opponent_score);
        pros::screen::print(pros::E_TEXT_LARGE, 353, 5, "%d", player_score);

		pros::delay(20);                               // Run for 20 ms then update
	}
}

/*L0rd_0f_5h@d3s*/