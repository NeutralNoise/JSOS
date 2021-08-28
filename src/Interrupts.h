#ifndef __INTERRUPTS_H_INCULED
#define __INTERRUPTS_H_INCULED
#include "tier0/types.h"
#include "gdt.h"
#include "port.h"
 
extern void printfLow();

class InterruptManager {
public:

    InterruptManager(GlobalDescriptorTable * gdt);
    ~InterruptManager();

    void Activate();

    static uint32_t HandleInterrupt(uint8_t interruptNum, uint32_t esp);
    static void IgnoreInterruptRequest();

protected:

    struct GateDescritor
    {
        uint16_t handlerAddressLowBits;
        uint16_t gdt_codeSegmentSelector;
        uint8_t reserved;
        uint8_t access;
        uint16_t handlerAddressHighBits;
    } __attribute__((packed));
    
    static GateDescritor interruptDescriptorTable[256];

    struct InterruptDescriptorTablePointer
    {
        uint16_t size;
        uint32_t base; //Address of the table
    }__attribute__((packed));

    static void SetInterruptDesriptorTableEntry(uint8_t interruptNum,
                uint16_t gdt_codeSegmentSelectorOffset,
                void (*handler)(),
                uint8_t accessRights,
                uint8_t flags);

    static void HandleInterruptRequest0x00(); //Timer
    static void HandleInterruptRequest0x01(); //Keyborad


    Port8BitSlow m_picMasterCommand;
    Port8BitSlow m_picMasterData;
    Port8BitSlow m_picSlaveCommand;
    Port8BitSlow m_picSlaveData;

};


#endif //!__INTERRUPTS_H_INCULED