
/* Includes */
#include "mbed.h"
#include "HTS221Sensor.h"
#include "LPS22HBSensor.h"
#include "LSM6DSLSensor.h"
#include "lis3mdl_class.h"
#include "VL53L0X.h"

// objects for various sensors
static DevI2C devI2c(PB_11,PB_10);
static LPS22HBSensor press_temp(&devI2c);
static HTS221Sensor hum_temp(&devI2c);
static LSM6DSLSensor acc_gyro(&devI2c,0xD4,D4,D5); // high address
static LIS3MDL magnetometer(&devI2c, 0x3C);
static DigitalOut shutdown_pin(PC_6);
static VL53L0X range(&devI2c, &shutdown_pin, PC_7, 0x52);

static UnbufferedSerial sp(USBTX, USBRX); // added to add tx and rx on serial port



// functions to print sensor data
void print_t_rh(){
    float value1, value2;
    hum_temp.get_temperature(&value1);
    hum_temp.get_humidity(&value2);

    value1=value2=0;    
    press_temp.get_temperature(&value1);
    press_temp.get_pressure(&value2);
    printf("LPS22HB: [temp] %.2f C, [press] %.2f mbar\r\n", value1, value2);
}

void print_mag(){
    int32_t axes[3];
    magnetometer.get_m_axes(axes);
    printf("LIS3MDL [mag/mgauss]:    %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);

}

void print_accel(){
    int32_t axes[3];
    acc_gyro.get_x_axes(axes);
    printf("LSM6DSL [acc/mg]:        %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);
}

void print_gyro(){
    int32_t axes[3];
    acc_gyro.get_g_axes(axes);
    printf("LSM6DSL [gyro/mdps]:     %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);
}

void print_distance(){
    uint32_t distance;
    int status = range.get_distance(&distance);
    if (status == VL53L0X_ERROR_NONE) {
        printf("VL53L0X [mm]:            %6ld\r\n", distance);
    } else {
        printf("VL53L0X [mm]:                --\r\n");
    }
}

char key;
volatile char keyB = 0;

void PCinterrupt(){
    if(sp.readable()){
        sp.read(&key, 1);
        keyB = 1;
    }}

/* Simple main function */
int main() {
    uint8_t id;
    float value1, value2;

    int32_t axes[3];

    hum_temp.init(NULL);

    press_temp.init(NULL);
    magnetometer.init(NULL);
    acc_gyro.init(NULL);

    range.init_sensor(0x52);

    hum_temp.enable();
    press_temp.enable();

    acc_gyro.enable_x();
    acc_gyro.enable_g();
  
    //printf("\033[2J\033[20A");
    printf ("\r\n--- Sensor terminal active ---\r\n\r\n");
printf ("\r\n--- Press a for Accel Data ---\r\n\r\n");
printf ("\r\n--- Press g for Gyro Data ---\r\n\r\n");
printf ("\r\n--- Press d for Distance Data ---\r\n\r\n");
printf ("\r\n--- Press t for Temp and pressure Data ---\r\n\r\n");
printf ("\r\n--- Press m for Magnetometer Data ---\r\n\r\n");
  /*  hum_temp.read_id(&id);
    printf("HTS221  humidity & temperature    = 0x%X\r\n", id);

    press_temp.read_id(&id);
    printf("LPS22HB pressure & temperature    = 0x%X\r\n", id);
    magnetometer.read_id(&id);
    printf("LIS3MDL magnetometer              = 0x%X\r\n", id);
    acc_gyro.read_id(&id);
    printf("LSM6DSL accelerometer & gyroscope = 0x%X\r\n", id);
    */
   /* printf("\n\r--- Reading sensor values ---\n\r"); ;
    print_t_rh();
    print_mag();
    print_accel();
    print_gyro();
    print_distance();
    printf("\r\n"); */
    

sp.attach(&PCinterrupt);

    while (1) {
        if(keyB){
            switch (key) {
                case 'a': // If button a is pressed on the keyboard the the function print_accel is activated and acceleration will be printed on the console
                    print_accel();
                    break;
                case 'g': // If button g is pressed on the keyboard the the function print_gyro is activated and gyroscopre details will be shown
                    print_gyro();
                    break;
                case 'd': // If button d is pressed on the keyboard the the function print_distanme is activated and distance sensor value will be shown
                    print_distance();
                    break; 
                    case 't': // If button t is pressed on the keyboard the the function print_t_rh is activated and temp/pressure data will be shown
                      print_t_rh();
                      break;
                      case 'm': // If button m is pressed on the keyboard the the function print_mag is activated
                       print_mag();    
                       break;

            }
            keyB = 0;
        }




    
    }
}