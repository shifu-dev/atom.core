#pragma once

extern "C"
{
    #include "WjCryptLib_Md5.h"
}

#include "Atom/Core.h"
#include "Atom/Exceptions.h"
#include "Atom/Math.h"
#include "Atom/Containers/StaticArray.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Md5 Hash output.
    /// --------------------------------------------------------------------------------------------
    struct Md5Hash
    {
        StaticArray<byte, 16> bytes;
    };

    /// --------------------------------------------------------------------------------------------
    /// Md5 Hash Generator.
    /// --------------------------------------------------------------------------------------------
    class Md5HashGenerator
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        /// 
        /// Same as calling Reset.
        /// ----------------------------------------------------------------------------------------
        Md5HashGenerator() noexcept
        {
            Reset();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Resets the {Md5HashGenerator} to its initial state.
        /// ----------------------------------------------------------------------------------------
        void Reset() noexcept
        {
            Md5Initialise(&m_context);
        }

        /// ----------------------------------------------------------------------------------------
        /// Processes data. This can be called infinite times.
        /// 
        /// @PARAM[IN] in_data Ptr to the input data.
        /// @PARAM[IN] in_dataSize Size of the data.
        /// 
        /// @EXPECTS {in_data != nullptr}.
        /// @EXPECTS {in_dataSize > 0}.
        /// 
        /// @THROWS AssertionException Expects {in_data != nullptr}.
        /// @THROWS AssertionException Expects {in_dataSize > 0}.
        /// ----------------------------------------------------------------------------------------
        void ProcessBytes(const void* in_data, usize in_dataSize)
        {
            ATOM_DEBUG_ASSERT(in_data != nullptr);
            ATOM_DEBUG_ASSERT(in_dataSize > 0);

            // The implementation accepts size of data in uint32 but we accept in usize,
            // which could be bigger than uint32, so we pass the data to the implementation 
            // in parts.

            // Max size of input WjCrypt accepts at once.
            static constexpr usize maxInput = NumLimits<uint32_t>::max();
            for (usize processed = 0; processed < in_dataSize; processed += maxInput)
            {
                Md5Update(&m_context, (byte*)in_data + processed,
                    Math::Min(maxInput, in_dataSize - processed));
            }

            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Processes single byte. This can be called infinite times.
        /// 
        /// @PARAM[IN] in_data Data to process.
        /// ----------------------------------------------------------------------------------------
        void ProcessByte(byte in_data)
        {
            Md5Update(&m_context, &in_data, 1);
        }

        /// ----------------------------------------------------------------------------------------
        /// Generates Md5 Hash.
        /// 
        /// @RETURNS {Md5Hash} object.
        /// ----------------------------------------------------------------------------------------
        Md5Hash Generate() noexcept
        {
            Md5Hash hash;
            Md5Finalise(&m_context, (MD5_HASH*)hash.bytes.data());

            return hash;
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// Underlying Md5 implementation.
        /// ----------------------------------------------------------------------------------------
        Md5Context m_context;
    };
}