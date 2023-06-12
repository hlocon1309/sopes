/* See COPYRIGHT for copyright information. */

#include <inc/x86.h>
#include <kern/kclock.h>
#include <kern/trap.h>
#include <kern/picirq.h>

/* HINT: Note that selected CMOS
 * register is reset to the first one
 * after first access, i.e. it needs to be selected
 * on every access.
 *
 * Don't forget to disable NMI for the time of
 * operation (look up for the appropriate constant in kern/kclock.h)
 *
 * Why it is necessary?
 */

uint8_t
cmos_read8(uint8_t reg) {
    /* MC146818A controller */
    // LAB 4: Your code here

    nmi_disable();

    uint8_t res = 0;

    res = inb(reg);

    nmi_enable();

    return res;
}

void
cmos_write8(uint8_t reg, uint8_t value) {
    // LAB 4: Your code here

    nmi_disable();

    outb(reg, value);

    nmi_enable();
}

uint16_t
cmos_read16(uint8_t reg) {
    return cmos_read8(reg) | (cmos_read8(reg + 1) << 8);
}

void
rtc_timer_pic_interrupt(void) {
    // LAB 4: Your code here
    // Enable PIC interrupts.
    uint16_t mask = 0xFFFF & ~(1 << IRQ_CLOCK);
    pic_irq_mask(mask);
}

void
rtc_timer_pic_handle(void) {
    rtc_check_status();
    pic_send_eoi(IRQ_CLOCK);
}

void
rtc_timer_init(void) {
    // LAB 4: Your code here
    // (use cmos_read8/cmos_write8)

    uint8_t reg_a = 0;
    uint8_t reg_b = 0;

    cmos_write8(CMOS_CMD, RTC_AREG);
    reg_a = cmos_read8(CMOS_DATA);
    reg_a = reg_a | RTC_500MS_RATE;
    cmos_write8(CMOS_DATA, reg_a);

    cmos_write8(CMOS_CMD, RTC_BREG);
    reg_b = cmos_read8(CMOS_DATA);
    reg_b = reg_b | RTC_PIE;
    cmos_write8(CMOS_DATA, reg_b);

}

uint8_t
rtc_check_status(void) {
    // LAB 4: Your code here
    // (use cmos_read8)

    uint8_t status = 0;
    
    outb(CMOS_CMD, RTC_CREG);
    status = inb(CMOS_DATA);

    return status;
}
