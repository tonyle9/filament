/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef TNT_MATC_POSTPROCESS_PACKAGE_BUILDER_H
#define TNT_MATC_POSTPROCESS_PACKAGE_BUILDER_H

#include <filamat/Package.h>
#include <filamat/MaterialBuilder.h>

#include <filament/driver/DriverEnums.h>

#include <utils/bitset.h>

namespace filamat {

class UTILS_PUBLIC PostprocessMaterialBuilder : public MaterialBuilderBase {
public:

    Package build();

    // Each shader generated while building the package content can be post-processed via this
    // callback.
    PostprocessMaterialBuilder& postProcessor(PostProcessCallBack callback);

    // specifies desktop vs mobile; works in concert with TargetApi to determine the shader models
    // (used to generate code) and final output representations (spirv and/or text).
    PostprocessMaterialBuilder& platform(Platform platform) noexcept {
        mPlatform = platform;
        return *this;
    }

    // specifies vulkan vs opengl; works in concert with Platform to determine the shader models
    // (used to generate code) and final output representations (spirv and/or text).
    PostprocessMaterialBuilder& targetApi(TargetApi targetApi) noexcept {
        mTargetApi = targetApi;
        mCodeGenTargetApi = targetApi;
        return *this;
    }

    // specifies vulkan vs opengl; this method can be used to override which target API is used
    // during the code generation step. This can be useful when the post-processor uses a
    // different intermediate representation.
    PostprocessMaterialBuilder& codeGenTargetApi(TargetApi targetApi) noexcept {
        mCodeGenTargetApi = targetApi;
        return *this;
    }

private:
    PostProcessCallBack mPostprocessorCallback = nullptr;
};

} // namespace
#endif // TNT_MATC_POSTPROCESS_PACKAGE_BUILDER_H
