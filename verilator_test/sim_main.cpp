#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <verilated.h>
// Include model header, generated from Verilating "top.v"
#include "Vtop.h"
#include <verilated_vcd_c.h>

const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};
int main(int argc, char **argv, char **env)
{
    int k = 10;

    contextp->traceEverOn(true);
    contextp->commandArgs(argc, argv);
    const std::unique_ptr<Vtop> top{new Vtop{contextp.get(), "TOP"}};

    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 0); 
    Verilated::mkdir("logs");
    tfp->open("logs/vlt_dump.vcd");
    while (k--)
    {
        if (tfp)
            tfp->flush();

        int a = rand() & 1;
        int b = rand() & 1;
        top->a = a;
        top->b = b;
        top->eval();
        printf("a = %d, b = %d, f = %d\n", a, b, top->f);
        tfp->dump(contextp->time());
        contextp->timeInc(1);//推动仿真时间
        assert(top->f == (a ^ b));
    }
    // Final model cleanup
    top->final();
    // Destroy model

    // Return good completion status
    return 0;
}
