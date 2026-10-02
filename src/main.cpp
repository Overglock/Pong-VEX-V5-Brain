#include "main.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Controller partner(pros::E_CONTROLLER_PARTNER);

using namespace std;
int player_score = 0;
int opponent_score = 0;
const int fat = 480;
const int notfat = 272; // it should be 240 but if i change it its gonna mess a bunch of things up

class Ball {
public:
float x, y;
int speed_x, speed_y;
int radius;
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
    pros::screen::fill_circle(x, y, radius);
}

void Update() {
    x += speed_x;
    y += speed_y;

    if(y + radius*7 >= notfat || y - radius <= 0) { // Don't ask me why its rad*7. Remember "It just works"
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
    y = notfat/2;

    pride();
    int speed_choices[2] = {-4,4};
    speed_x = speed_choices[rand() % 2];
    speed_y = speed_choices[rand() % 2];
  }
};

class Racket {

protected:
pros::Controller& master;
void LimitMovement() {
    if(y <= 0) {
            y = 0;
            speed = maxspeed;
        }
    if(height + 32 >= notfat) {// Why plus 32, Say it with me "It just works"
            height = notfat;
            speed = -maxspeed;
        }
}

public:
float x, y;
float width, height;
float speed = 0.0f;
float acceleration = 0.125f;   // how fast you accelerate per second/frame
float maxspeed = 1.5f;  // you can figure it out
float damping = 0.900f; // slows when no key pressed (tune)

std::uint32_t color = pros::c::COLOR_WHITE;

    Racket(pros::Controller& m) : master(m) {}

    void CHEATS(std::uint32_t& opponent_color) {
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
            player_score ++;
        }

        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
            opponent_score --;
        }

        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
            acceleration = 0.25;
            height = y + 20;
            x = fat - 7;
            
        } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
            acceleration = 0.1;
            height = y + 30;
            x = fat - 10;
           
        } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y) && master.get_digital(pros::E_CONTROLLER_DIGITAL_A)){
            acceleration = 0.125;
            height = y + 30;
            x = fat - 10;
            
        } else {
            acceleration = 0.125;
            height = y + 30;
            x = fat - 10;
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
        pros::screen::fill_rect(x, y, width, height);
    }

    

    void Update() {
    if (master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) > 0) {   
        speed -= acceleration;
    } else if (master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) < 0) {
        speed += acceleration;
    }
    if (master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) == 0) {
        speed *= damping; // momentum
    }
    y += speed;
    height = y + 30; // i hate the pros coordinate system

    LimitMovement();
    }

};

class Opponent: public Racket {
    public:

    bool game_mode;

    Opponent(pros::Controller& c) : Racket(c) {}

    void ChangeGameMode() {
        if (partner.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2) && partner.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
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
            width = 8;
            height = y + 20;
            x = 5;
        } else if (partner.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT) && !partner.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
            acceleration = 0.1;
            width = 10;
            height = y + 30;
            x = 5;
        } else if (partner.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT) && partner.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
            acceleration = 0.125;
            width = 10;
            height = y + 30;
            x = 5;
        } else {
            acceleration = 0.125;
            width = 10;
            height = y + 30;
            x = 5;
        }
    }

    void UpdateP2() {
    if (partner.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) > 0) {   
        speed -= acceleration;
    } else if (partner.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) < 0) {
        speed += acceleration;
    }   
    if (partner.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) == 0) {
        speed *= damping; // momentum
    }

    y += speed;
    height = y + 30; // i hate the pros coordinate system

        LimitMovement();
    }

    void UpdateAlgorithim(int ball_y, std::uint32_t ball_color) { 
        bool isBlack = (ball_color == pros::c::COLOR_BLACK);
        if (isBlack) {
            pride(); //epilepsy 
        } else {  
            if (y + height/2 > ball_y) { //ball is above
                    speed -= acceleration;
            }

            if(y + height/2 <= ball_y) { //ball is below
                    speed += acceleration;
            }
            y += speed;
            height = y + 30;
        }
        LimitMovement();
    }
};

Ball ball;
Racket player(master);
Opponent opponent(partner);


void initialize() {
    std::srand(pros::millis());

    ball.radius = 5;
    ball.x = fat/2;
    ball.y = notfat/2;
    ball.speed_x = 2;
    ball.speed_y = 2;

    player.width = fat - 5;
    player.height = 50;
    player.x = fat - 10;
    player.y = notfat/2 - player.height/2;
    player.speed = 0;

    opponent.height = opponent.y + 30;
    opponent.width = 10;
    opponent.x = 5;
    opponent.y = notfat/2  - 15;
    opponent.speed = 0;
    opponent.game_mode = true;

}


void disabled() {}

void competition_initialize() {}

void autonomous() {}

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

        // Don't worry, I hate this as much as you
        if (ball.x >= player.x - ball.radius && ball.y >= player.y && ball.y <= player.height) {
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
            
        }

        if (ball.x <= opponent.x + opponent.width + ball.radius && ball.y >= opponent.y && ball.y <= opponent.height) {
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

        }

        //This for Drawing
        pros::screen::set_eraser(pros::c::COLOR_BLACK);
        pros::screen::erase();
        ball.Draw();
        opponent.Draw();
        player.Draw();
        pros::screen::set_pen(pros::c::COLOR_WHITE);
        pros::screen::draw_line(fat/2,0,fat/2,notfat);
        pros::screen::print(pros::E_TEXT_LARGE, 113, 5, "%d", opponent_score);
        pros::screen::print(pros::E_TEXT_LARGE, 353, 5, "%d", player_score);

		pros::delay(20); // Run for 20 ms then update
	}
}

