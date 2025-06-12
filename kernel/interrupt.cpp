/**
 * @file interrupt.cpp
 *
 * 割り込み用のプログラムを集めたファイル．
 */

 #include "interrupt.hpp"

 #include "asmfunc.h"
 #include "segment.hpp"
 #include "timer.hpp"
 #include "task.hpp"

 // IDT(割り込み記述子テーブル)の定義
 std::array<InterruptDescriptor, 256> idt;

void SetIDTEntry(InterruptDescriptor& desc,
    InterruptDescriptorAttribute attr,
    uint64_t offset,
    uint16_t segment_selector) {
  desc.attr = attr;
  desc.offset_low = offset & 0xffffu;
  desc.offset_middle = (offset >> 16) & 0xffffu;
  desc.offset_high = offset >> 32;
  desc.segment_selector = segment_selector;
}

// endofinterrupt(0xfee000b0)レジスタに0を入力
void NotifyEndOfInterrupt() {
  // 書き込むことに意味があるため、読みだされていないがコンパイル時に省略されないよう、volatileとする
  volatile auto end_of_interrupt = reinterpret_cast<uint32_t*>(0xfee000b0);
  *end_of_interrupt = 0;
}

namespace {
  // CPUアーキテクチャに依存した割り込みハンドラの前処理と後処理を挿入している
  __attribute__((interrupt))
  void IntHandlerXHCI(InterruptFrame* frame) {
    task_manager->SendMessage(1, Message{Message::kInterruptXHCI});
    NotifyEndOfInterrupt();
  }

  __attribute__((interrupt))
  void IntHandlerLAPICTimer(InterruptFrame* frame) {
    LAPICTimerOnInterrupt();
    NotifyEndOfInterrupt();
  }
}

void InitializeInterrupt() {
  SetIDTEntry(idt[InterruptVector::kXHCI],
              MakeIDTAttr(DescriptorType::kInterruptGate, 0),
              reinterpret_cast<uint64_t>(IntHandlerXHCI),
              kKernelCS);
  SetIDTEntry(idt[InterruptVector::kLAPICTimer],
              MakeIDTAttr(DescriptorType::kInterruptGate, 0),
              reinterpret_cast<uint64_t>(IntHandlerLAPICTimer),
              kKernelCS);
  LoadIDT(sizeof(idt) - 1, reinterpret_cast<uintptr_t>(&idt[0]));
}