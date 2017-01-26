int motorLeft[3]={3,4};
int motorRight[3]={5,6};
int led=13;
int PWM(int input)
{
  return (input/5)*1023;
}
void forward(int pwm=1023)
{
    analogWrite(motorLeft[2], pwm);
    analogWrite(motorRight[2], pwm);
    
    digitalWrite(motorLeft[0], HIGH);
    digitalWrite(motorLeft[1], LOW);
    digitalWrite(motorRight[0], HIGH);
    digitalWrite(motorRight[1], LOW);
}

void backward(int pwm=1023)
{
    analogWrite(motorLeft[2], pwm);
    analogWrite(motorRight[2], pwm);

    digitalWrite(motorLeft[1], HIGH);
    digitalWrite(motorLeft[0], LOW);
    digitalWrite(motorRight[1], HIGH);
    digitalWrite(motorRight[0], LOW);
}
void left(int pwm=1023)
{
    analogWrite(motorLeft[2], pwm);
    analogWrite(motorRight[2], pwm);
    
    digitalWrite(motorLeft[0], HIGH);
    digitalWrite(motorLeft[1], LOW);
    digitalWrite(motorRight[1], HIGH);
    digitalWrite(motorRight[0], LOW);
}

void right(int pwm=1023)
{
    analogWrite(motorLeft[2], pwm);
    analogWrite(motorRight[2], pwm);
    
    digitalWrite(motorLeft[1], HIGH);
    digitalWrite(motorLeft[0], LOW);
    digitalWrite(motorRight[0], HIGH);
    digitalWrite(motorRight[1], LOW);
}
void Stop()
{
    analogWrite(motorLeft[2], 0);
    analogWrite(motorRight[2], 0);

    digitalWrite(motorLeft[1], HIGH);
    digitalWrite(motorLeft[0], HIGH);
    digitalWrite(motorRight[0], HIGH);
    digitalWrite(motorRight[1], HIGH);
}
void blink()
{
    int voltage=1;
    for (int i=0;i<4;i++)
    {
        digitalWrite(led,voltage);
        voltage=1-voltage;
        delay(1000);
    }
    Stop();
}

void setup()
{
  Serial.begin(9600);

  pinMode(led,OUTPUT);
  for(int i = 0;i < 2;i++)
  pinMode(motorLeft[i],OUTPUT);
   for(int i = 0;i < 2;i++)
  pinMode(motorRight[i],OUTPUT);

}
char input;
void loop()
{
    if(Serial.available()>0)
    {
        input=Serial.read();
 
        switch(input)
        {
          case 'w':forward();
                    break;
          case 's':backward();
                    break;
          case 'a':left();
                    break;
          case 'd':right();
                    break; 
          case 'b':Stop();
                   blink();
                   break;
          default:Stop();
                  break;                  
        };
        Serial.println(input);
    }

}


/*

int calc()
{
    int num=0,x=0;
 
    for(x;x<=j;x++)
          num=num+(buff[x]-48)*pow(10,j-x);
 
    return num;
}
 
int input,num;
int buff[5];
int j=-1;
 
void loop()
{
    if(Serial.available()>0)
    {
        input=Serial.read();
 
        if(input==',')
        {
            num=calc();
            j=-1;
     
 
        switch(num)
        {
          case 'w':forward();
                    break;
          case 's':backward();
                    break;
          case 'a':left();
                    break;
          case 'd':right();
                    break; 
          case 'b':Stop();
                   blink();
                   break;
          default:Stop();
                  break;                  
        };
            
            Serial.println(num);
        }
        else
        {
            j++;
            buff[j]=input;
        }
    }
}
*/
