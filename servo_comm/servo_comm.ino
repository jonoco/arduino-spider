#include <Servo.h>


String command;
Servo servo[4][3];
int leg_pos[4][3];
const int move_rate = 10;

//define servos' ports
const int servo_pin[4][3] = { {13, 2, 3}, {12, 11, 10}, {9, 8, 7}, {6, 5, 4} };

void setup() {
    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);

    send("Ready");
}


void loop() {
    read_serial();

    delay(10);
}


void read_serial()
{
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');

    if (command.equals("init")) {
        initialize();
    }
    else if (command.equals("light")) {
       test_light();
    }
    else if (command.equals("reboot")) {
        reboot();
    }
    else if (command.equals("read")) {
        servo_read();
    }
    else if (command.equals("detach")) {
        servo_detach();
    }
    else if (command.substring(0, 4) == "test") {
      int pin = command.substring(4, 6).toInt();

      servo_test(pin, 60);
      delay(2000);
      servo_test(pin, 90);

    }
    else if (command.substring(0, 2) == "qq") {
      int leg = command.substring(2, 3).toInt();
      int joint = command.substring(3, 4).toInt();
      int pos = command.substring(4, 7).toInt();

      String msg = "Moving ";
      msg += leg;
      msg += " ";
      msg += joint;
      msg += " to ";
      msg += pos;
      msg += " | pin ";
      msg += servo_pin[leg][joint];
      send(msg);

      leg_pos[leg][joint] = pos;
      servo[leg][joint].write(leg_pos[leg][joint]);
    }
    else{
      send("Invalid command");
      send(command);
    }
  }
}


void servo_attach()
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      String msg = "Attaching ";
      msg += i;
      msg += " ";
      msg += j;
      msg += " to pin ";
      msg += servo_pin[i][j];
      send(msg);

      servo[i][j].attach(servo_pin[i][j]);
      delay(2000);
    }
  }
}


void servo_detach()
{
  send("Detaching servos...");

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      servo[i][j].detach();
      delay(100);
    }
  }
}


void servo_reset()
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      String msg = "Moving ";
      msg += j;
      msg += " ";
      msg += i;
      msg += " to ";
      msg += 90;

      send(msg);

      servo[i][j].write(90);
      leg_pos[i][j] = 90;
      delay(100);
    }
  }
}


void servo_read()
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      String msg = "Servo ";
      msg += j;
      msg += " ";
      msg += i;
      msg += " is at ";
      msg += servo[i][j].read();
      send(msg);
      delay(10);
    }
  }
}


void servo_test(int pin, int pos)
{
  Servo serv;
  serv.attach(pin);

  String msg = "Testing pin ";
  msg += pin;
  msg += " to ";
  msg += pos;
  send(msg);

  serv.write(pos);
  delay(3000);
  serv.detach();
}


void initialize()
{
  send("Initializing...");
  servo_attach();
  servo_reset();
  send("Initialization complete");
}


void release()
{
  send("Releasing servos...");
  servo_detach();
  send("Release complete");
}


void reboot()
{
  send("Rebooting...");
  servo_reset();
  send("Reboot complete");
}


void test_light()
{
  send("Light up...");

  digitalWrite(LED_BUILTIN, HIGH);
  delay(25);
  digitalWrite(LED_BUILTIN, LOW);
  delay(25);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(25);
  digitalWrite(LED_BUILTIN, LOW);
  delay(25);
}


void send(String msg)
{
  Serial.println(msg);
}
