/*
 * oldmovement.c
 *
 *  Created on: Mar 23, 2022
 *      Author: brodie
 */



/*
 * Movement.c
 *
 *  Created on: Feb 2, 2022
 *      Author: bmbates
 */

#include "open_interface.h"
#include "cyBot_uart.h"
#include "uart-interrupt.h"
#include "Movement.h"
#include "adc.h"
#include "ping_template.h"
#include "servo.h"
#include "sound.h"
#include "cliff.h"

extern volatile int command_flag;
extern volatile char input;
extern volatile char command_bye;
Object outside[10];
int leftToffset = -10;
int rightToffset = +10;
float pAv[90];
int angles[90]; //store all angle for scan
int iraws[90]; //store all Raw IR Vals
bool secScan = false;
bool collide = false;

void addAv(float* added, float* sum){
    *sum += *added;
}
float CalculateAv(float* sum, float* vals){
    float send = *sum / *vals;
    *sum = 0;
    return send;
}

void scan(int start, int end, oi_t *sensor, Object a[]){

    int angle; //current angle
    int size = (end-start)/2;
    float p; //ping sensor data
    int IR; //IR sensor data
    uart_sendChar('\r');
    uart_sendChar('\n');
    char top[] = "Degrees IR Dist (cm)";
    int printi;
    int i;
    for(printi = 0; printi < strlen(top);printi++){
        uart_sendChar(top[printi]);
    }
    char str[10];
    /**
     * Begin Actual Scan:
     * Store Max Distance. If greater than max, make new max. If distance found
     * less than 90% of max distance, new object has been found. Store start, end
     * angle and shortest distance of each object then display object and calculated
     * linear width in putty.
     */
    int objects = 1;

    float min_dist; //At obj
    float max_dist;

    int start_a; //start deg of obj
    int end_a; //end deg of obj
    int mid_angle; //middle angle, to be printed to putty

    float lin_width;
    float max_p = 2000; //OBJECT SCAN DISTANCE

    float sum = 0; //sum for average
    float vals = 0;
    float dist;

    angle = start;
    i = 0; //reset index
    int pIndex = 0; //ping index val
    int counter;

    servo_move(start);
    timer_waitMillis(600);
    int iraw = adc_read();
    for(angle = start; angle <= end; angle+=2){ //ACTUAL SCAN HAPPENS IN HERE
        if(command_flag == 1){
            oi_setWheels(0,0);                   //TODO implement interrupt functionality
            break;
        }
        servo_move(angle);
        timer_waitMillis(50);
        ping_trigger();
//TODO        p = ping_getDistance();
        IR = adc_read();
        angles[i] = angle;
        iraws[i] = IR; //raw ir vals
//TODO        pAv[i] = p;
        i++;
        // Display raw sensor data
        uart_sendChar('\r');
        uart_sendChar('\n');
        sprintf(str, "%d", angle);
        for(printi = 0; printi < strlen(str);printi++){
            uart_sendChar(str[printi]);
        }
        uart_sendChar('\t');
        sprintf(str, "%lf", p);
//        for(printi = 0; printi < strlen(str);printi++){
//            uart_sendChar(str[printi]);
//        }uart_sendChar('\t');
// TODO: Fix the ping value
        dist = (110841 * pow(IR,-1.156));
        sprintf(str, "%lf", dist);
        for(printi = 0; printi < strlen(str);printi++){
            uart_sendChar(str[printi]);
        }
        if(IR< iraw){ iraw=IR; }
    }


    /**
     * Scan through data to find objects using max p AKA max distance
     */
    int x;
    uart_sendChar('\r');
    uart_sendChar('\n');
    char pot[] = "Obj # Angle P Dist (cm) Lin Width (cm)";
    for(printi = 0; printi < strlen(pot);printi++){
        uart_sendChar(pot[printi]);
    }
    bool at_obj = false;
    for(x = 0; x<i;x++){
        dist = (110841 * pow(iraws[x],-1.156));
        if(dist < 110){ //IS OBJECT                        BACKGROUND VAL STORED HERE     && (pAv[x]< 70)
            if(at_obj){ //Look for closest distance and end of obj
                //                if(pAv[x]< max_p*0.9){ //still an object
                //                addAv(&pAv[x], &sum);
                //                if(pAv[x]>max_dist){ max_dist = pAv[x];}
                //                if(pAv[x]<min_dist){
                //
                //                    mid_angle = angles[x];
                //                }
                if(min_dist > dist){
                    min_dist = dist;
                }
                end_a = angles[x];
                //}
            }
            else{ //New Object Found
                //                if(pAv[x]< max_p*0.9){
                start_a = angles[x];
                end_a = angles[x];
                //                addAv(&pAv[x], &sum);
                //                max_dist = pAv[x];
                max_dist = dist;
                min_dist = dist;
                mid_angle = angles[x];
                at_obj = true;

                //  }
            }
        }
        else{ //NO longer an object
            if(at_obj && (end_a != start_a)){
                uart_sendChar('\r');
                uart_sendChar('\n');
                sprintf(str, "%d", objects);
                for(printi = 0; printi < strlen(str);printi++){
                    uart_sendChar(str[printi]); //Print object Num to Putty
                }
                uart_sendChar('\t');


                end_a = angles[x]-2;
                mid_angle = (end_a + start_a) / 2; //angle for object
                a[objects-1].angle = mid_angle;
                sprintf(str, "%d", mid_angle);
                for(printi = 0; printi < strlen(str);printi++){
                    uart_sendChar(str[printi]); //Print mid angle object in Putty
                }
                uart_sendChar('\t');
                //Calculate average

                vals = (end_a - start_a) / 2;
                max_dist = CalculateAv(&sum, &vals); //AVERAGE DISTANCE IS STORED IN MAX_DIST
                sprintf(str, "%lf", min_dist);
                a[objects-1].distance = min_dist;
                for(printi = 0; printi < strlen(str);printi++){
                    uart_sendChar(str[printi]); //Print dist to object in Putty
                }
                uart_sendChar('\t');
                lin_width = ((end_a - start_a)* (3.14/180) * (min_dist)); //calculate lin width
//                lin_width -=5; //offset to account for increased evaluation of distance
                a[objects-1].width = lin_width;
                sprintf(str, "%lf", lin_width);
                for(printi = 0; printi < strlen(str);printi++){
                    uart_sendChar(str[printi]); //Print linear width to object in Putty
                }
                uart_sendChar('\t');

                at_obj = false;
                objects++;
            }
            else{
                at_obj = false;
            }
        }
    }
    if(at_obj && (end_a != start_a)){ //End of scan
        at_obj = false;
    }

//    if(objects != 1 ){ //if there are objects [starts at 1, increments upon object detection
//        move_to(a, sensor, objects);
//    }


}


