#pragma once

// #@@range_begin(pixel_color_def)
#include "frame_buffer_config.hpp"

struct PixelColor {
  uint8_t r, g, b;
};
// #@@range_end(pixel_color_def)

class PixelWriter {
  public:
    // メンバ変数config_を引数configで初期化
    PixelWriter(const FrameBufferConfig& config) : config_{config} {
    }
    // デストラクタ
    virtual ~PixelWriter() = default;
    virtual void Write(int x, int y, const PixelColor& c) = 0;
  
  protected:
    uint8_t* PixelAt(int x, int y) {
      return config_.frame_buffer + 4 * (config_.pixels_per_scan_line * y + x);
    }

  private:
    const FrameBufferConfig& config_;
};

// プロトタイプ宣言
class RGBResv8BitPerColorPixelWriter : public PixelWriter {
  public:
    // 継承コンストラクタ
    using PixelWriter::PixelWriter;
    virtual void Write(int x, int y, const PixelColor& c) override;
};

class BGRResv8BitPerColorPixelWriter : public PixelWriter {
  public:
    using PixelWriter::PixelWriter;
    virtual void Write(int x, int y, const PixelColor& c) override;
};