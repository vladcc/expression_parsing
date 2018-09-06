#ifndef VM_H
#define VM_H

enum {vmINTERPRET, vmCOMPILE, vmPUSH, vmPOP, vmADD, vmSUB, vmMULT, vmDIV};

void vm_set_mode(int mod);
void vm_load_num(int num);
void vm_exec(int action);
void vm_stop(void);
void vm_reset(void);
#endif
