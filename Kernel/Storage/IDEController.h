/*
 * Copyright (c) 2020, Liav A. <liavalb@hotmail.co.il>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <AK/OwnPtr.h>
#include <AK/RefPtr.h>
#include <AK/Types.h>
#include <Kernel/Storage/IDEChannel.h>
#include <Kernel/Storage/StorageController.h>
#include <Kernel/Storage/StorageDevice.h>

namespace Kernel {

class AsyncBlockDeviceRequest;

class IDEController final : public StorageController
    , public PCI::DeviceController {
    AK_MAKE_ETERNAL
public:
    static NonnullRefPtr<IDEController> initialize(PCI::Address address, bool force_pio);
    virtual ~IDEController() override;

    virtual Type type() const override { return Type::IDE; }
    virtual RefPtr<StorageDevice> device(u32 index) const override;
    virtual bool reset() override;
    virtual bool shutdown() override;
    virtual size_t devices_count() const override;
    virtual void start_request(const StorageDevice&, AsyncBlockDeviceRequest&) override;
    virtual void complete_current_request(AsyncDeviceRequest::RequestResult) override;

    bool is_bus_master_capable() const;
    bool is_pci_native_mode_enabled() const;

private:
    bool is_pci_native_mode_enabled_on_primary_channel() const;
    bool is_pci_native_mode_enabled_on_secondary_channel() const;
    IDEController(PCI::Address address, bool force_pio);

    RefPtr<StorageDevice> device_by_channel_and_position(u32 index) const;
    void initialize(bool force_pio);
    void detect_disks();

    NonnullRefPtrVector<IDEChannel> m_channels;
};
}