double move_forward(oi_t *sensor_data, double distance_mm){
    char C = cliffDetect(sensor_data);
    char t = tapeDetect(sensor_data);
    double sum = 0; // distance member in oi_t struct is type double
    bool hit = false;
    while (sum < distance_mm && distance_mm > 0) {
    // Cliff and tape detection
        C = cliffDetect(sensor_data);
        t = tapeDetect(sensor_data);
        if(C != 'N' || t != 'N'){
            playSoundCliff();
            move_back(sensor_data, 5);
            uart_sendChar(C);
            uart_sendChar(t);
            return;
        }

        oi_setWheels(100,100); //move forward at full speed
        oi_update(sensor_data);
        hit = Collision(sensor_data);
        if(hit){
            oi_setWheels(100, 100);
            hit = false;
        }
        sum += sensor_data -> distance; // use -> notation since pointer
        lcd_printf("%lf" , sum);
    }
    oi_setWheels(0,0); //stop
    return sum;
}
double move_back(oi_t *sensor_data, double distance_mm){
    double sum = 0; // distance member in oi_t struct is type double
    while (sum >= -1 *distance_mm) {//?
        oi_setWheels(-100,-100); //move forward at full speed
        oi_update(sensor_data);
        sum += sensor_data -> distance; // use -> notation since pointer
        lcd_printf("%lf" , sum);
    }
    oi_setWheels(0,0); //stop
    return sum;
}
double turn_right(oi_t *sensor,double degrees){
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(-50,50); //move forward at full speed
    while (sum > degrees * -.98) {
        oi_update(sensor);
        sum += sensor -> angle; // use -> notation since pointer
        lcd_printf("%lf" , sum);
    }
    oi_setWheels(0,0); //stop
    return sum;
}
double turn_left(oi_t *sensor, double degrees){
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(50,-50); //move forward at full speed
    while (sum < degrees * .9) {
        oi_update(sensor);
        sum += sensor -> angle; // use -> notation since pointer
        lcd_printf("%lf" , sum);
    }
    oi_setWheels(0,0); //stop
    return sum;
}

