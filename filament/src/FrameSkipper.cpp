/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "details/FrameSkipper.h"

#include "details/Engine.h"

namespace filament {
namespace details {

FrameSkipper::FrameSkipper(FEngine& engine, size_t latency) noexcept
    : mEngine(engine) {
    latency = std::max(latency, size_t(1));
    mFences.resize(latency);
}

FrameSkipper::~FrameSkipper() noexcept {
    for (FFence* fence : mFences) {
        if (fence) {
            mEngine.destroy(fence);
        }
    }
}

void FrameSkipper::endFrame() noexcept {
    mFences.push_back( mEngine.createFence(Fence::Type::HARD) );
}

bool FrameSkipper::skipFrameNeeded() const noexcept {
    if (mExtraSkipCount) {
        mExtraSkipCount--;
        return true;
    }

    if (mFences.empty()) {
        return true;
    }

    auto& fences = mFences;
    FFence* fence = fences.front();
    if (fence) {
        auto status = fence->wait(Fence::Mode::DONT_FLUSH, 0);
        if (status == Fence::FenceStatus::TIMEOUT_EXPIRED) {
            // fence not ready, skip frame
            mExtraSkipCount = 0;
            return true;
        }
        mEngine.destroy(fence);
    }
    fences.pop_front();
    return false;
}


} // namespace details
} // namespace filament
