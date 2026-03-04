#include "pch.h"

#include "Animation.h"

namespace FalkonEngine {

// Animation --- Container for frame sequences and atlas slicing logic ---
//-----------------------------------------------------------------------------------------------------------
void Animation::AddFrame(int x, int y, int w, int h, float duration) {
  // REGISTRATION: Push a manually defined frame into the sequence
  m_frames.push_back({{x, y, w, h}, duration});
}

//-----------------------------------------------------------------------------------------------------------
void Animation::AddFramesLine(int startX, int startY, int width, int height, int count, float frameTime) {
  // GENERATION: Automatically slice a horizontal row from the texture atlas
  for (int i = 0; i < count; ++i) {
    int currentX = startX + (i * width);
    AddFrame(currentX, startY, width, height, frameTime);
  }
}

//-----------------------------------------------------------------------------------------------------------
const AnimationFrame& Animation::GetFrame(size_t index) const {
  // SAFETY: Use .at() to ensure index is within bounds (throws out_of_range)
  return m_frames.at(index);
}

//-----------------------------------------------------------------------------------------------------------
size_t Animation::GetFrameCount() const { return m_frames.size(); }

}  // namespace FalkonEngine