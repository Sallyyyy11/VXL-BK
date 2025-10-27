#ifndef SCHEDULER_H_
#define SCHEDULER_H_
#include "main.h"

#define ERROR_SCH_TOO_MANY_TASKS 1
#define ERROR_SCH_CANNOT_DELETE_TASK 2
#define RETURN_ERROR 1
#define RETURN_NORMAL 0

typedef struct {
    void (* pTask)(void);
    uint32_t Delay;
    uint32_t Period;
    uint8_t RunMe;
    uint32_t TaskID;
} sTask;


#define SCH_MAX_TASKS 40  // maximum number of tasks allowed
#define NO_TASK_ID 0

// Function prototypes
void SCH_Init(void);
uint32_t SCH_Add_Task(void (*pFunction)(void), uint32_t DELAY, uint32_t PERIOD);
void SCH_Update(void);
void SCH_Dispatch_Tasks(void);
uint8_t SCH_Delete_Task(uint32_t index);

#endif
