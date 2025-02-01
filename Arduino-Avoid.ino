#include <Servo.h> 

Servo Myservo;

#define trigPin 9           // Trig Pin Of HC-SR04
#define echoPin 8           // Echo Pin Of HC-SR04
#define MLa 4               // left motor 1st pin
#define MLb 5               // left motor 2nd pin
#define MRa 6               // right motor 1st pin
#define MRb 7               // right motor 2nd pin

long duration, distance;

void setup() {
  Serial.begin(9600);
  pinMode(MLa, OUTPUT);     // Set Motor Pins As O/P
  pinMode(MLb, OUTPUT);
  pinMode(MRa, OUTPUT);
  pinMode(MRb, OUTPUT);
  pinMode(trigPin, OUTPUT); // Set Trig Pin As O/P To Transmit Waves
  pinMode(echoPin, INPUT);  // Set Echo Pin As I/P To Receive Reflected Waves
  Myservo.attach(10);
}

void loop() {
  // Read distance in front
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);   
  digitalWrite(trigPin, HIGH);       // Transmit Waves For 10us
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  // Stops the trigPin

  duration = pulseIn(echoPin, HIGH); // Receive Reflected Waves
  distance = duration * 0.034 / 2;   // Get Distance

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 15) {               // Condition For Absence Of Obstacle            
    Myservo.write(90);               // Rotate Servo to 90 degrees
    digitalWrite(MRb, HIGH);         // Move Forward
    digitalWrite(MRa, LOW);
    digitalWrite(MLb, HIGH);                                
    digitalWrite(MLa, LOW);                                                       
  } else if (distance <= 15 && distance > 0) {  // Condition For Presence Of Obstacle
    digitalWrite(MRb, LOW);        // Stop                
    digitalWrite(MRa, LOW);
    digitalWrite(MLb, LOW);                                
    digitalWrite(MLa, LOW);
    delay(100);

    // Scanning all directions
    int foundPath = 0; 
    for (int angle = 0; angle <= 180; angle += 45) { // Scan Left, Right, and Center
      Myservo.write(angle);        
      delay(500);
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);   
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH); 
      distance = duration * 0.034 / 2;   
      if (distance > 15) {
        foundPath = angle;
        break;
      }
    }

    // Move to the detected direction if found
    if (foundPath == 0) {             // Move straight forward
      digitalWrite(MRb, HIGH);
      digitalWrite(MRa, LOW);
      digitalWrite(MLb, HIGH);                                
      digitalWrite(MLa, LOW); 
    } else if (foundPath == 45) {     // Move slightly right
      digitalWrite(MRb, HIGH);
      digitalWrite(MRa, LOW);
      digitalWrite(MLb, LOW);                                
      digitalWrite(MLa, LOW); 
    } else if (foundPath == 135) {    // Move slightly left
      digitalWrite(MRb, LOW);
      digitalWrite(MRa, LOW);
      digitalWrite(MLb, HIGH);                                
      digitalWrite(MLa, LOW); 
    } else if (foundPath == 90) {     // Move forward
      digitalWrite(MRb, HIGH);
      digitalWrite(MRa, LOW);
      digitalWrite(MLb, HIGH);                                
      digitalWrite(MLa, LOW); 
    } else {
      // No path found, move backward and re-scan
      digitalWrite(MRb, LOW);        // Move Backward             
      digitalWrite(MRa, HIGH);
      digitalWrite(MLb, LOW);                                
      digitalWrite(MLa, HIGH);
      delay(500);
      digitalWrite(MRb, LOW);        // Stop                
      digitalWrite(MRa, LOW);
      digitalWrite(MLb, LOW);                                
      digitalWrite(MLa, LOW);
      delay(100);  

      for (int angle = 0; angle <= 180; angle += 45) { // Re-scan after moving backward
        Myservo.write(angle);        
        delay(500);
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);   
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        duration = pulseIn(echoPin, HIGH); 
        distance = duration * 0.034 / 2;   
        if (distance > 15) {
          break;
        }
      }

      digitalWrite(MRb, HIGH); // Move Forward to the detected direction
      digitalWrite(MRa, LOW);
      digitalWrite(MLb, HIGH);                                
      digitalWrite(MLa, LOW); 
    }
  }

  delay(500); // الانتظار لمدة 0.5 ثانية قبل التكرار التالي
}
