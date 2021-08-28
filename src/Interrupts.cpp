#include "Interrupts.h"
#include "tier0/string.h"
//#include "tier0/stdio.h"

void printf(char * str, ...); //TODO Remove this.

extern "C" void load_idt(uint32_t);

void LoadIdt(uint16_t size, void * base) {
    struct
    {
        uint16_t lenght;
        void * base;
    }__attribute__((packed)) IDTR = {size, base };
    asm ( "lidt %0" : : "m"(IDTR) );  // let the compiler choose an addressing mode
}

bool are_interrupts_enabled()
{
    unsigned long flags;
    asm volatile ( "pushf\n\t"
                   "pop %0"
                   : "=g"(flags) );
    return flags & (1 << 9);
}

uint32_t InterruptManager::HandleInterrupt(uint8_t interruptNum, uint32_t esp) {
    //Just return the current stack pointer for now.
    printf("INTERRUPT");
    return esp;
}

InterruptManager::InterruptManager(GlobalDescriptorTable * gdt)
: m_picMasterCommand(0x20), m_picMasterData(0x21), m_picSlaveCommand(0xA0), m_picSlaveData(0xA1) {
    uint16_t CodeSement = gdt->CodeSegmentSelector();
    memset(&interruptDescriptorTable, 0, sizeof(GateDescritor) * 256);
    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    //Ignore all interrupts that we are not going to use.
    for(uint16_t i = 255; i > 0 ; --i) {
        SetInterruptDesriptorTableEntry(i, CodeSement, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
        //SetInterruptDesriptorTableEntry(i, CodeSement, &testAsmFunc, 0, IDT_INTERRUPT_GATE);
    }
    SetInterruptDesriptorTableEntry(0, CodeSement, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);

    SetInterruptDesriptorTableEntry(0x20, CodeSement, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE); //Timer
    SetInterruptDesriptorTableEntry(0x21, CodeSement, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE); //Keyborad

    //Initialize the PICS
    m_picMasterCommand.Write(0x11);
    m_picSlaveCommand.Write(0x11);
    //Tell the pics how much to add to any interrputs. OFFSET
    m_picMasterData.Write(0x20);
    m_picSlaveData.Write(0x28);
    //Tell the Master PIC there is a slave PIC
    m_picMasterData.Write(0x04);
    m_picSlaveData.Write(0x02);
    // 8086/88 (MCS-80/85) mode
    m_picMasterData.Write(0x01);
    m_picSlaveData.Write(0x01);
    m_picMasterData.Write(0x00);
    m_picSlaveData.Write(0x00);
    const uint16_t idtLength = (256 * sizeof(GateDescritor) - 1);
    //InterruptDescriptorTablePointer idt_pointer;
    //Tell the cpu to use the interrupt descriptor table
    //idt_pointer.size = idtLength
    //idt_pointer.base  = (uint32_t)interruptDescriptorTable;
    //asm volatile("lidt %0" : : "m" (idt_pointer)); //Load the Interrupt descriptor table.
    //load_idt((uint32_t)&idt_pointer);
    LoadIdt(idtLength, interruptDescriptorTable);
}

InterruptManager::~InterruptManager() {

}

void InterruptManager::Activate() {
    asm("sti");
    if(are_interrupts_enabled()) {
        printf("Interrupts enabled\n");
    }
    else {
        printf("Interrupts disabled\n");
    }
}

InterruptManager::GateDescritor InterruptManager::interruptDescriptorTable[256];

void InterruptManager::SetInterruptDesriptorTableEntry(uint8_t interruptNum,
            uint16_t gdt_codeSegmentSelectorOffset, void (*handler)(), uint8_t accessRights, uint8_t flags)
{
    const uint8_t IDT_DESC_PRESET = 0x80;
    interruptDescriptorTable[interruptNum].handlerAddressLowBits = (uint32_t)handler & 0xFFFF;
    interruptDescriptorTable[interruptNum].handlerAddressHighBits = (((uint32_t)handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interruptNum].gdt_codeSegmentSelector = gdt_codeSegmentSelectorOffset;

    interruptDescriptorTable[interruptNum].access = IDT_DESC_PRESET | ((accessRights&3)  << 5) | flags;
    interruptDescriptorTable[interruptNum].reserved = 0; 

}