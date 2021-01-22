#include <Wire.h>
#include "domain/blade_state.h"

#define IDLE_THRESHOLD 12
#define CLASH_THRESHOLD 94
class Accelerometer
{

public:
    // Acceleration on X: Translate right & left
    int16_t ax;
    // Acceleration on Y: Poke
    int16_t ay;
    // Acceleration on Z: Translate up & down
    int16_t az;

    // Rotational acceleration on X: Rotate vertically
    int16_t gx;
    // Rotational acceleration on Y: Rotate around blade axis
    int16_t gy;
    // Rotational acceleration on Z: Rotate horizontally
    int16_t gz;

    int16_t temp; // Temperature of the sensor
    void begin()
    {
        Wire.begin();
        Wire.beginTransmission(MPU_addr);
        Wire.write(0x6B); // PWR_MGMT_1 register
        Wire.write(0);    // set to zero (wakes up the MPU-6050)
        Wire.endTransmission(true);
    }

    void calibrate()
    {
        Wire.beginTransmission(MPU_addr);
        Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
        Wire.endTransmission(false);
        Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers

        // read acceleration values
        this->def_ax = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
        this->def_ay = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
        this->def_az = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

        // read temperature value
        this->temp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)

        // read rotational acceleration values
        this->def_gx = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
        this->def_gy = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
        this->def_gz = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    }

    void read(bool verbose)
    {
        Wire.beginTransmission(MPU_addr);
        Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
        Wire.endTransmission(false);
        Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers

        // read acceleration values
        this->ax = Wire.read() << 8 | Wire.read() - def_ax; // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
        this->ay = Wire.read() << 8 | Wire.read() - def_ay; // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
        this->az = Wire.read() << 8 | Wire.read() - def_az; // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

        // read temperature value
        this->temp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)

        // read rotational acceleration values
        this->gx = Wire.read() << 8 | Wire.read() - def_gx; // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
        this->gy = Wire.read() << 8 | Wire.read() - def_gy; // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
        this->gz = Wire.read() << 8 | Wire.read() - def_gz; // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

        // Print readings if enabled
        if (verbose)
        {
            Serial.print("ax = ");
            Serial.print(ax);
            Serial.print(" | ay = ");
            Serial.print(ay);
            Serial.print(" | az = ");
            Serial.print(az);
            Serial.print(" | gx = ");
            Serial.print(gx);
            Serial.print(" | gy = ");
            Serial.print(gy);
            Serial.print(" | gz = ");
            Serial.println(gz);
        }

        // Give it some time to breathe
        delay(5);
    }

    BladeState get_blade_state(bool verbose)
    {
        BladeState state = IDLE;
        // update the motion values
        this->read(false);

        // get the current acceleration
        curr_rot = get_curr_rot();
        curr_acc = past_acc - get_curr_acc();
        if (past_rot - curr_rot >= CLASH_THRESHOLD)
        {
            state = CLASH;
        }
        else if (curr_rot >= IDLE_THRESHOLD || curr_acc >= IDLE_THRESHOLD)
        {
            state = SWING;
        }

        // print stuff
        if (verbose)
        {
            Serial.print(curr_rot);
            Serial.print('\t');
            Serial.println(state_to_string(state));
        }

        // update acceleration and rotation values
        this->past_rot = this->curr_rot;
        return state;
    }

private:
    // Default address of the sensor
    static const int MPU_addr = 0x68;

    // Default acceleration on X: Translate right & left
    int16_t def_ax;
    // Default acceleration on Y: Poke
    int16_t def_ay;
    // Default acceleration on Z: Translate up & down
    int16_t def_az;

    // Default rotational acceleration on X
    int16_t def_gx;
    // Default rotational acceleration on Y
    int16_t def_gy;
    // Default rotational acceleration on Z
    int16_t def_gz;

    double curr_acc;
    double past_acc = 0.0;
    double get_curr_acc()
    {
        return (abs(ax / def_ax) + abs(ay / def_ay) + abs(az / def_az)) / 3;
    }

    double curr_rot;
    double past_rot = 0.0;
    double get_curr_rot()
    {
        return (abs(gx / def_gx) + abs(gy / def_gy) / 2 + abs(gz / def_gz)) / 3;
    }
};
