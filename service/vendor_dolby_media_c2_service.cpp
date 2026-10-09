/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.dolby.media.c2@1.0-service"

#include <android/hardware/media/c2/1.0/IComponentStore.h>
#include <binder/ProcessState.h>
#include <codec2/hidl/1.0/ComponentStore.h>
#include <hidl/HidlTransportSupport.h>
#include <log/log.h>
#include <utils/Errors.h>
#include <utils/StrongPointer.h>

#include <csignal>
#include <cstdlib>
#include <memory>

namespace android {
std::shared_ptr<C2ComponentStore> GetCodec2DolbyComponentStore();
}  // namespace android

int main() {
    using android::hardware::configureRpcThreadpool;
    using android::hardware::joinRpcThreadpool;
    using android::hardware::media::c2::V1_0::utils::ComponentStore;

    ALOGI("Dolby Codec2 service starting");
    signal(SIGPIPE, SIG_IGN);

    android::ProcessState::initWithDriver("/dev/vndbinder");
    android::ProcessState::self()->startThreadPool();
    configureRpcThreadpool(8, true);

    const std::shared_ptr<C2ComponentStore> store =
            android::GetCodec2DolbyComponentStore();
    if (store == nullptr) {
        ALOGE("Cannot create Dolby Codec2 component store");
        return EXIT_FAILURE;
    }

    android::sp<ComponentStore> service = android::sp<ComponentStore>::make(store);
    if (service->status() != C2_OK) {
        ALOGE("Dolby Codec2 component store initialization failed: %d", service->status());
        return EXIT_FAILURE;
    }

    const android::status_t status = service->registerAsService("default1");
    if (status != android::OK) {
        ALOGE("Cannot register Dolby Codec2 component store: %d", status);
        return EXIT_FAILURE;
    }

    ALOGI("Dolby Codec2 component store registered as default1");
    joinRpcThreadpool();
    ALOGE("Dolby Codec2 thread pool exited unexpectedly");
    return EXIT_FAILURE;
}
