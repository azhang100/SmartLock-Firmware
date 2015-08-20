#ifndef DRV8833_CONFIG_H
#define DRV8833_CONFIG_H

#define DRV8833_B_IN1 10                // 10 --> Motor B Input A --> MOTOR B +
#define DRV8833_B_IN2 9                 // 9 --> Motor B Input B --> MOTOR B -
#define DRV8833_nSLP 8                  // bring high to activate; will be pulled low
#define DRV8833_nFAULT 2                // fault indicator, active low

// functional connections
// // Both H-Bridges are in parallel in our case, so A / B immaterial
#define MOTOR_IN1 DRV8833_B_IN1
#define MOTOR_IN2 DRV8833_B_IN2

#endif
