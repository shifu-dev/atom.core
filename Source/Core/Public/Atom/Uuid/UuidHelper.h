#pragma once
#include "Atom/Uuid/Uuid.h"

namespace Atom
{
    struct UuidHelper
    {
        constexpr EUuidVariant GetVariant(const Uuid& uuid) const noexcept
        {
            if ((uuid.bytes[8] & 0x80) == 0x00)
                return EUuidVariant::NCS;
            else if ((uuid.bytes[8] & 0xC0) == 0x80)
                return EUuidVariant::RFC;
            else if ((uuid.bytes[8] & 0xE0) == 0xC0)
                return EUuidVariant::Microsoft;

            return EUuidVariant::Reserved;
        }

        constexpr EUuidVersion GetVersion(const Uuid& uuid) const noexcept
        {
            if ((uuid.bytes[6] & 0xF0) == 0x10)
                return EUuidVersion::V1;
            else if ((uuid.bytes[6] & 0xF0) == 0x20)
                return EUuidVersion::V2;
            else if ((uuid.bytes[6] & 0xF0) == 0x30)
                return EUuidVersion::V3;
            else if ((uuid.bytes[6] & 0xF0) == 0x40)
                return EUuidVersion::V4;
            else if ((uuid.bytes[6] & 0xF0) == 0x50)
                return EUuidVersion::V5;

            return EUuidVersion::None;
        }
    };
}