bool Collision(oi_t *sensor){
    if(sensor->bumpLeft){
        playSoundBump();
        move_back(sensor, 15);
        turn_right(sensor, 90);
        move_forward(sensor, 220);
        turn_left(sensor, 90);
        move_forward(sensor, 220);
        turn_left(sensor, 15);
        if(!secScan){
            secScan = true;
            scan(0, 180, sensor, outside);

        }
//        secScan = true;
        return true;
    }
    else if(sensor->bumpRight){
        move_back(sensor, 15);
        turn_left(sensor, 90);
        move_forward(sensor, 220);
        turn_right(sensor, 90);
        move_forward(sensor,220);
        turn_right(sensor, 15);
        if(!secScan){
        secScan = true;
        scan(0, 180, sensor, outside);

        }
//        secScan = true;
        return true;
    }
    else{
        return false;
    }
}
void move_to(Object obstacles[], oi_t* sensor,int objects){
    int i;
    int printi;
    float small = obstacles[0].width;
    int index = 0;
    int dIndex = 0;
    int smallDis = obstacles[0].distance;
    for(i = 1; i < (objects-1); i++){ // going through ever other element of the array in order to determine smallest width.
        if(small > obstacles[i].width){
            small = obstacles[i].width;
            index = i;
        }
    }
    for(i = 1; i < (objects-1); i++){ // going through ever other element of the array in order to determine smallest distance.
            if(smallDis > obstacles[i].distance){
                smallDis = obstacles[i].distance;
                dIndex = i;
            }
        }
    if((obstacles[dIndex].distance -10) <10){
        char l[] = "Arrived";
                for(printi = 0; printi < strlen(l);printi++){
                    uart_sendChar(l[printi]);
                }
    }
    else{ //not at object
        double angel = (double) obstacles[index].angle; //gotta pass them doubles
        char str[10];
        sprintf(str, "%lf", angel);
        for(printi = 0; printi < strlen(str);printi++){
            uart_sendChar(str[printi]); //Print mid angle object in Putty
        }
        uart_sendChar('\t');
        if(angel < 90){ //repeat for other angles > 90 and = 90;
            char l[] = "Turning Right";
            for(printi = 0; printi < strlen(l);printi++){
                uart_sendChar(l[printi]);
            }
            turn_right(sensor, 90-angel);
            move_forward(sensor, (obstacles[index].distance - 25) * 10);
        }
        else if(angel > 90){
            char l[] = "Turning Left";
            for(printi = 0; printi < strlen(l);printi++){
                uart_sendChar(l[printi]);
            }
            turn_left(sensor, angel-90);
            move_forward(sensor, (obstacles[index].distance - 25) * 10);
        }
        else if(angel == 90){
            move_forward(sensor, (obstacles[index].distance - 25) * 10);
        }
        else{
            char oops[] = "Do a barrel roll";
            for(printi = 0; printi < strlen(oops);printi++){
                uart_sendChar(oops[printi]);
            }
            turn_right(sensor, 360);
        }

    }
}
