#include "Motor.h"

using namespace myRIO;

// work only for PWM0 & PWM1
Motor::Motor(uint32_t port) : speed(0), direction(CCW),
		pin(port==PWM0?A2:A3)
{
	channel = new PWM(port, 1e3, speed);
	MyRio_ReturnIfNotSuccess(status,
			"PWM initialisation error");
	enc = new Encoder(port!=PWM0); // if port==PWM0 : (port!=PWM0)=0=ENCA
	MyRio_ReturnIfNotSuccess(status,
			"Encoder initialisation error");
}

void Motor::setSpeed(uint8_t speed) {
	this->speed = speed;
	channel->setDutyCycle(speed);
}

uint8_t Motor::getSpeed() {
	return speed;
}

void Motor::setDirection(bool dir) {
	this->direction = dir;
	DIO::writePin(pin, dir);
}

void Motor::setInterrupt(std::function<void(long, bool)> func, unsigned int threshold) {
	enc->startThread(func, threshold);
}

Motor::~Motor() {
	channel->setDutyCycle(0);
	enc->stopThread();
}
