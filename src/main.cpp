#include <Arduino.h>
#include <Servo.h>
#include <IRremote.h>
Servo servoM;
IRrecv sensor_pin(10);
decode_results results;
 int motor1 = 6;
 int motor2 = 7;
enum Direction
{
    FOWARD,
    BACK,
    STOP
};
Direction motor_direction = STOP;
 int motor_speed_in = 5;
int motor_speed = 90;
int back_motor_speed = 80;

 void changeSpeed(int speed, int backspeed)
{
    motor_speed = speed;
    back_motor_speed = backspeed;
    if (motor_direction == BACK)
    {
        analogWrite(motor_speed_in, back_motor_speed);
    }
    else if (motor_direction == FOWARD)
    {
        analogWrite(motor_speed_in, motor_speed);
    }
}

void startMotor(Direction type)
{

    if (motor_direction != type)
    {
        if (type == FOWARD)
        {
            analogWrite(motor_speed_in, motor_speed);
            digitalWrite(motor1, LOW);
            digitalWrite(motor2, HIGH);
        }
        else if (type == BACK)
        {
            analogWrite(motor_speed_in, back_motor_speed);
            digitalWrite(motor1, HIGH);
            digitalWrite(motor2, LOW);
        }
        motor_direction = type;
    }
    else
    {
        digitalWrite(motor1, LOW);
        digitalWrite(motor2, LOW);
        motor_direction = STOP;
    }
}
void setup()
{
    servoM.attach(9);
    sensor_pin.enableIRIn();
    Serial.begin(9600);
    servoM.write(110);
    pinMode(motor1, OUTPUT);
    pinMode(motor2, OUTPUT);
    pinMode(motor_speed_in, OUTPUT);
}
void loop()
{

    if (sensor_pin.decode(&results))
    {
        Serial.println(results.value, HEX);

        switch (results.value)
        {
        case 0xFFE01F:
        case 0x807FE01F:
            servoM.write(150);
            break;
        case 0xFFE21D:
        case 0x807FE21D:
            servoM.write(70);
            break;
        case 0x807F58A7:
        case 0xFF58A7:
            startMotor(FOWARD);
            break;
        case 0x807F12ED:
        case 0xFF12ED:
            startMotor(BACK);
            break;
        case 0xFF2AD5:
        case 0x807F2AD5:
            changeSpeed(90, 80);
            break;
        case 0xFF6897:
        case 0x807F6897:
            changeSpeed(200, 160);
            break;
        case 0xFFA857:
        case 0x807FA857:
            changeSpeed(255, 200);
            break;
        default:
            servoM.write(110);
            break;
        }
    }

    sensor_pin.resume();
}
