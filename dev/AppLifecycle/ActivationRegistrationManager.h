// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
#pragma once

#include <ActivationRegistrationManager.g.h>

namespace winrt::Microsoft::ProjectReunion::implementation
{
    // Registration constant values.
    static const std::wstring c_argumentPrefix = L"----";
    static const std::wstring c_argumentSuffix = L":";
    static const std::wstring c_protocolArgumentString = L"ms-protocol";
    static const std::wstring c_launchSchemeName = L"ms-launch";
    static const std::wstring c_encodedLaunchSchemeName = L"ms-encodedlaunch";
    static const std::wstring c_runKeyPath = LR"(Software\Microsoft\Windows\CurrentVersion\Run\)";

    struct ActivationRegistrationManager
    {
        ActivationRegistrationManager() = default;

        static void RegisterForFileTypeActivation(array_view<hstring const> supportedFileTypes,
            hstring const& logo, hstring const& displayName, array_view<hstring const> supportedVerbs,
            hstring const& exePath);
        static void UnregisterForFileTypeActivation(array_view<hstring const> fileTypes,
            hstring const& exePath);

        static void RegisterForProtocolActivation(hstring const& schemeName,
            hstring const& logo, hstring const& displayName, hstring const& exePath);
        static void UnregisterForProtocolActivation(hstring const& scheme, hstring const& exePath);

        static void RegisterForStartupActivation(hstring const& taskId, hstring const& exePath);
        static void UnregisterForStartupActivation(hstring const& taskId);

    private:
        static void RegisterForProtocolActivationInternal(std::wstring const& schemeName,
            std::wstring const& appUserModelId, std::wstring const& logo, std::wstring const& displayName,
            std::wstring const& exePath);
        static void RegisterEncodedLaunchCommand();
        static void RegisterEncodedLaunchSupport(std::wstring const& appUserModelId, std::wstring const& exePath);
    };
}

namespace winrt::Microsoft::ProjectReunion::factory_implementation
{
    struct ActivationRegistrationManager : ActivationRegistrationManagerT<ActivationRegistrationManager,
        implementation::ActivationRegistrationManager>
    {
    };
}
