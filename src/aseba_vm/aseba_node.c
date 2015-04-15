#include "ch.h"
#include "hal.h"
#include "skel.h"
#include "skel_user.h"
#include "aseba_node.h"

void update_aseba_variables_read(void);

void update_aseba_variables_write(void);

static THD_WORKING_AREA(aseba_vm_thd_wa, 1024);
static THD_FUNCTION(aseba_vm_thd, arg)
{
    (void)arg;

    while (TRUE) {
        palSetPad(GPIOD, GPIOD_LED3);
        chThdSleepMilliseconds(500);
        palClearPad(GPIOD, GPIOD_LED3);
        chThdSleepMilliseconds(500);
    }
    return 0;
}

void aseba_vm_init(void)
{
    vmState.nodeId = 1;

    AsebaVMInit(&vmState);
    vmVariables.id = vmState.nodeId;

    palSetPad(GPIOD, GPIOD_LED3);
    chThdSleepMilliseconds(200);
    palClearPad(GPIOD, GPIOD_LED3);
    chThdSleepMilliseconds(200);
    palSetPad(GPIOD, GPIOD_LED3);
    chThdSleepMilliseconds(200);
    palClearPad(GPIOD, GPIOD_LED3);
}

void aseba_vm_start(void)
{
    aseba_vm_init();

    chThdCreateStatic(aseba_vm_thd_wa, sizeof(aseba_vm_thd_wa), NORMALPRIO, aseba_vm_thd, NULL);
}
