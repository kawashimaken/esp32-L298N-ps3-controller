
// get and install the library for your PS3 controller from https://github.com/jvpernis/esp32-ps3
#include <Ps3Controller.h>

int player = 0;
int battery = 0;

// Motor Pins
// motor B
int IN3 = 13;  //M2  Motor B+
int IN4 = 12;  //M1  Motor B-
// motor A
int IN1 = 17;  //M4  Motor A+
int IN2 = 16;  //M3  Motor A-

//
int enB = 18; //GPIO18
int enA = 19; //GPIO19
// virtual channel
int channel_for_enA = 1;
int channel_for_enB = 0;

int motorSpeedA = 255;
int motorSpeedB = 255;

const int LEDC_TIMER_BIT = 8;   // PWM (8bit:0〜255、10bit:0〜1023)
const int LEDC_BASE_FREQ = 100; // Freqency(Hz)
const int VALUE_MAX = 255;      // Max value for PWM

void connect_ps3_controller() {
  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  // PS3 controller MAC address
  // use sixaxispair to update to the following bluetooth address  (in Windows）
  Ps3.begin("01:02:03:04:05:06");

  Serial.println("PS3 Controller is Ready.");
  Ps3.setPlayer(player);

}
void setup ( ) {
  Serial.begin(115200);
  connect_ps3_controller();
  // Pins to control motors as output
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // make A,B motor enable pin as output too
  pinMode(enB, OUTPUT);
  pinMode(enA, OUTPUT);
  //
  ledcSetup(channel_for_enA, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcSetup(channel_for_enB, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  // link the virtual channel to each enable pin
  ledcAttachPin(enB, channel_for_enB);
  ledcAttachPin(enA, channel_for_enA);

}

void notify()
{
  //--------------- Analog D-pad button events ----------------
  if ( abs(Ps3.event.analog_changed.button.up) ) {
    Serial.print("Pressing the up button: ");
    Serial.println(Ps3.data.analog.button.up, DEC);
    leftForward(Ps3.data.analog.button.up);
    rightForward(Ps3.data.analog.button.up);
  }

  if ( abs(Ps3.event.analog_changed.button.right) ) {
    Serial.print("Pressing the right button: ");
    Serial.println(Ps3.data.analog.button.right, DEC);
    rightForward(Ps3.data.analog.button.right);
  }

  if ( abs(Ps3.event.analog_changed.button.down) ) {
    Serial.print("Pressing the down button: ");
    Serial.println(Ps3.data.analog.button.down, DEC);
    leftBackward(Ps3.data.analog.button.down);
    rightBackward(Ps3.data.analog.button.down);
  }

  if ( abs(Ps3.event.analog_changed.button.left) ) {
    Serial.print("Pressing the left button: ");
    Serial.println(Ps3.data.analog.button.left, DEC);
    leftForward(Ps3.data.analog.button.left);
  }

}

void onConnect() {
  Serial.println("Connected.");
}

// Left wheel
void leftBackward(int volume) {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH );
  ledcWrite(channel_for_enB, volume);
  delay(10);
}

void leftForward(int volume) {
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  ledcWrite(channel_for_enB, volume);
  delay(10);
}

// Right wheel
void rightBackward(int volume) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  ledcWrite(channel_for_enA, volume);
  delay(10);
}
void rightForward(int volume) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  ledcWrite(channel_for_enA, volume);
  delay(10);
}

void loop () {
  if (!Ps3.isConnected()) {
    return;
  } 
  delay(1000);
}
