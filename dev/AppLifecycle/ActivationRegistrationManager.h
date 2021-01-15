// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
#pragma once

#include <ActivationRegistrationManager.g.h>

enum BlockedItemStatus
{
    Blocked = 0x0001,
    Seen = 0x0002
};
DEFINE_ENUM_FLAG_OPERATORS(BlockedItemStatus);

struct BlockedItem
{
    BlockedItemStatus flags;
    FILETIME disabledTime;
};

namespace winrt::Microsoft::ProjectReunion::implementation
{
    struct ActivationRegistrationManager
    {
        ActivationRegistrationManager() = default;

        static void RegisterForFileTypeActivation(array_view<hstring const> supportedFileTypes,
            array_view<hstring const> supportedVerbs, hstring const& applicationDisplayName,
            hstring const& logo);
        static void RegisterForProtocolActivation(hstring const& scheme,
            hstring const& applicationDisplayName, hstring const& logo);
        static void RegisterForStartupActivation(hstring const& taskId, bool isEnabled);

        static void UnregisterForFileTypeActivation(hstring const& fileType);
        static void UnregisterForProtocolActivation(hstring const& scheme);
        static void UnregisterForStartupActivation(hstring const& taskId);
    };
}

namespace winrt::Microsoft::ProjectReunion::factory_implementation
{
    struct ActivationRegistrationManager : ActivationRegistrationManagerT<ActivationRegistrationManager,
        implementation::ActivationRegistrationManager>
    {
    };
}
