/**
 * @file interrupt.cpp
 *
 * 割り込み用のプログラムを集めたファイル．
 */

 #include "interrupt.hpp"

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